#include "pch.h"
#include "EnemyGuns.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include <algorithm>

extern int32 windowWidth;
extern int32 windowLength;

EnemyGuns::EnemyGuns() : Object(ObjectType::Projectile)
{
}

EnemyGuns::~EnemyGuns()
{
}

void EnemyGuns::Init()
{
	_stat.speed = 400; // 적 총알 속도
	_stat.damage = 10; // 요구하신 데미지 10 적용
}

void EnemyGuns::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	// 지정된 방향(_dir)으로 이동
	_pos += _dir * (_stat.speed * deltaTime);

	const vector<Object*> objects = GET_SINGLE(ObjectManager)->GetObjects();
	const vector<Object*>& currentObjects = GET_SINGLE(ObjectManager)->GetObjects();

	// 플레이어 충돌 검사
	for (Object* object : objects)
	{
		if (object == this)
			continue;

		if (std::find(currentObjects.begin(), currentObjects.end(), object) == currentObjects.end())
			continue;

		// 플레이어 타입만 피격 판정
		if (object->GetObjectType() != ObjectType::Player)
			continue;

		Pos p1 = GetPos();
		Pos p2 = object->GetPos();

		const float dx = p1.x - p2.x;
		const float dy = p1.y - p2.y;
		float dist = sqrt(dx * dx + dy * dy);

		// 플레이어 크기(70)의 절반을 고려하여 충돌 반경 35로 설정
		if (dist < 35)
		{
			// 플레이어 체력 차감
			object->GetStat().hp -= _stat.damage;

			// 총알은 삭제하고 함수 종료
			GET_SINGLE(ObjectManager)->Remove(this);
			return;
		}
	}

	// 화면 밖으로 나가면 삭제 (상하좌우 모두 검사)
	if (_pos.y < -100 || _pos.y > windowLength + 100 || _pos.x < -100 || _pos.x > windowWidth + 100)
	{
		GET_SINGLE(ObjectManager)->Remove(this);
		return;
	}
}

void EnemyGuns::Render(HDC hdc)
{
	// 적 총알은 구분을 위해 보라색 원으로 그립니다.
	HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(128, 0, 128));
	HPEN oldPen = (HPEN)::SelectObject(hdc, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(128, 0, 128));
	HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, brush);

	Utils::DrawCircle(hdc, _pos, 8);

	::SelectObject(hdc, oldPen);
	::SelectObject(hdc, oldBrush);
	::DeleteObject(pen);
	::DeleteObject(brush);
}