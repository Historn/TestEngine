#include "Application.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

bool ModuleUI::Init() 
{
    LOG("Creating IMGUI context");
    bool ret = true;

    return ret;
}

bool ModuleUI::Start() 
{
    LOG("Starting IMGUI");
    bool ret = true;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiContext* contextui = ImGui::GetCurrentContext();
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_ProcessEvent(&App->input->e);

    ImGuiWindowFlags flags = 0;

    return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
    if (show_guiwindow_1)
    {
        GUIWindow1();
    }

    return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
    LOG("Cleaning up IMGUI");
    bool ret = true;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return ret;
}

void ModuleUI::GUIWindow1() {
    ImGuiIO& io = ImGui::GetIO();

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Full Desktop", &full_desktop);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Minimize Window", &minimize_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}