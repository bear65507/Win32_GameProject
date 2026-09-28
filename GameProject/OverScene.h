#pragma once
#include "Scene.h"
class OverScene : public Scene
{
public:
	OverScene();
	virtual ~OverScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

