precision highp float;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;


void main (void)  
{  
   gl_FragColor = colorVarying;    
}