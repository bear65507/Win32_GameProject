#include "pch.h"
#include "Missile.h"
#include "TimeManager.h"
#include "ObjectManager.h"

Missile::Missile() : Object(ObjectType::Projectile)
{
}

Missile::~Missile()
{
}

void Missile::Init()
{
	_stat.hp = 1;
	_stat.maxHp = 1;
	_stat.speed = 600;
}

void Missile::Update()
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
			GET_SINGLE(ObjectManager)->Remove(object);
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

void Missile::Render(HDC hdc)
{
	Utils::DrawCircle(hdc, _pos, 10);
}
