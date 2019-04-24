#pragma once
#include "Animation/AnimationSystem.hpp"
#include "Audio/Audio.hpp"
#include "Core/Config.hpp"
#include "Core/FrameInfo.hpp"
#include "Core/Timer.hpp"
#include "Debug/Console.hpp"
#include "Events/Event.hpp"
#include "Input/Input.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Render/Renderer2D.hpp"
#include "State/StateStack.hpp"
#include <SDL2/SDL.h>

class Application
{
public:
    Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    ~Application();

    void initialize();

    void processClientEvent(const Event& event);
    void earlyUpdate();
    void lateUpdate();

    void draw();
    void drawGui();

    void run();
    void setRelativeMouseMode(bool val);

private:
    void setupSDL();
    void setupGL();
    void setupImGui();
    void setupSystems();

    Event convertEvent(const SDL_Event& event);
    void processEvent(const SDL_Event& event);

protected:
    SDL_Window*        m_window = nullptr;
    SDL_GLContext      m_context;
    SDL_Event          m_event;

    core::Timer        m_timer;
    StateStack         m_stateStack;

    bool               m_relativeMouse = true;
    bool               m_running = true;
    bool               m_captureMouse = true;
    bool               m_imguiWantsKeyboard = false;
};
