// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <string>


std::vector<ImVec2> circles_pos;
std::vector<ImVec2> circles_dir;
std::vector<int> circles_radius;


// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Escopos
void draw_circle(ImVec2 pos, int radius, ImU32 color);
void draw_line(ImVec2 pos1, ImVec2 pos2, ImU32 color, int radius);
void move_circles();
void draw_circles_and_lines(ImU32 color);
void setup_circles();


int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(GetConsoleWindow(), NULL);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool testbool = false;
    int inttest = 10;
    float floattest = 5.5f;
    float size = 30.f;
    bool logged = false;
    bool fov = false;



    // Main loop
    bool done = false;
    bool enableParticles = true;
    setup_circles();
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;


        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();



        {
            // Defina o tamanho inicial como o tamanho da tela
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)1680, (float)1050));

            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = 1.5f;

            ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse); // Create a window called "Login" and append into it.

            draw_circles_and_lines(IM_COL32(255, 0, 255, 255));


            // Get window size
            ImVec2 windowSize = ImGui::GetWindowSize();

            // Calculate text size
            ImVec2 textSize = ImGui::CalcTextSize("This is some useful text.");

            // Calculate position to center text
            ImVec2 textPos((windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f);

            // Set cursor position to center the text
            ImGui::SetCursorPos(textPos);

            // Centralizar o texto "Login" na janela
            ImVec2 titleSize = ImGui::CalcTextSize("Login");
            ImVec2 titlePos((windowSize.x - titleSize.x) * 0.5f, 50.0f);
            ImGui::SetCursorPos(titlePos);
            ImGui::Text("Login");

            // Campos de entrada para nome de usuário e senha
            static char username[128] = "Kiteyy";
            static char password[128] = "1234";

            float toDivide = 3.3f;
            ImGui::SetCursorPosY(150.0f);
            ImGui::SetCursorPosX(windowSize.x / toDivide);
            ImGui::Text("Nome de Usuário:");
            ImGui::SetCursorPosX(windowSize.x / toDivide);
            ImGui::SetNextItemWidth(450);
            ImGui::InputText("##username", username, IM_ARRAYSIZE(username));

            ImGui::SetCursorPosY(200.0f);
            ImGui::SetCursorPosX(windowSize.x / toDivide);
            ImGui::Text("Senha:");
            ImGui::SetCursorPosX(windowSize.x / toDivide);
            ImGui::SetNextItemWidth(450);
            ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

            // Botão "Entrar" com cor de fundo azul
            ImVec4 buttonColor = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
            ImGui::SetCursorPosY(260.0f);
            ImGui::SetCursorPosX(windowSize.x / toDivide);
            if (ImGui::Button("Entrar", ImVec2(450, 40.0f))) {

                logged = true;
                // Lógica de autenticação aqui...
                // Por exemplo, verificar o nome de usuário e senha inseridos
            }
            ImGui::PopStyleColor();

            ImGui::End();
        }


        if (GetAsyncKeyState(VK_DELETE) & 1)
            logged = !logged;

        if (logged)
        {

            ImGui::SetNextWindowSize(ImVec2(800, 300));
            if (ImGui::Begin("Inject"))
            {
                ImGui::Checkbox("Aimbot", &fov);
                ImGui::Checkbox("Silent", &testbool);
                if (ImGui::Button("Ativar Aimbot"))
                {

                    fov = true;
                }

                ImGui::SliderFloat("Fov_Aimbot", &size, 0, 150, "%.3f");


               



                if (fov) {

                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 300));

                    ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x, 400));
                    if (ImGui::Begin("##stfuffdfs", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar)) {

                        auto draw = ImGui::GetWindowDrawList();
                        draw->AddCircle(ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2)), size, IM_COL32(255, 0, 0, 255), 500, 5.0f);
                    }ImGui::End();
                }


            }ImGui::End();

        }



        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
UINT g_WindowWidth = 1280;
UINT g_WindowHeight = 800;
// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_WindowWidth = (UINT)LOWORD(lParam);
        g_WindowHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


void draw_circle(ImVec2 pos, int radius, ImU32 color)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    unsigned char* color_ptr = (unsigned char*)&color;
    draw_list->AddCircleFilled(pos, radius - 1, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], 75));
}

void draw_line(ImVec2 pos1, ImVec2 pos2, ImU32 color, int radius)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float distance = std::sqrt(
        std::pow(pos2.x - pos1.x, 2) + // std:: pow
        std::pow(pos2.y - pos1.y, 2)    // std:: pow
    );
    float alpha;
    if (distance <= 20.0f) {
        alpha = 255.0f;
    }
    else {
        alpha = (1.0f - ((distance - 20.0f) / 25.0f)) * 255.0f;
    }

    int r = (color & 0xFF0000) >> 16; // Extract red component
    int g = (color & 0x00FF00) >> 8;  // Extract green component
    int b = (color & 0x0000FF);       // Extract blue component

    unsigned char* color_ptr = (unsigned char*)&color;

    draw_list->AddLine(pos1, pos2, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], static_cast<int>(alpha)), 1.0f);
    if (distance >= 40.0f) {
        draw_list->AddCircleFilled(pos1, radius - 0.96f, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius - 0.96f, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
    else if (distance <= 20.0f) {
        draw_list->AddCircleFilled(pos1, radius, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
    else {
        float radius_factor = 1.0f - ((distance - 20.0f) / 20.0f);
        float offset_factor = 1.0f - radius_factor;
        float offset = (radius - radius * radius_factor) * offset_factor;
        draw_list->AddCircleFilled(pos1, radius - offset, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
        draw_list->AddCircleFilled(pos2, radius - offset, IM_COL32(color_ptr[0], color_ptr[1], color_ptr[2], alpha * 200.0f));
    }
}

void move_circles()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 360);

    for (int i = 0; i < circles_pos.size(); i++)
    {
        ImVec2& pos = circles_pos[i];
        ImVec2& dir = circles_dir[i];
        int radius = circles_radius[i];

        pos.x += dir.x * 0.4;
        pos.y += dir.y * 0.4;

        if (pos.x - radius < 0 || pos.x + radius > ImGui::GetWindowWidth())
        {
            dir.x = -dir.x;
            dir.y = dis(gen) % 2 == 0 ? -1 : 1;
        }

        if (pos.y - radius < 0 || pos.y + radius > ImGui::GetWindowHeight())
        {
            dir.y = -dir.y;
            dir.x = dis(gen) % 2 == 0 ? -1 : 1;
        }
    }
}

void draw_circles_and_lines(ImU32 color)
{
    move_circles();

    for (int i = 0; i < circles_pos.size(); i++)
    {
        draw_circle(circles_pos[i], circles_radius[i], color);

        for (int j = i + 1; j < circles_pos.size(); j++)
        {
            float distance = ImGui::GetIO().FontGlobalScale * std::sqrt(
                std::pow(circles_pos[j].x - circles_pos[i].x, 2) + // std:: pow
                std::pow(circles_pos[j].y - circles_pos[i].y, 2)    // std:: pow
            );

            if (distance <= 45.0f)
            {
                draw_line(circles_pos[i], circles_pos[j], color, circles_radius[i]);
            }
        }
    }
}

void setup_circles()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::uniform_int_distribution<> pos_dis(0, static_cast<int>(1980));
    std::uniform_int_distribution<> pos_dis_y(0, static_cast<int>(1080));

    for (int i = 0; i < 400; i++) // 1000 is the number of cricle you render i recommend 400.
    {
        circles_pos.push_back(ImVec2(pos_dis(gen), pos_dis_y(gen)));
        circles_dir.push_back(ImVec2(dis(gen) == 0 ? -1 : 1, dis(gen) == 0 ? -1 : 1));
        circles_radius.push_back(3);
    }
}
