#pragma once
#include <unordered_map>
#include <string>
#include <gdiplus.h>

class ResourceManager
{
public:
	DECLARE_SINGLE(ResourceManager);
	~ResourceManager() { Clear(); }

public:
	void Init();
	void Clear();

	// 이미지를 로드하고 맵에 저장
	Gdiplus::Image* LoadImageW(const wstring& key, const wstring& path);

	// 키 값을 통해 저장된 이미지 반환
	Gdiplus::Image* GetImage(const wstring& key);

private:
	unordered_map<wstring, Gdiplus::Image*> _images;
};

