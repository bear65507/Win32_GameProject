#include "pch.h"
#include "Enemy.h"

Enemy::Enemy() : Object(ObjectType::Monster)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	_stat.hp = 100;
	_stat.maxHp = 100;
	_stat.speed = 10;
	_stat.damage = 10;
}

void Enemy::Update()
{
}

void Enemy::Render(HDC hdc)
{
	Utils::DrawRect(hdc, _pos, 50, 50);
}
