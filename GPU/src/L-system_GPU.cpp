#include <iostream>
#include <fstream>
#include "IO/Parser.hpp"
#include "grammar/LSystemGenerator.hpp"
#include "graphics/Render.hpp"
#include <chrono>

#include <Kokkos_Core.hpp>

static const int MICROSECONDS = 100000;

int main(){

    // Debug section
    bool debugPrint = false;
    std::string readFile = "example.txt";
    std::string writeFile = "LSystem.svg";

    // Reads the file
    std::ifstream file(readFile);
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