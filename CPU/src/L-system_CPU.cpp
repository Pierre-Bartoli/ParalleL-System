#include <iostream>
#include <fstream>
#include "LSystemGenerator.hpp"
#include "IO/SvgWriter.hpp"
#include <chrono>
#include "graphics/Render.hpp"

int main(){

    // debug section
    bool debugPrint = false;
    std::string readFile = "example.txt";
    std::string writeFile = "LSystem.svg";

    // Reads the file to load the rules
    std::ifstream file(readFile);
    Parser parser(file);
    parser.read();
 
    // Creates the LSystem by using parser
    LSystemGenerator lSystemGenerator(parser);

    // compute the sentance and measure elapse time
    auto start = std::chrono::high_resolution_clock::now();
    lSystemGenerator.compute();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapseTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "L-system interpretation : " << elapseTime.count() << " microseconds\n";

    lSystemGenerator.toString(debugPrint);
    
    Turtle turtle;
    Render render(turtle, 10, parser.getRotation());

    start = std::chrono::high_resolution_clock::now();
    render.interpret(lSystemGenerator.getComputedSentance());
    end = std::chrono::high_resolution_clock::now();
    elapseTime = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    );
    std::cout << "Graphical interpretation : " << elapseTime.count() << " microseconds\n";
    
    // Write into svg
    std::ofstream svgFile(writeFile);
    SvgWriter svgWriter(svgFile);
    svgWriter.writePath(turtle.getPath());

    return 0;

}