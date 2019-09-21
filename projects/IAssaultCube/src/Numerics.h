#pragma once

#include <windows.h>
#include <math.h>

struct Vector2 { float x, y; };
struct Vector3 { float x, y, z; };
struct Vector4 { float x, y, z, w; };

class Vec3
{
public:

	Vec3()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

	Vec3(float px, float py, float pz)
	{
		this->x = px;
		this->y = py;
		this->z = pz;
	};

	Vec3 sub(Vec3 v)
	{
		Vec3 p = Vec3(this->x, this->y, this->z);
		p.x -= v.x;
		p.y -= v.y;
		p.z -= v.z;
		return p;
	};

	float dot(Vec3 vec)
	{
		float temp = 0;
		temp = this->x * vec.x + this->y * vec.y + this->z * vec.z;
		return temp;
	};

	float dist(Vec3 vec)
	{
		return sqrt((this->x - vec.x) * (this->x - vec.x) + (this->y - vec.y) * (this->y - vec.y) + (this->z - vec.z) * (this->z - vec.z));
	}

	Vec3 mul(Vec3 v)
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	};

	Vec3 mul(float f)
	{
		return Vec3(x * f, y * f, z * f);
	};

	Vec3 div(Vec3 v)
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	};

	Vec3& operator + (const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3& operator / (const float& div) const
	{
		return Vec3(x / div, y / div, z / div);
	}

	Vec3& operator - (const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	float Length()
	{
		float flLengthX, flLengthY, flLengthZ, flLength;

		flLengthX = x * x;
		flLengthY = y * y;
		flLengthZ = z * z;

		flLength = sqrt(flLengthX + flLengthY + flLengthZ);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength;
			this->z *= ilength;
		}
	}

	Vec3& div(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	float x;
	float y;
	float z;
};

class Vec4
{
public:

	Vec4()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	};

	Vec4(float px, float py, float pz, float pw)
	{
		this->x = px;
		this->y = py;
		this->z = pz;
		this->w = pw;
	};

	float Length()
	{
		float flLengthX, flLengthY, flLengthZ, flLengthW, flLength;

		flLengthX = x * x;
		flLengthY = y * y;
		flLengthZ = z * z;
		flLengthW = w * w;

		flLength = sqrt(flLengthX + flLengthY + flLengthZ + flLengthW);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength;
			this->z *= ilength;
			this->w *= ilength;
		}
	}

	Vec4& div(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}

	float x;
	float y;
	float z;
	float w;
};

struct glmatrixf
{
	float v[16];

	glmatrixf()
	{
	}
	/*
	glmatrixf multiply( glmatrixf pMatrix )
	{
	float vals[16];

	vals[0] = v[0] * pMatrix.v[0] + v[1] * pMatrix.v[4] + v


	return *this;
	}
	*/

	glmatrixf(float vals[16])
	{
		for (int i = 0; i < 16; i++)
			v[i] = vals[i];
	}

#define MULMAT( row, col ) \
	v[col + row] = x[row] * y[col] + x[row + 4] * y[col + 1] + x[row + 8] * y[col + 2] + x[row + 12] * y[col + 3];

	template<class XT, class YT>
	void mul(const XT x[16], const YT y[16])
	{
		MULMAT(0, 0); MULMAT(1, 0); MULMAT(2, 0); MULMAT(3, 0);
		MULMAT(0, 4); MULMAT(1, 4); MULMAT(2, 4); MULMAT(3, 4);
		MULMAT(0, 8); MULMAT(1, 8); MULMAT(2, 8); MULMAT(3, 8);
		MULMAT(0, 12); MULMAT(1, 12); MULMAT(2, 12); MULMAT(3, 12);
	}

	Vec4 multiplyVec4(Vec4 pVector)
	{
		return Vec4
		(
			pVector.x * v[0] + pVector.y * v[4] + pVector.z * v[8] + pVector.w * v[12],
			pVector.x * v[1] + pVector.y * v[5] + pVector.z * v[9] + pVector.w * v[13],
			pVector.x * v[2] + pVector.y * v[6] + pVector.z * v[10] + pVector.w * v[14],
			pVector.x * v[3] + pVector.y * v[7] + pVector.z * v[11] + pVector.w * v[15]
		);
	}

	float transformx(const Vec3& p) const
	{
		return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
	}

	float transformy(const Vec3& p) const
	{
		return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
	}

	float transformz(const Vec3& p) const
	{
		return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
	}

	float transformw(const Vec3& p) const
	{
		return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
	}
};

struct sqr
{
	unsigned char type;
	char floor, ceil;
	unsigned char wtex, ftex, ctex;
	unsigned char r, g, b;
	unsigned char vdelta;
	char defer;
	char occluded;
	unsigned char utex;
	unsigned char tag;
	unsigned char reserved[2];
};

