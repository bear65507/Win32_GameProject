#include "pch.h"
#include "ResourceManager.h"

void ResourceManager::Init()
{
	// 게임 시작 시 사용할 리소스들을 미리 로드해 둡니다.
	LoadImageW(L"Player", L"../Resource/player.png");
	LoadImageW(L"Enemy", L"../Resource/enemy.png");
	LoadImageW(L"Guns", L"../Resource/Guns.png");
	LoadImageW(L"EnemyGuns", L"../Resource/EnemyGuns.png");
}

void ResourceManager::Clear()
{
	for (auto& pair : _images)
	{
		if (pair.second != nullptr)
		{
			delete pair.second;
		}
	}
	_images.clear();
}

Gdiplus::Image* ResourceManager::LoadImageW(const wstring& key, const wstring& path)
{
	// 이미 로드된 이미지가 있다면 그것을 반환
	if (_images.find(key) != _images.end())
		return _images[key];

	// new Gdiplus::Image 대신 FromFile 메서드를 사용하여 생성합니다.
	Gdiplus::Image* image = Gdiplus::Image::FromFile(path.c_str());

	_images[key] = image;

	return image;
}

Gdiplus::Image* ResourceManager::GetImage(const wstring& key)
{
	if (_images.find(key) != _images.end())
		return _images[key];

	return nullptr;
}