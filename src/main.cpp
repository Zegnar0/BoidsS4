#include <cmath>
#define TINYOBJLOADER_IMPLEMENTATION
#include <cstddef>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "boids.hpp"
#include "boidsManager.hpp"
#include "p6/p6.h"
#include "parameters.hpp"
#include "src-common/glimac/BufferManager.hpp"
#include "src-common/glimac/FreeflyCamera.hpp"
#include "src-common/glimac/ModelLoader.hpp"
#include "src-common/glimac/common.hpp"
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
        "../src/shaders/3D.vs.glsl",
        "../src/shaders/pointLight.fs.glsl"
    );

    // Initialisation des paramètres
    Parameters parameters{};
    parameters.triangleRadius          = 0.1f;
    parameters.alignmentRadius         = 0.138f;
    parameters.separationRadius        = 0.286f;
    parameters.cohesionRadius          = -0.494f;
    parameters.maxVelocity             = 1.0f;
    parameters.min_separation_distance = parameters.triangleRadius * 2.f;
    parameters.LOD                     = 0;

    // Initialisation de BoidsManager
    BoidsManager boidsManager(parameters);
    boidsManager.randomInitBoids();

    // std::vector<std::string> modelPaths = {
    //     "../Models/Requin.obj",
    //     "../Models/zone.obj",
    //     "../Models/starwork.obj",
    //     "../Models/Boids.obj",
    //     "../Models/BoidsReduced.obj",
    //     "../Models/RequinReduced.obj",
    //     "../Models/starReduced.obj"
    // };

    auto          vertices1 = loadModel("../Models/Requin.obj");
    BufferManager bufferManager1;
    bufferManager1.createBuffers(vertices1);

    auto          vertices2 = loadModel("../Models/zone.obj");
    BufferManager bufferManager2;
    bufferManager2.createBuffers(vertices2);

    auto          vertices3 = loadModel("../Models/starwork.obj");
    BufferManager bufferManager3;
    bufferManager3.createBuffers(vertices3);

    auto          vertices4 = loadModel("../Models/Boids.obj");
    BufferManager bufferManager4;
    bufferManager4.createBuffers(vertices4);

    auto          vertices5 = loadModel("../Models/BoidsReduced.obj");
    BufferManager bufferManager5;
    bufferManager5.createBuffers(vertices5);

    auto          vertices6 = loadModel("../Models/RequinReduced.obj");
    BufferManager bufferManager6;
    bufferManager6.createBuffers(vertices6);

    auto          vertices7 = loadModel("../Models/starReduced.obj");
    BufferManager bufferManager7;
    bufferManager7.createBuffers(vertices7);

    // Texture loading (kept unchanged)
    GLuint triforceTexture;
    try
    {
        const std::string triforceImagePath = "../Textures/fish.png";
        img::Image        triforceImage     = p6::load_image_buffer(triforceImagePath);
        glGenTextures(1, &triforceTexture);
        glBindTexture(GL_TEXTURE_2D, triforceTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, triforceImage.width(), triforceImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, triforceImage.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading texture: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // On récupère tout les détails dans les shaders (lumiere, position, normale, texture et matrice) pou les envoyer dans la boucle de rendu
    GLuint uMVPMatrixLocation      = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation       = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation   = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLuint uKdLocation             = glGetUniformLocation(shader.id(), "uKd");
    GLuint uKsLocation             = glGetUniformLocation(shader.id(), "uKs");
    GLuint uShininessLocation      = glGetUniformLocation(shader.id(), "uShininess");
    GLuint uLightPos_vsLocation    = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLuint uLightIntensityLocation = glGetUniformLocation(shader.id(), "uLightIntensity");
    GLuint utextureLocation        = glGetUniformLocation(shader.id(), "uTexture");

    glEnable(GL_DEPTH_TEST);

    // La boucle de renduu
    ctx.update = [&]() {
        // ImGui
        ImGui::Begin("Paramètres");
        ImGui::SliderFloat("Taille du triangle", &parameters.triangleRadius, 0.f, 1.f);
        ImGui::SliderFloat("Alignement", &parameters.alignmentRadius, -2.f, 2.f);
        ImGui::SliderFloat("Séparation", &parameters.separationRadius, -2.f, 2.f);
        ImGui::SliderFloat("Cohésion", &parameters.cohesionRadius, -2.f, 2.f);
        ImGui::SliderInt("LOD", &parameters.LOD, 0, 1);
        ImGui::End();
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
        glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (parameters.LOD == 0)
        {
            bufferManager1.bindVAO();
            shader.use();
            glBindTexture(GL_TEXTURE_2D, triforceTexture);

            float     scale        = 0.03f;
            glm::mat4 MVMatrix     = glm::scale(glm::vec3(scale, scale, scale));
            MVMatrix               = glm::inverse(viewMatrix);
            MVMatrix               = glm::translate(MVMatrix, glm::vec3(4.0f, -1.0f, -2.0f));
            MVMatrix               = glm::rotate(MVMatrix, 1.0f, glm::vec3(0.f, 1.f, 0.f));
            glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glm::vec3 lightPos     = glm::vec3(glm::rotate(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ctx.time(), glm::vec3(0.0f, 1.0f, 0.0f)));
            lightPos *= 5.0f;
            glm::vec3 lightPos_vs = glm::vec3(viewMatrix * glm::vec4(lightPos, 1.0f));
            glm::mat4 MVPMatrix   = ProjMatrix * viewMatrix * MVMatrix;
            // On envoie toute les viarables uniformes qu'on a crée ligne 97 pour que les shaders puissent les utiliser et avoir tout qui fonctionne super
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);                         // Example values for diffuse reflectivity
            glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);                         // Example values for specular reflectivity
            glUniform1f(uShininessLocation, 32.0f);                             // Example value for shininess
            glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs)); // Send transformed light position
            glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);
            glUniform1i(utextureLocation, 0); // Example values for light intensity
                                              // On dessine le poisson
            glDrawArrays(GL_TRIANGLES, 0, vertices1.size());
            bufferManager1.unbindVAO();

            bufferManager2.bindVAO();
            // Position the second model and scale it by 10x
            glm::mat4 secondModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f)); // Translate the model
            secondModelMatrix           = glm::scale(secondModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));    // Scale the model by a factor of 10
            glm::mat4 secondMVPMatrix   = ProjMatrix * viewMatrix * secondModelMatrix;                   // Calculate the Model-View-Projection Matrix
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(secondMVPMatrix));        // Set the MVP matrix uniform
            glDrawArrays(GL_TRIANGLES, 0, vertices2.size());                                             // Draw the model
            bufferManager2.unbindVAO();

            bufferManager3.bindVAO();
            glm::mat4 thirdModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -15.0f, -5.0f)); // Translate the model
            thirdModelMatrix           = glm::scale(thirdModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));        // Scale the model by a factor of 10
            glm::mat4 thirdMVPMatrix   = ProjMatrix * viewMatrix * thirdModelMatrix;                       // Calculate the Model-View-Projection Matrix
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(thirdMVPMatrix));           // Set the MVP matrix uniform
            glDrawArrays(GL_TRIANGLES, 0, vertices3.size());                                               // Draw the model
            bufferManager3.unbindVAO();

            bufferManager4.bindVAO();
            const auto& boids = boidsManager.getBoids();
            for (const Boids& boid : boids)
            {
                glm::vec3 pos        = boid.getPosition();
                glm::mat4 Translate  = glm::translate(glm::mat4(1.0f), pos);         // Adjust position as needed
                glm::mat4 Scale      = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)); // Adjust scale as needed
                glm::mat4 MVMatrix4  = Translate * Scale;
                glm::mat4 MVPMatrix4 = ProjMatrix * viewMatrix * MVMatrix4;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix4));
                glDrawArrays(GL_TRIANGLES, 0, vertices4.size());
            }
            bufferManager4.unbindVAO();
        }
        else
        {
            bufferManager6.bindVAO();
            shader.use();

            float     scale        = 0.03f;
            glm::mat4 MVMatrix     = glm::scale(glm::vec3(scale, scale, scale));
            MVMatrix               = glm::inverse(viewMatrix);
            MVMatrix               = glm::translate(MVMatrix, glm::vec3(4.0f, -1.0f, -2.0f));
            MVMatrix               = glm::rotate(MVMatrix, 1.0f, glm::vec3(0.f, 1.f, 0.f));
            glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glm::vec3 lightPos     = glm::vec3(glm::rotate(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ctx.time(), glm::vec3(0.0f, 1.0f, 0.0f)));
            lightPos *= 5.0f;
            glm::vec3 lightPos_vs = glm::vec3(viewMatrix * glm::vec4(lightPos, 1.0f));
            glm::mat4 MVPMatrix   = ProjMatrix * viewMatrix * MVMatrix;
            // On envoie toute les viarables uniformes qu'on a crée ligne 97 pour que les shaders puissent les utiliser et avoir tout qui fonctionne super
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);                         // Example values for diffuse reflectivity
            glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);                         // Example values for specular reflectivity
            glUniform1f(uShininessLocation, 32.0f);                             // Example value for shininess
            glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs)); // Send transformed light position
            glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);
            glUniform1i(utextureLocation, 0); // Example values for light intensity
                                              // On dessine le poisson
            glDrawArrays(GL_TRIANGLES, 0, vertices1.size());
            bufferManager6.unbindVAO();

            bufferManager2.bindVAO();
            // Position the second model and scale it by 10x
            glm::mat4 secondModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f)); // Translate the model
            secondModelMatrix           = glm::scale(secondModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));    // Scale the model by a factor of 10
            glm::mat4 secondMVPMatrix   = ProjMatrix * viewMatrix * secondModelMatrix;                   // Calculate the Model-View-Projection Matrix
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(secondMVPMatrix));        // Set the MVP matrix uniform
            glDrawArrays(GL_TRIANGLES, 0, vertices2.size());                                             // Draw the model
            bufferManager2.unbindVAO();

            bufferManager5.bindVAO();
            const auto& boids = boidsManager.getBoids();
            for (const Boids& boid : boids)
            {
                glm::vec3 pos        = boid.getPosition();
                glm::mat4 Translate  = glm::translate(glm::mat4(1.0f), pos);         // Adjust position as needed
                glm::mat4 Scale      = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)); // Adjust scale as needed
                glm::mat4 MVMatrix4  = Translate * Scale;
                glm::mat4 MVPMatrix4 = ProjMatrix * viewMatrix * MVMatrix4;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix4));
                glDrawArrays(GL_TRIANGLES, 0, vertices4.size());
            }
            bufferManager5.unbindVAO();

            bufferManager7.bindVAO();
            glm::mat4 thirdModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -15.0f, -5.0f)); // Translate the model
            thirdModelMatrix           = glm::scale(thirdModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));        // Scale the model by a factor of 10
            glm::mat4 thirdMVPMatrix   = ProjMatrix * viewMatrix * thirdModelMatrix;                       // Calculate the Model-View-Projection Matrix
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(thirdMVPMatrix));           // Set the MVP matrix uniform
            glDrawArrays(GL_TRIANGLES, 0, vertices7.size());                                               // Draw the model
            bufferManager7.unbindVAO();
        }

        boidsManager.update(&ctx, parameters);
    };

    // Ca c'est pour démarre la boucle de rendu et la fermer proprement
    ctx.start();

    return EXIT_SUCCESS;
}
