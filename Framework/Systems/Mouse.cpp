#include "Framework.h"
#include "Mouse.h"

Mouse::Mouse()
{
	position = D3DXVECTOR3(0, 0, 0);

	wheelStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelOldStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelMoveValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	// 현재 Windows에 설정되어 있는 더블클릭 시간 간격 반환
	timeDblClk = GetDoubleClickTime();
	// OS부팅후 지난 시간 반환 (msec 단위)
	startDblClk[0] = GetTickCount();

	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	// 파라미터를 통해 윈도우즈 시스템 설정을 변경하거나 현재 설정 값을 얻어오는 함수
	// 휠에 대한 정보를 얻어옴 
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	// 마우스 왼쪽, 오른쪽, 가운데 버튼이 눌렸는지 안눌렸는지 확인
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// 이전에 button을 눌렀는지 안누렀는지 상태와 비교하여 계속 눌렀는지 아닌지 확인 
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	// 마우스 위치 이동
	POINT point;
	GetCursorPos(&point);
	// 전체 화면 기준인 커서의 좌표를 해당 클라이언트 기준으로 좌표를 변환 
	ScreenToClient(handle, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = float(point.x);
	wheelStatus.y = float(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus;


	// 눌렀을 때의 시간을 tButtonStatus에 임시로 저장
	DWORD tButtonStatus = GetTickCount();
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		// 버튼을 눌렀을 때
		if (buttonMap[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				// 더블 클릭 간격보다 클 경우 다시 0으로
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDblClk[i] = tButtonStatus;
		}

		// 버튼을 땟을 때
		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				// 더블 클릭 간격보다 클 경우 다시 0으로
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				// 더블 클릭
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}


// lparam : 마우스 위치(스크린 좌표값)
// wparam : 하위 워드에는 마우스 버튼, shift, ctrl 키의 상태를 나타내는 플래그 값, 상위 워드는 휠을 얼마나 돌렸는지
LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		position.x = (float)LOWORD(lParam);
		position.y = (float)(WinMaxHeight - HIWORD(lParam));
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += (float)tWheelValue;
	}

	return TRUE;
}
