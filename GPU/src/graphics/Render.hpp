#include <Kokkos_Core.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Matrix4.hpp>

class Render
{
public:

    Render(Kokkos::View<char*> Lsystem, float rotation, bool containsBracket, std::ofstream &writeFile);

    void withoutBracket();
    void bracket();

    Kokkos::View<int*> computeBracket();
    Kokkos::View<Matrix4*> computeMatrix();

    void writeSvg(Kokkos::View<Matrix4*> matrices);

    void interpret();

private:

    float m_stepForward = 10;
    float m_rotation;
    bool m_hasBracket;

    int m_size;
    std::ofstream &m_writeFile;

    Kokkos::View<char*> m_Lsystem;
    
};

struct WorkItem{
    int begin;
    int end;
    Matrix4 prefix;
};