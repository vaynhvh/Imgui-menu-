// dear imgui: standalone example application for DirectX 9
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "astriumwep.h"
#include "MuseoSansCyrl-300_0.h"
#include "MuseoSansCyrl-500_0.h"
#include "logo-menu.h"
#include <vector>
#include "avatar.h"
#include <map>
#include <imgui_internal.h>

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* LexendRegular;
ImFont* LexendLight;

ImFont* TabsFont;
ImFont* WeaponFont;
ImFont* MainFont;
IDirect3DTexture9* Logo;
IDirect3DTexture9* Bg;
IDirect3DTexture9* Avatar;
IDirect3DTexture9* Main;
IDirect3DTexture9* spectotr;
IDirect3DTexture9* keybinds;
IDirect3DTexture9* cloudy;

namespace menu
{
    static float switch_alpha[3], open_alpha = 0;
    static bool toggled = true;
    static int m_tabs;
    static int enemy_tab;

    void RenderSubTab()
    {
        if (m_tabs == 0)
        {
            ImGui::SetCursorPos(ImVec2{ 90, 67 });
            if (ImGui::Renderingsubtab("   Main", !enemy_tab)) enemy_tab = 0;

            ImGui::SetCursorPos(ImVec2{ 145, 67 });
            if (ImGui::Renderingsubtab("Weapon", enemy_tab == 1)) enemy_tab = 1;
        }
    }

    void RenderTab()
    {
        ImGui::SetCursorPos(ImVec2{ 33, 427 });
        ImGui::Image(Avatar, ImVec2(40, 40));//steam

        ImGui::PushFont(TabsFont);
        {
            ImGui::SetCursorPos(ImVec2{ 36, 100 });
            if (ImGui::Renderingtab("J", !m_tabs)) m_tabs = 0;

            ImGui::SetCursorPos(ImVec2{ 36, 151 });
            if (ImGui::Renderingtab("B", m_tabs == 1)) m_tabs = 1; // -

            ImGui::SetCursorPos(ImVec2{ 36, 207 });
            if (ImGui::Renderingtab("G", m_tabs == 2)) m_tabs = 2;

            ImGui::SetCursorPos(ImVec2{ 36, 260 });
            if (ImGui::Renderingtab("F", m_tabs == 3)) m_tabs = 3;

            ImGui::SetCursorPos(ImVec2{ 36, 313 });
            if (ImGui::Renderingtab("S", m_tabs == 4)) m_tabs = 4;

            ImGui::SetCursorPos(ImVec2{ 36, 366 });
            if (ImGui::Renderingtab("A", m_tabs == 5)) m_tabs = 5;

        }
        ImGui::PopFont();
    }

    void RenderRagetab()
    {
        if (enemy_tab == 0)
        {
            ImGui::SetCursorPos(ImVec2{ 125, 30 });
            ImGui::Image(Main, ImVec2(190, 40));//steam

            auto p = ImGui::GetWindowPos();
            ImGui::GetWindowDrawList()->AddText(TabsFont, 25, ImVec2(p.x + 100, p.y + 40), ImColor(96, 96, 96, int(255 * ImGui::GetStyle().Alpha)), "J");

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
            ImGui::SetCursorPos(ImVec2{ 100, 115 });
            {
                ImGui::MenuChild("General", ImVec2(240, 180), true);
                {
                    ImGui::Spacing(); ImGui::Spacing();

                    static bool active = true;
                    static bool inactive = false;
                    static int procent = 60;
                  
                    static KeyBind_t bind;
                
                    static float color[4] = { 1.f, 1.f, 1.f, 1.f };

                    ImGui::Checkbox("Active checkbox", &active);
                    ImGui::SameLine();
                    ImGui::KeyBind("##bind", &bind);

                    ImGui::Checkbox("Inactive checkbox", &inactive);
                    ImGui::Spacing();
                    ImGui::SliderInt("Slider", &procent, 0, 100, "%d%%");

                    ImGui::Spacing();
                    ImGui::ColorEdit4("Colorpicker", color);
                }
                ImGui::EndChild();

                ImGui::SameLine(0, 15);
                ImGui::MenuChild("Accuracy", ImVec2(240, 180), true);
                {
                    ImGui::Spacing(); ImGui::Spacing();

                    const char* type[] = { "Type", "Types", "Select" };
                    const char* types[] = { "Type 1", "Type 2", "Type 3", "Type 4", "Type 5", "Type 6" };
                    static bool multicombo[IM_ARRAYSIZE(types)];
                    static int combo;

                    static bool active = true;
                    ImGui::Checkbox("Active checkbox", &active);
                    ImGui::Spacing();

                    ImGui::Combo("Combo", &combo, type, IM_ARRAYSIZE(type));
                    ImGui::Spacing();
                    ImGui::MultiCombo("Multicombo", multicombo, types, IM_ARRAYSIZE(types));
                }
                ImGui::EndChild();
            }

            ImGui::SetCursorPos(ImVec2{ 100, 310 });
            {
                ImGui::MenuChild("Other", ImVec2(240, 150), true);
                {
                }
                ImGui::EndChild();

                ImGui::SameLine(0, 15);
                ImGui::MenuChild("Main", ImVec2(240, 150), true);
                {

                }
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
    }

    void Decoration()
    {
        ImGui::Image(Bg, ImVec2(634, 500));//bg

        auto p = ImGui::GetWindowPos();
        ImGui::GetWindowDrawList()->AddText(LexendRegular, 25, ImVec2(p.x + 475, p.y + 75), ImColor(96, 96, 96, int(255 * ImGui::GetStyle().Alpha)), "AUG  25, 13:32"); //data loader
    }

    void Render()
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f)
        {
            if (toggled && open_alpha > 0.8f || !toggled)
                switch_alpha[0] = ImClamp(switch_alpha[0] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

            ImGui::SetNextWindowSize(ImVec2(615, 480));
            ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("begin", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();
                RenderTab();
                

                if (switch_alpha[0] > 0.01f)
                {
                    if (toggled && switch_alpha[0] > 0.8f || !toggled)
                        switch_alpha[1] = ImClamp(switch_alpha[1] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
                    RenderSubTab();
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                    if (switch_alpha[1] > 0.01f)
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[1]);
                        ImGui::SetCursorPos(ImVec2(130, 60));
                        ImGui::PushFont(MainFont);
                        ImGui::BeginGroup();
                        {
                            switch (m_tabs)
                            {
                            case 0: RenderRagetab(); break;
                            case 1: break;
                            case 2: break;
                            case 3: break;
                            case 4: break;
                            }
                        }
                        ImGui::EndGroup();
                        ImGui::PopFont();
                        ImGui::PopStyleVar();
                    }
                    ImGui::PopStyleVar(2);
                }
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
    }

    void CreateStyle()
    {
        auto& style = ImGui::GetStyle();
        auto& color = style.Colors;

        style.FramePadding = ImVec2(1, 0);
        style.FrameRounding = 3;
        style.WindowBorderSize = 0;
        style.WindowPadding = ImVec2(5, 5);
        style.ScrollbarRounding = 3;
        style.ScrollbarSize = 5;
    }
}

namespace keybindsds
{
    static float open_alpha = 0;
    static bool toggled = true;
    static float spacing;
    std::vector<std::pair<std::string, std::string>> vec = {
        {"Double tap", "[toggle]"},
        {"Auto peek", "[hold]"},
        {" ","  "},
    };
    void BindsList()
    {
        auto d = ImGui::GetWindowDrawList();
        auto p = ImGui::GetWindowPos();
        auto s = ImGui::GetWindowSize();
        auto style = ImGui::GetStyle();

        for (int i = 0; i < int(vec.size()); i++)
        {
            auto keybind = vec.at(i);
            d->AddText(LexendLight, 16, ImVec2(p.x + 15, p.y + 30 + (spacing * i)), ImColor(255, 255, 255, int(255 * open_alpha)), keybind.first.c_str());

            auto textSize = LexendLight->CalcTextSizeA(16, FLT_MAX, NULL, keybind.second.c_str());
            d->AddText(LexendLight, 16, ImVec2(p.x + s.x - 25 - textSize.x, p.y + 30 + (spacing * i)), ImColor(255, 255, 255, int(255 * open_alpha)), keybind.second.c_str());
        }
    }

    void Decoration()
    {
        ImGui::Image(keybinds, ImVec2(170, 30));//bg
    }

    void Render()
    {
        spacing = LexendLight->FontSize + 2;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 0.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f && toggled) {
            ImGui::SetNextWindowSize(ImVec2(180, 45 + (spacing * vec.size())));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 150, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Binds", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();
                BindsList();
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
    }
}

namespace clouddy
{
    static float open_alpha = 0;
    static bool toggled = true;

    void Decoration()
    {
        ImGui::Image(cloudy, ImVec2(230, 100));//bg
    }

    void Render()
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 0.5f : -1.5f)), 0.f, 1.f);

        if (open_alpha > 0.01f)
        {
            ImGui::SetNextWindowSize(ImVec2(240, 100));
            ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y / 2 - 100), ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("ded", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
    }
}

namespace spectators
{
    static float open_alpha = 0;
    static bool toggled = true;
    static float spacing;
    std::vector<std::string> vec = {
        "Gabe",
        "Mike",
        " ",
    };

    void SpectatorsList()
    {
        auto d = ImGui::GetWindowDrawList();
        auto p = ImGui::GetWindowPos();
        auto s = ImGui::GetWindowSize();

        for (int i = 0; i < int(vec.size()); i++)
        {
            auto spectator = vec.at(i);
            auto textSize = LexendLight->CalcTextSizeA(17, FLT_MAX, NULL, spectator.c_str());
            d->AddText(LexendLight, 17, ImVec2(p.x + 15, p.y + 30 + (spacing * i)), ImColor(255, 255, 255, int(255 * open_alpha)), spectator.c_str());
        }
    }

    void Decoration()
    {
        ImGui::Image(spectotr, ImVec2(170, 30));//bg
    }

    void Render()
    {
        spacing = LexendLight->FontSize + 2;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            toggled = !toggled;

        open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 0.5f : -1.5f)), 0.f, 1.f);
        if (open_alpha > 0.01f) {
            ImGui::SetNextWindowSize(ImVec2(180, 45 + (spacing * vec.size())));
            ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y / 2 - 100), ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, open_alpha);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Spectators", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
            {
                Decoration();
                SpectatorsList();
            }
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
    }
}


// Main code
int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    menu::CreateStyle();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, logomenu, sizeof(logomenu), 92.52f, 77.66f, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Logo);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, bgs, sizeof(bgs), 634, 500, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Bg);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, avatars, sizeof(avatars), 40, 40, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Avatar);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, Maineny, sizeof(Maineny), 190, 40, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Main);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, spectotrors, sizeof(spectotrors), 250, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &spectotr);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, keybi, sizeof(keybi), 250, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &keybinds);
    D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, welcoud, sizeof(welcoud), 250, 200, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &cloudy);


    // Load Fonts
    MainFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl3000, sizeof(MuseoSansCyrl3000), 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
    TabsFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl5000, sizeof(MuseoSansCyrl5000), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    WeaponFont = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    LexendRegular = io.Fonts->AddFontFromMemoryTTF(LexendLightfont, sizeof(LexendLightfont), 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
    LexendLight = io.Fonts->AddFontFromMemoryTTF(LexendLightfont, sizeof(LexendLightfont), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        menu::Render();  //Renderin
        spectators::Render();
        keybindsds::Render();
        clouddy::Render();

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}



// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
