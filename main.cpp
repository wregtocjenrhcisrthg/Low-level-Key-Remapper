#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

using std::cout;
using std::endl; //no using namespace std here!!!!!!!!

DWORD firstKey, secondKey;

LRESULT CALLBACK Hook(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL fEatKeystroke = FALSE;

    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            case WM_KEYDOWN: //we need to handle these actions, otherwise they will be discalled too
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
                PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
                if (fEatKeystroke = (p->vkCode == firstKey)) //redirect keys
                {
                    cout << "[" << firstKey << "]" << " Hook called" << endl;

                    if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN))
                    {
                        keybd_event(secondKey, 0, 0, 0);
                    }
                    else if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP))
                    {
                        keybd_event(secondKey, 0, KEYEVENTF_KEYUP, 0);
                    }
                    break;
                }
                break;
        }
    }
    return(fEatKeystroke ? 1 : CallNextHookEx(0, nCode, wParam, lParam));
}

int main(int argc, char** argv)
{
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, Hook, 0, 0);

    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " [key to hook in hex] [target key in hex]" << endl;
        cout << "e.g. if you want remap A key to B key use '" << argv[0] << " 0x41 0x42'" << endl << endl;
        cout << "P.S. - Key codes: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes" << endl;
        _getch();
        ExitProcess(1337);
    }

    firstKey = strtoul(argv[1], 0, 16);
    secondKey = strtoul(argv[2], 0, 16);

    MSG msg;
    while (!GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hhkLowLevelKybd);
    return(0);
}
