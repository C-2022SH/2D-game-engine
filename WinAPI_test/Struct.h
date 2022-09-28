#pragma once


struct Vector2
{
	float x;
	float y;

public:
	// operators
	// 일반 연산
	Vector2 operator +(const Vector2& _pVec)
	{
		return Vector2(x + _pVec.x, y + _pVec.y);
	}

	Vector2 operator *(const Vector2& _pVec)
	{
		return Vector2(x * _pVec.x, y * _pVec.y);
	}

	Vector2 operator *(const float _i)
	{
		return Vector2(x * _i, y * _i);
	}

	Vector2 operator -(const Vector2& _pVec)
	{
		return Vector2(x - _pVec.x, y - _pVec.y);
	}

	Vector2 operator /(const Vector2& _pVec)
	{
		assert(_pVec.x != 0.f && _pVec.y != 0.f);

		return Vector2(x / _pVec.x, y / _pVec.y);
	}

	Vector2 operator /(const float _f)
	{
		assert(_f != 0.f);

		return Vector2(x / _f, y / _f);
	}

	// 복합 할당 연산
	Vector2& operator +=(const Vector2& _pVec)
	{
		x += _pVec.x;
		y += _pVec.y;

		return *this;
	}

	Vector2& operator -=(const Vector2& _pVec)
	{
		x -= _pVec.x;
		y -= _pVec.y;

		return *this;
	}

	// 비교 연산
	bool operator ==(const Vector2& _pVec)
	{
		return x == _pVec.x && y == _pVec.y;
	}

	bool operator !=(const Vector2& _pVec)
	{
		return x != _pVec.x || y != _pVec.y;
	}


public:
	float Length()
	{
		return sqrt(x * x + y * y);
	}
	
	// 그 벡터의 위치에 값을 넣어 두어야 하므로 레퍼런스 값 반환
	Vector2& Normalize()
	{
		float fLength = Length();

		assert(fLength != 0.f);

		x /= fLength;
		y /= fLength;

		return *this;
	}

	float Dot()
	{
		return x * x + y * y;
	}

	float Dot(const Vector2& _pVec)
	{
		return x * _pVec.x + y * _pVec.y;
	}

	float CCW(const Vector2& _pFirstVec, const Vector2& _pSecondVec)
	{
		return (_pFirstVec.x - x) * (_pSecondVec.y - y) - (_pFirstVec.y - y) * (_pSecondVec.x - x);
	}

	Vector2& Reverse()
	{
		x = x + y;

		y = x - y;
		x = x - y;

		return *this;
	}


public:
	Vector2() :
		x(0.f),
		y(0.f)
	{}

	Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{}

	Vector2(long _x, long _y) :
		x((float) _x),
		y((float) _y)
	{}

	Vector2(int _x, int _y) :
		x((float) _x),
		y((float) _y)
	{}
	
	Vector2(UINT _x, UINT _y) :
		x((float)_x),
		y((float)_y)
	{}

	Vector2(const POINT& _point) :
		x((float) _point.x),
		y((float) _point.y)
	{}
};


struct Dir4
{
	int left;
	int up;
	int right;
	int down;

public:
	// operators
	//일반 연산
	Dir4 operator +(const Dir4& _dir)
	{
		return Dir4(left + _dir.left, up + _dir.up, right + _dir.right, down + _dir.down);
	}

	Dir4 operator -(const Dir4& _dir)
	{
		return Dir4(left - _dir.left, up - _dir.up, right - _dir.right, down - _dir.down);
	}

	// 복합 할당 연산
	Dir4& operator +=(const Dir4& _pDir)
	{
		left += _pDir.left;
		up += _pDir.up;
		right += _pDir.right;
		down += _pDir.down;

		return *this;
	}

	Dir4& operator -=(const Dir4& _pDir)
	{
		left -= _pDir.left;
		up -= _pDir.up;
		right -= _pDir.right;
		down -= _pDir.down;

		return *this;
	}

	// 비교 연산
	bool operator ==(const Dir4& _pDir)
	{
		return left == _pDir.left
			&& up == _pDir.up
			&& right == _pDir.right
			&& down == _pDir.down;
	}

	bool operator !=(const Dir4& _pDir)
	{
		return left != _pDir.left
			|| up != _pDir.up
			|| right != _pDir.right
			|| down != _pDir.down;
	}

public:
	Dir4& Reverse()
	{
		left ^= right ^= left ^= right;
		up ^= down ^= up ^= down;

		return *this;
	}


public:
	Dir4() :
		left(0),
		up(0),
		right(0),
		down(0)
	{}

	Dir4(int _left, int _up, int _right, int _down) :
		left(_left),
		up(_up),
		right(_right),
		down(_down)
	{}
};