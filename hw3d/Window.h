#pragma once
#include "myWin.h"

/* Class to represent a Window: It is going to encapsulate the creation, 
   the destruction and message handling of a window */

class Window
{
private:
	/* Singleton manages registration / cleanup of window class */
	/* This is only instantiated once, and can provide easy access to a single instance */
	/* To make things cleaner, register and unregister WindowClass at the end */
	class WindowClass
	{
	public:
		/* noexcept to make sure that no exceptions are thrown */
		/* Get the name of the class */
		static const char* GetName() noexcept;
		/* Get the handle to the instance */
		static HINSTANCE GetInstance() noexcept;

	private:
		/* Constructor that register on WINAPI side*/
		WindowClass() noexcept;
		/* Destructor that register on WINAPI side*/
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		/* Name of the window class */
		static constexpr const char* wndClassName = "Luigi's Direct3D Engine Window";
		/* Window class */
		static WindowClass wndClass;
		/* Store handle to the instance. hInst has handle to the instance now */
		HINSTANCE hInst;
	};

/* Actual window class */
public:
	/* Window constructor, that is going to create the window */
	Window(int width, int height, const char* name) noexcept;
	/* Window destructor, that is going to destroy the window */
	~Window();
	Window(const Window& ) = delete;
	Window& operator = (const Window&) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	/* Handle windows messages */
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};
