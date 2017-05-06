//#version 100

varying vec2 vertex_uv;
//out vec4 out_color;

uniform sampler2D image;

void main() {
	gl_FragColor = texture2D(image, vertex_uv);
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}