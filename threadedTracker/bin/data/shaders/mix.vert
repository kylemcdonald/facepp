

attribute float mixAmount;
varying float mixamt;
varying vec3 normal;
varying vec4 pos;
varying vec4 rawpos;

void main() {
	normal = gl_NormalMatrix * gl_Normal;
	pos = gl_ModelViewMatrix * gl_Vertex;
	rawpos = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	mixamt = mixAmount;
} 
