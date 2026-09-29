#include "pch.h"
#include "Background.h"
#include "TimeManager.h"
#include "ResourceManager.h"

extern int32 windowWidth;
extern int32 windowLength;

Background::Background() : Object(ObjectType::None)
{
}

Background::~Background()
{
}

void Background::Init()
{
	_stat.speed = 100; // 스크롤 속도
	_pos.x = 0;
	_pos.y = 0; // _pos.y를 스크롤 오프셋으로 사용합니다.

	_image = GET_SINGLE(ResourceManager)->GetImage(L"Background");
}

void Background::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_pos.y += _stat.speed * deltaTime;

	if (_image != nullptr)
	{
		// 1. 이미지가 창 아래로 완전히 벗어나는 기준을 '이미지의 높이'로 변경합니다.
		int height = _image->GetHeight();

		if (_pos.y >= height)
		{
			_pos.y -= height;
		}
	}
}

void Background::Render(HDC hdc)
{
	if (_image != nullptr)
	{
		Gdiplus::Graphics graphics(hdc);
		int width = _image->GetWidth();
		int height = _image->GetHeight();

		// 2. 현재 화면을 채우는 배경
		graphics.DrawImage(_image,
			0, static_cast<int>(_pos.y),
			width, height);

		// 3. 화면 위쪽에서 대기하다가 따라 내려오는 똑같은 배경
		graphics.DrawImage(_image,
			0, static_cast<int>(_pos.y) - height,
			width, height);
	}
}