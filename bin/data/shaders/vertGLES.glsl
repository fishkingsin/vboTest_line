attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texcoord;
varying vec4 colorVarying;
varying vec2 texCoordVarying;
void main()
{

	colorVarying = color;
	texCoordVarying = texcoord;
	
	gl_FrontColor =  gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
}
