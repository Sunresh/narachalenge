#include <opencv2/opencv.hpp>
#include <windows.h>
#include <CommCtrl.h>
#include "nara.h"
#include "element.cpp"

HINSTANCE hInst;
cv::VideoCapture cap;  // Declare VideoCapture outside to keep the capture open
HWND hVideoWindow;  // Handle for the video display window
bool isCameraRunning = false;

void ResizeVideoWindow(HWND hwnd, cv::Mat& frame)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Resize the video window to fit the client area
	MoveWindow(hVideoWindow, 0, 0, clientRect.right, clientRect.bottom, TRUE);

	// Resize and display the current frame
	cv::Mat resizedFrame;
	cv::resize(frame, resizedFrame, cv::Size(clientRect.right, clientRect.bottom));

	// Convert OpenCV Mat to Windows BITMAP
	BITMAPINFOHEADER bmInfo;
	bmInfo.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.biWidth = resizedFrame.cols;
	bmInfo.biHeight = -resizedFrame.rows;  // Negative height to flip the image
	bmInfo.biPlanes = 1;
	bmInfo.biBitCount = 24;  // 3 channels (RGB)
	bmInfo.biCompression = BI_RGB;
	bmInfo.biSizeImage = 0;
	bmInfo.biXPelsPerMeter = 0;
	bmInfo.biYPelsPerMeter = 0;
	bmInfo.biClrUsed = 0;
	bmInfo.biClrImportant = 0;

	HDC hdc = GetDC(hVideoWindow);
	SetDIBitsToDevice(
		hdc,
		0, 0,
		resizedFrame.cols, resizedFrame.rows,
		0, 0,
		0, resizedFrame.rows,
		resizedFrame.data,
		(BITMAPINFO*)&bmInfo,
		DIB_RGB_COLORS
	);
	ReleaseDC(hVideoWindow, hdc);
}

void CaptureAndShowVideo()
{
	cv::Mat frame;
	cap >> frame;  // Capture a frame from the camera

	if (!frame.empty())
	{
		ResizeVideoWindow(hVideoWindow, frame);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		Element maya(hwnd, uMsg, wParam, lParam);
		hVideoWindow = CreateWindowW(L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 10, 480, 360, hwnd, nullptr, hInst, nullptr);
		SetTimer(hwnd, 1, 30, NULL);  // Set a timer to update video every 30 milliseconds
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1)  // Start Camera button clicked
		{
			if (!isCameraRunning)
			{
				cap.open(0);
				isCameraRunning = true;
			}
		}
		else if (LOWORD(wParam) == 2)  // Stop Camera button clicked
		{
			if (isCameraRunning)
			{
				cap.release();
				isCameraRunning = false;
			}
		}
	}
	break;
	case WM_TIMER:
	{
		if (wParam == 1 && isCameraRunning)
		{
			CaptureAndShowVideo();
		}
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (isCameraRunning)
		{
			// Resize the video window when the main window is resized
			CaptureAndShowVideo();
		}
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Register the window class
	hInst = hInstance;
	const char CLASS_NAME[] = "CMakeProject1";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the main window
	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"CMake Project",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 520, 460,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
