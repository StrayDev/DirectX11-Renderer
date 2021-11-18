cbuffer CBuffer
{
	matrix world_matrix;
	matrix inverse_transpose;
};

struct VSOut
{
	float3 worldPos : Position;
	float3 normal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), world_matrix);
    vso.normal = mul(n, (float3x3) world_matrix);
    vso.pos = mul(float4(pos, 1.0f), inverse_transpose);
	return vso;
}

// - - - - - - 
//cbuffer CBuffer
//{
//	matrix transform;
//};
//
//float4 main( float3 pos : Position ) : SV_Position
//{
//	return mul( float4(pos, 1.0f), transform );
//}
//  - - - - - - - 
//cbuffer cbPerObject
//{
//	float4x4 gWorld;
//	float4x4 gWorldInvTranspose;
//	float4x4 gWorldViewProj;
//	//Material gMaterial;
//};
//
//struct VertexIn
//{
//	float3 PosL : POSITION;
//	float3 NormalL : NORMAL;
//};
//
//struct VertexOut
//{
//	float4 PosH : SV_POSITION;
//	float3 PosW : POSITION;
//	float3 NormalW : NORMAL;
//};
//
//// is this meant to be the vertex shader??
//VertexOut main(VertexIn vin)
//{
//	VertexOut vout;
//
//	// transform to world space
//	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
//	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
//
//	// transform to same clip space
//	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
//
//	return vout;
//}