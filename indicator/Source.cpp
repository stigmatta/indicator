#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>
#include <ctime>
#include <string>
#include "resource.h"

#pragma comment(lib,"comctl32")

using namespace std;

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;
HWND hDialog, hSpin, hEdit, hProgress, hList;
bool isStarted;
const int  arrSize = 16;
static unsigned short selected = 0;
int numArray[arrSize];



void SetNumbers()
{
    srand(time(0));
    HWND button;
    int randNum;
    int x = 0;
    for (int i = IDC_BUTTON1; i <= IDC_BUTTON16; i++)
    {
        button = GetDlgItem(hDialog, i);
        EnableWindow(button, TRUE);
        randNum = 1 + rand() % 100;
        numArray[x] = randNum;
        SetWindowText(button, to_wstring(randNum).c_str());
        x++;
    }
    for (int i = 0; i < arrSize; i++)
    {
        for (int j = arrSize - 1; j > i; j--)
        {
            if (numArray[j - 1] > numArray[j])
            {
                swap(numArray[j - 1], numArray[j]);
            }
        }
    }
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    hInst = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{

    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    case WM_INITDIALOG:
        hDialog = hWnd;
        hSpin = GetDlgItem(hDialog, IDC_SPIN1);
        hEdit = GetDlgItem(hDialog, IDC_EDIT1);
        hProgress = GetDlgItem(hDialog, IDC_PROGRESS1);
        hList = GetDlgItem(hDialog, IDC_LIST1);

        SendMessage(hProgress, PBM_SETSTEP, 1, 0);
        SendMessage(hProgress, PBM_SETPOS, 0, 0);
        SendMessage(hSpin, UDM_SETRANGE32, 5, 50);

        SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
        SetWindowText(hEdit, TEXT("5"));
        SetNumbers();
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wp) == IDC_NEWGAME && HIWORD(wp) == BN_CLICKED) {
            isStarted = false;
            SendMessage(hProgress, PBM_SETPOS, 0, 0);
            SendMessage(hList, LB_RESETCONTENT, 0, 0);
            SetNumbers();
            WCHAR timeStr[4];
            int time;
            GetWindowText(hEdit, timeStr, 3);
            time = wcstol(timeStr, 0, 0);
            SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, time * 10));
            SetTimer(hWnd, 1, 100, NULL);
            selected = 0;
            isStarted = true; 
        }
        if (HIWORD(wp) == BN_CLICKED && isStarted) {
            int controlId = LOWORD(wp);
            if (controlId >= IDC_BUTTON1 && controlId <= IDC_BUTTON16) {
                HWND button = GetDlgItem(hWnd, controlId);
                WCHAR buttonText[10];
                GetWindowText(button, buttonText, 10);
                int number = _wtoi(buttonText);
                if (number == numArray[selected])
                {
                    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buttonText);
                    selected++;
                    EnableWindow(button, FALSE);
                }

            }
        }
        return TRUE;
    case WM_TIMER:
        int nPos = SendMessage(hProgress, PBM_GETPOS, 0, 0);
        int nMax = SendMessage(hProgress, PBM_GETRANGE, 0, 0);
        if (nPos >= nMax) {
            KillTimer(hWnd, 1);
            isStarted = 0;
            MessageBox(hWnd, L"Игра окончена!", L"Информация", MB_OK | MB_ICONINFORMATION);
        }
        else {
            SendMessage(hProgress, PBM_STEPIT, 0, 0);
        }
        return TRUE;
    }
    return FALSE;
}