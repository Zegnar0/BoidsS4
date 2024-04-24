#pragma once
#include <vector>
#include "parameters.hpp"

// Définition des états possibles
enum BoidState {
    STATE_NORMAL,
    STATE_ALIGNMENT,
    STATE_COHESION,
    STATE_SEPARATION,
    NUM_STATES
};

// Classe représentant une chaîne de Markov pour la simulation de l'état des boids
class BoidsMarkovState {
public:
    // Constructeur
    BoidsMarkovState();

    void randomInitBoids();

    // Méthode pour mettre à jour l'état d'un boid
    BoidState updateState();

    void updateParams(Parameters& parameters, BoidState newState);

private:
    // Type pour représenter une matrice stochastique
    using StochasticMatrix = std::vector<std::vector<double>>;

    // Matrice stochastique des transitions
    StochasticMatrix transitionMatrix_;

    // Initialiser la matrice stochastique
    void initTransitionMatrix();

    BoidState currentState;
};