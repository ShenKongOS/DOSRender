
#include <dos.h>

#include "sk.hpp"

#define resolutionX 320
#define resolutionY 200
#define VGA_SEGMENT 0xA000

extern "C" {
	void dosmemput(const void *src, size_t len, unsigned long destoffs);
}


class Shader_frag{
	double u_time;
public:
	
float opSmoothUnion( float d1, float d2, float k )
{
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
} 

float map(vec3 p,float  iTime)
{
	float d = 2.0;
	for (int i = 0; i < 16; i++) {
		float fi = float(i);
		float time = iTime * (fract(fi * 412.531 + 0.513) - 0.5) * 2.0;
		d = opSmoothUnion(
            sdSphere(p + sin(time + fi * vec3(52.5126, 64.62744, 632.25)) * vec3(2.0, 2.0, 0.8), mix(0.5, 1.0, fract(fi * 412.531 + 0.5124))),
			d,
			0.4
		);
	}
	return d;
}

vec3 calcNormal( in vec3 p ,float iTime)
{
    const float h = 1e-5; // or some other value
    const vec2 k = vec2(1,-1);
    return normalize( k.xyy()*map( p + k.xyy()*h ,iTime) + 
                      k.yyx()*map( p + k.yyx()*h ,iTime) + 
                      k.yxy()*map( p + k.yxy()*h ,iTime) + 
                      k.xxx()*map( p + k.xxx()*h ,iTime) );
}


	
	
	vec4 frag_GL(vec2 cFragCoord,vec2 u_resolution,double u_time){
		this->u_time=u_time;

		vec2 iResolution = vec2(u_resolution.x, u_resolution.y);
		float iTime = u_time;
		vec2 fragCoord = cFragCoord;
		vec4 fragColor = vec4(0.0);

    vec2 uv = fragCoord.xy()/iResolution.xy();
    
    // screen size is 6m x 6m
	vec3 rayOri = vec3((uv - 0.5) * vec2(iResolution.x/iResolution.y, 1.0) * 6.0, 3.0);
	vec3 rayDir = vec3(0.0, 0.0, -1.0);
	
	float depth = 0.0;
	vec3 p;
	
	for(int i = 0; i < 64; i++) {
		p = rayOri + rayDir * depth;
		float dist = map(p, iTime);
        depth += dist;
		if (dist < 1e-6) {
			break;
		}
	}
	
    depth = min(6.0, depth);
	vec3 n = calcNormal(p,iTime);
    float b = max(0.0, dot(n, vec3(0.577)));
    vec3 col = (0.5 + 0.5 * cos((b + iTime * 3.0) + uv.xyx() * 2.0 + vec3(0,2,4))) * (0.85 + b * 0.35);
    col *= exp( -depth * 0.15 );
	
    // maximum thickness is 2m in alpha channel
    fragColor = vec4(col, 1.0 - (depth - 0.5) / 2.0);
		


		return fragColor;
	}
};



int main(void) {

	//分辨率
	//RESOLUTION u_resolution = {resolutionX,resolutionY};
	vec2 u_resolution = vec2(resolutionX,resolutionY);
	//对应像素位置坐标
	//FRAGCOORD cFragCoord = {0,0};
	vec2 cFragCoord = vec2(0);
	//输出颜色
	vec4 outColor(0.);
	//时间
	clock_t start_time = clock();
	double time_now = 0.0;
	//片段着色器
	Shader_frag shader_frag;
	
	//DOS绘制设置
	Dos32Draw dosDraw(vec2(320.0f, 200.0f));
	if (!dosDraw.init_graphics()) {
		printf("Init failed!\n");
		return 1;
	}
	
	while(!kbhit()){
		time_now = (double)(clock() - start_time) / CLOCKS_PER_SEC;
		//绘制一帧
		for(cFragCoord.y=0;cFragCoord.y<u_resolution.y;cFragCoord.y++){
			for(cFragCoord.x=0;cFragCoord.x<u_resolution.x;cFragCoord.x++){
				
				
				outColor = shader_frag.frag_GL(cFragCoord,u_resolution,time_now);
				//outColor = vec4(cFragCoord.x / 320.0f, cFragCoord.y / 200.0f, 0.5f, 1.0f); // 临时测试
				//dosDraw.draw(outColor,cFragCoord,u_resolution);
				dosDraw.put_pixel(cFragCoord, outColor);

			}
			
		}

		dosDraw.update_screen();
	}
	/* 恢复文本模式并退出 */
	dosDraw.close_graphics();
	return 0;
}

