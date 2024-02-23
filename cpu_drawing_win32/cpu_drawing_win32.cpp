// Creating the window and bitmap
WNDCLASSA wc = { 0 };
wc.lpfnWndProc = wndProc;
wc.hInstance = GetModuleHandle(0);
wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.lpszClassName = WINDOW_CLASS;

if (!RegisterClassA(&wc)) {
    return 1;
}

DWORD dwStyle =
    WS_CAPTION
    | WS_THICKFRAME
    | WS_MINIMIZEBOX
    | WS_MAXIMIZEBOX
    | WS_SYSMENU
    | WS_VISIBLE;

RECT wr = { 0, 0, width * scale, height * scale };
AdjustWindowRect(&wr, dwStyle, FALSE);

hWnd = CreateWindowA(wc.lpszClassName, "MyWindow", dwStyle,
    0, 0, (wr.right - wr.left), (wr.bottom - wr.top), 0, 0, GetModuleHandle(0), 0);
if (!hWnd) {
    return 2;
}
image_width = width;
image_height = height;

RECT rc_client;
GetClientRect(hWnd, &rc_client);
window_width = rc_client.right - rc_client.left;
window_height = rc_client.bottom - rc_client.top;

HDC hdcScreen = GetDC(NULL);

BITMAPINFO bmi = {};
bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bmi.bmiHeader.biWidth = image_width;
bmi.bmiHeader.biHeight = image_height;
bmi.bmiHeader.biPlanes = 1;
bmi.bmiHeader.biBitCount = 32;
bmi.bmiHeader.biCompression = BI_RGB;

hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&backbuffer), 0, 0);
ReleaseDC(NULL, hdcScreen);


// Messages to handle
case WM_SIZE: {
    RECT rc_client;
    GetClientRect(hWnd, &rc_client);
    window_width = rc_client.right - rc_client.left;
    window_height = rc_client.bottom - rc_client.top;
    break;
}
case WM_PAINT: {
    ValidateRect(hWnd, 0);
    break;
}


// Copy the bitmap to the window
HDC hdc = GetDC(hWnd);
HDC hdc_bmp = CreateCompatibleDC(hdc);
auto old_bmp = SelectObject(hdc_bmp, hbm);
StretchBlt(hdc, 0, 0, window_width, window_height, hdc_bmp, 0, 0, image_width, image_height, SRCCOPY);
//BitBlt(hdc, 0, 0, client_width, client_height, hdc_bmp, 0, 0, SRCCOPY);
SelectObject(hdc, old_bmp);
DeleteDC(hdc_bmp);
ReleaseDC(hWnd, hdc);
ValidateRect(hWnd, 0);


// Draw
uint32_t color = 0xFFFFFFFF;
backbuffer[x + y * image_width] = color;

