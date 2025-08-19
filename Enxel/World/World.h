#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include <array>
#include <random>
#include <numeric>    
#include <algorithm>

class World 
{
    World();
    virtual ~World();
}



struct PerlinNoise
{
	PerlinNoise(float x, float y, int seed = 1) : seed(seed), pseudoRandomNumber(seed)
    {
        int x0 = (int)x;
        int x1 = (int)x + 1;
        int y0 = (int)y;
        int y1 = (int)y + 1;

        std::iota(perm.begin(), perm.begin() + 256, 0);
        std::shuffle(perm.begin(), perm.begin() + 256, std::mt19937(seed));
        std::copy(perm.begin(), perm.begin() + 256, perm.begin() + 256);


        glm::vec2 g00 = randomGradient(x0, y0);
        glm::vec2 g10 = randomGradient(x1, y0);
        glm::vec2 g01 = randomGradient(x0, y1);
        glm::vec2 g11 = randomGradient(x1, y1);

        glm::vec2 distance00(x - x0, y - y0);
        glm::vec2 distance10(x - x1, y - y0);
        glm::vec2 distance01(x - x0, y - y1);
        glm::vec2 distance11(x - x1, y - y1);

        float scalar00 = glm::dot(g00, distance00);
        float scalar10 = glm::dot(g10, distance10);
        float scalar01 = glm::dot(g01, distance01);
        float scalar11 = glm::dot(g11, distance11);

        


    }
    glm::vec2 randomGradient(int x, int y)
    {
        int hash = perm[(perm[x & 255] + y) & 255];
        int randIndex = hash % randomUniVecSet.size();
        return randomUniVecSet[randIndex];
    }
    float smoothFunction(float x)
    {
        return x * x * x (10 - (x * 15 + ( x * 6 )))
    }
int Width;
int Height;

std::array<glm::vec2, 8> randomUniVecSet = {
    glm::vec2( 1,  0),
    glm::vec2( 0,  1),
    glm::vec2(-1,  0),
    glm::vec2( 0, -1),
    glm::vec2(0.7071f,  0.7071f),
    glm::vec2(-0.7071f,  0.7071f),
    glm::vec2(0.7071f, -0.7071f),
    glm::vec2(-0.7071f, -0.7071f)
};
    unsigned int seed;
    std::mt19937 pseudoRandomNumber;

    std::array<int, 512> perm;

}