// 
cbuffer PerFrame : register(b0)
{
    matrix ViewProjectionMatrix;
}

struct AppData
{
    // per-vertex data
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    
    // per-instance
    matrix WorldMatrix : WORLDMATRIX;
    matrix InverseTranspose : INVERSETRANSPOSEWORLDMATRIX;
};

// output from the VS
struct VSO
{
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float2 TexCoord : TEXCOORD0;
    
    // SV_POS is consumed by the rasterizor 
    float4 Position : SV_Position;
};

VSO main( AppData In ) 
{
    VSO Out;
    
    matrix MVP = mul( ViewProjectionMatrix, In.WorldMatrix );
    
    Out.Position   = mul( MVP, float4( In.Position, 1.0f ) );
    Out.PositionWS = mul( In.WorldMatrix, float4( In.Position, 1.0f ) );
    Out.NormalWS   = mul( (float3x3)In.InverseTranspose, In.Normal );
    Out.TexCoord   = In.TexCoord;
	
    return Out;
}