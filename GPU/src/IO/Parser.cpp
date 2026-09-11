#include "Parser.hpp"
#include <iostream>

Parser::Parser(std::ifstream &file): m_file(file)
{}

void Parser::read()
{
    // Init before reading
    int lineNumber = 0;
    std::string line;

    if(m_file.is_open())
    {

        while (std::getline(m_file, line))
        {

            // First line is rotation
            if(lineNumber == 0)
            {
                m_rotation = std::stoi(line);
            }

            // Second recursvie depth
            if (lineNumber == 1)
            {
                m_recursiveLevel = std::stoi(line);
            }

            // Third the axiom
            if(lineNumber == 2)
            {
                for (int i = 0; i < line.size(); i++)
                {
                    m_axiom.push_back(line[i]);
                }
            }

            // Then for every other lines reads it with line reader
            if(lineNumber > 2)
            {
                m_rules.push_back(readRule(line));
            }

            lineNumber ++;

        }
        
        m_file.close();
        
    }

}

Rule Parser::readRule(std::string line)
{
    // Gets the first letter of the rule
    char predecessor = line[0];
    std::vector<char> succesor;

    // Gets the second part of the rule, after the DEFINES symbol
    for (int i = 1; i < line.size(); i++)
    {
        
        if(line[i] == DEFINES){
            i ++;
        }

        succesor.push_back(line[i]);

    }

    // Creates the rule & return it
    Rule rule(predecessor, succesor);
    return rule;

}

std::vector<char> Parser::getAxiom()
{
    return m_axiom;
}

std::vector<Rule> Parser::getRules()
{
    return m_rules;
}

int Parser::getRecursiveLevel()
{
    return m_recursiveLevel;
}

float Parser::getRotation()
{
    return m_rotation;
}