#pragma once
#include "PerlinNoise.h"
#include "WorldSettings.h"
#include "Chunk.h"


namespace Enxel
{
	class World
	{
	public:
		World(WorldSettings worldSettings = WorldSettings());
		virtual ~World() = default;

		void Generate(int length, int width);

		//inline std::vector<Chunk>& GetChunks() { return m_Chunks; }
		std::vector<Chunk> chunks;


	private:

		WorldSettings m_Settings;

		PerlinNoise m_PerlinNoise;

	};
}