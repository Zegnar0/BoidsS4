#include <cmath>
#define TINYOBJLOADER_IMPLEMENTATION
#include <cstddef>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "boids.hpp"
#include "boidsManager.hpp"
#include "p6/p6.h"
#include "parameters.hpp"
#include "src-common/glimac/Camera/FreeflyCamera.hpp"
#include "src-common/glimac/Input/InputManager.hpp"
#include "src-common/glimac/Math/Math.hpp"
#include "src-common/glimac/ModelManager/BufferManager.hpp"
#include "src-common/glimac/ModelManager/ModelLoader.hpp"
// #include "src-common/glimac/Texture.hpp"
#include "tiny_obj_loader.h"
int main()
{
    auto ctx = p6::Context{{1280, 720, "Boids3D"}};
    ctx.maximize_window();
    // Init Camera et input
    FreeflyCamera camera;
    InputManager  inputManager(ctx, camera);

    const p6::Shader shader = p6::load_shader(
        "../src/shaders/3D.vs.glsl",
        "../src/shaders/pointLight.fs.glsl"
    );

    // Init Parametre et Boids
    Parameters   parameters = getData();
    BoidsManager boidsManager(parameters);
    boidsManager.randomInitBoids();

    // Chargement de Tout les modèles
    auto          Requin = loadModel("../Models/Requin.obj");
    BufferManager bufferManager1;
    bufferManager1.createBuffers(Requin);

    auto          Zone = loadModel("../Models/zone.obj");
    BufferManager bufferManager2;
    bufferManager2.createBuffers(Zone);

    auto          Starwork = loadModel("../Models/starwork.obj");
    BufferManager bufferManager3;
    bufferManager3.createBuffers(Starwork);

    auto          boidsModel = loadModel("../Models/Boids.obj");
    BufferManager bufferManager4;
    bufferManager4.createBuffers(boidsModel);

    auto          boidsModelLod = loadModel("../Models/BoidsReduced.obj");
    BufferManager bufferManager5;
    bufferManager5.createBuffers(boidsModelLod);

    auto          RequinLod = loadModel("../Models/RequinReduced.obj");
    BufferManager bufferManager6;
    bufferManager6.createBuffers(RequinLod);

    auto          starworkLod = loadModel("../Models/starReduced.obj");
    BufferManager bufferManager7;
    bufferManager7.createBuffers(starworkLod);
    // Essai de texture mais ca marche pas
    //  Texture fishTexture("test");

    // Init des Variables pour les shaders
    GLuint uMVPMatrixLocation       = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation        = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation    = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLuint uKdLocation              = glGetUniformLocation(shader.id(), "uKd");
    GLuint uKsLocation              = glGetUniformLocation(shader.id(), "uKs");
    GLuint uShininessLocation       = glGetUniformLocation(shader.id(), "uShininess");
    GLuint uLightPos_vsLocation     = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLuint uLightIntensityLocation  = glGetUniformLocation(shader.id(), "uLightIntensity");
    GLuint utextureLocation         = glGetUniformLocation(shader.id(), "uTexture");
    GLuint uLightPos_vsLocation1    = glGetUniformLocation(shader.id(), "uLightPos2_vs");
    GLuint uLightIntensityLocation2 = glGetUniformLocation(shader.id(), "uLightIntensity2");

    glEnable(GL_DEPTH_TEST);

    ctx.update = [&]() {
        // Mise a jour caméra GUI et Input
        glm::mat4 viewMatrix = camera.getViewMatrix();
        parameters.drawImGui();
        inputManager.update();

        glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // If pour le LOD
        if (parameters.LOD == 0)
        {
            // Exemple de transformation pour les Models avec bufferManager
            bufferManager1.bindVAO();
            shader.use();

            float     scale        = 0.03f;
            glm::mat4 MVMatrix     = glm::scale(glm::vec3(scale, scale, scale));
            MVMatrix               = glm::inverse(viewMatrix);
            MVMatrix               = glm::translate(MVMatrix, glm::vec3(4.0f, -1.0f, -2.0f));
            MVMatrix               = glm::rotate(MVMatrix, 1.0f, glm::vec3(0.f, 1.f, 0.f));
            glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            glm::vec3 lightPos = glm::vec3(glm::rotate(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), ctx.time(), glm::vec3(0.0f, 1.0f, 0.0f)));
            lightPos *= 5.0f;
            glm::vec3 lightPos_vs = glm::vec3(viewMatrix * glm::vec4(lightPos, 1.0f));

            glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);
            glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);
            glUniform1f(uShininessLocation, 32.0f);
            glUniform1i(utextureLocation, 0);
            // Les Lumières
            glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs));
            glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);

            glUniform3f(uLightPos_vsLocation1, 10.f, -10.0f, 0.0f);
            glUniform3f(uLightIntensityLocation2, 0.0f, 0.0f, 12.0f);

            glDrawArrays(GL_TRIANGLES, 0, Requin.size());

            bufferManager1.unbindVAO();

            bufferManager2.bindVAO();

            glm::mat4 secondModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f));
            secondModelMatrix           = glm::scale(secondModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
            glm::mat4 secondMVPMatrix   = ProjMatrix * viewMatrix * secondModelMatrix;
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(secondMVPMatrix));
            glDrawArrays(GL_TRIANGLES, 0, Zone.size());
            bufferManager2.unbindVAO();

            bufferManager3.bindVAO();
            // La Boucle pour les Décors en grille
            int   numRows    = 5;
            int   numColumns = 5;
            float spacingX   = 10.0f;
            float spacingY   = 10.0f;

            std::vector<glm::mat4> starworkTransforms(numRows * numColumns);

            for (int row = 0; row < numRows; ++row)
            {
                for (int col = 0; col < numColumns; ++col)
                {
                    float xPosition           = col * spacingX;
                    float yPosition           = row * -spacingY;
                    int   index               = row * numColumns + col;
                    starworkTransforms[index] = glm::translate(glm::mat4(1.0f), glm::vec3(20 + yPosition, -25.0f, -20 + xPosition));
                    starworkTransforms[index] = glm::scale(starworkTransforms[index], glm::vec3(5.0f, 5.0f, 5.0f));
                }
            }

            for (const auto& transform : starworkTransforms)
            {
                glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * transform;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
                glDrawArrays(GL_TRIANGLES, 0, Starwork.size());
            }
            bufferManager3.unbindVAO();

            bufferManager4.bindVAO();
            const auto& boids = boidsManager.getBoids();
            for (const Boids& boid : boids)
            {
                glm::vec3 pos        = boid.getPosition();
                glm::mat4 Translate  = glm::translate(glm::mat4(1.0f), pos);
                glm::mat4 Scale      = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
                glm::mat4 MVMatrix4  = Translate * Scale;
                glm::mat4 MVPMatrix4 = ProjMatrix * viewMatrix * MVMatrix4;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix4));
                glDrawArrays(GL_TRIANGLES, 0, boidsModel.size());
            }
            bufferManager4.unbindVAO();
        }
        else
        // Le Lod 1
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

            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(uKdLocation, 0.6f, 0.2f, 0.3f);
            glUniform3f(uKsLocation, 0.8f, 0.8f, 0.8f);
            glUniform1f(uShininessLocation, 32.0f);
            glUniform3fv(uLightPos_vsLocation, 1, glm::value_ptr(lightPos_vs));
            glUniform3f(uLightIntensityLocation, 1.0f, 1.0f, 1.0f);
            glUniform3f(uLightPos_vsLocation1, 10.f, -10.0f, 0.0f);
            glUniform3f(uLightIntensityLocation2, 0.0f, 0.0f, 12.0f);

            glDrawArrays(GL_TRIANGLES, 0, Requin.size());

            bufferManager6.unbindVAO();

            bufferManager2.bindVAO();

            glm::mat4 secondModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f));
            secondModelMatrix           = glm::scale(secondModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
            glm::mat4 secondMVPMatrix   = ProjMatrix * viewMatrix * secondModelMatrix;
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(secondMVPMatrix));
            glDrawArrays(GL_TRIANGLES, 0, Zone.size());
            bufferManager2.unbindVAO();

            bufferManager5.bindVAO();
            const auto& boids = boidsManager.getBoids();
            for (const Boids& boid : boids)
            {
                glm::vec3 pos        = boid.getPosition();
                glm::mat4 Translate  = glm::translate(glm::mat4(1.0f), pos);
                glm::mat4 Scale      = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
                glm::mat4 MVMatrix4  = Translate * Scale;
                glm::mat4 MVPMatrix4 = ProjMatrix * viewMatrix * MVMatrix4;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix4));
                glDrawArrays(GL_TRIANGLES, 0, boidsModel.size());
            }
            bufferManager5.unbindVAO();

            bufferManager7.bindVAO();

            int   numRows    = 5;
            int   numColumns = 5;
            float spacingX   = 10.0f;
            float spacingY   = 10.0f;

            std::vector<glm::mat4> starworkTransforms(numRows * numColumns);

            for (int row = 0; row < numRows; ++row)
            {
                for (int col = 0; col < numColumns; ++col)
                {
                    float xPosition           = col * spacingX;
                    float yPosition           = row * -spacingY;
                    int   index               = row * numColumns + col;
                    starworkTransforms[index] = glm::translate(glm::mat4(1.0f), glm::vec3(20 + yPosition, -25.0f, -20 + xPosition));
                    starworkTransforms[index] = glm::scale(starworkTransforms[index], glm::vec3(5.0f, 5.0f, 5.0f));
                }
            }

            for (const auto& transform : starworkTransforms)
            {
                glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * transform;
                glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
                glDrawArrays(GL_TRIANGLES, 0, starworkLod.size());
            }
            bufferManager7.unbindVAO();
        }
        // Update des Boids a la fin
        boidsManager.update(&ctx, parameters);
    };

    ctx.start();

    return EXIT_SUCCESS;
}
