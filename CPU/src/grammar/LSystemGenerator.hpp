#pragma once

#include <vector>
#include <Rule.hpp>
#include "../IO/Parser.hpp"

class LSystemGenerator {
public:

    /**
     * Creates the sentance from the axiom and the number of passes (recursion)
     * @param axiom the starting point of the L-system
     * @param rules all the rules that is applied
     * @param recursiveLevel depth of recursions
     */
    LSystemGenerator(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel);
    
    /**
     * Creates the sentance from the axiom and the number of passes (recursion) by using parser
     * @param parser that read the file
     */
    LSystemGenerator(Parser parser);
    
    /**
     * Launches the computation of the L-system
     */
    void compute();

    /**
     * Prints the full sentance if print option is used for debug
     * @param debugPrint if the sentance is printed
     */
    void toString(bool debugPrint);

    /**
     * Get the full computed sentance
     */
    std::vector<char> getComputedSentance();

private:

    /**
     * Initialisation of the parameters
     * @param axiom starting point of L-system
     * @param rules that are applied to L-system
     */
    void initialize(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel);

    /**
     * Evolves the L-system sentance with recursion
     * @param rules that contains the information to evolve the sentance
     * @param recursiveLevel depth of recursion
     */
    void evolve(std::vector<Rule> rules, int recursiveLevel);

    std::vector<char> m_currentLine;

    std::vector<char> m_axiom;
    std::vector<Rule> m_rules;
    int m_recursiveLevel;

};