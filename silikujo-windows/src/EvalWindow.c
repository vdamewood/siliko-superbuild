/* EvalWindow.c: Expression evaluation window
 * Copyright 2012-2021 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <Siliko/StringSource.h>
#include <Siliko/InfixParser.h>

#include "EvalWindow.h"

#include "About.h"
#include "Id.h"

#define OUT_BORDER    8
#define IN_BORDER     4
#define TEXT_HEIGHT   24
#define BUTTON_HEIGHT 28
#define BUTTON_WIDTH  100

#define MIN_HEIGHT    (70 + OUT_BORDER * 2 + TEXT_HEIGHT + BUTTON_HEIGHT + IN_BORDER)
#define MIN_WIDTH     420

#define INPUT_X       OUT_BORDER
#define INPUT_Y       OUT_BORDER
#define INPUT_HT      TEXT_HEIGHT
#define INPUT_WT      (OUT_BORDER * 2)

#define OUTPUT_X      OUT_BORDER
#define OUTPUT_Y      (OUT_BORDER + TEXT_HEIGHT + IN_BORDER)
#define OUTPUT_HT     TEXT_HEIGHT
#define OUTPUT_WT     (OUT_BORDER * 2 + IN_BORDER + BUTTON_WIDTH)

#define BUTTON_X      (BUTTON_WIDTH + OUT_BORDER)
#define BUTTON_Y      OUTPUT_Y
#define BUTTON_HT     BUTTON_HEIGHT
#define BUTTON_WT     BUTTON_WIDTH

void OnCreate(HWND Handle)
{
	CreateWindowEx(
		WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
		INPUT_X, INPUT_Y, MIN_WIDTH - INPUT_WT, INPUT_HT,
		Handle, (HMENU)CALCULATOR_INPUT,
		GetModuleHandle(NULL), NULL);
	CreateWindowEx(
		0, "STATIC", "", WS_CHILD | WS_VISIBLE,
		OUTPUT_X, OUTPUT_Y, MIN_WIDTH - OUTPUT_WT, OUTPUT_HT,
		Handle, (HMENU)CALCULATOR_OUTPUT,
		GetModuleHandle(NULL), NULL);
	CreateWindowEx(
		0, "BUTTON", "Calculate", WS_CHILD | WS_VISIBLE,
		MIN_WIDTH - BUTTON_X, BUTTON_Y, BUTTON_WT, BUTTON_HT,
		Handle, (HMENU)CALCULATOR_BUTTON,
		GetModuleHandle(NULL), NULL);
}


void OnGetMinMaxInfo(HWND hwnd, MINMAXINFO *info)
{
	info->ptMinTrackSize.x = MIN_WIDTH;
	info->ptMinTrackSize.y = MIN_HEIGHT;
}

void OnSize(HWND hwnd)
{
	RECT Area;
	GetClientRect(hwnd, &Area);

	SetWindowPos(GetDlgItem(hwnd, CALCULATOR_INPUT), NULL,
		INPUT_X, INPUT_Y, Area.right - INPUT_WT, INPUT_HT,
		SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hwnd, CALCULATOR_OUTPUT), NULL,
		OUTPUT_X, OUTPUT_Y, Area.right - OUTPUT_WT, OUTPUT_HT,
		SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hwnd, CALCULATOR_BUTTON), NULL,
		Area.right - BUTTON_X, BUTTON_Y, BUTTON_WT, BUTTON_HT,
		SWP_NOZORDER);
}


void OnCalculate(HWND hwnd)
{
	int ExpressionSize;
	char *tmpString;
	SilikoSyntaxTreeNode *Node;
	SilikoValue Value;

	ExpressionSize = GetWindowTextLength(GetDlgItem(hwnd, CALCULATOR_INPUT)) + 1;
	tmpString = (char*)GlobalAlloc(GPTR, ExpressionSize);
	GetDlgItemText(hwnd, CALCULATOR_INPUT, tmpString, ExpressionSize);

	Node = SilikoParseInfix(SilikoStringSourceNew(tmpString));
	GlobalFree((HANDLE)tmpString);

	Value = SilikoSyntaxTreeEvaluate(Node);
	free(Node);

	tmpString = SilikoValueToString(Value);
	SetDlgItemText(hwnd, CALCULATOR_OUTPUT, tmpString);
	free(tmpString);
}

LRESULT CALLBACK EvalWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		OnCreate(hwnd);
		return 0;
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo(hwnd, (MINMAXINFO*)lParam);
		return 0;
	case WM_SIZE:
		OnSize(hwnd);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
        case WM_COMMAND:
        	switch(LOWORD(wParam))
        	{
			case CALCULATOR_BUTTON:
				OnCalculate(hwnd);
				return 0;
			case HELP_ABOUT:
				DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(ABOUT_DIALOG), hwnd, (DLGPROC)AboutDialogProc);
				return 0;
			case CALCULATOR_EXIT:
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				return 0;
			default:
				return 0;
			}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

ATOM EvalWindowRegister(HINSTANCE hInstance)
{
	WNDCLASSEX CalculatorClass;

	CalculatorClass.cbSize = sizeof(WNDCLASSEX);
	CalculatorClass.style = 0;
	CalculatorClass.lpfnWndProc = EvalWindowProc;
	CalculatorClass.cbClsExtra = 0;
	CalculatorClass.cbWndExtra = 0;
	CalculatorClass.hInstance = hInstance;
	CalculatorClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	CalculatorClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	CalculatorClass.lpszMenuName = MAKEINTRESOURCE(EVAL_WINDOW_MENU);
	CalculatorClass.lpszClassName = "EvalWindow";
	CalculatorClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	CalculatorClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return RegisterClassEx(&CalculatorClass);
}

HWND EvalWindowCreate(HINSTANCE hInstance)
{
	return CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EvalWindow",
		"Silikujo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, MIN_WIDTH, MIN_HEIGHT,
		NULL, NULL, hInstance, NULL);
}

int EvalWindowPretranslateMessage(HWND Handle, MSG *Message)
{
	if (GetDlgCtrlID(Message->hwnd) == CALCULATOR_INPUT
			&& Message->message == WM_KEYDOWN
			&& Message->wParam == VK_RETURN)
		{
			PostMessage(Handle, WM_COMMAND, CALCULATOR_BUTTON, 0);
			return TRUE;
		}

	return FALSE;
}
