#include "World.h"
#include "Chunk.h"

namespace Enxel
{
	World::World(WorldSettings worldSettings) : m_Settings(worldSettings)
	{
		m_PerlinNoise = PerlinNoise(m_Settings.Seed);
	}


	// Test purpose
	void World::Generate(int length, int width)
	{
		chunks.reserve(length * width);
		for (int x = -(length / 2); x < (length / 2); x++)
		{
			for (int z = -(width / 2); z < (width / 2); z++)
			{
				chunks.emplace_back(x, 0, z, m_Settings, m_PerlinNoise);
			}
		}
	}
}
