#pragma once
#include <vector>
#include <DirectXMath.h>

namespace DX = DirectX;
using vector3 = DirectX::XMFLOAT3;

struct Vertex
{
	vector3 pos;
	vector3 normal;

	static void GenerateFaceNormals(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices)
	{
		//////////////////////////////
		for (auto& v : vertices)
		{
			v.normal = {1.f,1.f,1.f};
		}

		///////////////////////////////

		//// number of triangles
		//auto num_tri = indices.size() / 3;

		//// for each triangle
		//for (auto i = 0; i < num_tri; ++i)
		//{
		//	// get the indices
		//	auto i0 = indices[i * 3 + 0];
		//	auto i1 = indices[i * 3 + 1];
		//	auto i2 = indices[i * 3 + 2];

		//	// get the vertices
		//	auto v0 = vertices[i0];
		//	auto v1 = vertices[i1];
		//	auto v2 = vertices[i2];

		//	// compute the face normal
		//	auto e0 = DX::XMVectorSubtract(v1.pos, v0.pos);
		//	auto e1 = DX::XMVectorSubtract(v2.pos, v0.pos);
		//	auto face_normal = DX::XMVector3Cross(e0, e1);

		//	// add the normal to the vertices of the face
		//	vertices[i0].normal = DX::XMVectorAdd(vertices[i0].normal, face_normal);
		//	vertices[i1].normal = DX::XMVectorAdd(vertices[i1].normal, face_normal);
		//	vertices[i2].normal = DX::XMVectorAdd(vertices[i2].normal, face_normal);
		//}

		//// normalise shared normals
		//for (auto& v : vertices)
		//{
		//	v.normal = DX::XMVector3Normalize(v.normal);
		//}

	};


};
