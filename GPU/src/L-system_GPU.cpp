#include <iostream>
#include <fstream>
#include "IO/Parser.hpp"
#include "grammar/LSystemGenerator.hpp"
#include "graphics/Render.hpp"
#include <chrono>

#include <Kokkos_Core.hpp>

static const int MICROSECONDS = 100000;

int main(int argc, char *argv[]){

    // Arguments
    std::string readFile;
    std::string writeFile;
    bool debugPrint = false;

    for (int i = 1; i < argc; ++i) {

        if (std::string(argv[i]) == "-i" && i + 1 < argc) {
            readFile = argv[++i];
        }
        else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            writeFile = argv[++i];
        }
        else if (std::string(argv[i]) == "-printDebug") {  
            debugPrint = true;
        }
        else {
            std::cerr << "Unknown argument" << argv[i] << '\n';
            return 1;
        }
    }

    if (readFile.empty()) {
        std::cerr << "Error : -i is required \n";
        return 1;
    }

    if (writeFile.empty()) {
        std::cerr << "Error : -o is required \n";
        return 1;
    }

    std::cout << "Input file  : " << readFile << '\n';
    std::cout << "Output file : " << writeFile << '\n';

    // Reads the file
    std::ifstream file(readFile);
    if (file.fail()) {
         std::cout << "Cannot open the input file, please use an existing file" << std::endl;
         return 1;
    }
    Parser parser(file);
    parser.read();

    Kokkos::initialize();
    {
        // Create the generator
        LSystemGenerator lSystemGenerator(parser);

        // Compute the sentance
        Kokkos::Timer timer;
        lSystemGenerator.compute();
        double elapsedTime = timer.seconds();
        elapsedTime *= MICROSECONDS;
        std::cout << "L-system interpretation : " << elapsedTime << " microseconds\n" << std::endl;
       
        // debug it
        lSystemGenerator.toString(debugPrint);

        // write the file
        std::ofstream file(writeFile);
        
        // Create the render
        Render render(lSystemGenerator.getComputedSentance(),
         parser.getRotation(), 
         lSystemGenerator.hasBracket(), file);

        // interpret and compute elapse time
        timer.reset();
        render.interpret();
        elapsedTime = timer.seconds();
        elapsedTime *= MICROSECONDS;

        std::cout << "Graphical interpretation : " << elapsedTime << " microseconds\n" << std::endl;

    }
    Kokkos::finalize();
    

    return 0;

}