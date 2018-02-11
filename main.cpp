#ifndef STD_PCH
#include "std.h"
#endif
#pragma hdrstop ( "build/std.h.pch" )

#define GLOBAL_STATE __declspec(dllexport)
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "Windowsx.h"

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


struct Mouse {
    vec2i pos = vec2f::O;
    vec2i dpos = vec2f::O;

    InputButton left;
    InputButton right;
};

Mouse mouse;



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


        case WM_LBUTTONDOWN:
            mouse.left.down = true;
            mouse.left.pressed = true;
            return 0;

        case WM_RBUTTONDOWN:
            mouse.right.down = true;
            mouse.right.pressed = true;
            return 0;

        case WM_LBUTTONUP:
            mouse.left.down = false;
            mouse.left.released = true;
            return 0;

        case WM_RBUTTONUP:
            mouse.right.down = false;
            mouse.right.released = true;
            return 0;

        case WM_MOUSEMOVE:
            vec2i next = vec2i { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            mouse.dpos = next - mouse.pos;
            mouse.pos = next;
            return 0;

    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


i32 main(i32 argc, i8** argv) {
    /* AttachConsole(ATTACH_PARENT_PROCESS); */
/*     /1* AllocConsole(); *1/ */
    /* freopen("CONOUT$", "w", stdout); */
    /* freopen("CONOUT$", "w", stderr); */

    printf("hi!\n");


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

    RECT wr = {0, 0, 1280, 720};
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



    // init swap chain & debug layer

    DXGI_SWAP_CHAIN_DESC scd;
    ID3D11Device* device = 0;
    ID3D11DeviceContext* ctx = 0;
    IDXGISwapChain* swapchain = 0;
    ID3D11RenderTargetView* screen_rt = 0;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = window;
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = true;                                    // windowed/full-screen mode

    u32 creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL,
                                                D3D_DRIVER_TYPE_HARDWARE,
                                                NULL,
                                                creation_flags,
                                                NULL,
                                                NULL,
                                                D3D11_SDK_VERSION,
                                                &scd,
                                                &swapchain,
                                                &device,
                                                NULL,
                                                &ctx);
    check(!FAILED(res));

    ID3D11Debug* d3d_debug = NULL;
    bool debug_enabled = false;
    if(!FAILED(device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3d_debug))) {
        ID3D11InfoQueue* info_queue = NULL;
        if(!FAILED(d3d_debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&info_queue))) {
            debug_enabled = true;

            info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);
            info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_INFO, true);
            info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_MESSAGE, true);

#if 0
            D3D11_MESSAGE_ID hide[] = {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
            };

            D3D11_INFO_QUEUE_FILTER filter;
            memset(&filter, 0, sizeof(filter));
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            info_queue->AddStorageFilterEntries(&filter);
#endif
            info_queue->Release();
        }
        d3d_debug->Release();
    }
    check(debug_enabled);

    ID3D11Texture2D* back_buffer = 0;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

    res = device->CreateRenderTargetView(back_buffer, NULL, &screen_rt);
    check(!FAILED(res));
    back_buffer->Release();



    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = wr.right - wr.left;
    viewport.Height = wr.bottom - wr.top;

    ctx->RSSetViewports(1, &viewport);



    // setup depth test

    ID3D11Texture2D* depth_stencil_buffer;
    ID3D11DepthStencilView* depth_stencil_view;
    D3D11_TEXTURE2D_DESC tdesc;
    ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
    tdesc.ArraySize = 1;
    tdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tdesc.CPUAccessFlags = 0;
    tdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tdesc.Width = viewport.Width;
    tdesc.Height = viewport.Height;
    tdesc.MipLevels = 1;
    tdesc.SampleDesc.Count = 1;
    tdesc.SampleDesc.Quality = 0;
    tdesc.Usage = D3D11_USAGE_DEFAULT;

    res = device->CreateTexture2D(&tdesc, NULL, &depth_stencil_buffer);
    check(!FAILED(res));
    res = device->CreateDepthStencilView(depth_stencil_buffer, NULL, &depth_stencil_view);
    check(!FAILED(res));

    ID3D11DepthStencilState* depth_stencil_state;
    D3D11_DEPTH_STENCIL_DESC dsdesc;
    ZeroMemory(&dsdesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsdesc.DepthEnable = true;
    dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsdesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsdesc.StencilEnable = false;

    res = device->CreateDepthStencilState(&dsdesc, &depth_stencil_state);
    check(!FAILED(res));



    ctx->OMSetRenderTargets(1, &screen_rt, NULL);
    ctx->OMSetDepthStencilState(depth_stencil_state, 1);



    // rasterization state

    ID3D11RasterizerState* rast_state;
    D3D11_RASTERIZER_DESC rast_desc;

    rast_desc.AntialiasedLineEnable = false;
    rast_desc.CullMode = D3D11_CULL_BACK;
    rast_desc.DepthBias = 0;
    rast_desc.DepthBiasClamp = 0.f;
    rast_desc.DepthClipEnable = true;
    rast_desc.FillMode = D3D11_FILL_SOLID;
    rast_desc.FrontCounterClockwise = true;
    rast_desc.MultisampleEnable = false;
    rast_desc.ScissorEnable = false;
    rast_desc.SlopeScaledDepthBias = 0.f;

    res = device->CreateRasterizerState(&rast_desc, &rast_state);
    assert(!FAILED(res));




    // setup other stuffs

    // TODO update input.h/cpp to work on windows
    /* setup_input_callbacks(window); */


    // setup vbo stuffs
    ID3D11Buffer* vbo;

    /* f32 vertices[] = { */
    /*     0.f,    0.5f,  0.f,         1.f, 0.f, 0.f, 1.f, */
    /*     0.5f,  -0.5f, 0.f,         0.f, 1.f, 0.f, 1.f, */
    /*     -0.5f, -0.5f, 0.f,         0.f, 0.f, 1.f, 1.f, */
    /* }; */

    /* vec3f vertices[] = { */
    /*     {0.f, 0.f, 1.f}, {0.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, // Z+ */
    /*     {0.f, 0.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 0.f, 1.f}, */

    /*     {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 1.f, 0.f}, // Z- */
    /*     {0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, {1.f, 0.f, 0.f}, */

    /*     {1.f, 0.f, 0.f}, {1.f, 0.f, 1.f}, {1.f, 1.f, 1.f}, // X+ */
    /*     {1.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 0.f}, */

    /*     {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 1.f}, // X- */
    /*     {0.f, 0.f, 0.f}, {0.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, */

    /*     {0.f, 1.f, 0.f}, {0.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, // Y+ */
    /*     {0.f, 1.f, 0.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 0.f}, */

    /*     {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 1.f}, // Y+ */
    /*     {0.f, 0.f, 0.f}, {1.f, 0.f, 1.f}, {1.f, 0.f, 0.f}, */
    /* }; */

    f32 vertices[] = {
        0.f, 0.f, 0.f,     0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,     0.f, 0.f, 1.f,
        1.f, 1.f, 0.f,     0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,     0.f, 1.f, 1.f,

        0.f, 0.f, 1.f,     1.f, 0.f, 0.f,
        0.f, 1.f, 1.f,     1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,     1.f, 1.f, 0.f,
        1.f, 0.f, 1.f,     1.f, 1.f, 1.f,
    };

    u16 indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        4, 5, 1, 4, 1, 0,
        3, 2, 6, 3, 6, 7,
        1, 5, 6, 1, 6, 2,
        4, 0, 3, 4, 3, 7,
    };


    D3D11_BUFFER_DESC b;
    ZeroMemory(&b, sizeof(b));
    b.Usage = D3D11_USAGE_DEFAULT;
    b.ByteWidth = sizeof(vertices);
    b.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    b.CPUAccessFlags = 0;
    b.MiscFlags = 0;
    b.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = vertices;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    res = device->CreateBuffer(&b, &data, &vbo);
    check(!FAILED(res));



    // setup ebo

    ID3D11Buffer* ebo;
    D3D11_BUFFER_DESC ebo_desc;
    ZeroMemory(&ebo_desc, sizeof(D3D11_BUFFER_DESC));
    ebo_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ebo_desc.ByteWidth = sizeof(indices);
    ebo_desc.CPUAccessFlags = 0;
    ebo_desc.Usage = D3D11_USAGE_DEFAULT;

    data.pSysMem = indices;

    res = device->CreateBuffer(&ebo_desc, &data, &ebo);
    check(!FAILED(res));




    // setup shader

    ID3D11VertexShader *VS;
    ID3D11PixelShader  *PS;
    ID3D11InputLayout  *attr_layout;
    {
        ID3D10Blob* _VS;
        ID3D10Blob* _PS;
        ID3D10Blob* err;

        HRESULT vsres = D3DX11CompileFromFile("triangle.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_VS, &err, 0);
        if(FAILED(vsres)) {
            if(err != NULL)
                printf("vs compile error: %s\n", err->GetBufferPointer());
            unreachable();
        }

        HRESULT hsres = D3DX11CompileFromFile("triangle.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_PS, &err, 0);
        if(FAILED(hsres)) {
            if(err != NULL)
                printf("ps compile error: %s\n", err->GetBufferPointer());
            unreachable();
        }

        device->CreateVertexShader(_VS->GetBufferPointer(), _VS->GetBufferSize(), NULL, &VS);
        device->CreatePixelShader( _PS->GetBufferPointer(), _PS->GetBufferSize(), NULL, &PS);

        ctx->VSSetShader(VS, 0, 0);
        ctx->PSSetShader(PS, 0, 0);

        D3D11_INPUT_ELEMENT_DESC attr[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3*sizeof(f32), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        device->CreateInputLayout(attr, array_size(attr), _VS->GetBufferPointer(), _VS->GetBufferSize(), &attr_layout);
        ctx->IASetInputLayout(attr_layout);
    }



    // ubo
    D3D11_BUFFER_DESC ubo_desc;
    ZeroMemory(&ubo_desc, sizeof(D3D11_BUFFER_DESC));
    ubo_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ubo_desc.ByteWidth = sizeof(mat4);
    ubo_desc.CPUAccessFlags = 0;
    ubo_desc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Buffer* ubo;
    res = device->CreateBuffer(&ubo_desc, NULL, &ubo);
    check(!FAILED(res));

    ctx->VSSetConstantBuffers(0, 1, &ubo);


    mat4 proj = mat4::proj(0.1f, 200.f, 60.f*DEG_TO_RAD, (u32)viewport.Width, (u32)viewport.Height);
    /* mat4 proj = mat4::id(); */
    vec3f cam_pos = vec3f { 0.5f, 0.5f, 10.f };
    f32 rot_x = 0.f;
    f32 rot_y = 0.f;



    // main loop

    MSG msg;

    bool should_close = false;
    auto last_time = std::chrono::high_resolution_clock::now();
    while(!should_close) {
        auto time = std::chrono::high_resolution_clock::now();
        f32 dt = std::chrono::duration<f32, std::milli>(time-last_time).count();
        dt /= 1000.f;
        last_time = time;

        // reset input stuff TODO update input.h/cpp
        for(u32 i=0 ; i<256 ; i++) {
            keys[i].pressed = false;
            keys[i].released = false;
            keys[i].repeat = false;
            keys[i].mods = 0;
        }

        mouse.dpos = vec2i::O;
        mouse.left.pressed = false;
        mouse.left.released = false;
        mouse.right.pressed = false;
        mouse.right.released = false;


        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT
                || keys[VK_ESCAPE].pressed)
            should_close = true;



        // cam controls

        vec3f dtrans = vec3f::O;
        if(keys['Z'].down) dtrans.z -= dt*10.f;
        if(keys['S'].down) dtrans.z += dt*10.f;
        if(keys['A'].down) dtrans.y -= dt*10.f;
        if(keys['E'].down) dtrans.y += dt*10.f;
        if(keys['Q'].down) dtrans.x -= dt*10.f;
        if(keys['D'].down) dtrans.x += dt*10.f;
        cam_pos += isom{} * rotate_axis { vec3f::Y, rot_y } * rotate_axis { vec3f::X, rot_x } * dtrans;

        if(mouse.right.down) {
            rot_x = clamp(rot_x - 0.001f * mouse.dpos.y, -M_PI/2, M_PI/2);
            rot_y = fmodf(rot_y - 0.001f * mouse.dpos.x, 2*M_PI);
        }




        // update

        isom cam_trans = isom{}
                        * translate { cam_pos }
                        * rotate_axis { vec3f::Y, rot_y }
                        * rotate_axis { vec3f::X, rot_x };
        /* mat4 cam_trans = lookat(cam_pos, vec3f { 0.5f, 0.5f, 0.5f }, vec3f::Y); */
        mat4 camproj = mat4::id() * proj * inv(cam_trans);

        ctx->UpdateSubresource(ubo, 0, NULL, &camproj, 0, 0);


        // render

        vec4f clear_color = {0.5f, 0.f, 0.5f, 1.0f};
        ctx->ClearRenderTargetView(screen_rt, &clear_color.x);
        ctx->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.f, 0);


        /* u32 stride = sizeof(f32)*7; */
        u32 stride = sizeof(f32)*6;
        u32 offset = 0;
        /* ctx->VSSetConstantBuffers(0, 1, &ubo); */
        ctx->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
        ctx->IASetIndexBuffer(ebo, DXGI_FORMAT_R16_UINT, 0);
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ctx->RSSetState(rast_state);

        /* ctx->Draw(array_size(vertices), 0); */
        ctx->DrawIndexed(array_size(indices), 0, 0);


        swapchain->Present(0, 0);
    }

    // cleanup
    printf("cleanup\n\n");
    DestroyWindow(window);

    return 0;
}
