#pragma once

#include <vector>
#include <string>

/**
 * Manages the rules of the L-system, 
 * the rule is made : 
 * - of read symbol that is the left and side of the equation, it describe the action that will be remplaced
 * - of the write symbol that is the right and side equation, it gives the list of actions that will be remplaced
 */
class Rule{
public:
    /** Creates a Rule made by a predecessor symbol and successor list of symbols
     * @param predecessor that indicate which letter is remplaced
     * @param successor the letters remplaced
     */
    Rule(char predecessor, std::vector<char> successor);

    //Getters
    char getPredecessor();
    std::vector<char> getSuccessor();
    
private:
    /** The symbol that is remplaced */
    char m_predecessor;
    /** The list of symobls that is remplaced */
    std::vector<char> m_successor;
};