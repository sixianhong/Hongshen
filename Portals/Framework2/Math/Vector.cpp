/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
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

#include "Vector.h"

void vec2::operator += (const float s){
	x += s;
	y += s;
}

void vec2::operator += (const vec2 &v){
	x += v.x;
	y += v.y;
}

void vec2::operator -= (const float s){
	x -= s;
	y -= s;
}

void vec2::operator -= (const vec2 &v){
	x -= v.x;
	y -= v.y;
}

void vec2::operator *= (const float s){
	x *= s;
	y *= s;
}

void vec2::operator *= (const vec2 &v){
	x *= v.x;
	y *= v.y;
}

void vec2::operator /= (const float s){
	x /= s;
	y /= s;
}

void vec2::operator /= (const vec2 &v){
	x /= v.x;
	y /= v.y;
}

vec2 operator + (const vec2 &u, const vec2 &v){
	return vec2(u.x + v.x, u.y + v.y);
}

vec2 operator + (const vec2 &v, const float s){
	return vec2(v.x + s, v.y + s);
}

vec2 operator + (const float s, const vec2 &v){
	return vec2(v.x + s, v.y + s);
}

vec2 operator - (const vec2 &u, const vec2 &v){
	return vec2(u.x - v.x, u.y - v.y);
}

vec2 operator - (const vec2 &v, const float s){
	return vec2(v.x - s, v.y - s);
}

vec2 operator - (const float s, const vec2 &v){
	return vec2(v.x - s, v.y - s);
}

vec2 operator - (const vec2 &v){
	return vec2(-v.x, -v.y);
}

vec2 operator * (const vec2 &u, const vec2 &v){
	return vec2(u.x * v.x, u.y * v.y);
}

vec2 operator * (const float s, const vec2 &v){
	return vec2(v.x * s, v.y * s);
}

vec2 operator * (const vec2 &v, const float s){
	return vec2(v.x * s, v.y * s);
}

vec2 operator / (const vec2 &u, const vec2 &v){
	return vec2(u.x / v.x, u.y / v.y);
}

vec2 operator / (const vec2 &v, const float s){
	return vec2(v.x / s, v.y / s);
}

vec2 operator / (const float s, const vec2 &v){
	return vec2(s / v.x, s / v.y);
}

/* --------------------------------------------------------------------------------- */

void vec3::operator += (const float s){
	x += s;
	y += s;
	z += s;
}

void vec3::operator += (const vec3 &v){
	x += v.x;
	y += v.y;
	z += v.z;
}

void vec3::operator -= (const float s){
	x -= s;
	y -= s;
	z -= s;
}

void vec3::operator -= (const vec3 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void vec3::operator *= (const float s){
	x *= s;
	y *= s;
	z *= s;
}

void vec3::operator *= (const vec3 &v){
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void vec3::operator /= (const float s){
	x /= s;
	y /= s;
	z /= s;
}

void vec3::operator /= (const vec3 &v){
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

vec3 operator + (const vec3 &u, const vec3 &v){
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 operator + (const vec3 &v, const float s){
	return vec3(v.x + s, v.y + s, v.z + s);
}

vec3 operator + (const float s, const vec3 &v){
	return vec3(v.x + s, v.y + s, v.z + s);
}

vec3 operator - (const vec3 &u, const vec3 &v){
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

vec3 operator - (const vec3 &v, const float s){
	return vec3(v.x - s, v.y - s, v.z - s);
}

vec3 operator - (const float s, const vec3 &v){
	return vec3(v.x - s, v.y - s, v.z - s);
}

vec3 operator - (const vec3 &v){
	return vec3(-v.x, -v.y, -v.z);
}

vec3 operator * (const vec3 &u, const vec3 &v){
	return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

vec3 operator * (const float s, const vec3 &v){
	return vec3(v.x * s, v.y * s, v.z * s);
}

vec3 operator * (const vec3 &v, const float s){
	return vec3(v.x * s, v.y * s, v.z * s);
}

vec3 operator / (const vec3 &u, const vec3 &v){
	return vec3(u.x / v.x, u.y / v.y, u.z / v.z);
}

vec3 operator / (const vec3 &v, const float s){
	return vec3(v.x / s, v.y / s, v.z / s);
}

vec3 operator / (const float s, const vec3 &v){
	return vec3(s / v.x, s / v.y, s / v.z);
}

/* --------------------------------------------------------------------------------- */

void vec4::operator += (const float s){
	x += s;
	y += s;
	z += s;
	w += s;
}

void vec4::operator += (const vec4 &v){
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void vec4::operator -= (const float s){
	x -= s;
	y -= s;
	z -= s;
	w -= s;
}

void vec4::operator -= (const vec4 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void vec4::operator *= (const float s){
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

void vec4::operator *= (const vec4 &v){
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void vec4::operator /= (const float s){
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

void vec4::operator /= (const vec4 &v){
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

vec4 operator + (const vec4 &u, const vec4 &v){
	return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

vec4 operator + (const vec4 &v, const float s){
	return vec4(v.x + s, v.y + s, v.z + s, v.w + s);
}

vec4 operator + (const float s, const vec4 &v){
	return vec4(v.x + s, v.y + s, v.z + s, v.w + s);
}

vec4 operator - (const vec4 &u, const vec4 &v){
	return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

vec4 operator - (const vec4 &v, const float s){
	return vec4(v.x - s, v.y - s, v.z - s, v.w - s);
}

vec4 operator - (const float s, const vec4 &v){
	return vec4(v.x - s, v.y - s, v.z - s, v.w - s);
}

vec4 operator - (const vec4 &v){
	return vec4(-v.x, -v.y, -v.z, -v.w);
}

vec4 operator * (const vec4 &u, const vec4 &v){
	return vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

vec4 operator * (const float s, const vec4 &v){
	return vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

vec4 operator * (const vec4 &v, const float s){
	return vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

vec4 operator / (const vec4 &u, const vec4 &v){
	return vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
}

vec4 operator / (const vec4 &v, const float s){
	return vec4(v.x / s, v.y / s, v.z / s, v.w / s);
}

vec4 operator / (const float s, const vec4 &v){
	return vec4(s / v.x, s / v.y, s / v.z, s / v.w);
}

/* --------------------------------------------------------------------------------- */

float dot(const vec2 &u, const vec2 &v){
	return u.x * v.x + u.y * v.y;
}

float dot(const vec3 &u, const vec3 &v){
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float dot(const vec4 &u, const vec4 &v){
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

float lerp(const float u, const float v, const float x){
	return u * (1 - x) + v * x;
}

vec2 lerp(const vec2 &u, const vec2 &v, const float x){
	return u * (1 - x) + v * x;
}

vec3 lerp(const vec3 &u, const vec3 &v, const float x){
	return u * (1 - x) + v * x;
}

vec4 lerp(const vec4 &u, const vec4 &v, const float x){
	return u * (1 - x) + v * x;
}

float cerp(const float u0, const float u1, const float u2, const float u3, float x){
	float p = (u3 - u2) - (u0 - u1);
	float q = (u0 - u1) - p;
	float r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}

vec2 cerp(const vec2 &u0, const vec2 &u1, const vec2 &u2, const vec2 &u3, float x){
	vec2 p = (u3 - u2) - (u0 - u1);
	vec2 q = (u0 - u1) - p;
	vec2 r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}

vec3 cerp(const vec3 &u0, const vec3 &u1, const vec3 &u2, const vec3 &u3, float x){
	vec3 p = (u3 - u2) - (u0 - u1);
	vec3 q = (u0 - u1) - p;
	vec3 r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}

vec4 cerp(const vec4 &u0, const vec4 &u1, const vec4 &u2, const vec4 &u3, float x){
	vec4 p = (u3 - u2) - (u0 - u1);
	vec4 q = (u0 - u1) - p;
	vec4 r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}

float sign(const float v){
	return (v > 0)? 1.0f : (v < 0)? -1.0f : 0.0f;
}

vec2 sign(const vec2 &v){
	return vec2(sign(v.x), sign(v.y));
}

vec3 sign(const vec3 &v){
	return vec3(sign(v.x), sign(v.y), sign(v.z));
}

vec4 sign(const vec4 &v){
	return vec4(sign(v.x), sign(v.y), sign(v.z), sign(v.w));
}

float clamp(const float v, const float c0, const float c1){
	return min(max(v, c0), c1);
}

vec2 clamp(const vec2 &v, const float c0, const float c1){
	return vec2(min(max(v.x, c0), c1), min(max(v.y, c0), c1));
}

vec2 clamp(const vec2 &v, const vec2 &c0, const vec2 &c1){
	return vec2(min(max(v.x, c0.x), c1.x), min(max(v.y, c0.y), c1.y));
}

vec3 clamp(const vec3 &v, const float c0, const float c1){
	return vec3(min(max(v.x, c0), c1), min(max(v.y, c0), c1), min(max(v.z, c0), c1));
}

vec3 clamp(const vec3 &v, const vec3 &c0, const vec3 &c1){
	return vec3(min(max(v.x, c0.x), c1.x), min(max(v.y, c0.y), c1.y), min(max(v.z, c0.z), c1.z));
}

vec4 clamp(const vec4 &v, const float c0, const float c1){
	return vec4(min(max(v.x, c0), c1), min(max(v.y, c0), c1), min(max(v.z, c0), c1), min(max(v.z, c0), c1));
}

vec4 clamp(const vec4 &v, const vec4 &c0, const vec4 &c1){
	return vec4(min(max(v.x, c0.x), c1.x), min(max(v.y, c0.y), c1.y), min(max(v.z, c0.z), c1.z), min(max(v.w, c0.w), c1.w));
}

vec2 normalize(const vec2 &v){
	float invLen = 1.0f / sqrtf(v.x * v.x + v.y * v.y);
	return v * invLen;
}

vec3 normalize(const vec3 &v){
	float invLen = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return v * invLen;
}

vec4 normalize(const vec4 &v){
	float invLen = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return v * invLen;
}

vec2 fastNormalize(const vec2 &v){
	float invLen = rsqrtf(v.x * v.x + v.y * v.y);
	return v * invLen;
}

vec3 fastNormalize(const vec3 &v){
	float invLen = rsqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return v * invLen;
}

vec4 fastNormalize(const vec4 &v){
	float invLen = rsqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return v * invLen;
}

float length(const vec2 &v){
	return sqrtf(v.x * v.x + v.y * v.y);
}

float length(const vec3 &v){
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float length(const vec4 &v){
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec3 reflect(const vec3 &v, const vec3 &normal){
	float n = dot(v, normal);
	return v - 2 * n * normal;
}

float sCurve(const float t){
	return t * t * (3 - 2 * t);
}

vec3 cross(const vec3 &u, const vec3 &v){
	return vec3(u.y * v.z - v.y * u.z, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float lineProjection(const vec3 &line0, const vec3 &line1, const vec3 &point){
	vec3 v = line1 - line0;
	return dot(v, point - line0) / dot(v, v);
}

unsigned int toRGBA(const vec4 &u){
	return (int(u.x * 255) | (int(u.y * 255) << 8) | (int(u.z * 255) << 16) | (int(u.w * 255) << 24));
}

unsigned int toBGRA(const vec4 &u){
	return (int(u.z * 255) | (int(u.y * 255) << 8) | (int(u.x * 255) << 16) | (int(u.w * 255) << 24));
}

/* --------------------------------------------------------------------------------- */

mat2 operator + (const mat2 &m, const mat2 &n){
	return mat2(
		m.elem[0][0] + n.elem[0][0], m.elem[0][1] + n.elem[0][1],
		m.elem[1][0] + n.elem[1][0], m.elem[1][1] + n.elem[1][1]);
}

mat2 operator - (const mat2 &m, const mat2 &n){
	return mat2(
		m.elem[0][0] - n.elem[0][0], m.elem[0][1] - n.elem[0][1],
		m.elem[1][0] - n.elem[1][0], m.elem[1][1] - n.elem[1][1]);
}

mat2 operator - (const mat2 &m){
	return mat2(-m.elem[0][0], -m.elem[0][1], -m.elem[1][0], -m.elem[1][1]);
}

mat2 operator * (const mat2 &m, const mat2 &n){
	return mat2(
		m.elem[0][0] * n.elem[0][0] + m.elem[0][1] * n.elem[1][0],   m.elem[0][0] * n.elem[0][1] + m.elem[0][1] * n.elem[1][1],
		m.elem[1][0] * n.elem[0][0] + m.elem[1][1] * n.elem[1][0],   m.elem[1][0] * n.elem[0][1] + m.elem[1][1] * n.elem[1][1]);
}

vec2 operator * (const mat2 &m, const vec2 &v){
	return vec2(m.elem[0][0] * v.x + m.elem[0][1] * v.y, m.elem[1][0] * v.x + m.elem[1][1] * v.y);
}

mat2 operator * (const mat2 &m, const float x){
	mat2 mat;
	mat.elem[0][0] = m.elem[0][0] * x;
	mat.elem[0][1] = m.elem[0][1] * x;
	mat.elem[1][0] = m.elem[1][0] * x;
	mat.elem[1][1] = m.elem[1][1] * x;
	return mat;
}

mat2 transpose(const mat2 &m){
	return mat2(
		m.elem[0][0], m.elem[1][0],
		m.elem[0][1], m.elem[1][1]);
}

/* --------------------------------------------------------------------------------- */

mat3 operator + (const mat3 &m, const mat3 &n){
	return mat3(
		m.elem[0][0] + n.elem[0][0], m.elem[0][1] + n.elem[0][1], m.elem[0][2] + n.elem[0][2],
		m.elem[1][0] + n.elem[1][0], m.elem[1][1] + n.elem[1][1], m.elem[1][2] + n.elem[1][2],
		m.elem[2][0] + n.elem[2][0], m.elem[2][1] + n.elem[2][1], m.elem[2][2] + n.elem[2][2]);
}

mat3 operator - (const mat3 &m, const mat3 &n){
	return mat3(
		m.elem[0][0] - n.elem[0][0], m.elem[0][1] - n.elem[0][1], m.elem[0][2] - n.elem[0][2],
		m.elem[1][0] - n.elem[1][0], m.elem[1][1] - n.elem[1][1], m.elem[1][2] - n.elem[1][2],
		m.elem[2][0] - n.elem[2][0], m.elem[2][1] - n.elem[2][1], m.elem[2][2] - n.elem[2][2]);
}

mat3 operator - (const mat3 &m){
	return mat3(
		-m.elem[0][0], -m.elem[0][1], -m.elem[0][2],
		-m.elem[1][0], -m.elem[1][1], -m.elem[1][2],
		-m.elem[2][0], -m.elem[2][1], -m.elem[2][2]);
}

mat3 operator * (const mat3 &m, const mat3 &n){
	mat3 mat;

	for (unsigned int k = 0; k < 3; k++){
		for (unsigned int j = 0; j < 3; j++){
			mat.elem[k][j] = 0;
			for (unsigned int i = 0; i < 3; i++){
				mat.elem[k][j] += m.elem[k][i] * n.elem[i][j];
			}
		}
	}
	return mat;
}

vec3 operator * (const mat3 &m, const vec3 &v){
	return vec3(
		m.elem[0][0] * v.x + m.elem[0][1] * v.y + m.elem[0][2] * v.z,
		m.elem[1][0] * v.x + m.elem[1][1] * v.y + m.elem[1][2] * v.z,
		m.elem[2][0] * v.x + m.elem[2][1] * v.y + m.elem[2][2] * v.z);
}

mat3 operator * (const mat3 &m, const float x){
	mat3 mat;
	for (unsigned int j = 0; j < 3; j++){
		for (unsigned int i = 0; i < 3; i++){
			mat.elem[j][i] = m.elem[j][i] * x;
		}
	}
	return mat;
}

mat3 transpose(const mat3 &m){
	return mat3(
		m.elem[0][0], m.elem[1][0], m.elem[2][0],
		m.elem[0][1], m.elem[1][1], m.elem[2][1],
		m.elem[0][2], m.elem[1][2], m.elem[2][2]);
}

/* --------------------------------------------------------------------------------- */

void mat4::translate(const vec3 &v){
	elem[0][3] += (v.x * elem[0][0] + v.y * elem[0][1] + v.z * elem[0][2]);
	elem[1][3] += (v.x * elem[1][0] + v.y * elem[1][1] + v.z * elem[1][2]);
	elem[2][3] += (v.x * elem[2][0] + v.y * elem[2][1] + v.z * elem[2][2]);
	elem[3][3] += (v.x * elem[3][0] + v.y * elem[3][1] + v.z * elem[3][2]);
}

mat4 operator + (const mat4 &m, const mat4 &n){
	mat4 mat;
	for (unsigned int i = 0; i < 16; i++){
		((float *) &mat)[i] = ((float *) &m)[i] + ((float *) &n)[i];
	}
	return mat;

/*	return mat4(
		m.elem[0][0] + n.elem[0][0], m.elem[0][1] + n.elem[0][1], m.elem[0][2] + n.elem[0][2], m.elem[0][3] + n.elem[0][3],
		m.elem[1][0] + n.elem[1][0], m.elem[1][1] + n.elem[1][1], m.elem[1][2] + n.elem[1][2], m.elem[1][3] + n.elem[1][3],
		m.elem[2][0] + n.elem[2][0], m.elem[2][1] + n.elem[2][1], m.elem[2][2] + n.elem[2][2], m.elem[2][3] + n.elem[2][3],
		m.elem[3][0] + n.elem[3][0], m.elem[3][1] + n.elem[3][1], m.elem[3][2] + n.elem[3][2], m.elem[3][3] + n.elem[3][3]);
		*/
}

mat4 operator - (const mat4 &m, const mat4 &n){
	mat4 mat;
	for (unsigned int i = 0; i < 16; i++){
		((float *) &mat)[i] = ((float *) &m)[i] - ((float *) &n)[i];
	}
	return mat;
}

mat4 operator - (const mat4 &m){
	mat4 mat;
	for (unsigned int i = 0; i < 16; i++){
		((float *) &mat)[i] = -((float *) &m)[i];
	}
	return mat;
}

mat4 operator * (const mat4 &m, const mat4 &n){
	mat4 mat;
	for (unsigned int k = 0; k < 4; k++){
		for (unsigned int j = 0; j < 4; j++){
			mat.elem[k][j] = 0;
			for (unsigned int i = 0; i < 4; i++){
				mat.elem[k][j] += m.elem[k][i] * n.elem[i][j];
			}
		}
	}
	return mat;
}

vec4 operator * (const mat4 &m, const vec4 &v){
	return vec4(
		m.elem[0][0] * v.x + m.elem[0][1] * v.y + m.elem[0][2] * v.z + m.elem[0][3] * v.w,
		m.elem[1][0] * v.x + m.elem[1][1] * v.y + m.elem[1][2] * v.z + m.elem[1][3] * v.w,
		m.elem[2][0] * v.x + m.elem[2][1] * v.y + m.elem[2][2] * v.z + m.elem[2][3] * v.w,
		m.elem[3][0] * v.x + m.elem[3][1] * v.y + m.elem[3][2] * v.z + m.elem[3][3] * v.w);
}

mat4 operator * (const mat4 &m, const float x){
	mat4 mat;
	for (unsigned int j = 0; j < 4; j++){
		for (unsigned int i = 0; i < 4; i++){
			mat.elem[j][i] = m.elem[j][i] * x;
		}
	}
	return mat;
}

mat4 transpose(const mat4 &m){
	return mat4(
		m.elem[0][0], m.elem[1][0], m.elem[2][0], m.elem[3][0],
		m.elem[0][1], m.elem[1][1], m.elem[2][1], m.elem[3][1],
		m.elem[0][2], m.elem[1][2], m.elem[2][2], m.elem[3][2],
		m.elem[0][3], m.elem[1][3], m.elem[2][3], m.elem[3][3]);
}

mat4 operator ! (const mat4 &m){
	mat4 mat;
	unsigned int i, j, k;
	float temp, newMat[4][8];

	for (k = i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			newMat[j][i] = ((float *) &m)[k];
			newMat[i][j + 4] = (i == j);
			k++;
		}
	}


	for (i = 0; i < 4; i++){
		// Swap rows if neccesary.
		if (fabsf(newMat[i][i]) < 0.0001f){
			j = i;
			float maxVal = fabsf(newMat[i][i]);
			for (k = 0; k < 4; k++){
				float f = fabsf(newMat[k][i]);
				if (f > maxVal){
					maxVal = f;
					j = k;
				}
			}


			for (k = i; k < 8; k++){
				temp = newMat[j][k];
				newMat[j][k] = newMat[i][k];
				newMat[i][k] = temp;
			}
		}
		
		// Shorten the numbers the current row to get a 1 at (i,i) ... the actual 1 is uninteresting and omitted
		for (j = i+1; j < 8; j++){
			newMat[i][j] /= newMat[i][i];
		}

		// Get zeros in the beginning ... the actual zeros are uninteresting and omitted
		for (j = 0; j < 4; j++){
			if (j != i){
				for (k = i+1; k < 8; k++){
					newMat[j][k] -= newMat[j][i] * newMat[i][k];
				}
			}
		}
	}

	// Pack it into the return Matrix object.
	for (k = 0, i = 4; i < 8; i++){
		for (j = 0; j < 4; j++){
			((float *) &mat)[k] = newMat[j][i];
			k++;
		}
	}

	return mat;
}


/* --------------------------------------------------------------------------------- */

mat4 rotateX(const float angle){
	float cosA = cosf(angle), sinA = sinf(angle);
	
	return mat4(
		1, 0,     0,    0,
		0, cosA, -sinA, 0,
		0, sinA,  cosA, 0,
		0, 0,     0,    1);
}

mat4 rotateY(const float angle){
	float cosA = cosf(angle), sinA = sinf(angle);

	return mat4(
		cosA, 0, -sinA, 0,
		0,    1,  0,    0,
		sinA, 0,  cosA, 0,
		0,    0,  0,    1);
}

mat4 rotateZ(const float angle){
	float cosA = cosf(angle), sinA = sinf(angle);

	return mat4(
		cosA, -sinA, 0, 0,
		sinA,  cosA, 0, 0,
		0,     0,    1, 0,
		0,     0,    0, 1);
}

mat4 rotateXY(const float angleX, const float angleY){
	float cosX = cosf(angleX), sinX = sinf(angleX), 
		  cosY = cosf(angleY), sinY = sinf(angleY);

	return mat4(
		 cosY,        0,    -sinY,        0,
		-sinX * sinY, cosX, -sinX * cosY, 0,
		 cosX * sinY, sinX,  cosX * cosY, 0,
		 0,           0,     0,           1);
}

mat4 rotateZXY(const float angleX, const float angleY, const float angleZ){
	float cosX = cosf(angleX), sinX = sinf(angleX), 
		  cosY = cosf(angleY), sinY = sinf(angleY),
		  cosZ = cosf(angleZ), sinZ = sinf(angleZ);

	return mat4(
		cosY * cosZ + sinX * sinY * sinZ,   -cosX * sinZ,    sinX * cosY * sinZ - sinY * cosZ,  0,
		cosY * sinZ - sinX * sinY * cosZ,    cosX * cosZ,   -sinY * sinZ - sinX * cosY * cosZ,  0,
		cosX * sinY,                         sinX,           cosX * cosY,                       0,
		0,                                   0,              0,                                 1);
}

mat4 translate(const vec3 &v){
	return mat4(1,0,0,v.x, 0,1,0,v.y, 0,0,1,v.z, 0,0,0,1);
}

mat4 projectionMatrixX(float fov, float aspect, float zNear, float zFar){
	float w = tanf(0.5f * fov);
	float h = w * aspect;

	return mat4(
		1.0f / w, 0,        0, 0,
		0,        1.0f / h, 0, 0,
		0,        0,        (zFar + zNear) / (zFar - zNear), -(2 * zFar * zNear) / (zFar - zNear),
		0,        0,        1, 0);
}

mat4 cubemapMatrix(const unsigned int side){
	switch(side){
	case POSITIVE_X:
		return mat4(
			0, 0, -1, 0,
			0, 1,  0, 0,
			1, 0,  0, 0,
			0, 0,  0, 1);
	case NEGATIVE_X:
		return mat4(
			 0, 0, 1, 0,
			 0, 1, 0, 0,
			-1, 0, 0, 0,
			 0, 0, 0, 1);
	case POSITIVE_Y:
		return mat4(
			1, 0,  0, 0,
			0, 0, -1, 0,
			0, 1,  0, 0,
			0, 0,  0, 1);
	case NEGATIVE_Y:
		return mat4(
			1,  0, 0, 0,
			0,  0, 1, 0,
			0, -1, 0, 0,
			0,  0, 0, 1);
	case POSITIVE_Z:
		return mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	//case NEGATIVE_Z:
	default:
		return mat4(
			-1, 0,  0, 0,
			 0, 1,  0, 0,
			 0, 0, -1, 0,
			 0, 0,  0, 1);
	}
}

mat2 identity2(){
	return mat2(1,0, 0,1);
}

mat3 identity3(){
	return mat3(1,0,0, 0,1,0, 0,0,1);
}

mat4 identity4(){
	return mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
}
