#pragma once
#include "Object.h"

class Guns : public Object
{
public:
	Guns();
	virtual ~Guns() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

