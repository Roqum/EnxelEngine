#pragma once 

#include "glm/glm.hpp"
#include <array>
#include <random>
#include <numeric>    
#include <algorithm>

class PerlinNoise
{
public:
    PerlinNoise(int seed = 1); 
    virtual ~PerlinNoise() = default;

    float Noise(float x, float y) const;

private:
    glm::vec2 RandomGradient(int x, int y) const;
    float SmoothFunction(float x) const;

    unsigned int seed;
    std::mt19937 pseudoRandomNumber;
    std::array<int, 512> perm;

    std::array<glm::vec2, 8> randomUniVecSet = {
        glm::vec2( 1,  0),
        glm::vec2( 0,  1),
        glm::vec2(-1,  0),
        glm::vec2( 0, -1),
        glm::vec2( 0.7071f,  0.7071f),
        glm::vec2(-0.7071f,  0.7071f),
        glm::vec2( 0.7071f, -0.7071f),
        glm::vec2(-0.7071f, -0.7071f)
    };
    

};