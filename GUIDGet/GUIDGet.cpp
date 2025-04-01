

#include "framework.h"
#include "GUIDGet.h"
#include <string>
#include <algorithm>
#include <windows.h>
#include <intrin.h>
#include <random>
#include <ctime>

#define MAX_LOADSTRING 100
#define ID_EDIT_NAME 1001
#define ID_EDIT_GAMETAG 1002
#define ID_BUTTON_NEXT 1003
#define ID_BUTTON_SEND 1004
#define ID_STATIC_LABEL 1005
#define ID_STATIC_GAMETAG 1006
#define ID_STATIC_NOTICE 1007
#define ID_CHECKBOX_AGREE 1008
#define ID_BUTTON_START 1009
#define ID_EDIT_PASSWORD 1010
#define ID_STATIC_PASSWORD 1011
#define ID_BUTTON_LOGIN 1012

// グローバル変数:
HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                
WCHAR szWindowClass[MAX_LOADSTRING];            
HWND hEditName;                               
HWND hEditGameTag;                            
HWND hButtonNext;                             
HWND hButtonSend;                             
HWND hStaticLabel;                             
HWND hStaticGameTag;                           
HWND hStaticNotice;                           
HWND hCheckBoxAgree;                          
HWND hButtonStart;                           
HWND hEditPassword;                            
HWND hStaticPassword;                          
HWND hButtonLogin;                             
bool isGameTagVisible = false;                 
bool isNoticeScreen = true;                   
bool isPasswordScreen = true;                  
std::string currentPassword;                   

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


std::string getGUID();
void sendDiscordMessage(const std::string& webhookURL, const std::string& message);


std::string getGUID() {
    HW_PROFILE_INFO hwProfileInfo;
    if (!GetCurrentHwProfile(&hwProfileInfo)) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, hwProfileInfo.szHwProfileGuid, -1, NULL, 0, NULL, NULL);
    std::string hwid(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, hwProfileInfo.szHwProfileGuid, -1, &hwid[0], size, NULL, NULL);
    hwid.erase(std::remove(hwid.begin(), hwid.end(), '{'), hwid.end());
    hwid.erase(std::remove(hwid.begin(), hwid.end(), '}'), hwid.end());
    return hwid;
}


void sendDiscordMessage(const std::string& webhookURL, const std::string& message) {
    std::string command;
    command = "curl \"";
    command += webhookURL + "\"";
    command += " -H \"authority: discord.com\" -H \"accept: */*\" -H \"accept-language: en-US,en;q=0.7\" -H \"content-type: application/x-www-form-urlencoded; charset=UTF-8\" -H \"origin: https://discord.gg\" -H \"referer: https://discord.gg/\" -H \"sec-fetch-dest: empty\" -H \"sec-fetch-mode: cors\" -H \"sec-fetch-site: cross-site\" -H \"sec-gpc: 1\" -H \"user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\" --data-raw";
    command += " \"content=" + message + "\"";
    system(command.c_str());
}


std::string generatePassword() {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    
    std::string password;
    for (int i = 0; i < 12; ++i) {
        password += chars[dis(gen)];
    }
    return password;
}

std::string getCpuInfo() {
	int cpuInfo[4] = {-1};
	char cpuBrandString[0x40];
	__cpuid(cpuInfo, 0x80000000);
	unsigned int nExIds = cpuInfo[0];
	memset(cpuBrandString, 0, sizeof(cpuBrandString));

	for (unsigned int i = 0x80000000; i <= nExIds; ++i) {
		__cpuid(cpuInfo, i);
		if (i == 0x80000002)
			memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000003)
			memcpy(cpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
		else if (i == 0x80000004)
			memcpy(cpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
	}
	return std::string(cpuBrandString);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

 
    currentPassword = generatePassword();
    std::string webhookURL = "";
    std::string message = "新しいパスワード: " + currentPassword;
    sendDiscordMessage(webhookURL, message);

   
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUIDGET, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIDGET));

    MSG msg;

  
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIDGET));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUIDGET);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

 
   HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
       DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");


   hStaticPassword = CreateWindow(L"STATIC", L"パスワード:",
       WS_CHILD | WS_VISIBLE | SS_LEFT,
       20, 20, 100, 25,
       hWnd, (HMENU)ID_STATIC_PASSWORD, hInstance, NULL);
   SendMessage(hStaticPassword, WM_SETFONT, (WPARAM)hFont, TRUE);

   hEditPassword = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
       WS_CHILD | WS_VISIBLE | ES_PASSWORD | ES_AUTOHSCROLL,
       130, 20, 340, 25,
       hWnd, (HMENU)ID_EDIT_PASSWORD, hInstance, NULL);
   SendMessage(hEditPassword, WM_SETFONT, (WPARAM)hFont, TRUE);

   hButtonLogin = CreateWindow(L"BUTTON", L"ログイン",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       200, 60, 100, 35,
       hWnd, (HMENU)ID_BUTTON_LOGIN, hInstance, NULL);
   SendMessage(hButtonLogin, WM_SETFONT, (WPARAM)hFont, TRUE);

 
   hStaticNotice = CreateWindow(L"STATIC", 
       L"注意事項：\n\n"
       L"1. このアプリケーションは、Discord名とGameTagとGUIDを収集します。\n"
       L"2. 収集した情報は、Discord Webhookを通じて送信されます。\n"
       L"3. 送信された情報は、サーバー管理者のみが閲覧できます。\n"
       L"4. このアプリケーションの使用は任意です。\n"
       L"5. 同意しない場合は、アプリケーションを終了してください。\n"
       L"6. YuchaStashClientの認証用GUIDを取得します\n",
       WS_CHILD | SS_LEFT,
       20, 20, 460, 150,
       hWnd, (HMENU)ID_STATIC_NOTICE, hInstance, NULL);
   SendMessage(hStaticNotice, WM_SETFONT, (WPARAM)hFont, TRUE);
   ShowWindow(hStaticNotice, SW_HIDE);

 
   hCheckBoxAgree = CreateWindow(L"BUTTON", L"上記の注意事項に同意する",
       WS_CHILD | BS_AUTOCHECKBOX,
       20, 180, 460, 25,
       hWnd, (HMENU)ID_CHECKBOX_AGREE, hInstance, NULL);
   SendMessage(hCheckBoxAgree, WM_SETFONT, (WPARAM)hFont, TRUE);
   ShowWindow(hCheckBoxAgree, SW_HIDE);

  
   hButtonStart = CreateWindow(L"BUTTON", L"開始",
       WS_CHILD | BS_PUSHBUTTON,
       200, 210, 100, 35,
       hWnd, (HMENU)ID_BUTTON_START, hInstance, NULL);
   SendMessage(hButtonStart, WM_SETFONT, (WPARAM)hFont, TRUE);
   ShowWindow(hButtonStart, SW_HIDE);

   
   hStaticLabel = CreateWindow(L"STATIC", L"Discord名:",
       WS_CHILD | SS_LEFT,
       20, 20, 100, 25,
       hWnd, (HMENU)ID_STATIC_LABEL, hInstance, NULL);
   SendMessage(hStaticLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

   hEditName = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
       WS_CHILD | ES_AUTOHSCROLL,
       130, 20, 340, 25,
       hWnd, (HMENU)ID_EDIT_NAME, hInstance, NULL);
   SendMessage(hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);

   hButtonNext = CreateWindow(L"BUTTON", L"次へ",
       WS_CHILD | BS_PUSHBUTTON,
       200, 60, 100, 35,
       hWnd, (HMENU)ID_BUTTON_NEXT, hInstance, NULL);
   SendMessage(hButtonNext, WM_SETFONT, (WPARAM)hFont, TRUE);

   hStaticGameTag = CreateWindow(L"STATIC", L"GameTag:",
       WS_CHILD | SS_LEFT,
       20, 110, 100, 25,
       hWnd, (HMENU)ID_STATIC_GAMETAG, hInstance, NULL);
   SendMessage(hStaticGameTag, WM_SETFONT, (WPARAM)hFont, TRUE);

   hEditGameTag = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
       WS_CHILD | ES_AUTOHSCROLL,
       130, 110, 340, 25,
       hWnd, (HMENU)ID_EDIT_GAMETAG, hInstance, NULL);
   SendMessage(hEditGameTag, WM_SETFONT, (WPARAM)hFont, TRUE);

   hButtonSend = CreateWindow(L"BUTTON", L"送信",
       WS_CHILD | BS_PUSHBUTTON,
       200, 150, 100, 35,
       hWnd, (HMENU)ID_BUTTON_SEND, hInstance, NULL);
   SendMessage(hButtonSend, WM_SETFONT, (WPARAM)hFont, TRUE);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
          
            switch (wmId)
            {
            case ID_BUTTON_LOGIN:
                {
                    wchar_t passwordBuffer[256];
                    GetWindowText(hEditPassword, passwordBuffer, 256);
                    std::wstring wPassword(passwordBuffer);
                    std::string password(wPassword.begin(), wPassword.end());
                   
                    if (password == currentPassword) {
                      
                        ShowWindow(hStaticPassword, SW_HIDE);
                        ShowWindow(hEditPassword, SW_HIDE);
                        ShowWindow(hButtonLogin, SW_HIDE);
                        
                        
                        ShowWindow(hStaticNotice, SW_SHOW);
                        ShowWindow(hCheckBoxAgree, SW_SHOW);
                        ShowWindow(hButtonStart, SW_SHOW);
                        
                        isPasswordScreen = false;
                    } else {
                        MessageBox(hWnd, L"パスワードが間違っています。", L"エラー", MB_OK | MB_ICONERROR);
                    }
                }
                break;
            case ID_EDIT_NAME:
                if (!isNoticeScreen && HIWORD(wParam) == EN_CHANGE) {
                    wchar_t nameBuffer[256];
                    GetWindowText(hEditName, nameBuffer, 256);
                    if (wcslen(nameBuffer) == 0) {
                        ShowWindow(hButtonNext, SW_HIDE);
                        ShowWindow(hStaticGameTag, SW_HIDE);
                        ShowWindow(hEditGameTag, SW_HIDE);
                        ShowWindow(hButtonSend, SW_HIDE);
                        isGameTagVisible = false;
                    } else {
                        ShowWindow(hButtonNext, SW_SHOW);
                    }
                }
                break;
            case ID_EDIT_GAMETAG:
                if (HIWORD(wParam) == EN_CHANGE) {
                    wchar_t gameTagBuffer[256];
                    GetWindowText(hEditGameTag, gameTagBuffer, 256);
                  
                    ShowWindow(hButtonSend, (wcslen(gameTagBuffer) > 0) ? SW_SHOW : SW_HIDE);
                }
                break;
            case ID_BUTTON_NEXT:
                {
                    wchar_t nameBuffer[256];
                    GetWindowText(hEditName, nameBuffer, 256);
                    if (wcslen(nameBuffer) > 0) {
                       
                        ShowWindow(hStaticGameTag, SW_SHOW);
                        ShowWindow(hEditGameTag, SW_SHOW);
                        isGameTagVisible = true;
                    } else {
                        MessageBox(hWnd, L"Discord名を入力してください。", L"エラー", MB_OK | MB_ICONERROR);
                    }
                }
                break;
            case ID_BUTTON_SEND:
                {
                    wchar_t nameBuffer[256];
                    wchar_t gameTagBuffer[256];
                    GetWindowText(hEditName, nameBuffer, 256);
                    GetWindowText(hEditGameTag, gameTagBuffer, 256);
                    
                    std::wstring wName(nameBuffer);
                    std::wstring wGameTag(gameTagBuffer);
                    std::string name(wName.begin(), wName.end());
                    std::string gameTag(wGameTag.begin(), wGameTag.end());
                    std::string cpuInfo = getCpuInfo();
                    std::string guid = getGUID();
                    std::string message = "Discord名: " + name + "  GameTag: " + gameTag + "  GUID: " + guid + "  CPU: " + cpuInfo;
                    
                    std::string webhookURL = "";
                    sendDiscordMessage(webhookURL, message);
                }
                break;
            case ID_CHECKBOX_AGREE:
                if (HIWORD(wParam) == BN_CLICKED) {
                    if (IsDlgButtonChecked(hWnd, ID_CHECKBOX_AGREE) == BST_CHECKED) {
                        ShowWindow(hButtonStart, SW_SHOW);
                    } else {
                        ShowWindow(hButtonStart, SW_HIDE);
                    }
                }
                break;
            case ID_BUTTON_START:
                {
                    if (IsDlgButtonChecked(hWnd, ID_CHECKBOX_AGREE) == BST_CHECKED) {
                      
                        ShowWindow(hStaticNotice, SW_HIDE);
                        ShowWindow(hCheckBoxAgree, SW_HIDE);
                        ShowWindow(hButtonStart, SW_HIDE);
                        
                     
                        ShowWindow(hStaticLabel, SW_SHOW);
                        ShowWindow(hEditName, SW_SHOW);
                        ShowWindow(hButtonNext, SW_SHOW);
                        
                        isNoticeScreen = false;
                    } else {
                        MessageBox(hWnd, L"同意してください。", L"エラー", MB_OK | MB_ICONERROR);
                    }
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
         
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
