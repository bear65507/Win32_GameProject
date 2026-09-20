#pragma once
#include "Object.h"

class Effect : public Object
{
public:
	Effect();
	virtual ~Effect() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	float _lifeTime = 0.0f; // 생존 시간을 기록할 변수
};

