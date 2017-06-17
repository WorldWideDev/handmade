/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Devon Newsom $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */
#include <windows.h>

LRESULT CALLBACK
MainWindowCallback(HWND Window,
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
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int  Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;

            static DWORD Operation = WHITENESS;
            
            PatBlt(DeviceContext, X, Y, Width, Height, Operation);

            if(Operation == WHITENESS)
            {
                Operation = BLACKNESS;
            }
            else
            {
                Operation = WHITENESS;
            }
            
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
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";

   
    if(RegisterClassA(&WindowClass))
    {
        HWND WindowHandle =
        CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            "Handmade Hero",
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
             for(;;)
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

