// GuessTheNumberGUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GuessTheNumberGUI.h"
#include <string>
#include <random>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hGTN;										// "Guess The Number" heading
HWND hRange;									// "Range: "
HWND hFN;										// First number
HWND hTo;										// hFN "to" hSN
HWND hSN;										// Second number
HWND hGuesses;									// "Number of Guesses: "
HWND hNGuesses;									// Number of guesses
HWND hStart;									// Start
HWND hGuess;									// "Guess:"
HWND hEnterGuess;								// Enter guess
HWND hGuessButton;								// Guess button
HWND hGuessesLeft;								// "Guesses Left:"
HWND hNGuessesLeft;								// Number of Guesses left			
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
unsigned long long lowerNumber;					// Lower number of range
unsigned long long higherNumber;				// Higher number of range
unsigned long long guesses;						// Number of guesses
unsigned long long guessesleft;					// Number of guesses remaining
unsigned long long number;						// Guess
unsigned long long lowerHelp;					// Used by Help
unsigned long long higherHelp;					// Used by Help
bool gameStarted = false;						// Tells WM_PAINT whether the game has started or not

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUESSTHENUMBERGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUESSTHENUMBERGUI));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GUESSTHENUMBERGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetClientRect(hWnd, &window);
	wchar_t fn[10], sn[10], ng[10], g[10];
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_START:
				if (!gameStarted)
				{
					GetWindowTextW(hFN, fn, 10);
					GetWindowTextW(hSN, sn, 10);
					GetWindowTextW(hNGuesses, ng, 10);
					if (std::wstring(fn) == L"" || std::wstring(sn) == L"" || std::wstring(ng) == L"")
					{
						int i = (std::wstring(fn) == L"") + (std::wstring(sn) == L"") + (std::wstring(ng) == L"");
						MessageBoxW(hWnd, L"Enter a whole number in each box", (L"Boxes left empty: " + std::to_wstring(i)).c_str(), MB_OK);
						break;
					}
					if (std::stoull(ng) == 0)
					{
						MessageBoxW(hWnd, L"Number of guesses cannot be 0", L"No guesses?", MB_OK);
						break;
					}
					lowerNumber = min(std::stoull(fn), std::stoull(sn));
					higherNumber = max(std::stoull(fn), std::stoull(sn));
					lowerHelp = lowerNumber;
					higherHelp = higherNumber;
					guesses = std::stoull(ng);
					guessesleft = guesses;
					std::wstring msg = std::wstring(L"Range: " + std::to_wstring(lowerNumber) + L" to " + std::to_wstring(higherNumber) + L"\n" + L"Guesses: " + std::to_wstring(guesses));
					gameStarted = true;
					DeleteObject(fn);
					DeleteObject(sn);
					DeleteObject(ng);
					DestroyWindow(hRange);
					DestroyWindow(hFN);
					DestroyWindow(hTo);
					DestroyWindow(hSN);
					DestroyWindow(hGuesses);
					DestroyWindow(hNGuesses);
					DestroyWindow(hStart);
					std::random_device rd;     // only used once to initialise (seed) engine
					std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
					std::uniform_int_distribution<unsigned long long> uni(lowerNumber, higherNumber);
					number = uni(rng);
					SendMessageW(hWnd, (UINT)WM_CREATE, (WPARAM)0, (LPARAM)0);
				}
				else if (gameStarted)
				{
					std::wstring msg = std::wstring(L"Range: " + std::to_wstring(lowerNumber) + L" to " + std::to_wstring(higherNumber) + L"\n" + L"Guesses left: " + std::to_wstring(guessesleft));
					MessageBoxW(hWnd, msg.c_str(), L"Range and Guesses Left", MB_OK);
				}
				break;
			case IDM_GUESS:
				if (gameStarted)
				{
					GetWindowTextW(hEnterGuess, g, 10);
					if (std::wstring(g) == L"")
					{
						break;
					}
					guessesleft--;
					std::wstring hol = L"";
					std::wstring title = L"";
					SetWindowTextW(hNGuessesLeft, std::to_wstring(guessesleft).c_str());
					if (std::stoull(g) == number)
					{
						double Scor = 100000 * log(higherNumber - lowerNumber + 1) / log(2) / (guesses - guessesleft);
						double score = ceil(Scor) / 100;
						title = L"You succeeded!";
						hol = L"You succeeded!\nTries used: " + std::to_wstring(guesses - guessesleft) + L"\nScore: " + std::to_wstring(score);
						MessageBoxW(hWnd, hol.c_str(), title.c_str(), MB_ICONINFORMATION);
						gameStarted = false;
						DestroyWindow(hGuess);
						DestroyWindow(hEnterGuess);
						DestroyWindow(hGuessButton);
						SendMessage(hWnd, (UINT)WM_CREATE, (WPARAM)0, (LPARAM)0);
					}
					else if (guessesleft == 0)
					{
						title = L"You failed!";
						hol = L"You failed!\nThe number was " + std::to_wstring(number) + L"\nTries used: " + std::to_wstring(guesses - guessesleft) + L"\nScore: 0";
						MessageBoxW(hWnd, hol.c_str(), title.c_str(), MB_ICONERROR);
						gameStarted = false;
						DestroyWindow(hGuess);
						DestroyWindow(hEnterGuess);
						DestroyWindow(hGuessButton);
						SendMessage(hWnd, (UINT)WM_CREATE, (WPARAM)0, (LPARAM)0);
					}
					else if (std::stoull(g) > number)
					{
						title = L"Lower";
						hol = L"The number is less than " + std::wstring(g) + L"\nTries left: " + std::to_wstring(guessesleft);
						MessageBoxW(hWnd, hol.c_str(), title.c_str(), MB_OK);
						higherHelp = std::stoull(g);
					}
					else if (std::stoull(g) < number)
					{
						title = L"Higher";
						hol = L"The number is more than " + std::wstring(g) + L"\nTries left: " + std::to_wstring(guessesleft);
						MessageBoxW(hWnd, hol.c_str(), title.c_str(), MB_OK);
						lowerHelp = std::stoull(g);
					}
				}
				else
				{
					MessageBoxW(hWnd, L"A game has not been started yet", L"Game not started", MB_OK);
				}
				break;
			case IDM_HELP:
				if (!gameStarted)
				{
					SetWindowTextW(hFN, L"1");
					SetWindowTextW(hSN, L"100");
					SetWindowTextW(hNGuesses, L"10");
					int choice = MessageBoxW(hWnd, L"Guess a random number in a specified range with a specified amount of guesses\n\nPress OK to start", L"Start Game", MB_OKCANCEL);
					if (choice == IDOK)
					{
						SendMessage(hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_START, (LPARAM)0);
					}
				}
				else if (gameStarted)
				{
					unsigned long long goodguess = (lowerHelp + higherHelp) / 2;
					SetWindowTextW(hEnterGuess, std::to_wstring(goodguess).c_str());
					int choice = MessageBoxW(hWnd, (L"Guess the random number between " + std::to_wstring(lowerNumber) + L" and " + std::to_wstring(higherNumber) + L" in " + std::to_wstring(guessesleft) + L" guesses\n\nPress OK to guess").c_str(), L"Enter Guess", MB_OKCANCEL);
					if (choice == IDOK)
					{
						SendMessage(hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_GUESS, (LPARAM)0);
					}
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		if (!gameStarted)
		{
			hGTN = CreateWindowExW(0L, L"static", L"Guess The Number", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 0, window.right, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hRange = CreateWindowExW(0L, L"static", L"Range:", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hFN = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hTo = CreateWindowExW(0L, L"static", L" to ", WS_VISIBLE | WS_CHILD | ES_CENTER, 2 * window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hSN = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 3 * window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hGuesses = CreateWindowExW(0L, L"static", L"Number of Guesses:", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 5 * window.bottom / 8, 2 * window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hNGuesses = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 2 * window.right / 4, 5 * window.bottom / 8, 2 * window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hStart = CreateWindowExW(0L, L"Button", L"Start", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 7 * window.bottom / 8, window.right, window.bottom / 8, hWnd, (HMENU)IDM_START, NULL, NULL);
			SendMessageW(hFN, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
			SendMessageW(hSN, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
			SendMessageW(hNGuesses, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
		}
		else if (gameStarted)
		{
			hGuess = CreateWindowExW(0L, L"static", L"Enter Guess:", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 5 * window.bottom / 16, window.right / 2, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hEnterGuess = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, window.right / 2, 5 * window.bottom / 16, window.right / 2, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hGuessesLeft = CreateWindowExW(0L, L"static", L"Guesses Left:", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 10 * window.bottom / 16, window.right / 2, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hNGuessesLeft = CreateWindowExW(0L, L"static", std::to_wstring(guessesleft).c_str(), WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 10 * window.bottom / 16, window.right / 2, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hGuessButton = hStart = CreateWindowExW(0L, L"Button", L"Guess", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, 0, 7 * window.bottom / 8, window.right, window.bottom / 8, hWnd, (HMENU)IDM_GUESS, NULL, NULL);
			SendMessageW(hEnterGuess, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
		}
	case WM_PAINT:
        {
			int length = window.right > window.bottom ? window.bottom : window.right;
			int font = (int)120 * length / 534;
			if (font > 200)
				font = 200;
			else if (font > 150)
				font = 150;
			else if (font > 100)
				font = 100;
			else if (font > 50)
				font = 50;
			else
				font = 30;
			HFONT hFont = CreateFont(font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
			HFONT hFont1 = CreateFont(font / 2, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
			SendMessage(hGTN, (UINT)WM_SETFONT, (WPARAM)hFont, (LPARAM)0);
			if (!gameStarted)
			{
				MoveWindow(hGTN, 0, 0, window.right, window.bottom / 4, true);
				MoveWindow(hRange, 0, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				SendMessage(hRange, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hFN, window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				SendMessage(hFN, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hTo, 2 * window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				SendMessage(hTo, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hSN, 3 * window.right / 4, 5 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				SendMessage(hSN, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hGuesses, 0, 5 * window.bottom / 8, 2 * window.right / 4, window.bottom / 4, true);
				SendMessage(hGuesses, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hNGuesses, 2 * window.right / 4, 5 * window.bottom / 8, 2 * window.right / 4, window.bottom / 4, true);
				SendMessage(hNGuesses, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hStart, 0, 7 * window.bottom / 8, window.right, window.bottom / 8, true);
				SendMessage(hStart, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
			}
			else
			{
				MoveWindow(hGTN, 0, 0, window.right, window.bottom / 4, true);
				MoveWindow(hGuess, 0, 5 * window.bottom / 16, window.right / 2, window.bottom / 4, true);
				SendMessage(hGuess, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hEnterGuess, window.right / 2, 5 * window.bottom / 16, window.right / 2, window.bottom / 4, true);
				SendMessage(hEnterGuess, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hGuessesLeft, 0, 10 * window.bottom / 16, window.right / 2, window.bottom / 4, true);
				SendMessage(hGuessesLeft, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hNGuessesLeft, window.right / 2, 10 * window.bottom / 16, window.right / 2, window.bottom / 4, true);
				SendMessage(hNGuessesLeft, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				MoveWindow(hGuessButton, 0, 7 * window.bottom / 8, window.right, window.bottom / 8, true);
				SendMessage(hGuessButton, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
			}
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CTLCOLORSTATIC:
		if (true)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkColor(hdcStatic, RGB(255, 255, 255));
			return (INT_PTR)hBrush;
		}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}