#ifndef STD_PCH
#include "std.h"
#endif
#pragma hdrstop ( "build/std.h.pch" )

#define GLOBAL_STATE __declspec(dllexport)
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "math.cpp"
/* #include "input.h" */
/* #include "input.cpp" */



struct InputButton {
    bool pressed = false;
    bool released = false;
    bool down = false;
    bool repeat = false;
    i32 mods = 0;
};

InputButton keys[256];


LRESULT CALLBACK WindowProc(HWND hWnd, u32 message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            keys[(u32)wParam].down = true;
            keys[(u32)wParam].pressed = true;
            return 0;

        case WM_KEYUP:
            keys[(u32)wParam].down = false;
            keys[(u32)wParam].released = true;
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


i32 main(i32 argc, i8** argv) {
    AttachConsole(ATTACH_PARENT_PROCESS);
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );


    // init dx11

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, 640, 480};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND window = CreateWindowEx(NULL,
                             "WindowClass",
                             ":-)",
                             WS_OVERLAPPEDWINDOW,
                             300,
                             300,
                             wr.right - wr.left,
                             wr.bottom - wr.top,
                             NULL,
                             NULL,
                             GetModuleHandle(NULL),
                             NULL);

    ShowWindow(window, 1);



    // init swap chain

    DXGI_SWAP_CHAIN_DESC scd;
    ID3D11Device* device = 0;
    ID3D11DeviceContext* ctx = 0;
    IDXGISwapChain* swapchain = 0;
    ID3D11RenderTargetView* screen_rt = 0;
    ID3D11DepthStencilView* screen_depth = 0;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = window;
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = true;                                    // windowed/full-screen mode

    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &device,
                                  NULL,
                                  &ctx);

    ID3D11Texture2D *back_buffer = 0;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

    device->CreateRenderTargetView(back_buffer, NULL, &screen_rt);
    back_buffer->Release();

    ctx->OMSetRenderTargets(1, &screen_rt, NULL);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = 640;
    viewport.Height = 480;

    ctx->RSSetViewports(1, &viewport);




    // setup other stuffs

    // TODO update input.h/cpp to work on windows
    /* setup_input_callbacks(window); */


    // setup vbo stuffs
    ID3D11Buffer* vbo;
    {
        f32 vertices[] = {
            0.f,    0.5f,  0.f,         1.f, 0.f, 0.f, 1.f,
            0.45f,  -0.5f, 0.f,         0.f, 1.f, 0.f, 1.f,
            -0.45f, -0.5f, 0.f,         0.f, 0.f, 1.f, 1.f,
        };

        D3D11_BUFFER_DESC b;
        ZeroMemory(&b, sizeof(b));
        b.Usage = D3D11_USAGE_DEFAULT;
        b.ByteWidth = sizeof(f32)*7*3;
        b.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        b.CPUAccessFlags = 0;
        b.MiscFlags = 0;
        b.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = vertices;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        HRESULT res = device->CreateBuffer(&b, &data, &vbo);
        check(!FAILED(res));
    }


    // setup shader
    ID3D11VertexShader *VS;
    ID3D11PixelShader  *PS;
    ID3D11InputLayout  *attr_layout;
    {
        ID3D10Blob* _VS;
        ID3D10Blob* _PS;
        D3DX11CompileFromFile("triangle.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_VS, 0, 0);
        D3DX11CompileFromFile("triangle.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_PS, 0, 0);

        device->CreateVertexShader(_VS->GetBufferPointer(), _VS->GetBufferSize(), NULL, &VS);
        device->CreatePixelShader( _PS->GetBufferPointer(), _PS->GetBufferSize(), NULL, &PS);

        ctx->VSSetShader(VS, 0, 0);
        ctx->PSSetShader(PS, 0, 0);

        D3D11_INPUT_ELEMENT_DESC attr[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        device->CreateInputLayout(attr, 2, _VS->GetBufferPointer(), _VS->GetBufferSize(), &attr_layout);
        ctx->IASetInputLayout(attr_layout);
    }


    // main loop

    MSG msg;

    bool should_close = false;
    while(!should_close) {

        // reset input stuff TODO update input.h/cpp
        for(u32 i=0 ; i<256 ; i++) {
            keys[i].pressed = false;
            keys[i].released = false;
            keys[i].repeat = false;
            keys[i].mods = 0;
        }


        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT
                || keys[VK_ESCAPE].pressed)
            should_close = true;



        // render

        vec4f clear_color = {1.f, 0.f, 1.f, 1.0f};
        ctx->ClearRenderTargetView(screen_rt, &clear_color.x);
        /* ctx->ClearDepthStencilView(screen_depth, D3D11_CLEAR_DEPTH, 1.f, 0); */


        u32 stride = sizeof(f32)*7;
        u32 offset = 0;
        ctx->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ctx->Draw(3, 0);


        swapchain->Present(0, 0);
    }

    // cleanup
    printf("cleanup\n\n");
    DestroyWindow(window);

    return 0;
}
