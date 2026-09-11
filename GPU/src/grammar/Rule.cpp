#include "Rule.hpp"

Rule::Rule(char predecessor, std::vector<char> successor):m_predecessor(predecessor), m_successor(successor)
{}

char Rule::getPredecessor()
{
    return m_predecessor;
}

std::vector<char> Rule::getSuccessor()
{
    return m_successor;
}

int Rule::getSuccessorSize()
{
    return m_successor.size();
}