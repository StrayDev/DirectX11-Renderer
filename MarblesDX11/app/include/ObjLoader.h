#pragma once
#include <string>
#include "Renderer\Vertex.h"
#include "Renderer\Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "lib/TinyObj.h"

using namespace tinyobj;

struct MeshData;

class ObjLoader
{
public:
	ObjLoader() = default;
	~ObjLoader() = default;

	bool LoadObj(std::string path)
	{
        // clear our data
        vertices.clear();
        indices.clear();

        // create needed objects
        auto reader = ObjReader { };
        auto reader_config = ObjReaderConfig { };
        
        // set the path to the material folder
        reader_config.mtl_search_path = "./";

        // try to read the file
        if (!reader.ParseFromFile(path, reader_config)) 
        {
            // if not print error and return false
            if (!reader.Error().empty()) 
            {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            return false;
        }

        // print warnings
        if (!reader.Warning().empty()) 
        {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        // get vectors of data
        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // count
        auto count = 0;

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++)
        {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
            {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    index_t idx = shapes[s].mesh.indices[index_offset + v];
                    real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                    // store the verts and the index
                    vertices.push_back( Vertex{ {vx, vy, vz} });
                    indices.push_back(count++);

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0)
                    {
                        real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

                        // store the normals
                        vertices.back().normal = { nx, ny, nz };
                        //std::cout << nx << " " << ny << " " << nz << "\n";
                    }


                    //////////////////Not using this yet
                    //// Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    //if (idx.texcoord_index >= 0)
                    //{
                    //    real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    //    real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    //}

                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }
        return true;
	}

	MeshData GetMeshData()
	{
        return MeshData{ vertices, indices };
	}


private:
    std::vector<Vertex> vertices { };
    std::vector<unsigned short> indices { };

};
