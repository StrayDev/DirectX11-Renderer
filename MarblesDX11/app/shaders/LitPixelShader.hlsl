//cbuffer LightCBuffer
//{
//	float3 lightPos;
//};

static const float3 lightPos = { 0.0f, 1.0f, 0.0f };

static const float3 materialColour = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 1.0f;
static const float attQuad = 1.0f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	//// fragment to light data
	const float3 vToL = lightPos - worldPos;
	const float distToL = length( vToL );
	const float3 dirToL = vToL / distToL;

	// diffuse attenuation
	const float att = attConst + attLin * distToL + attQuad * (distToL * distToL);

	// diffuse intensity
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

	// final colour
	return float4(saturate(diffuse + ambient), 1.0f);
}


//#include "Lighting.fx"

//cbuffer cbPerFrame
//{
//	DirectionalLight gDirLight;
//	float3 gEyePosW;
//};
//
//cbuffer cbPerObject
//{
//	float4x4 gWorld;
//	float4x4 gWorldInvTranspose;
//	float4x4 gWorldViewProj;
//	Material gMaterial;
//};
// 
//struct VertexIn
//{
//	float3 PosL : POSITION;
//	float3 NormalL : NORMAL;
//};

//struct VertexOut
//{
//	float4 PosH : SV_POSITION;
//	float3 PosW : POSITION;
//	float3 NormalW : NORMAL;
//};

//// is this meant to be the vertex shader??
//VertexOut VS(VertexIn vin)
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

//float4 main(VertexOut pin) : SV_Target
//{
//	// interpolation bad so normalize
//	pin.NormalW = normalize(pin.NormalW);
//	float3 toEyeW = normalize(gEyePosW - pin.PosW);
//
//	// start with zeros
//	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	// sum the light from each source
//	float4 A, D, S;
//	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
//	ambient += A;
//	diffuse += D;
//	specular += S;
//
//	// return the results as a colour
//	float4 colour = ambient + diffuse + specular;
//	colour.a = gMaterial.Diffuse.a;
//	return colour;
//}

