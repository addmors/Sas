//Texture Shader

#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

	
void main(){
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform vec3 u_Color;

uniform sampler2D u_Texture;



void main(){
	color = texture(u_Texture,v_TexCoord);
}
