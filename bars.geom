#version 400 compatibility
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

layout( triangles )  in;
layout( triangle_strip, max_vertices=100 )  out;

out vec3 gNormal;
out vec3 gEye;
out vec3 gLight;
out vec3 gPosition;
out float gData;

in float vData[3];

void emitVertex(float x, float y, float z,float nx, float ny, float nz){
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(x,y,z,1.);
	vec4 ecpos = gl_ModelViewMatrix * vec4(x,y,z,1.);
	gNormal = normalize( gl_NormalMatrix * vec3(nx,ny,nz));	
	gLight = vec3(10.,10.,10.) - ecpos.xyz;
	gEye = vec3( 0., 0., 0. ) - ecpos.xyz;			
	EmitVertex();
}

void main( ){
	vec3 v0 = gl_PositionIn[0].xyz;
	vec3 v1 = gl_PositionIn[1].xyz;
	vec3 v2 = gl_PositionIn[2].xyz;
	float data = vData[0]/100.; 
	if(data>=10)
		data=10;
	gData = data;
	emitVertex(v0.x,v0.y+data/1,v0.z,0.,1.,0.);
	emitVertex(v1.x,v1.y+data/1,v1.z,0.,1.,0.);
	emitVertex(v2.x,v2.y+data/1,v2.z,0.,1.,0.);

	EndPrimitive();
	int dir = 1;
	if(v0.x < v1.x || v0.z < v2.z){
		gData = data/5.;
		emitVertex(v2.x,v2.y,v2.z,	0.,0.,-1.);
		gData = data;
		emitVertex(v2.x,v2.y+data/1,v2.z,	0.,0.,-1.);
		gData = data/5.;
		emitVertex(v0.x,v0.y,v0.z,	0.,0.,-1.);
		gData = data;
		emitVertex(v0.x,v0.y+data/1,v0.z,	0.,0.,-1.);
		EndPrimitive();

		gData = data;
		emitVertex(v0.x,v0.y+data/1,v0.z,	-1.,0.,0.);
		gData = data/5;
		emitVertex(v0.x,v0.y,v0.z,	-1.,0.,0.);
		gData = data;
		emitVertex(v1.x,v1.y+data/1.,v1.z,	-1.,0.,0.);
		gData = data/5.;
		emitVertex(v1.x,v1.y,v1.z,	-1.,0.,0.);
	}else{
		gData = data/5.;
		emitVertex(v2.x,v2.y,v2.z,	1.,0.,0.);
		gData = data;
		emitVertex(v2.x,v2.y+data/1,v2.z,	1.,0.,0.);
		gData = data/5.;
		emitVertex(v0.x,v0.y,v0.z,	1.,0.,0.);
		gData = data;
		emitVertex(v0.x,v0.y+data/1,v0.z,	1.,0.,0);
		EndPrimitive();

		gData = data;
		emitVertex(v0.x,v0.y+data/1,v0.z,	0.,0.,1.);
		gData = data/5;
		emitVertex(v0.x,v0.y,v0.z,	0,0.,1.);
		gData = data;
		emitVertex(v1.x,v1.y+data/1,v1.z,	0.,0.,1.);	
		gData = data/5.;
		emitVertex(v1.x,v1.y,v1.z,	0.,0.,1.);
		
	}	
}
