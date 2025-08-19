#pragma once

#include <cmath>

namespace Enxel
{
	struct WorldSettings
	{
		unsigned int Seed;
		unsigned short MaxHeight;
		uint8_t ChunkSize;
		float VoxelSize;

		int ChunkMask;
		int ChunkShiftY; 
		int ChunkShiftZ;
		int ChunkVolume;

		WorldSettings(unsigned int seed = 1, unsigned short maxHeight = 32, uint8_t chunkSize = 32, float voxelSize = 1)
			: Seed(seed), MaxHeight(maxHeight), ChunkSize(chunkSize), VoxelSize(voxelSize)
		{
			ChunkVolume = ChunkSize * ChunkSize * ChunkSize;
			ChunkMask = ChunkSize - 1;
			ChunkShiftY = static_cast<int>(std::log2(ChunkSize));
			ChunkShiftZ = static_cast<int>(std::log2(ChunkSize * ChunkSize));
		};
	};
}
