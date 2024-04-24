#include "BoidsMarkovState.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>

// Constructeur
BoidsMarkovState::BoidsMarkovState()
{
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Initialiser la matrice stochastique des transitions
    initTransitionMatrix();
}

void BoidsMarkovState::randomInitBoids()
{
    currentState = static_cast<BoidState>(rand() % NUM_STATES);
}

BoidState BoidsMarkovState::updateState()
{
    double randomNum             = static_cast<double>(rand()) / RAND_MAX;
    double cumulativeProbability = 0.0;
    for (int nextState = 0; nextState < NUM_STATES; ++nextState)
    {
        cumulativeProbability += transitionMatrix_[currentState][nextState];
        if (randomNum < cumulativeProbability)
        {
            currentState = static_cast<BoidState>(nextState);

            break;
        }
    }
    return currentState;
}

void BoidsMarkovState::updateParams(Parameters& parameters, BoidState newState)
{
    switch (newState)
    {
    case STATE_NORMAL:
        parameters.alignmentRadius  = 0.138f;
        parameters.cohesionRadius   = -0.494f;
        parameters.separationRadius = 0.286f;
        std::cout << "Normal" << std::endl;
        break;
    case STATE_ALIGNMENT:
        parameters.alignmentRadius  = 0.5;
        parameters.cohesionRadius   = -0.494f;
        parameters.separationRadius = 0.286f;
        std::cout << "Alignement" << std::endl;
        break;
    case STATE_COHESION:
        parameters.alignmentRadius  = 0.138f;
        parameters.cohesionRadius   = 0.8f;
        parameters.separationRadius = 0.286f;
        std::cout << "cohesion" << std::endl;
        break;
    case STATE_SEPARATION:
        parameters.alignmentRadius  = 0.138f;
        parameters.cohesionRadius   = -0.494f;
        parameters.separationRadius = 0.8f;
        std::cout << "separation" << std::endl;
        break;
    case NUM_STATES: break;
    }
}

// Méthode pour initialiser la matrice stochastique des transitions
void BoidsMarkovState::initTransitionMatrix()
{
    transitionMatrix_ = {
        {0.2, 0.3, 0.4, 0.1}, // STATE_NORMAL
        {0.5, 0.1, 0.1, 0.3}, // STATE_ALIGNMENT
        {0.1, 0.5, 0.1, 0.3}, // STATE_COHESION
        {0.2, 0.1, 0.4, 0.3}  // STATE_SEPARATION
    };
}