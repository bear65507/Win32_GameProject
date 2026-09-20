#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"

Game::Game()
{
}

Game::~Game()
{
	// 마지막에 해야 효과 있음
	GET_SINGLE(SceneManager)->Clear();

	_CrtDumpMemoryLeaks();
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd; // 윈도우 번호
	_hdc = ::GetDC(hwnd); // 그릴 도화지의 번호

	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hcd와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->Init();

	GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void Game::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	/*
	// 마우스 위치
	{
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0},{1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int>(str.size()));
	}
	*/

	{
		int32 playerHp = 0;
		const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

		// 1. ObjectManager를 순회하며 플레이어 객체를 찾습니다.
		for (Object* object : objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
			{
				playerHp = object->GetStat().hp; // 이전 단계에서 GetStat()을 public으로 만들었다고 가정
				break;
			}
		}

		// HP가 0 미만으로 떨어져 음수 블록을 계산하는 것을 방지
		if (playerHp < 0)
			playerHp = 0;

		// 2. 10 HP당 1개의 █ 블록을 계산하여 문자열을 만듭니다.
		wstring hpBlocks = L"";
		int32 blockCount = playerHp / 10;
		for (int32 i = 0; i < blockCount; ++i)
		{
			hpBlocks += L"█ ";
		}

		wstring labelStr = L"HP : ";
		::TextOut(_hdcBack, 20, 1000, labelStr.c_str(), static_cast<int>(labelStr.size()));

		// 2. "HP : " 문자열의 픽셀 길이를 계산하여 다음에 그릴 X 좌표를 구함
		SIZE size;
		::GetTextExtentPoint32(_hdcBack, labelStr.c_str(), static_cast<int>(labelStr.size()), &size);

		// 3. 텍스트 색상을 빨간색으로 변경하고 체력 바 출력
		COLORREF oldColor = ::SetTextColor(_hdcBack, RGB(185, 238, 132));
		::TextOut(_hdcBack, 20 + size.cx, 1000, hpBlocks.c_str(), static_cast<int>(hpBlocks.size()));
		::SetTextColor(_hdcBack, oldColor);
	}

	{
		wstring str = std::format(L"(FPS {0}, DT({1} ms)", fps, static_cast<int32>(deltaTime * 1000));
		::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int>(str.size()));
	}

	{
		wstring str = std::format(L"(FPS {0}, DT({1} ms)", fps, static_cast<int32>(deltaTime * 1000));
		::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int>(str.size()));
	}

	GET_SINGLE(SceneManager)->Render(_hdcBack);

	// 더블 버퍼링
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
