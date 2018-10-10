#version 330 compatibility

in float gData;

in vec3 gLight;
in vec3 gNormal;
in vec3 gEye;
in vec3 gPosition;

void
main( )
{	
	vec4 color;
	float data = gData;
	float r,g,b;
	r=1.;g=1.;b=1.;
	color = vec4(mix(0.2,.5,data),0,mix(0.3,.7,data),1.);
    vec3 Normal = normalize(gNormal);
	vec3 Light=normalize(gLight);
	vec3 Eye=normalize(gEye);

	vec4 ambient = 0.3 * color;
	vec4 diffuse = 0.6 *  max( dot(Normal,Light),0. )* color;
	float s = 0.;
	if( dot(Normal,Light) > 0. ){
		vec3 ref = normalize( 2. * Normal * dot(Normal,Light) - Light );
		s = pow( max( dot(Eye,ref),0. ),20. );
	}
	vec4 specular = .6 * s * vec4(1.,1.,1.,1.);
	gl_FragColor = vec4( ambient.rgb + diffuse.rgb + specular.rgb, 1. );

}
