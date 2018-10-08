#version 330 core
in vec4 ParticleColor;
in float r;
in vec2 val;


out vec4 fragColor;




void main()
{
    float R = r;
    float dist = sqrt(dot(val,val));
    if (dist >= R )
        discard;

    fragColor = ParticleColor;
    fragColor = vec4((ParticleColor.x + dist)/2.0,(ParticleColor.y + dist)/2.0,(ParticleColor.z + dist)/2.0,1.0);

}
