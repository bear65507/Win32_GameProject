#include "pch.h"
#include "Effect.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

// ObjectType에 Effect가 정의되어 있지 않다면 None이나 기타 타입으로 설정해 두셔도 무방합니다.
Effect::Effect() : Object(ObjectType::Effect)
{
}

Effect::~Effect()
{
}

void Effect::Init()
{
	_lifeTime = 0.0f;
	_image = GET_SINGLE(ResourceManager)->GetImage(L"Effect");
}

void Effect::Update()
{
	_lifeTime += GET_SINGLE(TimeManager)->GetDeltaTime();

	// 생성된 지 0.5초가 지나면 스스로를 매니저에게서 삭제 요청
	if (_lifeTime >= 0.1f)
	{
		GET_SINGLE(ObjectManager)->Remove(this);
	}
}

void Effect::Render(HDC hdc)
{
	if (_image != nullptr)
	{
		Gdiplus::Graphics graphics(hdc);
		int width = _image->GetWidth();
		int height = _image->GetHeight();

		// 객체의 중심 좌표를 기준으로 이펙트 이미지 출력
		graphics.DrawImage(_image,
			static_cast<int>(_pos.x) - width / 2,
			static_cast<int>(_pos.y) - height / 2,
			width, height);
	}
}