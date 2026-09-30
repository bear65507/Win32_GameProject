#pragma once
#include "Scene.h"
#include <gdiplus.h>

class MenuScene : public Scene
{
public:
	MenuScene();
	virtual ~MenuScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Gdiplus::Image* _titleImage = nullptr;
	Gdiplus::Image* _startImage = nullptr;

	float _blinkTimer = 0.0f; // 깜빡임 시간을 측정할 타이머
	bool _showStart = true;   // 이미지 출력 여부를 결정할 플래그
};
