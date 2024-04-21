#include "boidsManager.hpp"
#include <glm/glm.hpp>
#include "boids.hpp"
#include "tiny_obj_loader.h"

// Initialisation
BoidsManager::BoidsManager(const Parameters& params)
    : parameters(params) {}

// Destruction
BoidsManager::~BoidsManager() = default;

// Initialisation des boids
void BoidsManager::randomInitBoids()
{
    for (auto& boid : boids)
    {
        boid.randomInitBoids();
    }
}

// Update
void BoidsManager::update(p6::Context* ctx, const Parameters& params)
{
    // Charger le modèle des boids s'il n'a pas encore été chargé
    // if (boidsModel.empty())
    // {
    //     loadBoidsModel();
    // }
    for (auto& boid : boids)
    {
        boid.drawBoids(params, ctx);

        // Création des vecteurs des boids voisins
        std::vector<Boids> nearby_boids_alignment;
        std::vector<Boids> nearby_boids_separation;
        std::vector<Boids> nearby_boids_cohesion;

        for (const auto& other_boid : boids)
        {
            boid.updateBoids(params, other_boid, nearby_boids_alignment, nearby_boids_separation, nearby_boids_cohesion);
        }
    }
}

// void BoidsManager::loadBoidsModel()
// {
//     std::string              inputfile = "../Models/boid_model.obj"; // Mettez le chemin correct vers votre modèle de boid
//     tinyobj::ObjReader       reader;
//     tinyobj::ObjReaderConfig reader_config;
//     reader_config.mtl_search_path = "../Models"; // Mettez le chemin correct vers les fichiers MTL si nécessaire

//     if (!reader.ParseFromFile(inputfile, reader_config))
//     {
//         if (!reader.Error().empty())
//         {
//             std::cerr << "TinyObjReader: " << reader.Error();
//         }
//         exit(1);
//     }

//     auto& attrib = reader.GetAttrib();
//     auto& shapes = reader.GetShapes();

//     for (const auto& shape : shapes)
//     {
//         std::vector<glm::vec3>    vertices;
//         std::vector<unsigned int> indices;

//         for (const auto& index : shape.mesh.indices)
//         {
//             glm::vec3 vertex;
//             vertex.x = attrib.vertices[3 * index.vertex_index + 0];
//             vertex.y = attrib.vertices[3 * index.vertex_index + 1];
//             vertex.z = attrib.vertices[3 * index.vertex_index + 2];
//             vertices.push_back(vertex);
//             indices.push_back(indices.size());
//         }

//         boidsModel.push_back(BoidsModel(vertices, indices));
//     }
// }