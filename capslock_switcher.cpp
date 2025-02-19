#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <shellapi.h>

#define CAPSLOCK_KEY 0x14
#define WM_TRAYICON (WM_USER + 1)

HHOOK hHook;
NOTIFYICONDATAW nid;
HWND hWnd;

void SwitchLanguage()
{
    HWND hWnd = GetForegroundWindow();
    if (!hWnd) return;

    HKL hkl = GetKeyboardLayout(GetWindowThreadProcessId(hWnd, NULL));
    HKL newHkl = (hkl == LoadKeyboardLayoutW(L"00000419", KLF_ACTIVATE)) ? 
                 LoadKeyboardLayoutW(L"00000409", KLF_ACTIVATE) : 
                 LoadKeyboardLayoutW(L"00000419", KLF_ACTIVATE);

    PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)newHkl);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_TRAYICON && lParam == WM_RBUTTONUP) {
        HMENU hMenu = CreatePopupMenu();
        AppendMenuW(hMenu, MF_STRING, 1, L"Выход");

        POINT pt;
        GetCursorPos(&pt);
        SetForegroundWindow(hwnd);
        int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, NULL);
        if (cmd == 1) {
            Shell_NotifyIconW(NIM_DELETE, &nid);
            PostQuitMessage(0);
        }
        DestroyMenu(hMenu);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (kbdStruct->vkCode == CAPSLOCK_KEY) {
            SwitchLanguage();
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CapsLockSwitcherClass";
    RegisterClassW(&wc);
    hWnd = CreateWindowExW(0, wc.lpszClassName, L"CapsLockSwitcher", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
    wcscpy_s(nid.szTip, L"CapsLockSwitcher");
    Shell_NotifyIconW(NIM_ADD, &nid);

    hHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!hHook) return 1;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}
