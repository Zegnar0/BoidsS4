#version 330

in vec2 vTexCoords;
in vec3 vPosition_vs; 
in vec3 vNormal_vs; 

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform vec3 uLightPos2_vs;
uniform vec3 uLightIntensity2;


uniform sampler2D uTexture;

vec3 blinnPhong(){
    vec3 N = normalize(vNormal_vs);
    vec3 V = normalize(-vPosition_vs);

    vec3 color = vec3(0.0); 

    
    vec3 lightPositions[2];
    vec3 lightIntensities[2];

    
    lightPositions[0] = uLightPos_vs;
    lightPositions[1] = uLightPos2_vs;
    lightIntensities[0] = uLightIntensity;
    lightIntensities[1] = uLightIntensity2;

    for (int i = 0; i < 2; i++) {
        vec3 L = normalize(lightPositions[i] - vPosition_vs);
        vec3 H = normalize(L + V);
        
        
        float NdotL = max(0.0, dot(N, L));
        color += uKd * NdotL * lightIntensities[i];

       
        float NdotH = max(0.0, dot(N, H));
        color += uKs * pow(NdotH, uShininess) * lightIntensities[i];
    }

    return color;
}

void main() {

    vec4 color = texture(uTexture, vTexCoords);

    fFragColor = vec4(blinnPhong(), 1.0);
}
