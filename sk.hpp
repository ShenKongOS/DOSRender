#ifndef SK_GL
#define SK_GL

#include<math.h>
#include <string.h>
#include <cstdio>
#include <dpmi.h>       // DJGPP 专用的 DPMI 中断调用
#include <go32.h>
#include <sys/movedata.h>
#include <sys/nearptr.h> 
#include <sys/farptr.h> // DJGPP 专用的远指针访问库
#include <time.h>
#include <pc.h> // DJGPP 的底层硬件头文件
#define in
#define out




/*****************************************************************************
					向量模拟
 *****************************************************************************
 */

class vec2;
class vec3;
class vec4;

//宏定义
#define SWIZZLE2(a, b) vec2 a##b() const { return vec2(a, b); }
#define SWIZZLE3(a, b, c) vec3 a##b##c() const { return vec3(a, b, c); }
#define SWIZZLE4(a, b, c, d) vec4 a##b##c##d() const { return vec4(a, b, c, d); }

class vec2{
	
public:
	float x;
	float y;
	vec2 xy(){return vec2(x,y);}
	vec2 yx(){return vec2(y,x);}
	
	
	vec2(){
		x=0.;
		y=0.;
	}
	vec2(float a){
		x=a;
		y=a;
	}
	vec2(float a,float b){
		x=a;
		y=b;
	}
	
	vec2 operator+(const vec2& other) const {
		
		return vec2(x+other.x,y+other.y);
	}
	
	vec2 operator-(const vec2& other) const {
		return vec2(x-other.x,y-other.y);
	}
	
	vec2 operator*(const vec2& other) const {
		return vec2(x*other.x,y*other.y);
	}
	vec2 operator*(const float a) const {
		return vec2(x*a,y*a);
	}

	
	vec2 operator/(const vec2& other)const  {
		return vec2(x/other.x,y/other.y);
	}
	vec2 operator/(const float a)const {
		return vec2(x/a,y/a);
	}

	
	friend vec2 operator-(const float a, const vec2& st) {
		return vec2(a-st.x,a-st.y);
	}
	friend vec2 operator+(const float a, const vec2& st){
		return vec2(a+st.x,a+st.y);
	}
	friend vec2 operator*(const float a, const vec2& st){
		return vec2(st.x*a,st.y*a);
	}
	friend vec2 operator/(const float a, const vec2& st){
		return vec2(a/st.x,a/st.y);
	}
	
	
	
	vec2& operator+=(const vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	
	vec2& operator-=(const vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	
	vec2& operator*=(const vec2& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
	
	vec2& operator/=(const vec2& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}


	// --- Swizzle 2 字母 ---
	SWIZZLE2(x, x) SWIZZLE2(x, y)
	SWIZZLE2(y, x) SWIZZLE2(y, y)
	
	// --- Swizzle 3 字母 (在内部声明，在外部实现，因为此时 vec3 还不完整) ---
	vec3 xxx() const;
	vec3 xxy() const;
	vec3 xyx() const;
	vec3 xyy() const;
	vec3 yxx() const;
	vec3 yxy() const;
	vec3 yyx() const;
	vec3 yyy() const;
	
};


class vec3{
	
public:
	float x;
	float y;
	float z;

	// 默认构造函数
	vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	
	vec3(float a){
		x=a;
		y=a;
		z=a;
	}
	vec3(float a,float b,float c){
		x=a;
		y=b;
		z=c;
	}
	vec3(const vec2& v, double zValue) {
		x = v.x;
		y = v.y;
		z = (float)zValue; // double 转 float，符合 GLSL 精度
	}
	
	vec3 operator+(const vec3& other) const {
		
		return vec3(x+other.x,y+other.y,z+other.z);
	}
	friend vec3 operator+(float f, const vec3& v) {
		return vec3(f + v.x, f + v.y, f + v.z);
	}
	
	vec3 operator-(const vec3& other) const {
		return vec3(x-other.x,y-other.y,z-other.z);
	}
	friend vec3 operator-(float f, const vec3& v) {
		return vec3(f - v.x, f - v.y, f - v.z);
	}
	
	vec3 operator*(const vec3& other)const  {
		return vec3(x*other.x,y*other.y,z*other.z);
	}
	vec3 operator*(const float a)const {
		return vec3(x*a,y*a,z*a);
	}
	friend vec3 operator*(const float a, const vec3& inv) {
		return vec3(inv.x*a,inv.y*a,inv.z*a);
	}
	
	vec3 operator/(const vec3& other)const  {
		return vec3(x/other.x,y/other.y,z/other.z);
	}
	vec3 operator/(const float a)const {
		return vec3(x/a,y/a,z/a);
	}
	friend vec3 operator/(const float a, const vec3& inv) {
		return vec3(a/inv.x,a/inv.y,a/inv.z);
	}
	
	
	vec3& operator+=(const vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	
	vec3& operator-=(const vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	
	vec3& operator*=(const vec3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	
	vec3& operator/=(const vec3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}
	
	// --- Swizzle 2 字母 (9个) ---
	SWIZZLE2(x, x) SWIZZLE2(x, y) SWIZZLE2(x, z)
	SWIZZLE2(y, x) SWIZZLE2(y, y) SWIZZLE2(y, z)
	SWIZZLE2(z, x) SWIZZLE2(z, y) SWIZZLE2(z, z)
	
	// --- Swizzle 3 字母 (27个) ---
	SWIZZLE3(x, x, x) SWIZZLE3(x, x, y) SWIZZLE3(x, x, z) SWIZZLE3(x, y, x) SWIZZLE3(x, y, y) SWIZZLE3(x, y, z) SWIZZLE3(x, z, x) SWIZZLE3(x, z, y) SWIZZLE3(x, z, z)
	SWIZZLE3(y, x, x) SWIZZLE3(y, x, y) SWIZZLE3(y, x, z) SWIZZLE3(y, y, x) SWIZZLE3(y, y, y) SWIZZLE3(y, y, z) SWIZZLE3(y, z, x) SWIZZLE3(y, z, y) SWIZZLE3(y, z, z)
	SWIZZLE3(z, x, x) SWIZZLE3(z, x, y) SWIZZLE3(z, x, z) SWIZZLE3(z, y, x) SWIZZLE3(z, y, y) SWIZZLE3(z, y, z) SWIZZLE3(z, z, x) SWIZZLE3(z, z, y) SWIZZLE3(z, z, z)
	
};


inline vec3 vec2::xxx() const { return vec3(x, x, x); }
inline vec3 vec2::xxy() const { return vec3(x, x, y); }
inline vec3 vec2::xyx() const { return vec3(x, y, x); }
inline vec3 vec2::xyy() const { return vec3(x, y, y); }
inline vec3 vec2::yxx() const { return vec3(y, x, x); }
inline vec3 vec2::yxy() const { return vec3(y, x, y); }
inline vec3 vec2::yyx() const { return vec3(y, y, x); }
inline vec3 vec2::yyy() const { return vec3(y, y, y); }


class vec4 {
public:
	float x;
	float y;
	float z;
	float w;
	
	// 构造函数
	vec4(float a) {
		x = a;
		y = a;
		z = a;
		w = a;
	}
	vec4(float a, float b, float c, float d) {
		x = a;
		y = b;
		z = c;
		w = d;
	}
	vec4(vec3& a,float v){
		x=a.x;
		y=a.y;
		z=a.z;
		w=v;
	}
	

	vec4 operator+(const vec4& other) const  {
		return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	vec4 operator-(const vec4& other) const {
		return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	vec4 operator*(const vec4& other) const {
		return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
	}
	vec4 operator/(const vec4& other) const  {
		return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
	}
	
	vec4 operator*(const float a) {
		return vec4(x * a, y * a, z * a, w * a);
	}
	

	friend vec4 operator*(const float a, const vec4& inv) {
		return vec4(inv.x * a, inv.y * a, inv.z * a, inv.w * a);
	}
	

	vec4& operator+=(const vec4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	vec4& operator-=(const vec4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	vec4& operator*=(const vec4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}
	vec4& operator/=(const vec4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}
	

	void xy(const vec2& v) { x = v.x; y = v.y; }
	void xz(const vec2& v) { x = v.x; z = v.y; }
	void xw(const vec2& v) { x = v.x; w = v.y; }
	void yz(const vec2& v) { y = v.x; z = v.y; }
	void yw(const vec2& v) { y = v.x; w = v.y; }
	void zw(const vec2& v) { z = v.x; w = v.y; }
	void xyz(const vec3& v) { x = v.x; y = v.y; z = v.z; }
	void xyw(const vec3& v) { x = v.x; y = v.y; w = v.z; }
	void xzw(const vec3& v) { x = v.x; z = v.y; w = v.z; }
	void yzw(const vec3& v) { y = v.x; z = v.y; w = v.z; }
	
	//Swizzle 2 字母 (16个)
	SWIZZLE2(x, x) SWIZZLE2(x, y) SWIZZLE2(x, z) SWIZZLE2(x, w)
	SWIZZLE2(y, x) SWIZZLE2(y, y) SWIZZLE2(y, z) SWIZZLE2(y, w)
	SWIZZLE2(z, x) SWIZZLE2(z, y) SWIZZLE2(z, z) SWIZZLE2(z, w)
	SWIZZLE2(w, x) SWIZZLE2(w, y) SWIZZLE2(w, z) SWIZZLE2(w, w)
	
	//Swizzle 3 字母 (64个)
	SWIZZLE3(x, x, x) SWIZZLE3(x, x, y) SWIZZLE3(x, x, z) SWIZZLE3(x, x, w) SWIZZLE3(x, y, x) SWIZZLE3(x, y, y) SWIZZLE3(x, y, z) SWIZZLE3(x, y, w) SWIZZLE3(x, z, x) SWIZZLE3(x, z, y) SWIZZLE3(x, z, z) SWIZZLE3(x, z, w) SWIZZLE3(x, w, x) SWIZZLE3(x, w, y) SWIZZLE3(x, w, z) SWIZZLE3(x, w, w)
	SWIZZLE3(y, x, x) SWIZZLE3(y, x, y) SWIZZLE3(y, x, z) SWIZZLE3(y, x, w) SWIZZLE3(y, y, x) SWIZZLE3(y, y, y) SWIZZLE3(y, y, z) SWIZZLE3(y, y, w) SWIZZLE3(y, z, x) SWIZZLE3(y, z, y) SWIZZLE3(y, z, z) SWIZZLE3(y, z, w) SWIZZLE3(y, w, x) SWIZZLE3(y, w, y) SWIZZLE3(y, w, z) SWIZZLE3(y, w, w)
	SWIZZLE3(z, x, x) SWIZZLE3(z, x, y) SWIZZLE3(z, x, z) SWIZZLE3(z, x, w) SWIZZLE3(z, y, x) SWIZZLE3(z, y, y) SWIZZLE3(z, y, z) SWIZZLE3(z, y, w) SWIZZLE3(z, z, x) SWIZZLE3(z, z, y) SWIZZLE3(z, z, z) SWIZZLE3(z, z, w) SWIZZLE3(z, w, x) SWIZZLE3(z, w, y) SWIZZLE3(z, w, z) SWIZZLE3(z, w, w)
	SWIZZLE3(w, x, x) SWIZZLE3(w, x, y) SWIZZLE3(w, x, z) SWIZZLE3(w, x, w) SWIZZLE3(w, y, x) SWIZZLE3(w, y, y) SWIZZLE3(w, y, z) SWIZZLE3(w, y, w) SWIZZLE3(w, z, x) SWIZZLE3(w, z, y) SWIZZLE3(w, z, z) SWIZZLE3(w, z, w) SWIZZLE3(w, w, x) SWIZZLE3(w, w, y) SWIZZLE3(w, w, z) SWIZZLE3(w, w, w)
	
};

/*
GLSL相关函数模拟
*/
//step()
float step(float value,float i){
	if(i>value){
		return 1.0;	
	}
	else{
		return 0.;
	}
}
vec2 step(float value,vec2& i){
	vec2 returnValue(i.x,i.y);
	
	if(i.x>value){
		returnValue.x= 1.0;	
	}
	else{
		returnValue.x =0;
	}
	
	if(i.y>value){
		returnValue.y= 1.0;	
	}
	else{
		returnValue.y= 0;
	}
	
	return returnValue;
}
vec2 step(const vec2& edge, const vec2& x) {
	return vec2(step(edge.x, x.x), step(edge.y, x.y));
}
vec3 step(float edge, const vec3& x) {
	return vec3(step(edge, x.x), step(edge, x.y), step(edge, x.z));
}
vec3 step(const vec3& edge, const vec3& x) {
	return vec3(step(edge.x, x.x), step(edge.y, x.y), step(edge.z, x.z));
}
vec4 step(float edge, const vec4& x) {
	return vec4(step(edge, x.x), step(edge, x.y), step(edge, x.z), step(edge, x.w));
}
vec4 step(const vec4& edge, const vec4& x) {
	return vec4(step(edge.x, x.x), step(edge.y, x.y), step(edge.z, x.z), step(edge.w, x.w));
}

//smoothstep()
float smoothstep(float a1,float a2,float x){
	float u,t;
	
	t = (x - a1) / (a2 - a1);
	
	
	u= t * t * (3.0 - 2.0 * t);
	//等同于t = clamp(t, 0.0, 1.0);
	if(t>1.){
		return 1.;
	}
	else if(t<0.){
		return 0.;
	}
	else{
		
		return u;
	}
	
}
vec2 smoothstep(float edge0, float edge1, const vec2& x) {
	return vec2(smoothstep(edge0, edge1, x.x), smoothstep(edge0, edge1, x.y));
}
vec2 smoothstep(const vec2& edge0, const vec2& edge1, const vec2& x) {
	return vec2(
				smoothstep(edge0.x, edge1.x, x.x),
				smoothstep(edge0.y, edge1.y, x.y)
				);
}
vec3 smoothstep(float edge0, float edge1, const vec3& x) {
	return vec3(smoothstep(edge0, edge1, x.x), smoothstep(edge0, edge1, x.y), smoothstep(edge0, edge1, x.z));
}
vec3 smoothstep(const vec3& edge0, const vec3& edge1, const vec3& x) {
	return vec3(
				smoothstep(edge0.x, edge1.x, x.x),
				smoothstep(edge0.y, edge1.y, x.y),
				smoothstep(edge0.z, edge1.z, x.z)
				);
}
vec4 smoothstep(float edge0, float edge1, const vec4& x) {
	return vec4(smoothstep(edge0, edge1, x.x), smoothstep(edge0, edge1, x.y), smoothstep(edge0, edge1, x.z), smoothstep(edge0, edge1, x.w));
}
vec4 smoothstep(const vec4& edge0, const vec4& edge1, const vec4& x) {
	return vec4(
				smoothstep(edge0.x, edge1.x, x.x),
				smoothstep(edge0.y, edge1.y, x.y),
				smoothstep(edge0.z, edge1.z, x.z),
				smoothstep(edge0.w, edge1.w, x.w)
				);
}


//dot()	点乘
float dot(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}
float dot(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
float dot(vec4 a, vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// --- cross 函数 (叉积) ---
// 结果向量垂直于 a 和 b，遵循右手定则
inline vec3 cross(const vec3& a, const vec3& b) {
	return vec3(
				a.y * b.z - a.z * b.y,  // x 分量
				a.z * b.x - a.x * b.z,  // y 分量
				a.x * b.y - a.y * b.x   // z 分量
				);
}

//length()	取模

// 计算 vec2 的长度
inline float length(const vec2& v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

// 计算 vec3 的长度
inline float length(const vec3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//distance
float distance(const vec2& a, const vec2& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return std::sqrt(dx*dx + dy*dy);
}
float distance(const vec3& a, const vec3& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return std::sqrt(dx*dx + dy*dy + dz*dz);
}
float distance(const vec4& a, const vec4& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	float dw = a.w - b.w;
	return std::sqrt(dx*dx + dy*dy + dz*dz + dw*dw);
}

// 归一化 vec2
inline vec2 normalize(const vec2& v) {
	float len_sq = v.x * v.x + v.y * v.y;
	if (len_sq > 0.0f) {
		float inv_len = 1.0f / sqrt(len_sq);
		return vec2(v.x * inv_len, v.y * inv_len);
	}
	return vec2(0.0f, 0.0f); // 零向量保护
}

// 归一化 vec3
inline vec3 normalize(const vec3& v) {
	float len_sq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (len_sq > 0.0f) {
		float inv_len = 1.0f / sqrt(len_sq);
		return vec3(v.x * inv_len, v.y * inv_len, v.z * inv_len);
	}
	return vec3(0.0f, 0.0f, 0.0f); // 零向量保护
}


//sqrt()	开根
inline vec2 sqrt(const vec2& v) {
	return vec2(std::sqrt(v.x), std::sqrt(v.y));
}

inline vec3 sqrt(const vec3& v) {
	return vec3(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z));
}

inline vec4 sqrt(const vec4& v) {
	return vec4(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z), std::sqrt(v.w));
}

//mix()	混合
float mix(float a,float b,float value){
	
	return a*(1.-value)+b*value;
}
vec2 mix(vec2& st,vec2& uv,float value){
	vec2 xy(st.x*(1.-value)+uv.x*value,st.y*(1.-value)+uv.y*value);
	return xy;
}
vec3 mix(const vec3& a, const vec3& b, float t) {
	return vec3(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t
				);
}
vec4 mix(const vec4& a, const vec4& b, float t) {
	return vec4(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t,
				a.w + (b.w - a.w) * t
				);
}
// 向量版本的 mix (按分量混合)
vec2 mix(const vec2& a, const vec2& b, const vec2& t) {
    return vec2(
        a.x * (1.0f - t.x) + b.x * t.x,
        a.y * (1.0f - t.y) + b.y * t.y
    );
}

// 如果你的代码里还有 mix(vec3, vec3, vec3)，也需要添加类似版本
vec3 mix(const vec3& a, const vec3& b, const vec3& t) {
    return vec3(
        a.x * (1.0f - t.x) + b.x * t.x,
        a.y * (1.0f - t.y) + b.y * t.y,
        a.z * (1.0f - t.z) + b.z * t.z
    );
}


//min()	取小
float min(float a,float b){
	if(a>b){
		return b;
	}
	return a;
}

// vec2 与标量比较
inline vec2 min(const vec2& a, float b) {
	return vec2(
				(a.x < b) ? a.x : b,
				(a.y < b) ? a.y : b
				);
}

// 两个 vec2 比较
inline vec2 min(const vec2& a, const vec2& b) {
	return vec2(
				(a.x < b.x) ? a.x : b.x,
				(a.y < b.y) ? a.y : b.y
				);
}

// vec3 与标量比较
inline vec3 min(const vec3& a, float b) {
	return vec3(
				(a.x < b) ? a.x : b,
				(a.y < b) ? a.y : b,
				(a.z < b) ? a.z : b
				);
}

// 两个 vec3 比较
inline vec3 min(const vec3& a, const vec3& b) {
	return vec3(
				(a.x < b.x) ? a.x : b.x,
				(a.y < b.y) ? a.y : b.y,
				(a.z < b.z) ? a.z : b.z
				);
}



//max()	取大
float max(float a,float b){
	if(a<b){
		return b;
	}
	return a;
}

// vec2 与标量的最大值 (对应 max(d, 0.0f))
inline vec2 max(const vec2& a, float b) {
	return vec2(
				(a.x > b) ? a.x : b,
				(a.y > b) ? a.y : b
				);
}

// 两个 vec2 之间的最大值 (分量取大)
inline vec2 max(const vec2& a, const vec2& b) {
	return vec2(
				(a.x > b.x) ? a.x : b.x,
				(a.y > b.y) ? a.y : b.y
				);
}

// vec3 与标量的最大值
inline vec3 max(const vec3& a, float b) {
	return vec3(
				(a.x > b) ? a.x : b,
				(a.y > b) ? a.y : b,
				(a.z > b) ? a.z : b
				);
}

// 两个 vec3 之间的最大值
inline vec3 max(const vec3& a, const vec3& b) {
	return vec3(
				(a.x > b.x) ? a.x : b.x,
				(a.y > b.y) ? a.y : b.y,
				(a.z > b.z) ? a.z : b.z
				);
}


//reflect()
vec3 reflect(const vec3& I, const vec3& N) {
    // 公式: I - 2.0 * dot(N, I) * N
    return I - N * 2.0f * dot(N, I);
}

//floor()
vec2 floor(vec2 st){
	st.x = floor(st.x);
	st.y = floor(st.y);
	return st;
}

//fract()	取小数
float fract(float a){
	return a - floor(a); 
}
vec2 fract(vec2 st){
	st.x=fract(st.x);
	st.y=fract(st.y);
	return st;
}

//sin()
vec2 sin(vec2 st){
	st.x=sin(st.x);
	st.y=sin(st.y);
	return st;
}
vec3 sin(const vec3& v) {
	return vec3(std::sin(v.x), std::sin(v.y), std::sin(v.z));
}
vec4 sin(const vec4& v) {
	return vec4(std::sin(v.x), std::sin(v.y), std::sin(v.z), std::sin(v.w));
}

//cos()	余弦
vec2 cos(const vec2& v) {
	return vec2(std::cos(v.x), std::cos(v.y));
}
vec3 cos(const vec3& v) {
	return vec3(std::cos(v.x), std::cos(v.y), std::cos(v.z));
}
vec4 cos(const vec4& v) {
	return vec4(std::cos(v.x), std::cos(v.y), std::cos(v.z), std::cos(v.w));
}

//atan()
float atan(float y, float x) {
	return atan2(y, x);
}
float atan(const vec2& v) {
	return atan2(v.y, v.x);
}

//abs()
vec2 abs(const vec2& v){
	return vec2(abs(v.x),abs(v.y));
}
vec3 abs(const vec3& v){
	return vec3(abs(v.x),abs(v.y),abs(v.z));
}

//mod()
inline float mod(float x, float y) {
    return x - y * floorf(x / y);
}
// 向量对向量
inline vec2 mod(const vec2& a, const vec2& b) {
    return vec2(mod(a.x, b.x), mod(a.y, b.y));
}
// 向量对标量 (将标量广播到两个分量)
inline vec2 mod(const vec2& a, float b) {
    return vec2(mod(a.x, b), mod(a.y, b));
}
// 向量对向量
inline vec3 mod(const vec3& a, const vec3& b) {
    return vec3(mod(a.x, b.x), mod(a.y, b.y), mod(a.z, b.z));
}
// 向量对标量 (将标量广播到三个分量)
inline vec3 mod(const vec3& a, float b) {
    return vec3(mod(a.x, b), mod(a.y, b), mod(a.z, b));
}

//clamp()
inline float clamp(float x, float minVal, float maxVal) {
    if (x < minVal) return minVal;
    if (x > maxVal) return maxVal;
    return x;
}
// 向量对标量边界
inline vec2 clamp(const vec2& x, float minVal, float maxVal) {
    return vec2(
        clamp(x.x, minVal, maxVal),
        clamp(x.y, minVal, maxVal)
    );
}
// 向量对向量边界
inline vec2 clamp(const vec2& x, const vec2& minVal, const vec2& maxVal) {
    return vec2(
        clamp(x.x, minVal.x, maxVal.x),
        clamp(x.y, minVal.y, maxVal.y)
    );
}
// 向量对标量边界
inline vec3 clamp(const vec3& x, float minVal, float maxVal) {
    return vec3(
        clamp(x.x, minVal, maxVal),
        clamp(x.y, minVal, maxVal),
        clamp(x.z, minVal, maxVal)
    );
}
// 向量对向量边界
inline vec3 clamp(const vec3& x, const vec3& minVal, const vec3& maxVal) {
    return vec3(
        clamp(x.x, minVal.x, maxVal.x),
        clamp(x.y, minVal.y, maxVal.y),
        clamp(x.z, minVal.z, maxVal.z)
    );
}


//mat2()
class mat2 {
public:
	// 存储两列向量 (col[0] 是第一列, col[1] 是第二列)
	vec2 col[2];
	
	// 1. 默认构造：单位矩阵
	mat2() : col{ vec2(1.0f, 0.0f), vec2(0.0f, 1.0f) } {}
	
	// 2. 标量构造：对角线为该值，其余为0
	mat2(float s) : col{ vec2(s, 0.0f), vec2(0.0f, s) } {}
	
	// 3. 按列向量构造
	mat2(const vec2& c0, const vec2& c1) : col{ c0, c1 } {}
	
	// 4. 按列元素构造，对应 GLSL 的 mat2(m00, m10, m01, m11)
	// m00, m10 是第一列；m01, m11 是第二列
	mat2(float m00, float m10, float m01, float m11) : col{ vec2(m00, m10), vec2(m01, m11) } {}
	
	// 重载 [] 获取列向量，像 GLSL 那样 m[0], m[1]
	vec2& operator[](int i) { return col[i]; }
	const vec2& operator[](int i) const { return col[i]; }
	
	// 矩阵 * 矩阵
	mat2 operator*(const mat2& m) const {
		vec2 c0(
				col[0].x * m.col[0].x + col[1].x * m.col[0].y,
				col[0].y * m.col[0].x + col[1].y * m.col[0].y
				);
		vec2 c1(
				col[0].x * m.col[1].x + col[1].x * m.col[1].y,
				col[0].y * m.col[1].x + col[1].y * m.col[1].y
				);
		return mat2(c0, c1);
	}
	
	// 矩阵 * 向量 (列向量右乘: M * v)
	vec2 operator*(const vec2& v) const {
		return vec2(
					col[0].x * v.x + col[1].x * v.y,
					col[0].y * v.x + col[1].y * v.y
					);
	}
	
	// 向量 * 矩阵 (行向量左乘: v * M)
	friend vec2 operator*(const vec2& v, const mat2& m) {
		return vec2(
					v.x * m.col[0].x + v.y * m.col[0].y,
					v.x * m.col[1].x + v.y * m.col[1].y
					);
	}
	
	// 矩阵 * 标量
	mat2 operator*(float s) const {
		return mat2(col[0] * s, col[1] * s);
	}
	friend mat2 operator*(float s, const mat2& m) {
		return mat2(m.col[0] * s, m.col[1] * s);
	}
	
	// 矩阵 + 矩阵
	mat2 operator+(const mat2& m) const {
		return mat2(col[0] + m.col[0], col[1] + m.col[1]);
	}
	
	// 矩阵 - 矩阵
	mat2 operator-(const mat2& m) const {
		return mat2(col[0] - m.col[0], col[1] - m.col[1]);
	}
	
	// 常用矩阵函数：
	
	// 转置矩阵
	mat2 transpose() const {
		return mat2(
					col[0].x, col[1].x, // 第一列变成第一行
					col[0].y, col[1].y  // 第二列变成第二行
					);
	}
	
	// 行列式
	float determinant() const {
		return col[0].x * col[1].y - col[1].x * col[0].y;
	}
	
	// 逆矩阵
	mat2 inverse() const {
		float det = determinant();
		float invDet = 1.0f / det; // 注意：如果 det=0 会导致除零错误
		return mat2(
					col[1].y * invDet, -col[0].y * invDet,
					-col[1].x * invDet,  col[0].x * invDet
					);
	}
};



//mat3

class mat3 {
public:
    float m[3][3];  // 保持原有的内存布局 [行][列]

    // ---------------------------------------------------------
    // 内部代理类：用于实现 m[0] = vec3(...) 的列读写功能
    // ---------------------------------------------------------
    class ColProxy {
    public:
        float (*m)[3]; // 指向矩阵数据的指针
        int col_idx;   // 列索引

        ColProxy(float (*ptr)[3], int idx) : m(ptr), col_idx(idx) {}

        // 赋值操作：m[0] = vec3(...)
        void operator=(const vec3& v) {
            m[0][col_idx] = v.x;
            m[1][col_idx] = v.y;
            m[2][col_idx] = v.z;
        }
        
        // 类型转换：读取，vec3 v = m[0]
        operator vec3() const {
            return vec3(m[0][col_idx], m[1][col_idx], m[2][col_idx]);
        }
    };

    // ---------------------------------------------------------
    // 构造函数
    // ---------------------------------------------------------
    mat3() {
        identity();
    }

    mat3(float diagonal) {
        identity();
        m[0][0] = diagonal;
        m[1][1] = diagonal;
        m[2][2] = diagonal;
    }

    // 注意：GLSL 中 mat3 是列主序，这里为了方便，参数按列传入，内部存储到对应位置
    mat3(const vec3& v1, const vec3& v2, const vec3& v3) {
        // 第一列 v1
        m[0][0] = v1.x; m[1][0] = v1.y; m[2][0] = v1.z;
        // 第二列 v2
        m[0][1] = v2.x; m[1][1] = v2.y; m[2][1] = v2.z;
        // 第三列 v3
        m[0][2] = v3.x; m[1][2] = v3.y; m[2][2] = v3.z;
    }

    mat3(float a00, float a01, float a02,
         float a10, float a11, float a12,
         float a20, float a21, float a22) {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
    }

    void identity() {
        memset(m, 0, sizeof(m));
        m[0][0] = m[1][1] = m[2][2] = 1.0f;
    }

    // ---------------------------------------------------------
    // 运算符重载
    // ---------------------------------------------------------

    // 核心：下标运算符，返回代理对象以支持读写
    ColProxy operator[](int i) {
        return ColProxy(m, i);
    }
    
    // const 版本：只读
    vec3 operator[](int i) const {
        return vec3(m[0][i], m[1][i], m[2][i]);
    }

    // 矩阵乘法
    mat3 operator*(const mat3& other) const {
        mat3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 3; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // 矩阵与向量乘法
    vec3 operator*(const vec3& v) const {
        vec3 result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
        return result;
    }

    // 矩阵转置
    mat3 transpose() const {
        mat3 result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result.m[i][j] = m[j][i];
        return result;
    }

    // 行列式
    float determinant() const {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    // 伴随矩阵
    mat3 adjugate() const {
        mat3 result;
        result.m[0][0] =  (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
        result.m[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]);
        result.m[0][2] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]);

        result.m[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]);
        result.m[1][1] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
        result.m[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]);

        result.m[2][0] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
        result.m[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]);
        result.m[2][2] =  (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
        return result.transpose(); // 注意：这里需要转置才是伴随矩阵
    }
    
    // 求逆
    mat3 inverse() const {
        float det = determinant();
        if (fabs(det) < 1e-6f) return mat3();
        mat3 adj = adjugate();
        return adj * (1.0f / det);
    }
    
    // 其他运算符保持不变...
    mat3 operator+(const mat3& other) const { /*...*/ return *this; } // 简略
    mat3 operator-(const mat3& other) const { /*...*/ return *this; } // 简略
    mat3 operator*(float scalar) const { /*...*/ return *this; } // 简略
};


// class mat3 {
// public:
// 	float m[3][3];  // 3x3 矩阵数据
	
// 	// 构造函数
// 	mat3() {
// 		identity();
// 	}
	
// 	mat3(float diagonal) {
// 		identity();
// 		m[0][0] = diagonal;
// 		m[1][1] = diagonal;
// 		m[2][2] = diagonal;
// 	}
	
// 	mat3(float a00, float a01, float a02,
// 		 float a10, float a11, float a12,
// 		 float a20, float a21, float a22) {
// 		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
// 		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
// 		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
// 	}
	
// 	// 由三个列向量构造矩阵 (v1, v2, v3 分别为第一、二、三列)
// 	mat3(const vec3& v1, const vec3& v2, const vec3& v3) {
// 		// 假设 m[row][col] 存储方式：
// 		// 第一列 (v1) 放入 m[?][0]
// 		m[0][0] = v1.x; m[1][0] = v1.y; m[2][0] = v1.z;
// 		// 第二列 (v2) 放入 m[?][1]
// 		m[0][1] = v2.x; m[1][1] = v2.y; m[2][1] = v2.z;
// 		// 第三列 (v3) 放入 m[?][2]
// 		m[0][2] = v3.x; m[1][2] = v3.y; m[2][2] = v3.z;
// 	}
	
// 	// 从一维数组初始化
// 	mat3(const float* data) {
// 		memcpy(m, data, sizeof(m));
// 	}
	
// 	// 单位矩阵
// 	void identity() {
// 		memset(m, 0, sizeof(m));
// 		m[0][0] = m[1][1] = m[2][2] = 1.0f;
// 	}
	
// 	// 矩阵转置
// 	mat3 transpose() const {
// 		mat3 result;
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = 0; j < 3; j++) {
// 				result.m[i][j] = m[j][i];
// 			}
// 		}
// 		return result;
// 	}
	
// 	// 矩阵乘法
// 	mat3 operator*(const mat3& other) const {
// 		mat3 result;
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = 0; j < 3; j++) {
// 				result.m[i][j] = 0.0f;
// 				for (int k = 0; k < 3; k++) {
// 					result.m[i][j] += m[i][k] * other.m[k][j];
// 				}
// 			}
// 		}
// 		return result;
// 	}
	
// 	// 矩阵与向量乘法
// 	vec3 operator*(const vec3& v) const {
// 		vec3 result;
// 		result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
// 		result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
// 		result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
// 		return result;
// 	}
	
// 	// 矩阵加法
// 	mat3 operator+(const mat3& other) const {
// 		mat3 result;
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = 0; j < 3; j++) {
// 				result.m[i][j] = m[i][j] + other.m[i][j];
// 			}
// 		}
// 		return result;
// 	}
	
// 	// 矩阵减法
// 	mat3 operator-(const mat3& other) const {
// 		mat3 result;
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = 0; j < 3; j++) {
// 				result.m[i][j] = m[i][j] - other.m[i][j];
// 			}
// 		}
// 		return result;
// 	}
	
// 	// 矩阵标量乘法
// 	mat3 operator*(float scalar) const {
// 		mat3 result;
// 		for (int i = 0; i < 3; i++) {
// 			for (int j = 0; j < 3; j++) {
// 				result.m[i][j] = m[i][j] * scalar;
// 			}
// 		}
// 		return result;
// 	}
	
// 	// 矩阵求逆（使用伴随矩阵法）
// 	mat3 inverse() const {
// 		float det = determinant();
// 		if (fabs(det) < 1e-6f) {
// 			return mat3();  // 奇异矩阵，返回单位矩阵
// 		}
		
// 		mat3 adj = adjugate();
// 		return adj * (1.0f / det);
// 	}
	
// 	// 计算行列式
// 	float determinant() const {
// 		return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
// 		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
// 		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
// 	}
	
// 	// 计算伴随矩阵
// 	mat3 adjugate() const {
// 		mat3 result;
// 		result.m[0][0] =  (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
// 		result.m[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]);
// 		result.m[0][2] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
		
// 		result.m[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]);
// 		result.m[1][1] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
// 		result.m[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]);
		
// 		result.m[2][0] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
// 		result.m[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]);
// 		result.m[2][2] =  (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
		
// 		return result.transpose();
// 	}
	
// 	// 生成旋转矩阵（绕X轴）
// 	static mat3 rotationX(float angle) {
// 		float c = cos(angle);
// 		float s = sin(angle);
// 		return mat3(
// 					1.0f, 0.0f, 0.0f,
// 					0.0f, c, -s,
// 					0.0f, s, c
// 					);
// 	}
	
// 	// 生成旋转矩阵（绕Y轴）
// 	static mat3 rotationY(float angle) {
// 		float c = cos(angle);
// 		float s = sin(angle);
// 		return mat3(
// 					c, 0.0f, s,
// 					0.0f, 1.0f, 0.0f,
// 					-s, 0.0f, c
// 					);
// 	}
	
// 	// 生成旋转矩阵（绕Z轴）
// 	static mat3 rotationZ(float angle) {
// 		float c = cos(angle);
// 		float s = sin(angle);
// 		return mat3(
// 					c, -s, 0.0f,
// 					s, c, 0.0f,
// 					0.0f, 0.0f, 1.0f
// 					);
// 	}
	
// 	// 生成缩放矩阵
// 	static mat3 scale(float sx, float sy, float sz) {
// 		return mat3(
// 					sx, 0.0f, 0.0f,
// 					0.0f, sy, 0.0f,
// 					0.0f, 0.0f, sz
// 					);
// 	}
	
// 	// 生成均匀缩放矩阵
// 	static mat3 scale(float s) {
// 		return scale(s, s, s);
// 	}
	
// 	// 获取矩阵元素
// 	float& operator()(int row, int col) {
// 		return m[row][col];
// 	}
	
// 	const float& operator()(int row, int col) const {
// 		return m[row][col];
// 	}
// };


//DOS优化
//256色===========================================================
class Dos256Draw{
public:
	unsigned char screen_buffer[64000]; 
	Dos256Draw(){

	}
	
	
	// 设置 320x200 256色模式
	void setMode13h() {
		__dpmi_regs r;
		r.x.ax = 0x0013;
		__dpmi_int(0x10, &r);
	}
	
// 恢复文本模式
	void setTextColorMode() {
		__dpmi_regs r;
		r.x.ax = 0x0003;
		__dpmi_int(0x10, &r);
	}
	
// 设置 256 色调色板
	void setGrayscalePalette() {
		__dpmi_regs r;
		for (int i = 0; i < 256; i++) {
			r.x.ax = 0x1010;
			r.x.bx = i;
			r.h.ch = 0;
			
			//位运算拆解：把 0~255 的索引，逆向算出它代表的 R、G、B
			int red   = (i >> 5) & 0x07;       // 取高 3 位 (0-7)
			int green = (i >> 2) & 0x07;       // 取中 3 位 (0-7)
			int blue  = i & 0x03;             // 取低 2 位 (0-3)
			
			// 映射到 VGA 的 0-63 范围
			r.h.cl = red * 9;    // 7 * 9 = 63
			r.h.dh = green * 9;
			r.h.dl = blue * 21;  // 3 * 21 = 63
			
			__dpmi_int(0x10, &r);
		}
	}
	
//在屏幕上绘制
	void draw(vec4& outColor,vec2& cFragCoord,vec2& u_resolution){
		// 把 GLSL 算出来的浮点颜色 (0.0~1.0)，转成 0~7 或 0~3 的整数
		int r = (int)(outColor.x * 7.0f); if (r > 7) r = 7; if (r < 0) r = 0;
		int g = (int)(outColor.y * 7.0f); if (g > 7) g = 7; if (g < 0) g = 0;
		int b = (int)(outColor.z * 3.0f); if (b > 3) b = 3; if (b < 0) b = 0;
		
		// 用位运算把它们拼成一个 0~255 的调色板索引！
		// 这个计算方式和上面的 setTrueColorPalette 是完全镜像对应的
		int c_index = (r << 5) | (g << 2) | b;
		
		screen_buffer[64000 - (int)cFragCoord.y * (int)u_resolution.x + (int)cFragCoord.x] = (unsigned char)c_index;
	}
};





class Dos32Draw {
public:
    int SCREEN_W;
    int SCREEN_H;
    unsigned int* frame_buffer = NULL;
    unsigned char* line_buffer_24 = NULL; // 堆上行缓冲区，防止栈溢出
    __dpmi_meminfo vga_info;
    bool is_init = false;
    int vga_selector = 0;
    unsigned long mem_size = 0;
    int bytes_per_scanline = 0;
    bool is_24bpp = false;
    
#pragma pack(push, 1)
    struct VbeModeInfo {
        unsigned short mode_attributes; unsigned char a, b;
        unsigned short c, d, e, f; unsigned long g;
        unsigned short bytes_per_scanline; unsigned short x_res, y_res;
        unsigned char x_char, y_char, planes, bpp, banks, model, bank_size, pages, reserved;
        unsigned char r_mask, r_pos, g_mask, g_pos, b_mask, b_pos, rsv_mask, rsv_pos, direct_color;
        unsigned long phys_base_ptr;
    };
#pragma pack(pop)

    // 【关键】这里定义 get_vesa_mode，必须位于 init_graphics 之前或都在类内
    int get_vesa_mode(int w, int h) {
        if (w == 320 && h == 200) return 0x10F;
        if (w == 640 && h == 480) return 0x112;
        if (w == 800 && h == 600) return 0x115;
        if (w == 1024 && h == 768) return 0x118;
        return -1;
    }
    
    // 构造函数
    Dos32Draw(vec2 u_resolution) {
        SCREEN_W = (int)u_resolution.x;
        SCREEN_H = (int)u_resolution.y;
        mem_size = SCREEN_W * SCREEN_H * 4;
    }
    
    // 析构函数：确保退出时自动清理
    ~Dos32Draw() {
        close_graphics();
    }

    bool init_graphics() {
        if (is_init) return true;

        int target_mode = get_vesa_mode(SCREEN_W, SCREEN_H); // 现在可以找到了
        if (target_mode == -1) return false;
        
        // 分配主缓冲区
        frame_buffer = new unsigned int[SCREEN_W * SCREEN_H];
        if (!frame_buffer) return false;
        
        // 分配行缓冲区 (防止栈溢出)
        line_buffer_24 = new unsigned char[SCREEN_W * 3];
        if (!line_buffer_24) {
            delete[] frame_buffer; 
            frame_buffer = NULL;
            return false;
        }
        
        __dpmi_regs r;
        int dos_selector;
        int dos_segment = __dpmi_allocate_dos_memory((sizeof(VbeModeInfo) + 15) / 16, &dos_selector);
        if (dos_segment == -1) {
             delete[] frame_buffer; frame_buffer = NULL;
             delete[] line_buffer_24; line_buffer_24 = NULL;
             return false;
        }
        
        r.x.ax = 0x4F01; 
        r.x.cx = target_mode;
        r.x.es = dos_segment; r.x.di = 0;
        __dpmi_int(0x10, &r);
        
        VbeModeInfo info;
        movedata(dos_selector, 0, _my_ds(), (unsigned int)&info, sizeof(VbeModeInfo));
        __dpmi_free_dos_memory(dos_selector);
        
        if (info.phys_base_ptr == 0) {
             delete[] frame_buffer; frame_buffer = NULL;
             delete[] line_buffer_24; line_buffer_24 = NULL;
             return false;
        }
        
        is_24bpp = (info.bpp == 24);
        bytes_per_scanline = info.bytes_per_scanline;
        
        r.x.ax = 0x4F02; 
        r.x.bx = target_mode | 0x4000; 
        __dpmi_int(0x10, &r);
        
        vga_info.size = mem_size;
        vga_info.address = info.phys_base_ptr;
        if (__dpmi_physical_address_mapping(&vga_info) != 0) {
             delete[] frame_buffer; frame_buffer = NULL;
             delete[] line_buffer_24; line_buffer_24 = NULL;
             return false;
        }
        
        vga_selector = __dpmi_allocate_ldt_descriptors(1);
        if (vga_selector == -1) {
            __dpmi_free_physical_address_mapping(&vga_info);
             delete[] frame_buffer; frame_buffer = NULL;
             delete[] line_buffer_24; line_buffer_24 = NULL;
            return false;
        }
        
        if (__dpmi_set_segment_base_address(vga_selector, vga_info.address) == -1) return false;
        if (__dpmi_set_segment_limit(vga_selector, 0xFFFFFFFF) == -1) return false;
        
        is_init = true;
        return true;
    }
    
    inline void put_pixel(vec2 cFragCoord, vec4 color) {
        int x = (int)cFragCoord.x;
        int y = (int)cFragCoord.y;
        if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H) {
            int r = (int)(color.x * 255.0f);
            int g = (int)(color.y * 255.0f);
            int b = (int)(color.z * 255.0f);
            if (r < 0) r = 0; if (r > 255) r = 255;
            if (g < 0) g = 0; if (g > 255) g = 255;
            if (b < 0) b = 0; if (b > 255) b = 255;
            frame_buffer[(SCREEN_H - 1 - y) * SCREEN_W + x] = b | (g << 8) | (r << 16);
        }
    }

    inline void update_screen() {
        if (!is_init) return;
        
        if (!is_24bpp) {
             // 32bpp 快速拷贝
             if (bytes_per_scanline == SCREEN_W * 4) {
                 movedata(_my_ds(), (unsigned int)frame_buffer, vga_selector, 0, SCREEN_W * SCREEN_H * 4);
             } else {
                 unsigned int src_offset = 0;
                 unsigned int dest_offset = 0;
                 for (int y = 0; y < SCREEN_H; y++) {
                     movedata(_my_ds(), (unsigned int)frame_buffer + src_offset, vga_selector, dest_offset, SCREEN_W * 4);
                     src_offset += SCREEN_W * 4;
                     dest_offset += bytes_per_scanline;
                 }
             }
        } else {
             // 24bpp 模式转换
             for (int y = 0; y < SCREEN_H; y++) {
                 unsigned int* src_row = &frame_buffer[y * SCREEN_W];
                 for (int x = 0; x < SCREEN_W; x++) {
                     unsigned int col = src_row[x];
                     line_buffer_24[x * 3 + 0] = col & 0xFF;        // B
                     line_buffer_24[x * 3 + 1] = (col >> 8) & 0xFF; // G
                     line_buffer_24[x * 3 + 2] = (col >> 16) & 0xFF;// R
                 }
                 movedata(_my_ds(), (unsigned int)line_buffer_24, vga_selector, y * bytes_per_scanline, SCREEN_W * 3);
             }
        }
    }
    
    void close_graphics() {
        if (frame_buffer) {
            delete[] frame_buffer;
            frame_buffer = NULL;
        }
        if (line_buffer_24) {
            delete[] line_buffer_24;
            line_buffer_24 = NULL;
        }
        
        if (is_init) {
            if (vga_selector != 0) {
                __dpmi_free_ldt_descriptor(vga_selector);
                vga_selector = 0;
            }
            __dpmi_free_physical_address_mapping(&vga_info);
            is_init = false; 
        }
        
        __dpmi_regs r;
        r.x.ax = 0x0003; 
        __dpmi_int(0x10, &r);
    }
};




#endif

