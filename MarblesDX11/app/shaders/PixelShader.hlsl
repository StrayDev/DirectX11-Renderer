cbuffer CBuffer
{
	float4 face_colors[6];
};

float4 main(uint tri_id : SV_PrimitiveID) : SV_Target
{
	return face_colors[tri_id / 2];
}