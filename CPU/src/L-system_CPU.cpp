#include <iostream>
#include <fstream>
#include "LSystemGenerator.hpp"
#include "IO/SvgWriter.hpp"
#include <chrono>
#include "graphics/Render.hpp"

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

    // Reads the file to load the rules
    std::ifstream file(readFile);
    if (file.fail()) {
         std::cout << "Cannot open the input file, please use an existing file" << std::endl;
         return 1;
    }
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