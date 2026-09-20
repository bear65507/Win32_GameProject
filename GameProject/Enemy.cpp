#include "pch.h"
#include "Enemy.h"
#include "TimeManager.h"
#include "ObjectManager.h"

extern int32 windowWidth;
extern int32 windowLength;

Enemy::Enemy() : Object(ObjectType::Monster)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	_stat.hp = 50;
	_stat.maxHp = 50;
	_stat.speed = 300;
	_stat.damage = 10;

	_dir = Vector(0.0f, 1.0f);
	_isRandomMoving = false;
}

void Enemy::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	// 1. 이동 적용 (직진이든 무작위든 공통 적용)
	_pos += _dir * (_stat.speed * deltaTime);

	// 2. Y좌표가 200에 도달하기 전이라면 무작위 방향으로 전환
	if (!_isRandomMoving)
	{
		if (_pos.y >= 200.0f)
		{
			_isRandomMoving = true; // 무작위 이동 모드 온
			_stat.speed = 200;

			float rx = static_cast<float>(rand() % 100 - 50);
			float ry = static_cast<float>(rand() % 100 - 50);

			if (rx == 0.0f && ry == 0.0f)
				rx = 1.0f;

			_dir = Vector(rx, ry);
			_dir.Normalize();
		}
	}
	else
	{
		// 3. 무작위 이동 모드일 때만 경계 충돌 및 몬스터 간 충돌 처리
		float margin = 25.0f;

		// 화면 경계 충돌 처리
		if (_pos.x < margin)
		{
			_pos.x = margin;
			_dir.x *= -1.0f;
		}
		else if (_pos.x > windowWidth - margin)
		{
			_pos.x = windowWidth - margin;
			_dir.x *= -1.0f;
		}

		if (_pos.y < margin)
		{
			_pos.y = margin;
			_dir.y *= -1.0f;
		}
		else if (_pos.y > 550.0f - margin)
		{
			_pos.y = 550.0f - margin;
			_dir.y *= -1.0f;
		}

		// 몬스터 간의 충돌 방지 및 반사 로직
		const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();
		for (Object* obj : objects)
		{
			if (obj == this)
				continue;

			Enemy* otherEnemy = dynamic_cast<Enemy*>(obj);
			if (otherEnemy != nullptr)
			{
				Vector diff = _pos - otherEnemy->_pos;
				float dist = diff.Length();
				float minDist = 50.0f;

				if (dist < minDist && dist > 0.0f)
				{
					diff.Normalize();
					float overlap = minDist - dist;

					_pos += diff * (overlap * 0.5f);
					otherEnemy->_pos -= diff * (overlap * 0.5f);

					Vector tempDir = _dir;
					_dir = otherEnemy->_dir;
					otherEnemy->_dir = tempDir;
				}
			}
		}
	}
}

void Enemy::Render(HDC hdc)
{
	// Enemy를 빨간색으로
	HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)::SelectObject(hdc, pen);
	{
		Utils::DrawTriangleInvert(hdc, _pos, 50);
	}
	::SelectObject(hdc, oldPen);
	::DeleteObject(pen);
}
