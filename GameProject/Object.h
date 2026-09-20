#pragma once

enum class ObjectType
{
	None,
	Effect,
	Player,
	Monster,
	Projectile,
};

class Object
{
public:
	Object(ObjectType type);
	virtual ~Object();

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC hdc) abstract;

public:
	ObjectType GetObjectType() { return _type; }

	Pos		GetPos() { return _pos; }
	void	SetPos(Pos pos) { this->_pos = pos; }

	Stat& GetStat() { return _stat; }

protected:
	ObjectType	_type = ObjectType::None;
	Stat		_stat = {};
	Pos			_pos = {};
};

