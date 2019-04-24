#pragma once
#include "State.hpp"
#include <vector>

class StateStack
{
public:
    StateStack() = default;
    ~StateStack() = default;

    void sendSystemEvent(const Event& event);
    void update();
    void lateUpdate();
    void draw();
    void drawGui();

    void push(State::Type type);
    void pop();
    void clear();

    bool isEmpty() const;

private:
    void executeCommands();
    State::Ptr createState(State::Type type);

private:
    enum class CommandType
    {
        Push,
        Pop,
        Clear
    };

    struct Command
    {
        CommandType type;
        State::Type state;

        Command(CommandType t, State::Type s = State::Count)
            : type(t), state(s)
        {}
    };

    std::vector<State::Ptr> m_stack;
    std::vector<Command> m_commands;
};
