#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Guns.h"
#include "ObjectManager.h"

extern int32 windowWidth;
extern int32 windowLength;

Player::Player() : Object(ObjectType::Player)
{
}

Player::~Player()
{
}

void Player::Init()
{
	_stat.hp = 120;
	_stat.maxHp = 120;
	_stat.speed = 500;

	_pos.x = 400;
	_pos.y = 500;
}

void Player::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();


	if (GET_SINGLE(InputManager)->GetButton(KeyType::A))
	{
		_pos.x -= _stat.speed * deltaTime;
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::D))
	{
		_pos.x += _stat.speed * deltaTime;
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
	{
		_pos.y -= _stat.speed * deltaTime;
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
	{
		_pos.y += _stat.speed * deltaTime;
	}

	// ==============================================================
	// 위치 제한 코드 추가
	// 렌더링 시 삼각형 크기(75)를 고려해 경계를 넘어가지 않도록 여백을 둡니다.
	// (중심점 기준으로 그려진다면 크기의 절반 정도를 margin으로 설정)
	float margin = 35.0f;

	if (_pos.x < margin)
		_pos.x = margin;
	else if (_pos.x > windowWidth - margin)
		_pos.x = windowWidth - margin;

	if (_pos.y < margin)
		_pos.y = margin;
	else if (_pos.y > windowLength - margin)
		_pos.y = windowLength - margin;
	// ==============================================================

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
	{
		Guns* guns = GET_SINGLE(ObjectManager)->CreateObject<Guns>();
		guns->SetPos(_pos);
		GET_SINGLE(ObjectManager)->Add(guns);
	}
}

void Player::Render(HDC hdc)
{
	Utils::DrawTriangle(hdc, _pos, 70);
}
