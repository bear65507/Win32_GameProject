#include "pch.h"
#include "Guns.h"
#include "Effect.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

extern int32 g_score;

Guns::Guns() : Object(ObjectType::Projectile)
{
}

Guns::~Guns()
{
}

void Guns::Init()
{
	_stat.speed = 600;
	_stat.damage = 20;

	_image = GET_SINGLE(ResourceManager)->GetImage(L"Guns");
}

void Guns::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	_pos.y -= deltaTime * _stat.speed;

	const vector<Object*> objects = GET_SINGLE(ObjectManager)->GetObjects();
	const vector<Object*>& currentObjects = GET_SINGLE(ObjectManager)->GetObjects();

	// 충돌 검사
	for (Object* object : objects)
	{
		if (object == this)
			continue;

		// 다른 미사일에 의해 이미 삭제된 몬스터인지 검사
		if (std::find(currentObjects.begin(), currentObjects.end(), object) == currentObjects.end())
			continue;

		if (object->GetObjectType() != ObjectType::Monster)
			continue;

		Pos p1 = GetPos();
		Pos p2 = object->GetPos();

		const float dx = p1.x - p2.x;
		const float dy = p1.y - p2.y;
		float dist = sqrt(dx * dx + dy * dy);

		if (dist < 25)
		{
			// 피격 이펙트
			Effect* hitEffect = GET_SINGLE(ObjectManager)->CreateObject<Effect>();
			hitEffect->SetPos(_pos);
			GET_SINGLE(ObjectManager)->Add(hitEffect);

			object->GetStat().hp -= _stat.damage;

			if (object->GetStat().hp <= 0)
			{
				GET_SINGLE(ObjectManager)->Remove(object);
				g_score += 200;
			}
			GET_SINGLE(ObjectManager)->Remove(this);
			return; // 나 자신(미사일)이 삭제되었으므로 즉시 함수 종료
		}
	}

	// 화면 밖으로 나가면 삭제
	if (_pos.y < -200)
	{
		GET_SINGLE(ObjectManager)->Remove(this);
		return;
	}
}

void Guns::Render(HDC hdc)
{
	if (_image != nullptr)
	{
		Gdiplus::Graphics graphics(hdc);
		int width = _image->GetWidth();
		int height = _image->GetHeight();

		// 객체의 중심 좌표를 기준으로 이미지 출력
		graphics.DrawImage(_image,
			static_cast<int>(_pos.x) - width / 2,
			static_cast<int>(_pos.y) - height / 2,
			width, height);
	}
}
