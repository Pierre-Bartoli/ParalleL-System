#include <string>
#include <Command.hpp>
#include <memory>
#include <unordered_map>

class Render
{
public:
    /**
     * Create the render that use turtle
     */
    Render(Turtle& turtle, float step, float rotation);

    /**
     * Graphically interpret the L-system
     */
    void interpret(std::vector<char>);

private:

    /**
     * Define all posible commands
     */
    void defineCommands(float step, float rotation);

    std::vector<char> m_sentance;
    std::unordered_map<char, std::unique_ptr<Command>> m_commands;

    Turtle& m_turtle;

};