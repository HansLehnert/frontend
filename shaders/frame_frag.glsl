//#version 100

varying vec2 vertex_uv;
//out vec4 out_color;

uniform vec4 color;

void main() {
	gl_FragColor = color;
}
