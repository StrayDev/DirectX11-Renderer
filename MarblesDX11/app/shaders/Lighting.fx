
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

void ComputeDirectionalLight(
	Material mat, DirectionalLight light, float3 normal, float3 to_eye,
	out float4 ambient,	out float4 diffuse,	out float4 specular)
{
	// init outputs
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// light vector is the inverse of the light rays
	float3 light_vector = -light.Direction;

	// add ambient
	ambient = mat.Ambient * light.Ambient;

	// get the diffuse factor 
	float diffuse_factor = dot(light_vector, normal);

	
	[flatten] // flatten to avoid dynamic branching???
	if (diffuse_factor > 0.0f) // if surface is in line of sight
	{
		// get the specular factor
		float3 v = reflect(-light_vector, normal);
		float specular_factor = pow(max(dot(v, to_eye), 0.0f), mat.Specular.w);

		// set the diffuse and the specular values
		diffuse = diffuse_factor * mat.Diffuse * light.Diffuse;
		specular = specular_factor * mat.Specular * light.Specular;
	}

}
