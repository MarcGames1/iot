#version 330 core
out vec4 FragColor;

uniform vec3 lightColor = vec3( 1.0f, 1.0f, 1.0f );

uniform float intensity = 1.0;

void main()
{
    vec4 finalColor = vec4(lightColor, intensity);
    FragColor = finalColor;
}