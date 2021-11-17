#define MAX_LIGHTS 8

#define DL 0    // Directional Light
#define PL 1    // Point Light 
#define SL 2    // Spot Light

Texture2D Texture : register( t0 );
sampler Sampler   : register( s0 );

struct Material
{
    float4 Emissive;
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    float SpecularPower;
    bool  UseTexture;
    
    // for refrence
    float2 PADDING;
};

cbuffer MaterialProperties : register( b0 )
{
    Material material;
}

struct Light
{
    float4 Position;
    float4 Direction;
    float4 Colour;
    
    float SpotAngle;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    
    int LightType;
    bool Enabled;
    
    // for refrence
    int2 PADDING;
};

cbuffer LightProperties
{
    float4 EyePosition;
    float4 GlobalAmbient;
    Light Lights[MAX_LIGHTS];
};

float4 GetDiffuse( Light light, float3 L, float3 N )
{
    float dotNL = max( 0, dot( N, L ) );
    return light.Colour * dotNL;
}

float4 GetSpecular( Light light, float3 V, float3 L, float3 N )
{
    float3 R = normalize( reflect( -L, N ) );
    float dotRV = max( 0, dot( R, V ) );
    return light.Colour * pow( dotRV, material.SpecularPower );
}

float4 GetAttenuation( Light light, float d )
{
    return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

LightingResult PointLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;
    
    float3 L = (light.Position - P).xyz;
    float distance = length(L);
    L = L / distance;
    
    float attenuation = GetAttenuation(light, distance);
    
    result.Diffuse = GetDiffuse(light, L, N) * attenuation;
    result.Specular = GetSpecular(light, V, L, N) * attenuation;
    
    return result;
}

LightingResult DirectionalLight(Light light, float3 V, float4 P, float3 N)
{
    LightingResult result;
    
    float3 L = -light.Direction.xyz;
    
    result.Diffuse = GetDiffuse(light, L, N);
    result.Specular = GetSpecular(light, V, L, N);
    
    return result;
}

float SpotLightCone(Light light, float3 L)
{
    float minCos = cos( light.SpotAngle );
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot( light.Direction.xyz, -L );
    
    return smoothstep(minCos, maxCos, cosAngle);
}


LightingResult SpotLight(Light light, float3 V, float4 P, float3 N)
{
    LightingResult result;
    
    float3 L = (light.Position = P).xyz;
    float distance = length(L);
    L = L / distance;
    
    float attenuation = GetAttenuation( light, distance );
    float intensity = SpotLightCone( light, L );
    
    result.Diffuse = GetDiffuse(light, L, N) * attenuation * intensity;
    result.Specular = GetSpecular(light, V, L, N) * attenuation * intensity;
    
    return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
    LightingResult finalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    
    float3 V = normalize(EyePosition - P).xyz;
    
    [unroll]
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (!Lights[i].Enabled) continue;
        
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
        switch (Lights[i].LightType)
        {
            case DL:
            {
                result = DirectionalLight(Lights[i], V, P, N);
                break;
            }  
            case PL:
            {
                result = PointLight(Lights[i], V, P, N);
                break;
            }
            case SL:
            {
                result = SpotLight(Lights[i], V, P, N);
                break;
            }
        }
        
        finalResult.Diffuse += result.Diffuse;
        finalResult.Specular += result.Specular;
    }
    
    finalResult.Diffuse = saturate(finalResult.Diffuse);
    finalResult.Specular = saturate(finalResult.Specular);
    
    return finalResult;
}

struct PSInput
{
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS   : TEXCOORD2;
    float2 TexCoord   : TEXCOORD0;
};

float4 main( PSInput In ) : SV_TARGET
{
    LightingResult lit = ComputeLighting(In.PositionWS, normalize(In.NormalWS));
    
    float4 emissive = material.Emissive;
    float4 ambient  = material.Ambient * GlobalAmbient;
    float4 diffuse  = material.Diffuse * lit.Diffuse;
    float4 specular = material.Specular * lit.Specular;
    
    float4 texColour = { 1, 1, 1, 1 };
    if ( material.UseTexture )
    {
        texColour = Texture.Sample(Sampler, In.TexCoord);
    }
    
    float4 finalColour = (emissive + ambient + diffuse + specular) * texColour;
    
    return finalColour;
}