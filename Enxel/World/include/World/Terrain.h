#include <cstdint>
#include "Chunk.h"
#include <vcruntime_string.h>


struct Terrain
{
	int maxWidth;
	int maxLength;
	int maxDepth;

	std::vector<Chunk> world;
	Terrain(int width, int length, int depth)
	{
		maxWidth = width;
		maxLength = length;
		maxDepth = depth;
	}


	void draw(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		for (int x = 0; x < maxWidth; x++) {
			for (int y = 0; y < maxLength; y++) {
				for (int z = 0; z < maxDepth; z++)
				{
				}
			}
		}
	}
	//Chunk world[32][32][1];
};