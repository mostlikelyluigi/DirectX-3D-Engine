#include "Window.h"
#include <sstream>
#include "WindowsMessageMap.h"

/* Current Chilli Tutorial Number: 7 */

/*WPARAM: WORD Argument. A message parameter. This illustrates the point of these Windows aliases; they are there to protext programs from change
  LPARAM: LONG Argument. A message parameter */

//LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	static WindowsMessageMap mm; //mm: message map
//	OutputDebugString(mm(msg, lParam, wParam).c_str()); /*Every message now gets converted and outputted in a nice
//														formatted message:
//														.c_str just turns it into a string*/
//														/*All this can be seen outputted in the debug window*/
//
//	/* MSDN docs to the WIN messages: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-reference */
//
//	/* NC = Non-Client region -> Where the window bar is (- X square are) */
//
//	switch (msg)
//	{
//
//	//This is to close the window when the "X" is pressed.
//	case WM_CLOSE:
//		PostQuitMessage( 69 );
//		break;
//
//	//When I press the 'W' key on the keyboard, the window title is changed to "Dub".
//	case WM_KEYDOWN:
//		if (wParam == 'W')
//		{
//			SetWindowText(hWnd, "Dub"); //This will give me feedback that messages are being handles properly.
//		}
//		break;
//
//	/* When the user releases the 'W' key on keyboard, window title is changed again */															   
//	case WM_KEYUP:
//		if (wParam == 'F')
//		{
//			SetWindowText(hWnd, "Not a Dub anymore :("); 
//		}
//		break;
//
//
//	/* WM_CHAR is a message for text input -> when you're typing into a text field, input box, word processor and you
//	   want to get text */
//
//	/* WM_CHAR is used when you're looking for text input, when you're typing in text and sentences */
//
//	/* Difference between WM_CHAR and WM_KEYDOWN is that you use WM_KEYDOWN when you're looking for for keystroke inputs.
//	   For example, when you use WASD on keyboard to make a character move. 
//	   Then you would use WM_CHAR messages to then write that characters name in a screen */
//
//	/* The code below allows the me to type/change the window title live, but then when I press and RELEASE the 'F' key,  
//	   "Not a Dub anymore :(" appears because WM_KEYUP comes after WM_CHAR */
//	/* WM_CHAR also supports lower case and upper case letters */
//	/* https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-char */
//	case WM_CHAR:
//		{
//			static std::string title;
//			title.push_back((char)wParam);
//			SetWindowText(hWnd, title.c_str());
//		}
//		break;
//
//	/* https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttondown */
//	/* On LEFTBUTTONCLICK, return the x- and y- coordinates of a point in a client area (where mouse click happened) */
//	/* This is made possible with POINTS structure: https://docs.microsoft.com/en-us/windows/win32/api/windef/ns-windef-points */
//	case WM_LBUTTONDOWN:
//		{
//			/* https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-makepoints */
//			/* MAKEPOINTS converts a value that contains the x- and y-coordinates of a point into a POINTS structure */
//			const POINTS pt = MAKEPOINTS (lParam); 
//
//			/* Outputting the coordinates of the mouse to the WindowText */
//			std::ostringstream oss;
//			/* Format the coordinates */
//			oss << "(" << pt.x << "," << pt.y << ")";
//			/* Transfer the formatted coordinates on WindowText */
//			SetWindowText(hWnd, oss.str().c_str());
//		}
//		break;
//	}
//
//	
//
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}


int CALLBACK WinMain(					//Telling C++ to use CDECL Calling Convention.
	HINSTANCE hInstance,				//Pointers to structures that hold info about the program (When loaded into memory).
	HINSTANCE hPrevInstance,			//This is always NULL.
	LPSTR	  lpCmdLine,				//Just the command line.
	int       nCmdShow)					//Indication on how the window should be shown when created.
{
	//Show Message (MESSAGE LOOP) https://docs.microsoft.com/en-us/windows/win32/learnwin32/window-messages

	/* You can make however many windows :D */
	Window wnd(800, 300, "Luigi's DirectX 3D Engine");

	//This func removes the first message from head of queue
	MSG msg; //Message Structure
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		/* This checks the message: If it's a WM_KEYDOWN message, it will post a WM_CHAR message to the message queue */
		/* It will generate CHAR messages from the keydown messages (Check debug window for all the messages) */
		/* If TranslateMessage is removed, then WM_CHAR won't appear on the debug window, just comment the code below to test */
		TranslateMessage(&msg);	

		/* This tells the operating system to call the window procedure of the window that is the target of the message */
		DispatchMessage(&msg);	
	}

	/*EXTRA INFO
	
	-POSTING a message means that the message goes on the queue and is dispactched 
	through the message loop (TranslateMessage & DispatchMessage)

	-SENDING a message means the message skips the queue and the OS calls the 
	window procedure directly.
	
	*/

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
	
}