#include "pch.h"
#include "MenuScene.h"
#include "InputManager.h"
#include "SceneManager.h"

extern int32 windowWidth;
extern int32 windowLength;

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
}

void MenuScene::Update()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar))
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::GameScene);
}

void MenuScene::Render(HDC hdc)
{
	wstring str = std::format(L"Press \'Space Bar\' to Start");
	::TextOut(hdc, 280, 700, str.c_str(), static_cast<int>(str.size()));
}
