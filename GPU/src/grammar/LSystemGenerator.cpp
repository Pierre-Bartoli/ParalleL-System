#include "LSystemGenerator.hpp"
#include <iostream>
#include <Kokkos_Printf.hpp>

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

void LSystemGenerator::initialize(std::vector<char> axiom, std::vector<Rule> rules, int recursiveLevel)
{
    // Recursive level
    m_recursiveLevel = recursiveLevel;

    // Creates an index rule where a char = index 
    // and for each rules index this gives number of modules
    m_ruleIndex       = Kokkos::View<int*> ("ruleIndex", 256);
    m_ruleSize        = Kokkos::View<int*> ("ruleSize", rules.size());
    m_successorOffset = Kokkos::View<int*> ("successorOffset", rules.size());
    auto h_ruleIndex       = Kokkos::create_mirror_view(m_ruleIndex);
    auto h_ruleSize        = Kokkos::create_mirror_view(m_ruleSize);
    auto h_successorOffset = Kokkos::create_mirror_view(m_successorOffset);

    // Fill with -1 if the rule doesn't exist
    for (int i = 0; i < 256; i++)
    {
        h_ruleIndex(i) = -1;
    }

    int totalSuccessorsSize = 0;
    std::vector<char> totalChars;

    // For each existing rule index it and give it number of modules
    // Constructs also the vector of all concanated char's rules
    // Consturcts the offset of each starting rule
    for (int i = 0; i < rules.size(); i++)
    {
        // Converts char to 0 -> 256 unsigned char
        const unsigned char symbol = static_cast<unsigned char>(rules[i].getPredecessor());
        h_ruleIndex(symbol) = i;
        h_ruleSize(i) = rules[i].getSuccessorSize();
        
        // Count the number of symbol of all 
        // put all char in a vector
        totalSuccessorsSize += rules[i].getSuccessorSize();
        for (int j = 0; j < rules[i].getSuccessorSize(); j++)
        {
            totalChars.push_back(rules[i].getSuccessor()[j]);
        }

        // Gives the offset of succesors of the ruleID
        if (i > 0)
            h_successorOffset(i) = h_successorOffset(i - 1) + rules[i - 1].getSuccessorSize();
        else
            h_successorOffset(i) = 0;

    }

    // Put it in successors, it so contains all successor's rules
    m_successors = Kokkos::View<char*> ("successors", totalSuccessorsSize);
    auto h_successors = Kokkos::create_mirror_view(m_successors);
    for (int i = 0; i < totalSuccessorsSize; i++)
    {
        h_successors(i) = totalChars[i];
    }
    
    // Fill the current line with starting point axiom
    m_currentSize = axiom.size();
    m_currentLine = Kokkos::View<char*> ("currentLine", m_currentSize);
    auto h_currentLine = Kokkos::create_mirror_view(m_currentLine);
    for (int i = 0; i < axiom.size(); i++)
    {
        h_currentLine(i) = axiom[i];
    }

    //copy it into device
    Kokkos::deep_copy(m_ruleIndex, h_ruleIndex);
    Kokkos::deep_copy(m_ruleSize, h_ruleSize);
    Kokkos::deep_copy(m_currentLine, h_currentLine);
    Kokkos::deep_copy(m_successors, h_successors);
    Kokkos::deep_copy(m_successorOffset, h_successorOffset);

}

void LSystemGenerator::compute()
{

    Kokkos::View<int*> ruleIndex = m_ruleIndex;
    Kokkos::View<int*> ruleSize = m_ruleSize;
    Kokkos::View<char*> successors = m_successors;
    Kokkos::View<int*> successorOffset = m_successorOffset;
    Kokkos::View<char*> currentLine = m_currentLine;
    
    // Create an offset when generate the sentance every succesors must know its place
    Kokkos::View<int*> offsets ("offsets", m_currentSize);

    // If the final sentance has bracket in it
    Kokkos::View<bool> hasBracket("hasBracket");
    
    // The line generated
    Kokkos::View<char*> newLine ("newLine", 0);

    // For each level of recursion compute newLine
    for (int n = 0; n < m_recursiveLevel; n++)
    {

        // Avoid recreating the offsets, just resize it at the current size
        Kokkos::resize(Kokkos::WithoutInitializing, offsets, m_currentSize);

        // For each predecessors put the successors size
        // And give 1 if the predeccesors has no successor
        Kokkos::parallel_for("FirstPass", m_currentSize, KOKKOS_LAMBDA(const int i)
        {
            const unsigned char symbol = static_cast<unsigned char>(currentLine(i));
            const int ruleID = ruleIndex(symbol);

            if (ruleID >= 0){
                offsets(i) = ruleSize(ruleID);
            }
            else{
                offsets(i) = 1;
            }

        });

        // then scan to attribute the offset and the final size
        int newSize = 0;
        Kokkos::parallel_scan("Scan", m_currentSize, KOKKOS_LAMBDA (const int i, int& update, const bool final) {
            const int value = offsets(i);

            if(final)
                offsets(i) = update;

            update += value; 

        }, newSize);

        // Resize the new line at the given final size
        Kokkos::resize(Kokkos::WithoutInitializing, newLine, newSize);

        // Then remplace the precessors with the successors at give offset
        Kokkos::parallel_for("Rewrite", m_currentSize, KOKKOS_LAMBDA (const int i){

            const unsigned char symbol = static_cast<unsigned char>(currentLine(i));
            const int ruleID = ruleIndex(symbol);

            int offset = offsets(i);

            if (ruleID < 0){
                newLine(offset) = symbol;
            }

            else{
                int count = ruleSize(ruleID);
                int successorBegin = successorOffset(ruleID);
                for (int j = 0; j < count; j++)
                {
                    newLine(offset + j) = successors(successorBegin + j);

                    if(!hasBracket()) // compute if the final sentance has bracket
                        hasBracket() = successors(successorBegin + j) == '[' or successors(successorBegin + j) == ']';
                }
            }
        });

        m_currentSize = newSize;
        Kokkos::kokkos_swap(currentLine, newLine);
        
    }

    // has bracket or not
    auto h_hasBracket = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, hasBracket);
    m_hasBracket = h_hasBracket();

    // Then store the current line into attribut to get it
    m_currentLine = currentLine;

}

void LSystemGenerator::toString(bool printDebug)
{
    if(!printDebug)
        return;

    auto h_currentLine = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, m_currentLine);
    for (int i = 0; i < h_currentLine.extent(0); i++)
    {
        std::cout << h_currentLine(i);
    }
}

Kokkos::View<char*> LSystemGenerator::getComputedSentance()
{
    return m_currentLine;
}

bool LSystemGenerator::hasBracket()
{
    return m_hasBracket;
}