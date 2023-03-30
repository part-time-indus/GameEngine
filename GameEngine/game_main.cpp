#define WIN32_LEAN_AND_MEAN
#include <windows.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 480;
bool vk_keys[256];


WCHAR ch = L' ';
LPCWSTR CLASS_NAME = L"WinMain";
LPCWSTR WINDOW_TITLE = L"Game";
RECT rect;
PAINTSTRUCT ps;
HDC hdc;


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
	for (int i = 0; i < 256; i++) {
		vk_keys[i] = 0;
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

	return true;
}

LRESULT WindowProc(HWND win_handle, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		const wchar_t* key_pressed = L"P";
		const wchar_t* key_released = L"R";
		const SHORT SHIFTED = 0x8000;
		TEXTMETRIC tm;
		DWORD chWidth = 20;
		DWORD chHeight = 20;
		SHORT key_state;
		switch (msg) {
			case WM_CREATE:
				hdc = GetDC(win_handle);
				GetTextMetrics(hdc, &tm);
				ReleaseDC(win_handle, hdc);
				chWidth = tm.tmAveCharWidth;
				chHeight = tm.tmHeight;
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return(0);
			case WM_CHAR:
				switch (wParam) {
				case 0x09:
				case 0x0A:
				case 0x0D:
				case 0x1B:
					MessageBeep((UINT)-1);
					return (0);
				default:
					ch = (WCHAR) wParam;
					ch = ch + 0x00;
					InvalidateRect(win_handle, NULL, false);
					return (0);
			
				}
			case WM_KEYDOWN:
				vk_keys[wParam] = true;
				switch (wParam) {
				case VK_CONTROL:
					key_state = GetKeyState(VK_LCONTROL);
					if ((key_state & SHIFTED) == SHIFTED) {
						vk_keys[VK_LCONTROL] = true;
					}

					key_state = GetKeyState(VK_RCONTROL);
					if ((key_state & SHIFTED) == 0) {
						vk_keys[VK_RCONTROL] = true;
					}
					break;
				case VK_SHIFT: 
					key_state = GetKeyState(VK_LSHIFT);
					if ((key_state & SHIFTED) == SHIFTED) {
						vk_keys[VK_LCONTROL] = true;
					}

					key_state = GetKeyState(VK_RCONTROL);
					if ((key_state & SHIFTED) == 0) {
						vk_keys[VK_RCONTROL] = true;
					}
					break;
				}
			
				InvalidateRect(win_handle, NULL, TRUE);
				return 0;

			case WM_KEYUP:
				vk_keys[wParam] = false;
				switch (wParam) {
					case VK_SHIFT:
						key_state = GetKeyState(VK_LSHIFT);
						if ((key_state & SHIFTED) == 0) {
							vk_keys[VK_LSHIFT] = false;
						}

						key_state = GetKeyState(VK_RSHIFT);
						if ((key_state & SHIFTED) == 0) {
							vk_keys[VK_RSHIFT] = false;
						}
						break;
					case VK_CONTROL:
						key_state = GetKeyState(VK_LCONTROL);
						if ((key_state & SHIFTED) == 0) {
							vk_keys[VK_LCONTROL] = false;
						}

						key_state = GetKeyState(VK_RCONTROL);
						if ((key_state & SHIFTED) == 0) {
							vk_keys[VK_RCONTROL] = false;
						}

						break;
				}
				InvalidateRect(win_handle, NULL, TRUE);
				return (0);
			case WM_PAINT:
				hdc = BeginPaint(win_handle,&ps);
				TextOut(hdc, 0, 0, &ch, 1);
				
				for (int row = 0; row < 16; row++) {
					for (int col = 0; col < 16; col++) {
						//(ith row * column width = (i+1)th row) + jth column = (i,j)th element
						int current_key = ((row * 16) + col);
						if (vk_keys[current_key]) {
							SetBkMode(hdc, OPAQUE);
							TextOut(hdc, col * chWidth + chWidth * 2, row * chHeight + chHeight * 2, key_pressed, 2);
						}
						else {
							SetBkMode(hdc, OPAQUE);
							TextOut(hdc, col * chWidth + chWidth * 2, row * chHeight + chHeight * 2, key_released, 2);
						}
					}
				}

				EndPaint(win_handle, &ps);
				return(0);
		}

	// NOTE: For default processing of the messages not handled here 
	return DefWindowProcW(win_handle, msg, wParam, lParam);
}
