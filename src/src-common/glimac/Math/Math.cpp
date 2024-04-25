#include "Math.hpp"
#include <cstdlib>
#include <vector>

// Fonction pour générer un échantillon aléatoire selon la loi de Bernoulli
int bernoulli(double p)
{
    return (rand() < p * RAND_MAX) ? 1 : 0;
}

// Fonction pour générer un échantillon aléatoire selon la loi binomiale
double binomiale_continue(int n, double p)
{
    double count = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double r = (double)rand() / RAND_MAX; // Génère un nombre réel aléatoire entre 0 et 1
        if (r < p)
        {
            count += r; // Incrémenter par la valeur aléatoire si elle est inférieure à p
        }
        else
        {
            count += (1 - r) * (1 - p); // Ajoute une contribution basée sur la partie non favorable
        }
    }
    return count;
}

// Fonction pour générer un échantillon aléatoire selon la loi hypergéométrique
double hypergeometrique_continue(int N, int K, int n)
{
    double count = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double random_value = (double)rand() / RAND_MAX;
        double p            = (double)K / N;
        if (random_value < p)
        {
            count += 1.0;
            --K;
        }
        else
        {
            // Ajouter une fraction basée sur la probabilité inversée
            count += (1 - p);
        }
        --N; // Réduire la taille totale de la population à chaque échantillon
    }
    return count;
}

double multinomiale(int n, const double* probs)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sum += probs[i];
    }
    double r = (rand() / (double)RAND_MAX) * sum;
    sum      = 0.0;
    for (int i = 0; i < n; ++i)
    {
        sum += probs[i];
        if (r < sum)
        {
            return i;
        }
    }
    return n - 1;
}

// Fonction pour générer un échantillon aléatoire selon la loi uniforme discrète
double uniforme(double a, double b)
{
    double random = ((double)rand()) / RAND_MAX;
    return a + random * (b - a);
}

// Fonction pour générer un nombre aléatoire suivant une loi uniforme discrète dans une plage [min, max]
int uniforme_discrete(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// Fonction pour générer un échantillon aléatoire selon la loi de l'indépendance
bool independance(double p)
{
    return static_cast<double>(rand()) / RAND_MAX < p;
}

// // Fonction pour générer un échantillon aléatoire selon la loi des permutations
// std::vector<int> permutations(int n)
// {
//     std::vector<int> perm(n);
//     for (int i = 0; i < n; ++i)
//     {
//         perm[i] = i;
//     }
//     for (int i = n - 1; i > 0; --i)
//     {
//         int j = rand() % (i + 1);
//         std::swap(perm[i], perm[j]);
//     }
//     return perm;
// }