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
		chunks.reserve(4);
		for (int x = 0; x < 2; x++)
		{
			for (int z = 0; z < 2; z++)
			{
				chunks.emplace_back(x, 0, z, m_Settings, m_PerlinNoise);
			}
		}
	}
}
