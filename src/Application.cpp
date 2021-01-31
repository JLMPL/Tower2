#include "Application.hpp"
#include "Animation/Animation.hpp"
#include "Core/Config.hpp"
#include "Core/FrameInfo.hpp"
#include "Core/Timer.hpp"
#include "Debug/DebugMenu.hpp"
#include "Debug/Log.hpp"
#include "Events/Event.hpp"
#include "Input/Input.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Render/MaterialManager.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/SceneRenderer.hpp"
#include "Render/Geometry/Geometry.hpp"
#include "Gameplay/Level.hpp"
#include <SDL2/SDL.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

SDL_Window*        l_window = nullptr;
SDL_GLContext      l_context;
SDL_Event          l_event;

core::Timer        l_timer;
std::unique_ptr<Level> m_level;

bool               l_relativeMouse = true;
bool               l_running = true;
bool               l_captureMouse = true;
bool               l_imguiWantsKeyboard = false;

LOCAL void setupSDL()
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

    auto display = core::getDisplayConfig();
    if (display.fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    else
        flags |= SDL_WINDOW_SHOWN;

    l_window = SDL_CreateWindow(
        "Tower",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        display.width, display.height,
        flags
    );

    if (!l_window)
    {
        Log::error("Window could not be created! %s\n", SDL_GetError());
        SDL_ClearError();
        exit(EXIT_FAILURE);
    }

    l_context = SDL_GL_CreateContext(l_window);
}

LOCAL void setupGL()
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

    gfx::initScreenQuad();
}

LOCAL void setupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.MouseDrawCursor = true;

    ImGui_ImplSDL2_InitForOpenGL(l_window, l_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();
}

LOCAL void setupSystems()
{
    gfx::g_MatMgr.loadMaterials();
    gfx::g_SceneRenderer.init();
    gfx::g_Renderer2D.init();
    // aud::g_AudioSys.init();

    gInput.init();
}

LOCAL void setRelativeMouseMode(bool val)
{
    SDL_SetRelativeMouseMode(val ? SDL_TRUE : SDL_FALSE);
}

LOCAL void init()
{
    core::loadConfigurationFile();

    setupSDL();
    setupGL();
    setupImGui();
    setupSystems();
    setRelativeMouseMode(l_relativeMouse);

    m_level = std::make_unique<Level>();
    m_level->initFromScript("Maps/Level0.lua");
}

LOCAL void shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(l_context);
    SDL_DestroyWindow(l_window);
    SDL_Quit();
}

LOCAL void processEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        l_running = false;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_F4)
        {
            l_running = false;
        }
    }

    if (l_imguiWantsKeyboard)
        SDL_StartTextInput();

    ImGui_ImplSDL2_ProcessEvent(&l_event);

    if (!l_imguiWantsKeyboard)
    {
        SDL_StopTextInput();
    }
}

LOCAL Event convertEvent(const SDL_Event& event)
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

LOCAL void processClientEvent(const Event& event)
{
    if (event.type == Event::ButtonPressed)
    {
        switch (event.button)
        {
            case Button::K_F1:
                l_relativeMouse = !l_relativeMouse;
                setRelativeMouseMode(l_relativeMouse);
                debug::g_Menu.toggle();
                break;
        }
    }
}

LOCAL void update()
{
    m_level->update();
}

void runApplication()
{
    init();

    while (l_running)
    {
        while (SDL_PollEvent(&l_event))
        {
            processEvent(l_event);
            processClientEvent(convertEvent(l_event));
        }

        core::g_FInfo.delta = resetTimer(l_timer);

        gInput.update();

        update();

        gfx::g_Renderer2D.beginFrame();
        m_level->draw();
        gfx::g_Renderer2D.endFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(l_window);
        ImGui::NewFrame();

        debug::g_Menu.drawGui();

        ImGui::Render();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        l_imguiWantsKeyboard = io.WantCaptureKeyboard;

        SDL_GL_MakeCurrent(l_window, l_context);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(l_window);
    }

    shutdown();
}
