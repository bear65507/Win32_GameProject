#pragma once
#include "Object.h"

class EnemyGuns : public Object
{
public:
	EnemyGuns();
	virtual ~EnemyGuns() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void SetDir(Vector dir) { _dir = dir; }

private:
	Vector _dir; // 타겟(플레이어)을 향하는 방향 벡터
	Gdiplus::Image* _image = nullptr;
};