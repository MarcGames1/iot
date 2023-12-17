#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 objectColor;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform float Ka=0.1;
uniform float Kd=0.1;
uniform float Ks=0.7;
uniform float KConstant=1.0;
uniform float KLiniar=0.5;
uniform float KPatratic=0.5;
uniform float n=0.5;

void main()
{
    vec3 ambient = lightColor*Ka;

    vec3 LightDir=normalize(lightPos-FragPos);

    vec3 norm=normalize(Normal);

    vec3 diffuse=lightColor*Kd*max(dot(Normal,LightDir),0.0);

    
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-LightDir,norm);

    float spec = pow(max(dot(viewDir,reflectDir),0.0),n);

    vec3 specular=Ks*spec*lightColor;

    float distance=length(lightPos-FragPos);

    float KAtenuare=1.0/(KConstant+KLiniar*distance+KPatratic*(distance*distance));

    FragColor =vec4(ambient+ KAtenuare*(diffuse+specular),1.0)* vec4(objectColor, 1.0);
}