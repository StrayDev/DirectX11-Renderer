#pragma once
#include <memory>
#include "Renderer/IRendereable.h"
#include "Renderer/Pipeline/IBindable.h"
#include "Renderer/Pipeline/IndexBuffer.h"
#include "Renderer/Pipeline/VertexBuffer.h"
#include "Renderer/Pipeline/ConstantBuffer.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Renderer/Mesh.h"

class Renderer;

enum struct Shapes
{
	cube
};

class Cube;

namespace Primitive
{
	template<typename T>
	static std::unique_ptr<T> MakeUnique(Renderer& renderer)
	{
		return std::make_unique<T>(renderer);
	}
}

class Cube : public Mesh
{
public:
	~Cube() override = default;
	Cube(Renderer& renderer)
	{
		
		std::vector<Vertex> vertices = {
		{{-1.f, -1.f, -1.f }}, {{ 1.f, -1.f, -1.f }}, {{-1.f,  1.f, -1.f }}, {{ 1.f, 1.f, -1.f }},	
		{{ 1.f, -1.f, -1.f }}, {{ 1.f,  1.f, -1.f }}, {{ 1.f, -1.f,  1.f }}, {{ 1.f,  1.f,  1.f }}, 
		{{-1.f,  1.f, -1.f }}, {{-1.f,  1.f,  1.f }}, {{ 1.f,  1.f, -1.f }}, {{ 1.f,  1.f,  1.f }}, 
		{{-1.f, -1.f,  1.f }}, {{ 1.f, -1.f,  1.f }}, {{ 1.f,  1.f,  1.f }}, {{-1.f,  1.f,  1.f }},
		{{-1.f, -1.f, -1.f }}, {{-1.f, -1.f,  1.f }}, {{-1.f,  1.f, -1.f }}, {{-1.f,  1.f,  1.f }},
		{{-1.f, -1.f, -1.f }}, {{ 1.f, -1.f, -1.f }}, {{-1.f, -1.f,  1.f }}, {{ 1.f, -1.f,  1.f }},
		};

		std::vector<unsigned short> indices =
		{
			0,2,1,    2,3,1,
			4,5,6,    5,7,6,
			8,9,10,   10,9,11,
			12,13,14, 12,14,15,
			16,17,18, 18,17,19,
			20,21,22, 21,23,22,
		};

		//Vertex::GenerateFaceNormalsIndependant(vertices, indices);

		CreateVertexShader(renderer);
		CreatePixelShader(renderer);
		CreateInputLayout(renderer);

		CreateIndexBuffer(renderer, indices);
		CreateVertexBuffer(renderer, vertices);

		CreateTransformBuffer(renderer, GetTransform());
	}

};

class Sphere : public Mesh
{
public:
	~Sphere() override = default;
	Sphere(Renderer& renderer)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;
		
		GenerateSphere(vertices, indices);
		//Vertex::GenerateFaceNormals(vertices, indices);


		for (auto& v : vertices)
		{
			//v.normal = DX::XMVectorSubtract({}, v.normal);
		}

		CreateVertexShader(renderer);
		CreatePixelShader(renderer);
		CreateInputLayout(renderer);
		CreateIndexBuffer(renderer, indices);
		CreateVertexBuffer(renderer, vertices);
		CreateTransformBuffer(renderer, GetTransform());
	}
	
	void GenerateSphere(std::vector<Vertex>& v, std::vector<unsigned short>& t)
	{
		auto radius = 1.f;
		auto resolution = 16;

		v.push_back({ { 0.0F, +radius, 0.0F }});

		float phiStep = DirectX::XM_PI / resolution;
		float thetaStep = phiStep * 2.0F;

		for (unsigned short i = 1; i <= resolution - 1; i++)
		{
			float phi = i * phiStep;
			for (unsigned short j = 0; j <= resolution; j++)
			{
				float theta = j * thetaStep;
				Vertex vp;
				vp.pos = 
				{
					radius * std::sinf(phi) * std::cosf(theta),
					radius * std::cosf(phi),
					radius * std::sinf(phi) * std::sinf(theta)
				};
				v.push_back(vp);
			}
		}

		v.push_back({{ 0.0F, -radius, 0.0F }});

		// Top
		for (unsigned int i = 1; i <= resolution; i++)
		{
			t.push_back(0);
			t.push_back(i + 1);
			t.push_back(i);
		}
		//  Middle
		unsigned short baseIndex = 1;
		unsigned short ringVertexCount = resolution + 1;
		for (unsigned short i = 0; i < resolution - 2; i++)
		{
			for (unsigned short j = 0; j < resolution; j++)
			{
				t.push_back(baseIndex + i * ringVertexCount + j);
				t.push_back(baseIndex + i * ringVertexCount + j + 1);
				t.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				t.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				t.push_back(baseIndex + i * ringVertexCount + j + 1);
				t.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}
		// Bottom
		unsigned short southPoleIndex = static_cast<unsigned short>(v.size()) - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (unsigned short i = 0; i < resolution; i++)
		{
			t.push_back(southPoleIndex);
			t.push_back(baseIndex + i);
			t.push_back(baseIndex + i + 1);
		}
	}

};
