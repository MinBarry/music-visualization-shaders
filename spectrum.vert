#version 330 compatibility

out float	vData;
void main( ) { 
	vec3 vert = gl_Vertex.xyz;
	float data = gl_Vertex.w;
	if(data>4.)
		vert = vert+(4.*normalize(gl_Normal));
	else
		vert = vert+((data)*normalize(gl_Normal));
	vData = data;
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
