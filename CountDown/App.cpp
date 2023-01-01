#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <random>
#include <thread>
#include <conio.h>
#include <dxgi.h>

void gotoxy(SHORT posX, SHORT posY) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position;
	Position.X = posX;
	Position.Y = posY;

	SetConsoleCursorPosition(hStdout, Position);
}

void showCur(bool CursorVisibility) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 1; // size of cursor 1 - 100
	ConCurInf.bVisible = CursorVisibility;
	// show if true, otherwise hide the cursor

	SetConsoleCursorInfo(handle, &ConCurInf);
} // show cursor or hide

void disableResizeWindow() {
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE,
		GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void setWindowSize(SHORT width, SHORT height) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void showScrollBar(bool _show) {
	HWND _hwnd = GetConsoleWindow();
	ShowScrollBar(_hwnd, SB_BOTH, _show);
}

void setFullScreen() {
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	showScrollBar(0);
}

void setFont(int _fontsize) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = _fontsize;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

int buttonCancle(char& ch) {
	while (ch != 27 && ch != 10) {
		ch = _getch();
	}
	exit(1);
}

void setColor(int background_color, int text_color) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color_code = background_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

int main() {
	char ch = ' ';
	int randColor;
	std::thread t1(buttonCancle, std::ref(ch));
	std::thread t2([&]() {
		while (1) {
			srand(time(NULL));
			randColor = rand() % 14 + 1;
			setColor(0, randColor);
		}});
	setFullScreen();
	setFont(76);
	showCur(0); 
	int A[11];
	for (int i = 0; i < 11; ++i)
		A[i] = i;
	int i = 10;
	while (i >= 0) {
		gotoxy(20, 5);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "       ";
		gotoxy(20, 5);
		switch (A[i--]) {
		case 0:
			std::cout << "ZERO";
			gotoxy(20, 5);
			std::cout << "    ";
			break;
		case 1:
			std::cout << "ONE";
			break;
		case 2:
			std::cout << "TWO";
			break;
		case 3:
			std::cout << "THREE";
			break;
		case 4:
			std::cout << "FOUR";
			break;
		case 5:
			std::cout << "FIVE";
			break;
		case 6:
			std::cout << "SIX";
			break;
		case 7:
			std::cout << "SEVEN";
			break;
		case 8:
			std::cout << "EIGHT";
			break;
		case 9:
			std::cout << "NINE";
			break;
		case 10:
			std::cout << "TEN";
			break;
		}
	}
	int x = 15, y = 1, action = 1, _sleepTime = rand() % 150 + 50, count = 0;
	while (1) {
		gotoxy(x, y);
		std::cout << "HAPPY NEW YEAR";
		std::this_thread::sleep_for(std::chrono::milliseconds(_sleepTime));
		if (((action == 0 || action == 4 || action == 5) && y <= 0) || 
			((action == 1 || action == 6 || action == 7) && y >= 10) || 
			((action == 2 || action == 4 || action == 7) && x <= 0) || 
			((action == 3 || action == 5 || action == 6) && x >= 28)) {
		randomSpot:
			action = rand() % 8;
			if (((action == 0 || action == 4 || action == 5) && y <= 0) ||
				((action == 1 || action == 6 || action == 7) && y >= 10) ||
				((action == 2 || action == 4 || action == 7) && x <= 0) ||
				((action == 3 || action == 5 || action == 6) && x >= 28))
				goto randomSpot;
			if (++count == 5) {
				count = 0;
				_sleepTime = rand() % 150 + 50;
			}
		}
		gotoxy(x, y);
		std::cout << "              ";
		switch (action) {
		case 0: --y;
			break;
		case 1: ++y;
			break;
		case 2: --x;
			break;
		case 3: ++x;
			break;
		case 4: --y, --x;
			break;
		case 5: --y, ++x;
			break;
		case 6: ++y, ++x;
			break;
		case 7: ++y, --x;
			break;
		}
	}
	if (t1.joinable())
		t1.join();
	if (t2.joinable())
		t2.join();
	return 0;
}