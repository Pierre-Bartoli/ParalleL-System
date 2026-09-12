#include <Render.hpp>
#include <iostream>
#include "../IO/SvgBuffer.hpp"

Render::Render(Kokkos::View<char*> Lsystem, float rotation, bool hasBracket, std::ofstream &writeFile):
    m_Lsystem(Lsystem), 
    m_hasBracket(hasBracket),
    m_writeFile(writeFile)
{
    m_size = Lsystem.extent(0);
    m_rotation = rotation/180*M_PI;
}

void Render::interpret()
{
    if(m_hasBracket)
        bracket();
    else
        withoutBracket();
}

void Render::withoutBracket()
{
    Kokkos::View<Matrix4*> matrices = computeMatrix();
    writeSvg(matrices);
}


Kokkos::View<Matrix4*> Render::computeMatrix()
{

    float stepForward = m_stepForward;
    int size = m_size;
    float rotation = m_rotation;
    Kokkos::View<char*> Lsystem = m_Lsystem;

    // To compute the scan
    int power = powf(2, static_cast<int>(log2f(size) + 1));

    // Matrices that are inputs
    Kokkos::View<Matrix4*> inputMatrices ("inputMatrices", size);

    // compute the transform for each symbol
    Kokkos::parallel_for("FirstPass", size, KOKKOS_LAMBDA(const int i){

        inputMatrices(i).identity(); 

        if(i < size){
            char symbol = Lsystem(i);
            inputMatrices(i).setTransform(symbol, stepForward, rotation);
        }

    });

    // Then copy input into final matrices
    Kokkos::View<Matrix4*> matrices ("matrices", size);
    Kokkos::deep_copy(matrices, inputMatrices);

    // Then do a belloch scan for non commutative element (matrix multiplication)
    // Kokkos scan has a bug, on nvidia backend join function reverse side at a certain size
    for (int stride = 1; stride < power; stride*=2)
    {
        const int step = stride*2;
        const int count = power/step;

        Kokkos::parallel_for("Upsweep", count, KOKKOS_LAMBDA(const int i){
            const int right = (i + 1)*step - 1;
            const int left = right - stride;

            matrices(right) = matrices(left).multiply(matrices(right));
        });

    }

    Kokkos::parallel_for("ID", 1, KOKKOS_LAMBDA(const int i)
    {
        matrices(power - 1).identity();
    });

    for (int stride = power/2; stride >= 1; stride /= 2)
    {
        const int step = stride*2;
        const int count = power/step;

        Kokkos::parallel_for("Downsweep", count, KOKKOS_LAMBDA(const int i){
            const int right = (i + 1)*step - 1;
            const int left = right - stride;

            Matrix4 tmp = matrices(left);
            matrices(left) = matrices(right);
            matrices(right) = matrices(right).multiply(tmp);

        });
    }

    // Then do the last for to make scan inclusive
    Kokkos::parallel_for("InclusiveScan", size, KOKKOS_LAMBDA(const int i){
        matrices(i) = matrices(i).multiply(inputMatrices(i));
    });

    return matrices;
}

Kokkos::View<int*> Render::computeBracket()
{
    int size = m_size;
    Kokkos::View<char*> Lsystem = m_Lsystem;

    // Init bracket depth
    Kokkos::View<int*> bracketDepth("bracket_depth", size);

    // If the save add 1 to depth, if pop remove one to depth
    Kokkos::parallel_for("PassDepth", size, KOKKOS_LAMBDA(const int i){
        
        if(Lsystem(i) == SAVE)
        {
            bracketDepth(i) = 1;
        }
        if(Lsystem(i) == POP)
        {
            bracketDepth(i) = -1;
        }

    });

    // Then scan to know for each index the depth
    Kokkos::parallel_scan("ScanDepth", size, KOKKOS_LAMBDA(const int i, int& update, bool final){
        
        if(final){
            bracketDepth(i) = update;
        }
        
        update += bracketDepth(i); 
    });

    // Get the maximum depth
    int maxDepth = 0;
    Kokkos::parallel_reduce("MaxDepth", size, KOKKOS_LAMBDA(const int i, int& max){
        
        if(bracketDepth(i) > max)
            max = bracketDepth(i);

    }, Kokkos::Max<int>(maxDepth));

    // Compute the number of bracket in each depth
    Kokkos::View<int**> count("count", maxDepth, size);
    Kokkos::parallel_for("PassOffset", size, KOKKOS_LAMBDA(const int i){
        if(Lsystem(i) == SAVE)
        {
            count(bracketDepth(i), i) += 1;
        }
        if(Lsystem(i) == POP)
        {
            count(bracketDepth(i) - 1, i) += 1;
        }
    });

    // Compute the correct offset and for each depth the number of brackets (bracketCount)
    Kokkos::View<int**> offset("offset", maxDepth, size);
    Kokkos::View<int*> bracketCount("bucketCount", maxDepth);
    Kokkos::parallel_for("ScanEachDepth", Kokkos::TeamPolicy<>(maxDepth, Kokkos::AUTO), KOKKOS_LAMBDA(const Kokkos::TeamPolicy<>::member_type& team){

        const int d = team.league_rank();

        Kokkos::parallel_scan(Kokkos::TeamThreadRange(team, size), [&](const int i, int& update, bool final){
            
            if(final){
                offset(d, i) = update;
                if(i == size - 1)
                    bracketCount(d) = update + count(d, i);
            }
            
            update += count(d, i);        
        });
    });

    // scan over the depth
    int bracketTotal = 0;
    Kokkos::parallel_scan("ScanBracketCount", maxDepth, KOKKOS_LAMBDA(const int d, int& update, bool final){
            
            if(final){
                bracketCount(d) = update;
            }
            
            update += bracketCount(d);        
        }, bracketTotal);
    
    // Consturct the bucket array containing save pop, save pop... for each bracket
    Kokkos::View<int*> bucketArray("bucketArray", bracketTotal);
    Kokkos::parallel_for("ConstructBrackets", size, KOKKOS_LAMBDA(const int i){

        if(Lsystem(i) == SAVE)
        {
            int depth = bracketDepth(i);
            int index = bracketCount(depth) + offset(depth, i);
            bucketArray(index) = i;
        }
        else if(Lsystem(i) == POP)
        {
            int depth = bracketDepth(i) - 1;
            int index = bracketCount(depth) + offset(depth, i);
            bucketArray(index) = i;
        }

    });

    // Creates the return array that contains for each save the correct pop
    Kokkos::View<int*> popIndex("popIndex", size);
    Kokkos::parallel_for("SearchPopIndex", bracketTotal/2, KOKKOS_LAMBDA(const int i){
        int index = i*2;
        popIndex(bucketArray(index)) = bucketArray(index + 1);
    });

    return popIndex;
}

void Render::bracket()
{
    
    int size = m_size;
    int stepForward = m_stepForward;
    float rotation = m_rotation;
    Kokkos::View<char*> Lsystem = m_Lsystem;

    // Compute the pop position for each save
    Kokkos::View<int*> popIndex = computeBracket();

    // Create the queue
    Kokkos::View<WorkItem*> currentQueue ("currentQueue", size + 1);
    Kokkos::View<WorkItem*> nextQueue ("nextQueue", size + 1);
    Kokkos::View<int> nextCount ("nextCount");

    // First queue
    auto hostQueue = Kokkos::create_mirror_view(currentQueue);
    Matrix4 id{};
    id.identity();
    hostQueue(0) = {
        0,
        size - 1,
        id
    };
    Kokkos::deep_copy(currentQueue, hostQueue);
    int numWorkItems = 1;

    // Creates matrices of the turtle path
    Kokkos::View<Matrix4*> matrices ("matrices", size);

    while (numWorkItems > 0)
    {
        Kokkos::deep_copy(nextCount, 0);

        Kokkos::parallel_for("threadedInterpretation", numWorkItems, KOKKOS_LAMBDA(const int work){
            
            WorkItem item = currentQueue(work);
            Matrix4 prefix = item.prefix;

            for (int i = item.begin; i < item.end; i++)
            {
                Matrix4 matrix{};
                char symbol = Lsystem(i);

                matrix.setTransform(symbol, stepForward, rotation);
                prefix = prefix.multiply(matrix);
                matrices(i) = prefix;

                if(symbol == SAVE)
                {
                    int pop = popIndex(i);
                    int q = Kokkos::atomic_fetch_add(&nextCount(), 2);

                    nextQueue(q) = {
                        i + 1,
                        pop,
                        prefix
                    };
                    nextQueue(q + 1) = {
                        pop,
                        item.end,
                        prefix
                    };

                    break;
                }
            }
        });

        int hostNextCount = 0;

        Kokkos::deep_copy(hostNextCount, nextCount);
        numWorkItems = hostNextCount;
        std::swap(currentQueue, nextQueue);
    }

    writeSvg(matrices);

}

void Render::writeSvg(Kokkos::View<Matrix4*> matrices)
{
    
    int size = m_size;
    Kokkos::View<char*> Lsystem = m_Lsystem;

    // Init the buffer offset
    Kokkos::View<int*> bufferOffset("bufferOffset", size);
    SvgBuffer svgBuffer{};
    int bufferDefaultSize = svgBuffer.defaultLineSize();
    int bufferTotal = 0;

    // Compute the buffer size and setup the offset
    Kokkos::parallel_scan("ScanBuffer", size, KOKKOS_LAMBDA(const int i, int& update, bool final){
        
        int bufferSize = 0;

        // if and only if the symbol is a step, compute the line size 
        if(Lsystem(i) == FORWARD){

            float x1 = 0;
            float y1 = 0;
            
            if(i > 0){
                x1 = matrices(i - 1).getMatrixAt(3);
                y1 = matrices(i - 1).getMatrixAt(7);
            }
        
            float x2 = matrices(i).getMatrixAt(3);
            float y2 = matrices(i).getMatrixAt(7);

            bufferSize = svgBuffer.computeBufferSize(x1, y1, x2, y2, bufferDefaultSize);
        }

        if(final)
            bufferOffset(i) = update;

        update += bufferSize;

    }, bufferTotal);

    // Computes the size of the svg
    float minX, minY, maxX, maxY;
    Kokkos::parallel_reduce("SvgBounds", size, KOKKOS_LAMBDA(const int i,
            float& locMinX,
            float& locMinY,
            float& locMaxX,
            float& locMaxY
        )
        {

            const float x1 = matrices(i - 1).getMatrixAt(3);
            const float y1 = matrices(i - 1).getMatrixAt(7);
        
            const float x2 = matrices(i).getMatrixAt(3);
            const float y2 = matrices(i).getMatrixAt(7);

            const float localMinX = x1 < x2 ? x1 : x2;
            const float localMinY = y1 < y2 ? y1 : y2;

            const float localMaxX = x1 > x2 ? x1 : x2;
            const float localMaxY = y1 > y2 ? y1 : y2;

            if (localMinX < locMinX) locMinX = localMinX;
            if (localMinY < locMinY) locMinY = localMinY;

            if (localMaxX > locMaxX) locMaxX = localMaxX;
            if (localMaxY > locMaxY) locMaxY = localMaxY;

        },

        Kokkos::Min<float>(minX),
        Kokkos::Min<float>(minY),
        Kokkos::Max<float>(maxX),
        Kokkos::Max<float>(maxY)
    );

    // Then write in it
    Kokkos::View<char*> buffer("buffer", bufferTotal + 1);
    Kokkos::parallel_for("WriteBuffer", size, KOKKOS_LAMBDA(const int i){
        
        int offset = bufferOffset(i);
        char* out = &buffer(offset);

        if(Lsystem(i) == FORWARD){
            
            float x1 = 0;
            float y1 = 0;
            
            if(i > 0){
                x1 = matrices(i - 1).getMatrixAt(3);
                y1 = matrices(i - 1).getMatrixAt(7);
            }
        
            float x2 = matrices(i).getMatrixAt(3);
            float y2 = matrices(i).getMatrixAt(7);
            
            svgBuffer.writeLine(out, x1, x2, y1, y2);

            if(i == size - 1)
            {
                *out = '\0';
            }

        }
    });

    auto h_buffer = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, buffer);
    svgBuffer.finaliseSvg(h_buffer, m_writeFile, minX, maxX, minY, maxY);
    
}
