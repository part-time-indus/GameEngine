#define WIN32_LEAN_AND_MEAN
#include <windows.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
WCHAR ch;



LPCWSTR CLASS_NAME = L"WinMain";
LPCWSTR WINDOW_TITLE = L"Game";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 480;

HINSTANCE hInstance;

//--------------------------------------------------------------------------

// TODO: WinMain Entry Point

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) 
{
	MSG msg;
	int done = false;
	if (!CreateMainWindow(hInstance, nCmdShow)) {
		return false;
	}
	while(!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			switch (msg.message) {
			case WM_QUIT:
				done = true;
			}

		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// TODO: Message Loop


	return((int) msg.lParam);
}




// TODO: Create Window Class
// TODO: Register Window Class
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX window_class;
	HWND window_handle;
	window_class.cbSize = sizeof(window_class);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProc;
	window_class.cbClsExtra = 0;
	window_class.hInstance = hInstance;
	window_class.cbWndExtra = 0;
	window_class.hIcon = NULL;

	//Don't set it to null
	window_class.hCursor = LoadCursorW(NULL, IDC_ARROW);

	//Don't set it to null
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = CLASS_NAME;
	window_class.hIconSm = NULL;

	if (RegisterClassEx(&window_class) == 0) {
		return false;
	}

	window_handle = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (window_handle == NULL) {
		return false;
	}

	ShowWindow(window_handle, nCmdShow);
	UpdateWindow(window_handle);

	return true;;
}

LRESULT WindowProc(HWND win_handle, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc;
		PAINTSTRUCT lp;
		RECT rect;
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
		case WM_CHAR:
			switch (wParam) {
			case VK_ESCAPE:
			case VK_RETURN:
			case VK_TAB:
			case VK_SPACE:
			case VK_DELETE:
			case VK_BACK:
				MessageBeep(UINT(-1));
				return (0);
			default:
				ch = (WCHAR) wParam;
				ch = ch + '\0';
				InvalidateRect(win_handle, NULL, false);
				return (0);
			
			}
		case WM_PAINT:
			hdc = BeginPaint(win_handle,&lp);
			GetClientRect(win_handle, &rect);
			FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
			EndPaint(win_handle, &lp);
			return(0);
		}

	// NOTE: For default processing of the messages not handled here 
	return DefWindowProcW(win_handle, msg, wParam, lParam);
}
// TODO: Window Process Callback Function
// TODO: Message Loop

//--------------------------------------------------------------------------