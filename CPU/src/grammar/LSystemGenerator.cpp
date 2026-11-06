#include "LSystemGenerator.hpp"
#include <iostream>

LSystemGenerator::LSystemGenerator(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel)
{
    initialize(axiom, rules, recursiveLevel);
}

LSystemGenerator::LSystemGenerator(Parser parser)
{
    
    std::vector<char> axiom = parser.getAxiom();
    std::vector<Rule> rules = parser.getRules();
    int recursiveLevel = parser.getRecursiveLevel();

    initialize(axiom, rules, recursiveLevel);
    
}

void LSystemGenerator::initialize(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel){
    
    // Set the starting point as the axiom
    for (int i = 0; i < axiom.size(); i++)
    {
        m_currentLine.push_back(axiom[i]);
    }

    // Fill the rules and recursive level
    m_rules = rules;
    m_recursiveLevel = recursiveLevel;

}

void LSystemGenerator::compute()
{
    evolve(m_rules, m_recursiveLevel);
}

void LSystemGenerator::evolve(std::vector<Rule> rules, int recursiveLevel)
{

    recursiveLevel--;

    if (recursiveLevel >= 0)
    {
        // Recall the fonction when doing recursive
        evolve(rules, recursiveLevel);

        // For each recursion write on a new line
        std::vector<char> newLine;

        // For each symbol check if the symbol is in a rule
        for(char symbol : m_currentLine)
        {
            
            // Checks if the symbol has no rule
            bool isNotARule = true;

            for (int i = 0; i < rules.size(); i++)
            {
                // If the symbol is a read symbol of a rule, 
                //remplace it by the entire write part of the rule
                if(rules[i].getPredecessor() == symbol)
                {
                    
                    for (int j = 0; j < rules[i].getSuccessor().size(); j++)
                    {
                        char tmpSymbol = rules[i].getSuccessor()[j];
                        newLine.push_back(tmpSymbol);
                    }
                    
                    // And the symbol is not added to new line
                    isNotARule = false;
                    
                }
                
            }

            // If not a rule just add it to new line
            if (isNotARule)
            {
                newLine.push_back(symbol);
            }
            
        }

        // Current becomes newline, to reinject it to the loop
        m_currentLine = newLine;

    }

}

void LSystemGenerator::toString(bool debugPrint)
{

    if(!debugPrint)
        return;
        
    std::vector<char> evolveSentance = getComputedSentance();

    for (int i = 0; i < evolveSentance.size(); i++)
    {
        std::cout << evolveSentance[i];
    }

    std::cout << std::endl;
}

std::vector<char> LSystemGenerator::getComputedSentance()
{
    return m_currentLine;
}