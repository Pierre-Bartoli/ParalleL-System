#pragma once

#include <fstream>
#include <vector>
#include "../grammar/Rule.hpp"

/**
 * Reads the file
 */
class Parser{
public:

    /** Const that check for a rule definition */
    const char DEFINES='=';

    /**
     * Creates the parser with the given file
     */
    Parser(std::ifstream &file);
    
    /**
     * Read the created file into recursive number, axiom and rules
     */
    void read();

    // Getter
    std::vector<char> getAxiom();
    std::vector<Rule> getRules();
    int getRecursiveLevel();
    float getRotation();

private:

    /**
     * Reads the rule from a line
     */
    Rule readRule(std::string line);
    
    std::vector<char> m_axiom;
    std::vector<Rule> m_rules;
    int m_recursiveLevel;
    float m_rotation;

    std::ifstream &m_file;

};