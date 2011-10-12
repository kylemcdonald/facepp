varying vec3 N;
varying vec3 v;    

void main (void)  
{  


gl_FragColor = vec4(0.0,0.0,0.0,1.0);

  for (int i = 0; i < 3; i++){
   vec3 L = normalize(gl_LightSource[i].position.xyz - v);   
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[i].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);    
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[i].specular 
                * pow(max(dot(R,E),0.0),1.0);
   // write Total Color:  
   gl_FragColor += (gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec) / 3.0;   
   
   }
   gl_FragColor.x = pow(gl_FragColor.x, 2.3);
   gl_FragColor.y = pow(gl_FragColor.y, 2.3);
   gl_FragColor.z = pow(gl_FragColor.z, 2.3);
  
}