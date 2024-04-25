#pragma once
#include <cstdlib>
#include <vector>

// Fonction pour générer un échantillon aléatoire selon la loi de Bernoulli
int bernoulli(double p);

// Fonction pour générer un échantillon aléatoire selon la loi binomiale
double binomiale_continue(int n, double p);

// Fonction pour générer un échantillon aléatoire selon la loi hypergéométrique
double hypergeometrique_continue(int N, int K, int n);

// Fonction pour générer un échantillon aléatoire selon la loi multinomiale
double multinomiale(int n, const double* probs);

// Fonction pour générer un échantillon aléatoire selon la loi uniforme discrète
double uniforme(double a, double b);

// Fonction pour générer un nombre aléatoire suivant une loi uniforme discrète dans une plage [min, max]
int uniforme_discrete(int min, int max);

// Fonction pour générer un échantillon aléatoire selon la loi de l'indépendance
double normale(double mu, double sigma);

// Fonction pour générer un échantillon aléatoire selon la loi des permutations
std::vector<int> permutations(int n);
