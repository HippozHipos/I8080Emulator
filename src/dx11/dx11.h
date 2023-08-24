#ifndef DX11_H
#define DX11_H

#include <d3d11.h>

struct dx11
{
    //data
    static ID3D11Device* g_pd3dDevice;
    static ID3D11DeviceContext* g_pd3dDeviceContext;
    static IDXGISwapChain* g_pSwapChain;
    static ID3D11RenderTargetView* g_mainRenderTargetView;

    //creation
    static bool CreateDeviceD3D(HWND hWnd);
    static void CreateRenderTarget();

    //cleanup
    static void CleanupDeviceD3D();
    static void CleanupRenderTarget();
    static void ClearScreen(float r, float g, float b, float a);

    static int Init(HWND);
    static void Render();

    static void Destroy();
};


#endif // !DX11_H
