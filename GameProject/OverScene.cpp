#include "pch.h"
#include "OverScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"

extern int32 windowWidth;
extern int32 windowLength;

OverScene::OverScene()
{
}

OverScene::~OverScene()
{
}

void OverScene::Init()
{
	// 리소스 매니저에서 이미지 로드
	_gameoverImage = GET_SINGLE(ResourceManager)->GetImage(L"GameOver");
	_retryImage = GET_SINGLE(ResourceManager)->GetImage(L"Retry");

	_blinkTimer = 0.0f;
	_showStart = true;
}

void OverScene::Update()
{
	// 1초 간격 깜빡임 로직
	_blinkTimer += GET_SINGLE(TimeManager)->GetDeltaTime();
	if (_blinkTimer >= 1.0f)
	{
		_showStart = !_showStart; // true/false 반전
		_blinkTimer = 0.0f;       // 타이머 초기화
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::R))
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void OverScene::Render(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);

	// 1. 타이틀 배경 그리기 (화면 전체 크기로 출력)
	if (_gameoverImage != nullptr)
	{
		graphics.DrawImage(_gameoverImage, 0, 0, windowWidth, windowLength);
	}

	// 2. 시작 안내 문구 그리기
	if (_retryImage != nullptr && _showStart)
	{
		int startWidth = _retryImage->GetWidth();
		int startHeight = _retryImage->GetHeight();

		// 화면 가로 중앙 계산: (전체 너비 / 2) - (이미지 너비 / 2)
		int xPos = (windowWidth / 2) - (startWidth / 2);
		int yPos = 700;

		graphics.DrawImage(_retryImage, xPos, yPos, startWidth, startHeight);
	}
}
