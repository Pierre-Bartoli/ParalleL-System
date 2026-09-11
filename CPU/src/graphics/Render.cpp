#include <Render.hpp>

Render::Render(Turtle& turtle, float step, float rotation):m_turtle(turtle)
{
    defineCommands(step, rotation);
}

void Render::defineCommands(float step, float rotation){

    m_commands['F'] = std::make_unique<StepForward>();
    m_commands['+'] = std::make_unique<RotatePositive>();
    m_commands['-'] = std::make_unique<RotateNegative>();
    m_commands['['] = std::make_unique<savePosition>();
    m_commands[']'] = std::make_unique<usePosition>();

    for (auto& symbol : m_commands) {
        symbol.second->initDefault(step, rotation);
    }
}

void Render::interpret(std::vector<char> sentance){

    for (char symbol : sentance)
    {
        auto it = m_commands.find(symbol);

        if (it != m_commands.end()) {
            it->second->execute(m_turtle);
        }
    }
    
}