#include "Application.hpp"
#include "Core/Config.hpp"
#include "Debug/DebugMenu.hpp"
#include "Debug/Log.hpp"
#include "Debug/Switches.hpp"
#include "Input/Input.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/GraphRenderer.hpp"
#include "Script/Lua.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

Application::Application()
{
    core::g_Config.load();

    setupSDL();
    setupGL();
    setupImGui();
    setupSystems();
    setRelativeMouseMode(m_relativeMouse);

    Lua::checkInitialization();

    m_stateStack.push(State::Menu);
    // m_stateStack.push(State::Playing);
    // m_stateStack.push(State::Loading);
    // m_stateStack.push(State::Debug);
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Application::setupSDL()
{
    if (SDL_Init(SDL_INIT_EVENTS))
    {
        Log::error("SDL_Init failed! %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    u32 flags = SDL_WINDOW_OPENGL;

    auto display = core::g_Config.getDisplay();
    if (display.fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    else
        flags |= SDL_WINDOW_SHOWN;

    m_window = SDL_CreateWindow(
        "Tower",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        display.width, display.height,
        flags
    );

    if (!m_window)
    {
        Log::error("Window could not be created! %s\n", SDL_GetError());
        SDL_ClearError();
        exit(EXIT_FAILURE);
    }

    m_context = SDL_GL_CreateContext(m_window);
}

void Application::setupGL()
{
    GLenum error = glewInit();

    if(error != GLEW_OK)
    {
        Log::error("Could not initialize GLEW\n");
        exit(EXIT_FAILURE);
    }

    GL(glClearColor(0,0,0,1));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glEnable(GL_CULL_FACE));
    GL(glCullFace(GL_BACK));
    GL(glEnable(GL_TEXTURE_2D));
    GL(glEnable(GL_TEXTURE_3D));
    GL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
}

void Application::setupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.MouseDrawCursor = true;

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();
}

void Application::setupSystems()
{
    gfx::g_MatMgr.loadMaterials();
    gfx::g_GraphRenderer.init();
    gfx::g_Renderer2D.init();
    aud::g_AudioSys.init();

    gInput.init();

    anim::g_AnimSys.init();
    phys::g_PhysSys.init();
}

void Application::processEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        m_running = false;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_F4)
        {
            m_running = false;
        }
    }

    if (m_imguiWantsKeyboard)
        SDL_StartTextInput();

    ImGui_ImplSDL2_ProcessEvent(&m_event);

    if (!m_imguiWantsKeyboard)
    {
        SDL_StopTextInput();
    }
}

Event Application::convertEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_w:
                return Event(Event::ButtonPressed, Button::K_W);
                break;
            case SDLK_s:
                return Event(Event::ButtonPressed, Button::K_S);
                break;
            case SDLK_a:
                return Event(Event::ButtonPressed, Button::K_A);
                break;
            case SDLK_d:
                return Event(Event::ButtonPressed, Button::K_D);
                break;
            case SDLK_e:
                return Event(Event::ButtonPressed, Button::K_E);
                break;
            case SDLK_f:
                return Event(Event::ButtonPressed, Button::K_F);
                break;
            case SDLK_TAB:
                return Event(Event::ButtonPressed, Button::K_Tab);
                break;
            case SDLK_F1:
                return Event(Event::ButtonPressed, Button::K_F1);
                break;
            case SDLK_F2:
                return Event(Event::ButtonPressed, Button::K_F2);
                break;
            case SDLK_F3:
                return Event(Event::ButtonPressed, Button::K_F3);
                break;
            case SDLK_F4:
                return Event(Event::ButtonPressed, Button::K_F4);
                break;
        }

        return Event(Event::Invalid);
    }

    return Event(Event::Invalid);
}

void Application::processClientEvent(const Event& event)
{
    if (event.type == Event::ButtonPressed)
    {
        switch (event.button)
        {
            case Button::K_F1:
                m_relativeMouse = !m_relativeMouse;
                setRelativeMouseMode(m_relativeMouse);
                debug::g_Menu.toggle();
                break;
            case Button::K_F2:
                DebugSwitches::drawPhysX = !DebugSwitches::drawPhysX;
                break;

            case Button::K_F3:
                DebugSwitches::drawWaynet = !DebugSwitches::drawWaynet;
                break;
        }
    }

    m_stateStack.sendSystemEvent(event);
}

void Application::earlyUpdate()
{
    m_stateStack.update();
}

void Application::lateUpdate()
{
    m_stateStack.lateUpdate();

    if (DebugSwitches::drawPhysX)
        phys::g_PhysSys.debugDraw();
}

void Application::run()
{
    while (m_running)
    {
        while (SDL_PollEvent(&m_event))
        {
            processEvent(m_event);
            processClientEvent(convertEvent(m_event));
        }

        core::g_FInfo.delta = m_timer.reset();
        core::g_FInfo.globalTime += core::g_FInfo.delta;

        gInput.update();

        phys::g_PhysSys.preSimulationUpdate();

        earlyUpdate();

        phys::g_PhysSys.stepSimulation();

        lateUpdate();
        anim::g_AnimSys.animate();

        gfx::g_Renderer2D.beginFrame();
        m_stateStack.draw();
        gfx::g_Renderer2D.endFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();

        m_stateStack.drawGui();

        debug::g_Menu.drawGui();

        ImGui::Render();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        m_imguiWantsKeyboard = io.WantCaptureKeyboard;

        SDL_GL_MakeCurrent(m_window, m_context);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window);
        core::g_FInfo.globalFrame++;
    }
}

void Application::setRelativeMouseMode(bool val)
{
    SDL_SetRelativeMouseMode(val ? SDL_TRUE : SDL_FALSE);
}
