// ModelLoader.cpp
#include "ModelLoader.hpp"
#include <iostream>
#include "tiny_obj_loader.h"

std::vector<glimac::ShapeVertex> loadModel(const std::string& filePath)
{
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = "../Models/"; // Set the path for material files

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filePath, readerConfig))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader Error: " << reader.Error();
            exit(1); // or handle more gracefully
        }
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader Warning: " << reader.Warning();
    }

    std::vector<glimac::ShapeVertex> vertices;
    const auto&                      attrib = reader.GetAttrib();
    const auto&                      shapes = reader.GetShapes();

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            glimac::ShapeVertex vertex;
            vertex.position = glm::vec3{
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (!attrib.normals.empty() && index.normal_index >= 0)
            {
                vertex.normal = glm::vec3{
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }
            else
            {
                vertex.normal = glm::vec3(0, 0, 0); // Default normal if none are provided
            }

            vertices.push_back(vertex);
        }
    }

    return vertices;
}
