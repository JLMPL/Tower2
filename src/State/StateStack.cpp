#include "StateStack.hpp"
#include "PlayingState.hpp"
#include "LoadingState.hpp"
#include "MenuState.hpp"
#include "SplashState.hpp"

void StateStack::sendSystemEvent(const Event& event)
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); i++)
    {
        (*i)->sendSystemEvent(event);
    }
}

State::Ptr StateStack::createState(State::Type type)
{
    switch (type)
    {
        case State::Menu:
            return State::Ptr(new MenuState(*this));
            break;
        case State::Playing:
            return State::Ptr(new PlayingState(*this));
            break;
        case State::Loading:
            return State::Ptr(new LoadingState(*this));
            break;
        case State::Splash:
            return State::Ptr(new SplashState(*this));
            break;

        default: break;
    }
}

void StateStack::executeCommands()
{
    for (auto i = m_commands.begin(); i != m_commands.end(); i++)
    {
        auto& command = *i;

        switch (command.type)
        {
            case CommandType::Push:
                m_stack.push_back(createState(command.state));
                m_stack.back()->init();
                break;
            case CommandType::Pop:
                m_stack.pop_back();
                break;
            case CommandType::Clear:
                m_stack.clear();
                break;
        }
    }

    m_commands.clear();
}

void StateStack::update()
{
    executeCommands();

    for (auto i = m_stack.rbegin(); i != m_stack.rend(); i++)
    {
        (*i)->update();
    }
}

void StateStack::draw()
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); i++)
    {
        (*i)->draw();
    }
}

void StateStack::drawGui()
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); i++)
    {
        (*i)->drawGui();
    }
}

void StateStack::push(State::Type state)
{
    m_commands.push_back(Command(CommandType::Push, state));
}

void StateStack::pop()
{
    m_commands.push_back(Command(CommandType::Pop));
}

void StateStack::clear()
{
    m_commands.push_back(Command(CommandType::Clear));
}

bool StateStack::isEmpty() const
{
    return m_stack.empty();
}
