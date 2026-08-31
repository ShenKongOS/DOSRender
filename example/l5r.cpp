
#include <dos.h>

#include "sk.hpp"



#define resolutionX 320
#define resolutionY 200
#define VGA_SEGMENT 0xA000

//#define shadertoy

extern "C" {
	void dosmemput(const void *src, size_t len, unsigned long destoffs);
}


class Shader_frag{
	double u_time;
public:
	

	
	vec4 frag_GL(vec2 cFragCoord,vec2 u_resolution,double u_time){
		this->u_time=u_time;
#ifdef shadertoy
		vec2 iResolution = vec2(u_resolution.x, u_resolution.y);
		float iTime = u_time;
		vec2 fragCoord = cFragCoord;
		vec4 fragColor = vec4(0.0);
#endif


#define TWO_PI 6.28318530718
  vec2 st = cFragCoord.xy()/u_resolution.xy();
  st.x *= u_resolution.x/u_resolution.y;
  vec3 color = vec3(0.0);
  float d = 0.0;

  st = st *2.-1.;

  int N = 3;

  float a = atan(st.y,st.x)+PI*sin(u_time);//+PI/2.
  float r = TWO_PI/float(N);
  float my_a =atan(st.y,st.x)+PI*sin(u_time);
  float my_r = TWO_PI/3.0;
    
  d = cos(floor(.5+a/r)*r-a)*length(st);
  float d2 = cos(floor(.5+my_a/my_r)*my_r-my_a)*length(st);
  color = vec3(1.0-smoothstep(.4,.41,d));
  color = mix(color,vec3(abs(sin(u_time))+0.2,0.2,0.3),d2);

   
    
        vec4 fragColor = vec4(color, 1.0);


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

