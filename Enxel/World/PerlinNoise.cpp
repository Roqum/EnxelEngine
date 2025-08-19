#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(int seed) : seed(seed), pseudoRandomNumber(seed)
{
    std::iota(perm.begin(), perm.begin() + 256, 0);
    std::shuffle(perm.begin(), perm.begin() + 256, std::mt19937(seed));
    std::copy(perm.begin(), perm.begin() + 256, perm.begin() + 256);
}

float PerlinNoise::Noise(float x, float y) const
{
    int x0 = (int)x;
    int x1 = (int)x + 1;
    int y0 = (int)y;
    int y1 = (int)y + 1;

    glm::vec2 g00 = RandomGradient(x0, y0);
    glm::vec2 g10 = RandomGradient(x1, y0);
    glm::vec2 g01 = RandomGradient(x0, y1);
    glm::vec2 g11 = RandomGradient(x1, y1);

    glm::vec2 distance00(x - x0, y - y0);
    glm::vec2 distance10(x - x1, y - y0);
    glm::vec2 distance01(x - x0, y - y1);
    glm::vec2 distance11(x - x1, y - y1);

    float scalar00 = glm::dot(g00, distance00);
    float scalar10 = glm::dot(g10, distance10);
    float scalar01 = glm::dot(g01, distance01);
    float scalar11 = glm::dot(g11, distance11);

    float u = x - x0;      
    float v = y - y0;     
    float uf = SmoothFunction(u);     
    float vf = SmoothFunction(v);

    float lerpX1 = glm::mix(scalar00, scalar10, uf);
    float lerpX2 = glm::mix(scalar01, scalar11, uf);
    float noise = glm::mix(lerpX1, lerpX2, vf);

    return (noise + 1.0f) * 0.5f; // remap from [-1,1] -> [0,1];
}

glm::vec2 PerlinNoise::RandomGradient(int x, int y) const
{
    int hash = perm[(perm[x & 255] + y) & 255];
    int randIndex = hash % randomUniVecSet.size();
    return randomUniVecSet[randIndex];
}

float PerlinNoise::SmoothFunction(float x) const
{
    return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}