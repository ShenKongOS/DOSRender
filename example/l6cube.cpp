
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
	
    vec3 getMaterialColor(vec3 p) {

    vec3 color = vec3(0.659,0.981,1.000); 
    return color;
}

float sdRoundBox( vec3 p, vec3 b, float r )
{
  vec3 q = abs(p) - b + r;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - r;
}

float sdf(vec3 p){
    float plant = p.y;
	 float box = sdRoundBox(p-vec3(0.,1.,0.),vec3(0.580,0.580,0.580),abs(0.972*sin(u_time*.3)));
    
    float d = min(plant,box);
    return d;
}

mat3 getmat(vec3 ro,vec3 ta,vec3 up){
    vec3 f = normalize( ta - ro );
    vec3 r = normalize(cross(up,f));
    vec3 u = normalize(cross(f,r));
    
    return mat3(r,u,f);
}

float ray(vec3 ro,vec3 rd){
    float d0=0.;
    
    for(int i = 0;i<50;i++){
        vec3 p = ro + rd*d0;
        float ds =sdf(p);
        d0+=ds;
        
        if(ds<0.0001||ds>100.){
            break;
        }
    }
    return d0;
}

vec3 getnormal(vec3 p){
    float esp = 0.001;
    vec2 h = vec2(esp,0.);
    
    float d = sdf(p);
    return normalize(d-vec3(sdf(p-h.xyy()),sdf(p-h.yxy()),sdf(p-h.yyx())));
}


float light(vec3 lightP,vec3 p){
    vec3 l = normalize(lightP-p);
    vec3 n = getnormal(p);
    float dif = max(dot(n,l),0.);
    
    float d = ray(p+0.01*n,l);
    
    if(d<length(lightP-p)){
        dif*=0.1;
    }
    return dif;
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

    vec3 color = vec3(0.);
    st-=vec2(.5);
    
    vec3 ro = vec3(5.*cos(u_time),1.440,5.*sin(u_time));
    vec3 ta = vec3(0.,0.,0.);
    
    vec3 rd = getmat(ro,ta,vec3(0.,1.0,0.)) * vec3(st,1.);
    rd = normalize(rd);
    
    float d = ray(ro,rd);
    
    vec3 p = ro+rd*d;
    
    float au = light(vec3(2.886,3.000,2.125),p);
    vec3 s =getMaterialColor(p);
    color = vec3(s*au);
        //color = vec3(st.x);
    
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

