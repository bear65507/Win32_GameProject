#include "pch.h"
#include "Effect.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Utils.h"

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
	// Utils에 정의된 DrawRect를 사용해 20x20 크기의 사각형을 그립니다.
	Utils::DrawRect(hdc, _pos, 20, 20);
}