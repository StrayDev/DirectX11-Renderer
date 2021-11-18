// per object data
cbuffer PerObject : register(b0)
{
    matrix WorldMatrix;
    matrix InverseTransposeWorldMatrix;
    matrix WorldViewProjectionMatrix;
}

// vertex attributes
struct AppData
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};

// output from the VS
struct VSO
{
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS   : TEXCOORD2;
    float2 TexCoord   : TEXCOORD0;
    
    // SV_POS is consumed by the rasterizor 
    float4 Position   : SV_Position; 
};

VSO main( AppData In )
{
    VSO Out;
    
    Out.Position   = mul(WorldViewProjectionMatrix, float4( In.Position, 1.0f ));
    Out.PositionWS = mul(WorldMatrix, float4(In.Position, 1.0f));
    Out.NormalWS   = mul((float3x3)InverseTransposeWorldMatrix, In.Normal);
    Out.TexCoord   = In.TexCoord;

	return Out;
}