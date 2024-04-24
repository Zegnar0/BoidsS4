#include "Math.hpp"
#include <cstdlib>
#include <vector>

// Fonction pour générer un échantillon aléatoire selon la loi de Bernoulli
int bernoulli(double p)
{
    return (rand() < p * RAND_MAX) ? 1 : 0;
}

// Fonction pour générer un échantillon aléatoire selon la loi binomiale
int binomiale(int n, double p)
{
    int count = 0;
    for (int i = 0; i < n; ++i)
    {
        if (bernoulli(p))
        {
            ++count;
        }
    }
    return count;
}

// Fonction pour générer un échantillon aléatoire selon la loi hypergéométrique
int hypergeometrique(int N, int K, int n)
{
    int count = 0;
    for (int i = 0; i < n; ++i)
    {
        if (rand() % N < K)
        {
            ++count;
            --K;
        }
        --N;
    }
    return count;
}

// Fonction pour générer un échantillon aléatoire selon la loi multinomiale
void multinomiale(int n, const double* probs, int* samples)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sum += probs[i];
    }
    double r = (rand() / (double)RAND_MAX) * sum;
    sum      = 0.0;
    for (int i = 0; i < n - 1; ++i)
    {
        sum += probs[i];
        if (r < sum)
        {
            samples[i] = 1;
            for (int j = i + 1; j < n; ++j)
            {
                samples[j] = 0;
            }
            return;
        }
    }
    samples[n - 1] = 1;
    for (int i = 0; i < n - 1; ++i)
    {
        samples[i] = 0;
    }
}

// Fonction pour générer un échantillon aléatoire selon la loi uniforme discrète
int uniforme(int a, int b)
{
    return a + rand() % (b - a + 1);
}

// Fonction pour générer un nombre aléatoire suivant une loi uniforme discrète dans une plage [min, max]
int uniforme_discrete(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// Fonction pour générer un échantillon aléatoire selon la loi de l'indépendance
bool independance(double p)
{
    return (rand() < p * RAND_MAX);
}

// Fonction pour générer un échantillon aléatoire selon la loi des permutations
std::vector<int> permutations(int n)
{
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i)
    {
        perm[i] = i;
    }
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        std::swap(perm[i], perm[j]);
    }
    return perm;
}