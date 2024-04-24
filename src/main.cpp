
#include <cmath>
#define TINYOBJLOADER_IMPLEMENTATION
#include <cstddef>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "boidsManager.hpp"
#include "p6/p6.h"
#include "parameters.hpp"
#include "src-common/glimac/FreeflyCamera.hpp"
#include "src-common/glimac/common.hpp"
#include "src-common/glimac/cone_vertices.hpp"
#include "src-common/glimac/default_shader.hpp"
#include "src-common/glimac/sphere_vertices.hpp"
#include "tiny_obj_loader.h"


int main()
{
    // Initialisation
    auto ctx = p6::Context{{1280, 720, "Boids3D"}};
    ctx.maximize_window();
    // Init de la camera
    FreeflyCamera camera;
    // Init des Shaders
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/pointLight.fs.glsl"
    );

    // Initialisation des paramètres
    Parameters parameters{};
    parameters.triangleRadius          = 0.1f;
    parameters.alignmentRadius         = 0.138f;
    parameters.separationRadius        = 0.286f;
    parameters.cohesionRadius          = -0.494f;
    parameters.maxVelocity             = 1.0f;
    parameters.min_separation_distance = parameters.triangleRadius * 2.f;

    // Initialisation de BoidsManager
    BoidsManager boidsManager(parameters);
    boidsManager.randomInitBoids();

    // Initialisation de la graine pour la fonction rand()
    srand(time(nullptr));

    // Exemples d'utilisation des fonctions
    // std::cout << "Loi uniforme continue : " << uniforme_continue() << std::endl;
    // std::cout << "Loi normale : " << normale(0, 1) << std::endl;
    // std::cout << "Loi exponentielle : " << exponentielle(1) << std::endl;
    // std::cout << "Loi binomiale : " << binomiale(10, 0.5) << std::endl;
    // std::cout << "Loi uniforme discrete : " << uniforme_discrete(0, 10) << std::endl;
    // std::cout << "Loi de Poisson : " << poisson(3) << std::endl;
    // std::cout << "Loi de Bernoulli : " << bernoulli(0.3) << std::endl;
    // std::cout << "Loi triangulaire : " << triangulaire(0, 10) << std::endl;

    // double p = 0.3;
    // std::cout << "Bernoulli : " << bernoulli(p) << std::endl;

    // int n     = 10;
    // int binom = binomiale(n, p);
    // std::cout << "Binomiale : " << binom << std::endl;

    // int N = 20, K = 10;
    // int hyperg = hypergeometrique(N, K, n);
    // std::cout << "Hypergeometrique : " << hyperg << std::endl;

    // double probs[] = {0.2, 0.3, 0.5};
    // int    samples[3];
    // multinomiale(3, probs, samples);
    // std::cout << "Multinomiale : ";
    // for (int i = 0; i < 3; ++i)
    // {
    //     std::cout << samples[i] << " ";
    // }
    // std::cout << std::endl;

    // int a = 5, b = 10;
    // std::cout << "Uniforme : " << uniforme(a, b) << std::endl;

    // std::cout << "Loi uniforme discrete : " << uniforme_discrete(0, 10) << std::endl;

    // // Exemple d 'utilisation de la fonction d' indépendance double p = 0.6;
    // std::cout << "Indépendance : " << independance(p) << std::endl;

    // // Exemple d'utilisation de la fonction des permutations
    // int              m    = 5;
    // std::vector<int> perm = permutations(m);
    // std::cout << "Permutations : ";
    // for (int i = 0; i < m; ++i)
    // {
    //     std::cout << perm[i] << " ";
    // }
    // std::cout << std::endl;

    // Création du VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::string warn;
    std::string err;
    // Ajout des Models
    std::string              inputfile = "../Models/boids.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "../Models"; // Path to material files
    // TinyObjReader qui s'occupe de lire le model et de le parser
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    // Ajout des points et des faces du model au bon endroit ( les textures ne marchent pas encore avec materials en gris)
    auto& attrib    = reader.GetAttrib();
    auto& shapes    = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Placement des points et des faces au bon endroits
    std::vector<glimac::ShapeVertex> vertices;
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            glimac::ShapeVertex v;
            v.position  = glm::vec3{attrib.vertices[3 * index.vertex_index], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]};
            v.normal    = glm::vec3{attrib.normals[3 * index.normal_index], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]};
            v.texCoords = glm::vec2{attrib.texcoords[2 * index.texcoord_index], attrib.texcoords[2 * index.texcoord_index + 1]}; // Déplacer cette ligne à l'intérieur de la boucle interne
            vertices.push_back(v);
        }
    }

    // const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Création du VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_normal   = 1;
    static constexpr GLuint vertex_attr_UV       = 2;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_UV);

    // On explique à OpenGL comment lire les données avec les attributs des tout ce qu'on a mis dans le VBO et le VAO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(vertex_attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // On récupère tout les détails dans les shaders (lumière, position, normale, texture et matrice) pou les envoyer dans la boucle de rendu
    GLuint uMVPMatrixLocation      = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation       = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation   = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLuint uKdLocation             = glGetUniformLocation(shader.id(), "uKd");
    GLuint uKsLocation             = glGetUniformLocation(shader.id(), "uKs");
    GLuint uShininessLocation      = glGetUniformLocation(shader.id(), "uShininess");
    GLuint uLightPos_vsLocation    = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLuint uLightIntensityLocation = glGetUniformLocation(shader.id(), "uLightIntensity");

    glEnable(GL_DEPTH_TEST);

    // La boucle de rendu
    ctx.update = [&]() {
        // ImGui
        ImGui::Begin("Paramètres");
        ImGui::SliderFloat("Taille du triangle", &parameters.triangleRadius, 0.f, 1.f);
        ImGui::SliderFloat("Alignement", &parameters.alignmentRadius, -2.f, 2.f);
        ImGui::SliderFloat("Séparation", &parameters.separationRadius, -2.f, 2.f);
        ImGui::SliderFloat("Cohésion", &parameters.cohesionRadius, -2.f, 2.f);
        ImGui::End();

        // Fond
        ctx.background(p6::NamedColor::Blue);

        // On démarre la cam
        glm::mat4 viewMatrix = camera.getViewMatrix();
        // On récupère les inputs clavier pour bouger la caméra
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera.moveLeft(-0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera.moveLeft(0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            camera.moveFront(0.1f);
        };
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera.moveFront(-0.1f);
        };
        // On récupère les inputs souris pour bouger la caméra
        ctx.mouse_moved = [&camera](p6::MouseMove button) {
            camera.rotateLeft(-button.position.x);
            camera.rotateUp(button.position.y);
        };
        // On indique que il faut lire les shaders et tout réadapter a cette frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();
        // On crée les matrices pour faire tourner l'objet au milieu de l'écran
        glActiveTexture(GL_TEXTURE0);
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        MVMatrix               = glm::rotate(MVMatrix, ctx.time(), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        // On S'occupe de la lumière pour qu'elle se comporte comme il faut
        glm::vec3 lightPos = glm::vec3(glm::rotate(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ctx.time(), glm::vec3(0.0f, 1.0f, 0.0f)));
        lightPos *= 5.0f;
        glm::vec3 lightPos_vs = glm::vec3(viewMatrix * glm::vec4(lightPos, 1.0f));
        // On crée une matrice avec tout les éléments avant pour que l'on puisse la donner propre aux shaders
        glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;
        // On envoie toute les variables uniformes qu'on a crée ligne 97 pour que les shaders puissent les utiliser et avoir tout qui fonctionne super
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);                         // Example values for diffuse reflectivity
        glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);                         // Example values for specular reflectivity
        glUniform1f(uShininessLocation, 32.0f);                             // Example value for shininess
        glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs)); // Send transformed light position
        glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);             // Example values for light intensity
        // On dessine le poisson
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        // On nettoie tout pour que tout soit propre et que il y a pas des textures qui se mélangent ou restent dans la mémoire
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture unit GL_TEXTURE0
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture unit GL_TEXTURE1
        glBindVertexArray(0);

        // Update et Draw des boids
        boidsManager.update(&ctx, parameters);
    };

    // Ca c'est pour démarre la boucle de rendu et la fermer proprement
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
