#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixer;
  
void main() {
    FragColor = mix(texture(texture1, texCoord) * vec4(ourColor, 1.0), 
                    texture(texture2, texCoord) * vec4(ourColor, 1.0),
                    mixer);

}