uniform sampler2DRect src_tex_unit0;
uniform sampler2DRect src_tex_unit1;

varying float mixamt;

varying vec3 normal;
varying vec4 pos;

uniform int useLighting;


void main( void ){  

	if (useLighting != 0){
	
		vec4 color = gl_FrontMaterial.diffuse;
		vec4 matspec = gl_FrontMaterial.specular;
		float shininess = gl_FrontMaterial.shininess;
		vec4 lightspec = gl_LightSource[0].specular;
		vec4 lpos = gl_LightSource[0].position;
		vec4 s = -normalize(pos-lpos); 

		vec3 light = s.xyz;
		vec3 n = normalize(normal);
		vec3 r = -reflect(light, n);
		r = normalize(r);
		vec3 v = -pos.xyz;
		v = normalize(v);

		vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) *             gl_LightSource[0].diffuse;
		vec4 specular;
		if (shininess != 0.0) {
			specular = lightspec * matspec * pow(max(0.0,                 dot(r, v)), shininess);
		} else {
			specular = vec4(0.0, 0.0, 0.0, 0.0);
		}

		vec2 st = gl_TexCoord[0].st;
		vec4 texel0 = texture2DRect(src_tex_unit0, st);
		vec2 st2 = gl_TexCoord[1].st;
		vec4 texel1 = texture2DRect(src_tex_unit1, st2);
		gl_FragColor = (gl_Color +  (diffuse + specular)*0.3) * mix(texel0, texel1, mixamt);
	
	} else {
	
		vec2 st = gl_TexCoord[0].st;
		vec4 texel0 = texture2DRect(src_tex_unit0, st);
		vec2 st2 = gl_TexCoord[1].st;
		vec4 texel1 = texture2DRect(src_tex_unit1, st2);
		gl_FragColor = gl_Color * mix(texel0, texel1, mixamt);
	}
	
	
}


