#pragma once
#include "Object.h"
#include <gdiplus.h>

class Background : public Object
{
public:
	Background();
	virtual ~Background() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Gdiplus::Image* _image = nullptr;
};
