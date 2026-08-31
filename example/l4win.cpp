
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


    vec2 st = cFragCoord.xy()/u_resolution.xy();
    st.x *= u_resolution.x/u_resolution.y;
	 vec3 color = vec3(0.);
    vec2 pos = st;
    //双边
    float l = 0.220;
	vec2 tr = smoothstep(l,l+0.01,st);
	vec2 bl = smoothstep(l,l+0.01,1.-st);
    
    //斜边
    float ykbottom=st.y*3.+-0.216;
    float yktop =st.y*3.+-1.840;
    float xubottom =1.- smoothstep(ykbottom,ykbottom+0.05,1.-st.x);
    float xutop =1.- smoothstep(1.-yktop,1.-yktop+0.05,1.-st.x);
    
    pos *= 2.000;
    pos-=vec2(-0.09,0.0);
    pos = fract(pos);
    float lk = 0.004;
	vec2 trk = smoothstep(lk,lk+0.01,pos);
	vec2 blk = smoothstep(lk,lk+0.01,1.-pos);
    float bk = trk.x*trk.y*blk.x*blk.y;
    
    float d = tr.x*bl.x*xubottom*xutop*bk;
    vec3 orgcolor = vec3(0.695*sin(u_time),0.159*sin(u_time+.4),0.547*sin(u_time+1.));
    
    
    vec3 ys =sin(st.x*6.+u_time*5.)*d*vec3(0.603,0.620,0.415);// abs((mod(st.x+u_time,1.)-.5)*2.)*max(0.,floor(sin(u_time)*2.))*d;
    
    color = vec3(d,d,d)*orgcolor+vec3(ys);

   
    
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

