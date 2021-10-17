#include <string>
#include "Window.h"

/* There are four parameters:

	- LRESULT: Int value that program returns to Windows
	- CALLBACK: calling convention for the function
	- hwnd is a handle to the window
	- msg is the message code; for example, the WM_SIZE message indicates the window was resized
	- wParam and lParam contain additional data that pertains to the message. The exact meaning depends on the message code

*/

/* On testing, in the debug screen, the WPARAM seems to be storing info about what key, on the keyboard is being pressed */

/* All Virtual-Key Codes: https://docs.microsoft.com/en-gb/windows/win32/inputdev/virtual-key-codes */
/* These can be compared against the WPARAM on debug screen to see what key is being pressed (by the user) */
/* For the alphanumeric keys, there are no Virtual-Key Codes as the code is just the ASCII value of the character */

//Window class stuff
Window::WindowClass Window::WindowClass::wndClass;

/* Defining constructor */
Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr)) //Getting handle to the instance so that it can be saved in the data. It is now static
{
	// 1. Register window class
	WNDCLASSEX wc = { 0 };				//WNDCLASSEX msdn for more info on wc. {x} parameters
	wc.cbSize = sizeof(wc);//Set to size of structure         //https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	wc.style = CS_OWNDC;				//Allocates a unique device context for each window in the class (for multiple windows).
	wc.lpfnWndProc = HandleMsgSetup;     //https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-defwindowproca?redirectedfrom=MSDN
	wc.cbClsExtra = 0;					//lpfn = long pointer function, set to my own Window Procedure that I defined.
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa */
	RegisterClassEx(&wc);             //Passed the pointer to the configuration at line 14. ("&" For passing pointers).

	/* We can now refer to this whenever creating a new window */
}

/* Unregister the window class, freeing memory required for the class */
/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-unregisterclassa */
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

/* Two interface functions below */
/* Get name the window classname */
const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

/* Get handle to the instance */
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

/* Window stuff */
/* Take width, height and name of the window */
Window::Window(int width, int height, const char* name) noexcept
{
	/* Calculate window size based on desired client region size */
	/* The RECT structure defines a rectangle by the coordinates of its upper-left and lower-right corners */
	/* https://docs.microsoft.com/en-us/windows/win32/api/windef/ns-windef-rect */
	RECT wr;
	wr.left = 100; //Specifies the x-coordinate of the upper-left corner of the rectangle
	wr.right = width + wr.left; //Specifies the x-coordinate of the lower-right corner of the rectangle
	wr.top = 100; //Specifies the y-coordinate of the upper-left corner of the rectangle
	wr.bottom = height + wr.top; //Specifies the y-coordinate of the lower-right corner of the rectangle

	/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-adjustwindowrect */
	/* Calculates the required size of the window rectangle, based on the desired client-rectangle size. 
	   The window rectangle can then be passed to the CreateWindow function to create a window whose client area is the desired size */
	/* Window styles: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles */
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	/* Create window and get the handle to it*/
	// 2. Create window instance	->	/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa */
	// This not take a config structure! Just some parameters as seen in the link above.
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //This is the style setup of the window(Check full params at link above)
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this //-> lParam set to this Window Instance

		/* Default starting position of window decided by Windows -> CW_USEDEFAULT*/
	);
	/* Show the window: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow */
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

/* Destructor */
Window::~Window()
{
	DestroyWindow(hWnd);
}


/*This exists to set up the pointer to instance*/
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	/* Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side */
	/* NCCREATE = Non-client create */
	/* This is sent when a window is first created: https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate */
	if (msg == WM_NCCREATE)
	{
		//Extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams); //Pointer to a window instance

		/* Set WinAPI-managed user data to store ptr to window class */
		/* https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra */
		/* GWLP_USERDATA will create a link between the actual window and the window class that is being used to 
		   control that window */
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		/* Set message procedure to normal (non-setup) handler now that setup is finished */
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		//Forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* From now, messages are handle by this */
/* This adapts from WIN32 call convention to the C++ member function call convention */
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	/* Retrieve ptr to window class */
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	/* Forward message to window class handler */
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	/* MSDN docs to the WIN messages: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-reference */

	/* NC = Non-Client region -> Where the window bar is (- X square are) */

	switch (msg)
	{

	//This is to close the window when the "X" is pressed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}