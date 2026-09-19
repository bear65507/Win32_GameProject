#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "ObjectManager.h"
#include "TimeManager.h"

extern int32 windowWidth;
extern int32 windowLength;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	{
		Player* player = GET_SINGLE(ObjectManager)->CreateObject<Player>();
		player->SetPos(Pos{ static_cast<float>(windowWidth) / 2, static_cast<float>(windowLength) - 200 });
		GET_SINGLE(ObjectManager)->Add(player);
	}

	// 1. 시작 시 5마리 소환으로 변경
	for (int32 i = 0; i < 5; i++)
	{
		Enemy* monster = GET_SINGLE(ObjectManager)->CreateObject<Enemy>();
		// X좌표를 100 ~ 580 정도로 겹치지 않게 분산
		float xPos = 100.0f + (i * 120.0f);
		monster->SetPos(Pos{ xPos, -100.0f });
		GET_SINGLE(ObjectManager)->Add(monster);
	}

	_spawnTimer = 0.0f;
}

void GameScene::Update()
{
	const vector<Object*> objects = GET_SINGLE(ObjectManager)->GetObjects();
	const vector<Object*>& currentObjects = GET_SINGLE(ObjectManager)->GetObjects();

	int32 enemyCount = 0;

	for (Object* object : objects)
	{
		// 1. 객체가 이미 삭제되어 원본 리스트에 없다면 스킵 (Dangling Pointer 방지)
		if (std::find(currentObjects.begin(), currentObjects.end(), object) == currentObjects.end())
			continue;

		// 2. Update 과정에서 미사일이나 몬스터가 삭제될 수 있으므로, 카운트를 먼저 수행합니다.
		if (dynamic_cast<Enemy*>(object) != nullptr)
		{
			enemyCount++;
		}

		object->Update();
	}

	if (enemyCount < 7)
	{
		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_spawnTimer += deltaTime;

		if (_spawnTimer >= 1.5f)
		{
			Enemy* monster = GET_SINGLE(ObjectManager)->CreateObject<Enemy>();
			float randX = static_cast<float>(rand() % 700 + 50);
			monster->SetPos(Pos{ randX, -100.0f });
			GET_SINGLE(ObjectManager)->Add(monster);

			_spawnTimer = 0.0f;
		}
	}
	else
	{
		_spawnTimer = 0.0f;
	}
}

void GameScene::Render(HDC hdc)
{
	const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();
	for (Object* object : objects)
	{
		object->Render(hdc);
	}
}
