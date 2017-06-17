/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Devon Newsom $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */

#include <windows.h>
#include <tchar.h>

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("Handmade Hero");

LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch(Message)
    {
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

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        default:
        {
            OutputDebugStringA("Default\n");
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
    
    WNDCLASSEX WindowClass;
    
    //TODO(Devon): check if OWNDC/HREDRAW/VREDRAW still matter
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = szWindowClass;

    //SUP

    /*
    WNDCLASSEX WindowClass;
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = szWindowClass;
    WindowClass.hIconSm = LoadIcon(WindowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    */
   
    if(!RegisterClassEx(&WindowClass))
    {
        MessageBox(NULL,
                   _T("Call to RegisterClass failed!"),
                   _T("Handmade hero"),
                   NULL);
        
        return 1;
   
    }
    HWND WindowHandle =
        CreateWindowEx(
            0,
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            500, 100,
            0,
            0,
            Instance,
            0);

    
    if(!WindowHandle)
    {
        MessageBox(NULL,
                   _T("Call to CreateWindowEx failed!"),
                   _T("Handmade hero"),
                   NULL);
        return 1;
    }
    else
    {
        ShowWindow(WindowHandle,
                   ShowCode);
        UpdateWindow(WindowHandle);
        for(;;)
        {
            MSG Message;
            BOOL GetMessageResult = GetMessage(&Message, 0, 0, 0);
            if(GetMessageResult > 0)
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
            else
            {
                break;
            }
        }
        
    }
   
    OutputDebugStringA("hellooo");
    return 0;
}

