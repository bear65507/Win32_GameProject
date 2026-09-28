#include "pch.h"
#include "OverScene.h"
#include "InputManager.h"
#include "SceneManager.h"

OverScene::OverScene()
{
}

OverScene::~OverScene()
{
}

void OverScene::Init()
{
}

void OverScene::Update()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::R))
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void OverScene::Render(HDC hdc)
{
	wstring str = std::format(L"Press \'R\' to Retry");
	::TextOut(hdc, 280, 700, str.c_str(), static_cast<int>(str.size()));
}
