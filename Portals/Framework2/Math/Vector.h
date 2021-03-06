/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / � ` \     * ---------------- *
* Author's *     \_  (__\      \_�v�_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "MyMath.h"

#define POSITIVE_X 0
#define NEGATIVE_X 1
#define POSITIVE_Y 2
#define NEGATIVE_Y 3
#define POSITIVE_Z 4
#define NEGATIVE_Z 5

struct vec2 {
	float x, y;

	vec2(){}
	vec2(const float ixy){
		x = y = ixy;
	}
	vec2(const float ix, const float iy){
		x = ix;
		y = iy;
	}
	operator const float *() const { return &x;	}
	float &operator[](unsigned int index){ return *(((float *) &x) + index); }

	void operator += (const float s);
	void operator += (const vec2 &v);
	void operator -= (const float s);
	void operator -= (const vec2 &v);
	void operator *= (const float s);
	void operator *= (const vec2 &v);
	void operator /= (const float s);
	void operator /= (const vec2 &v);
};

vec2 operator + (const vec2 &u, const vec2 &v);
vec2 operator + (const vec2 &v, const float s);
vec2 operator + (const float s, const vec2 &v);

vec2 operator - (const vec2 &u, const vec2 &v);
vec2 operator - (const vec2 &v, const float s);
vec2 operator - (const float s, const vec2 &v);

vec2 operator - (const vec2 &v);

vec2 operator * (const vec2 &u, const vec2 &v);
vec2 operator * (const float s, const vec2 &v);
vec2 operator * (const vec2 &v, const float s);

vec2 operator / (const vec2 &u, const vec2 &v);
vec2 operator / (const vec2 &v, const float s);
vec2 operator / (const float s, const vec2 &v);

/* --------------------------------------------------------------------------------- */

struct vec3 {
	float x, y, z;

	vec3(){}
	vec3(const float ixyz){
		x = y = z = ixyz;
	}
	vec3(const float ix, const float iy, const float iz){
		x = ix;
		y = iy;
		z = iz;
	}
	vec3(const vec2 iv, const float iz){
		x = iv.x;
		y = iv.y;
		z = iz;
	}
	vec3(const float ix, const vec2 iv){
		x = ix;
		y = iv.x;
		z = iv.y;
	}
	operator const float *() const { return &x;	}
	float &operator[](unsigned int index){ return *(((float *) &x) + index); }

	vec2 xy() const { return vec2(x, y); }
	vec2 yz() const { return vec2(y, z); }
	vec2 xz() const { return vec2(x, z); }

	void operator += (const float s);
	void operator += (const vec3 &v);
	void operator -= (const float s);
	void operator -= (const vec3 &v);
	void operator *= (const float s);
	void operator *= (const vec3 &v);
	void operator /= (const float s);
	void operator /= (const vec3 &v);
};

vec3 operator + (const vec3 &u, const vec3 &v);
vec3 operator + (const vec3 &v, const float s);
vec3 operator + (const float s, const vec3 &v);

vec3 operator - (const vec3 &u, const vec3 &v);
vec3 operator - (const vec3 &v, const float s);
vec3 operator - (const float s, const vec3 &v);

vec3 operator - (const vec3 &v);

vec3 operator * (const vec3 &u, const vec3 &v);
vec3 operator * (const float s, const vec3 &v);
vec3 operator * (const vec3 &v, const float s);

vec3 operator / (const vec3 &u, const vec3 &v);
vec3 operator / (const vec3 &v, const float s);
vec3 operator / (const float s, const vec3 &v);

/* --------------------------------------------------------------------------------- */

struct vec4 {
	float x, y, z, w;

	vec4(){}
	vec4(const float ixyzw){
		x = y = z = w = ixyzw;
	}
	vec4(const float ix, const float iy, const float iz, const float iw){
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
	vec4(const vec2 iv, const float iz, const float iw){
		x = iv.x;
		y = iv.y;
		z = iz;
		w = iw;
	}
	vec4(const float ix, const vec2 iv, const float iw){
		x = ix;
		y = iv.x;
		z = iv.y;
		w = iw;
	}
	vec4(const float ix, const float iy, const vec2 iv){
		x = ix;
		y = iy;
		z = iv.x;
		w = iv.y;
	}
	vec4(const vec2 iv0, const vec2 iv1){
		x = iv0.x;
		y = iv0.y;
		z = iv1.x;
		w = iv1.y;
	}
	vec4(const vec3 iv, const float iw){
		x = iv.x;
		y = iv.y;
		z = iv.z;
		w = iw;
	}
	vec4(const float ix, const vec3 iv){
		x = ix;
		y = iv.x;
		z = iv.y;
		w = iv.z;
	}
	operator const float *() const { return &x;	}
	float &operator[](unsigned int index){ return *(((float *) &x) + index); }

	vec2 xy() const { return vec2(x, y); }
	vec2 xz() const { return vec2(x, z); }
	vec2 xw() const { return vec2(x, w); }
	vec2 yz() const { return vec2(y, z); }
	vec2 yw() const { return vec2(y, w); }
	vec2 zw() const { return vec2(z, w); }
	vec3 xyz() const { return vec3(x, y, z); }
	vec3 yzw() const { return vec3(y, z, w); }

	void operator += (const float s);
	void operator += (const vec4 &v);
	void operator -= (const float s);
	void operator -= (const vec4 &v);
	void operator *= (const float s);
	void operator *= (const vec4 &v);
	void operator /= (const float s);
	void operator /= (const vec4 &v);
};

vec4 operator + (const vec4 &u, const vec4 &v);
vec4 operator + (const vec4 &v, const float s);
vec4 operator + (const float s, const vec4 &v);

vec4 operator - (const vec4 &u, const vec4 &v);
vec4 operator - (const vec4 &v, const float s);
vec4 operator - (const float s, const vec4 &v);

vec4 operator - (const vec4 &v);

vec4 operator * (const vec4 &u, const vec4 &v);
vec4 operator * (const float s, const vec4 &v);
vec4 operator * (const vec4 &v, const float s);

vec4 operator / (const vec4 &u, const vec4 &v);
vec4 operator / (const vec4 &v, const float s);
vec4 operator / (const float s, const vec4 &v);

/* --------------------------------------------------------------------------------- */

typedef vec2 float2;
typedef vec3 float3;
typedef vec4 float4;

float dot(const vec2 &u, const vec2 &v);
float dot(const vec3 &u, const vec3 &v);
float dot(const vec4 &u, const vec4 &v);

float lerp(const float u, const float v, const float x);
vec2  lerp(const vec2 &u, const vec2 &v, const float x);
vec3  lerp(const vec3 &u, const vec3 &v, const float x);
vec4  lerp(const vec4 &u, const vec4 &v, const float x);

float cerp(const float u0, const float u1, const float u2, const float u3, float x);
vec2  cerp(const vec2 &u0, const vec2 &u1, const vec2 &u2, const vec2 &u3, float x);
vec3  cerp(const vec3 &u0, const vec3 &u1, const vec3 &u2, const vec3 &u3, float x);
vec4  cerp(const vec4 &u0, const vec4 &u1, const vec4 &u2, const vec4 &u3, float x);

float sign(const float v);
vec2  sign(const vec2 &v);
vec3  sign(const vec3 &v);
vec4  sign(const vec4 &v);

float clamp(const float v, const float c0, const float c1);
vec2  clamp(const vec2 &v, const float c0, const float c1);
vec2  clamp(const vec2 &v, const vec2 &c0, const vec2 &c1);
vec3  clamp(const vec3 &v, const float c0, const float c1);
vec3  clamp(const vec3 &v, const vec3 &c0, const vec3 &c1);
vec4  clamp(const vec4 &v, const float c0, const float c1);
vec4  clamp(const vec4 &v, const vec4 &c0, const vec4 &c1);

//#define clamp(x, lo, hi) max(min(x, hi), lo)
#define saturate(x) clamp(x, 0, 1)

vec2 normalize(const vec2 &v);
vec3 normalize(const vec3 &v);
vec4 normalize(const vec4 &v);

vec2 fastNormalize(const vec2 &v);
vec3 fastNormalize(const vec3 &v);
vec4 fastNormalize(const vec4 &v);

float length(const vec2 &v);
float length(const vec3 &v);
float length(const vec4 &v);

vec3 reflect(const vec3 &v, const vec3 &normal);

#define distance(u, v) length((u) - (v))

float sCurve(const float t);

vec3 cross(const vec3 &u, const vec3 &v);

float lineProjection(const vec3 &line0, const vec3 &line1, const vec3 &point);

unsigned int toRGBA(const vec4 &u);
unsigned int toBGRA(const vec4 &u);

/* --------------------------------------------------------------------------------- */

struct mat2 {
	float elem[2][2];

	mat2(){}
	mat2(const float m00, const float m01,
		 const float m10, const float m11){
		elem[0][0] = m00; elem[0][1] = m01;
		elem[1][0] = m10; elem[1][1] = m11;
	}

	operator const float *() const { return (const float *) elem; }
};

mat2 operator + (const mat2 &m, const mat2 &n);
mat2 operator - (const mat2 &m, const mat2 &n);
mat2 operator - (const mat2 &m);

mat2 operator * (const mat2 &m, const mat2 &n);
vec2 operator * (const mat2 &m, const vec2 &v);
mat2 operator * (const mat2 &m, const float x);

mat2 transpose(const mat2 &m);

/* --------------------------------------------------------------------------------- */

struct mat3 {
	float elem[3][3];

	mat3(){}
	mat3(const float m00, const float m01, const float m02,
		 const float m10, const float m11, const float m12,
		 const float m20, const float m21, const float m22){
		elem[0][0] = m00; elem[0][1] = m01; elem[0][2] = m02;
		elem[1][0] = m10; elem[1][1] = m11; elem[1][2] = m12;
		elem[2][0] = m20; elem[2][1] = m21; elem[2][2] = m22;
	}

	operator const float *() const { return (const float *) elem; }
};

mat3 operator + (const mat3 &m, const mat3 &n);
mat3 operator - (const mat3 &m, const mat3 &n);
mat3 operator - (const mat3 &m);

mat3 operator * (const mat3 &m, const mat3 &n);
vec3 operator * (const mat3 &m, const vec3 &v);
mat3 operator * (const mat3 &m, const float x);

mat3 transpose(const mat3 &m);

/* --------------------------------------------------------------------------------- */

struct mat4 {
	float elem[4][4];

	mat4(){}
	mat4(const float m00, const float m01, const float m02, const float m03,
		 const float m10, const float m11, const float m12, const float m13,
		 const float m20, const float m21, const float m22, const float m23,
		 const float m30, const float m31, const float m32, const float m33){
		elem[0][0] = m00; elem[0][1] = m01; elem[0][2] = m02; elem[0][3] = m03;
		elem[1][0] = m10; elem[1][1] = m11; elem[1][2] = m12; elem[1][3] = m13;
		elem[2][0] = m20; elem[2][1] = m21; elem[2][2] = m22; elem[2][3] = m23;
		elem[3][0] = m30; elem[3][1] = m31; elem[3][2] = m32; elem[3][3] = m33;
	}

	operator const float *() const { return (const float *) elem; }

	void translate(const vec3 &v);
};

mat4 operator + (const mat4 &m, const mat4 &n);
mat4 operator - (const mat4 &m, const mat4 &n);
mat4 operator - (const mat4 &m);

mat4 operator * (const mat4 &m, const mat4 &n);
vec4 operator * (const mat4 &m, const vec4 &v);
mat4 operator * (const mat4 &m, const float x);

mat4 transpose(const mat4 &m);
mat4 operator ! (const mat4 &m);

/* --------------------------------------------------------------------------------- */

mat4 rotateX(const float angle);
mat4 rotateY(const float angle);
mat4 rotateZ(const float angle);
mat4 rotateXY(const float angleX, const float angleY);
mat4 rotateZXY(const float angleX, const float angleY, const float angleZ);
mat4 translate(const vec3 &v);
mat4 projectionMatrixX(float fov, float aspect, float zNear, float zFar);
mat4 cubemapMatrix(const unsigned int side);

mat2 identity2();
mat3 identity3();
mat4 identity4();

#endif
