#pragma once

#include <vector>
#include "Rule.hpp"
#include "../IO/Parser.hpp"
#include <Kokkos_Core.hpp>

class LSystemGenerator {
public:

    /**
     * Creates the sentance from the axiom and the number of passes (recursion)
     * @param axiom the starting point of the L-system
     * @param rules all the rules that is applied
     * @param recursiveLevel number of recursions
     */
    LSystemGenerator(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel);
    
    /**
     * Creates the sentance from the axiom and the number of passes (recursion) by using parser
     * @param parser that read the file
     */
    LSystemGenerator(Parser parser);

    /**
     * Evolves the L-system sentance on n number of recursion
     */
    void compute();

    /**
     * Prints the full sentance if print option is used for debug
     * @param debugPrint if the sentance is printed
     */
    void toString(bool debugPrint);

    // Getters
    bool hasBracket();
    Kokkos::View<char*> getComputedSentance();

private:

    /**
     * Initialisation of the parameters
     */
    void initialize(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel);

    Kokkos::View<char*> m_currentLine;
    Kokkos::View<int*> m_ruleIndex;
    Kokkos::View<int*> m_ruleSize;
    Kokkos::View<char*> m_successors;
    Kokkos::View<int*> m_successorOffset;
    int m_recursiveLevel;
    int m_currentSize;

    bool m_hasBracket;
};