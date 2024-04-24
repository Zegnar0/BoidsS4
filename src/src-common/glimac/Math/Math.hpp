#pragma once
#include <cstdlib>
#include <vector>

// Fonction pour générer un échantillon aléatoire selon la loi de Bernoulli
int bernoulli(double p);

// Fonction pour générer un échantillon aléatoire selon la loi binomiale
int binomiale(int n, double p);

// Fonction pour générer un échantillon aléatoire selon la loi hypergéométrique
int hypergeometrique(int N, int K, int n);

// Fonction pour générer un échantillon aléatoire selon la loi multinomiale
void multinomiale(int n, const double* probs, int* samples);

// Fonction pour générer un échantillon aléatoire selon la loi uniforme discrète
int uniforme(int a, int b);

// Fonction pour générer un nombre aléatoire suivant une loi uniforme discrète dans une plage [min, max]
int uniforme_discrete(int min, int max);

// Fonction pour générer un échantillon aléatoire selon la loi de l'indépendance
bool independance(double p);

// Fonction pour générer un échantillon aléatoire selon la loi des permutations
std::vector<int> permutations(int n);
