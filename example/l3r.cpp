
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
	
    float rect(vec2 st){
    st = abs(st);
    float sdf = max(st.x,st.y);
	return sdf;
    }

float spr(vec2 st,float l){

    float dis =length(st);
    float dl =1.- smoothstep(l,l+.1,dis);
    dis=(1.-dis)*dl;
    return dis;
}
float rectr(vec2 st,float l){
    
    float dis =rect(st);
    float dl = 1.-smoothstep(l,l+0.1,dis);
    dis = (1.-dis)*dl;
    return dis;
}
	
	
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
	st-=vec2(.5);
    st*=2.;
    
    vec3 color = vec3(0.);
    
    //颜色
    vec3 colrect =vec3(0.611,0.915,0.237);
	 vec3 colsp =vec3(0.045,0.023,0.022);
    
    //矩形
    float rectDist = rectr(st-vec2(-0.680,0.200),0.280);
    float notIntegratedRRect =0.948;//设定不会混合颜色的半径(反比)
    float notIntegratedZone = smoothstep(notIntegratedRRect,notIntegratedRRect+0.484,rectDist);//不会混合颜色的区域
    vec3 crect = notIntegratedZone*colrect;//不混合颜色区域的颜色合成
    
    //圆
    float circleDist = 0.;
    float R = .5;//圆融合的扩散半径
    float notIntegratedRSp = 0.884;//设定不会混合颜色的半径(反比)
    float notIntegratedZoneSp =0.;//不会混合颜色的区域
    vec3 csp =vec3(0.);//不混合颜色区域的颜色合成
    for(int i = 0;i<5;i++){
    	float cellDist =spr(st-vec2(R*sin(u_time+float(i))+exp(sin(u_time*3.+float(i)))*0.148,R*cos(u_time+float(i))),0.172);
    	circleDist += cellDist;
    	notIntegratedZoneSp +=smoothstep(notIntegratedRSp,notIntegratedRSp+0.284,cellDist);
    	csp=notIntegratedZoneSp*(colsp+vec3(1.228*float(i),0.484*float(i),0.324*float(i)));
    }
       
    
    //融合
    float dis = rectDist+circleDist;
    float fr = 0.268;//融合高度
    float fusionSDF = smoothstep(fr,fr+.1,dis);
    
    
    vec3 col = crect+csp+	//不混合颜色区域的颜色显示
        fusionSDF*(1.-(notIntegratedZone+notIntegratedZoneSp))*mix(colrect,colsp,circleDist);//混合颜色的区域显示
    
    color = vec3(col);


    
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

