#version 330 compatibility

in float vData;
void main( ){
	vec3 color;	
	if(vData>=4.)
		color = vec3(0.,mix(0.,1.,vData),mix(0.,1.,vData/2.));
	else 
	if(vData<=.2)
		color = vec3(0.,0.2,0.3);
	else
		color = vec3(0.,mix(0.,1.,vData/2.),mix(0.,1.,vData));

	gl_FragColor = vec4( color,  1. );

}
