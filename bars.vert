#version 330 compatibility

out float	vData;

void
main( )
{ 
	vec3 vert = gl_Vertex.xyz;
	float data = gl_Vertex.w/10000.;
	vData = data;
	gl_Position = vec4(vert,1.);
}
