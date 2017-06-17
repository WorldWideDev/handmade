/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Devon Newsom $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */
#include <windows.h>

#define internal static;
#define local_persist static;
#define global_variable static;

//TODO(devon): This is global, for now
global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void
Win32ResizeDIBSection(int Width, int Height)
{
    //TODO(devon): bulletproof this
    //maybe dont free first, free after, then free first if that fails

    if(BitmapHandle)
    {
        DeleteObject(BitmapHandle);
    }
    
    if(!BitmapDeviceContext)
    {
        //TODO(devon): should we recreate these under certain special circumstances
        BitmapDeviceContext = CreateCompatibleDC(0);
    }
    
    BitmapInfo.bmiHeader.biSize= sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext,
        &BitmapInfo,
        DIB_RGB_COLORS,
        &BitmapMemory,
        0, 0);

}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    // rectange to rectange copy
    StretchDIBits(DeviceContext,
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
     
    
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_CREATE:
        {
            OutputDebugStringA("WM_CREATE\n");
        } break;
        
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Height = ClientRect.bottom - ClientRect.top;
            int Width = ClientRect.right - ClientRect.left;
            Win32ResizeDIBSection(Width, Height);
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_DESTROY:
        {
            //TODO(devon): Handle this with an error - recreate window?
            Running = false;
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            //TODO(devon): Handle this with a message to the user?
            Running = false;
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
            EndPaint(Window, &Paint);
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        default:
        {
            // OutputDebugStringA("Default\n");
            Result = DefWindowProc(Window, Message, WParam, LParam);
        }
    }
    
    return (Result);
}


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{

    WNDCLASS WindowClass = {};
    
    //TODO(Devon): check if OWNDC/HREDRAW/VREDRAW still matter
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";

   
    if(RegisterClassA(&WindowClass))
    {
        HWND WindowHandle =
        CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            "Devon Was Here",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0);  //  passes param to WM_CREATE

        if(WindowHandle)
        {
            Running = true;
            while(Running)
            {
                MSG Message;
                BOOL GetMessageResult = GetMessageA(&Message, 0, 0, 0);
                if(GetMessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }
                else
                {
                    break;
                }
            }
        }

        else
        {
            //TODO(Devon): Logging
        }
    }

    else
    {
        //TODO(Devon): Logging
    }
    
    OutputDebugStringA("hellooo");
    return 0;
}

