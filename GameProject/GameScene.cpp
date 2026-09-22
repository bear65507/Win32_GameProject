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

	{
		int32 playerHp = 0;
		const vector<Object*>& objects = GET_SINGLE(ObjectManager)->GetObjects();

		// 1. ObjectManager를 순회하며 플레이어 객체를 찾습니다.
		for (Object* object : objects)
		{
			if (object->GetObjectType() == ObjectType::Player)
			{
				playerHp = object->GetStat().hp; // 이전 단계에서 GetStat()을 public으로 만들었다고 가정
				break;
			}
		}

		// HP가 0 미만으로 떨어져 음수 블록을 계산하는 것을 방지
		if (playerHp < 0)
			playerHp = 0;

		// 2. 10 HP당 1개의 █ 블록을 계산하여 문자열을 만듭니다.
		wstring hpBlocks = L"";
		int32 blockCount = playerHp / 10;
		for (int32 i = 0; i < blockCount; ++i)
		{
			hpBlocks += L"█ ";
		}

		wstring labelStr = L"HP : ";
		::TextOut(hdc, 20, 1000, labelStr.c_str(), static_cast<int>(labelStr.size()));

		// 2. "HP : " 문자열의 픽셀 길이를 계산하여 다음에 그릴 X 좌표를 구함
		SIZE size;
		::GetTextExtentPoint32(hdc, labelStr.c_str(), static_cast<int>(labelStr.size()), &size);

		// 3. 텍스트 색상을 빨간색으로 변경하고 체력 바 출력
		COLORREF oldColor = ::SetTextColor(hdc, RGB(185, 238, 132));
		::TextOut(hdc, 20 + size.cx, 1000, hpBlocks.c_str(), static_cast<int>(hpBlocks.size()));
		::SetTextColor(hdc, oldColor);
	}
}
