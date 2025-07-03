#include "Voxel.h"
#include <vector>

struct Chunk {
	static const int CHUNK_SIZE = 32;

	uint8_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	void generateChunk() {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					blocks[x][y][z] = 1;
				}
			}
		}
	}
	void draw(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertices.clear();
		indices.clear();


		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					if (blocks[x][y][z] == 0)
					{
						continue;
					}
					std::vector<float> voxelPosition = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
					Voxel newVoxel;
					if ((y == (CHUNK_SIZE - 1)) || (blocks[x][y + 1][z] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::TOP);
					}
					if ((y == 0) || (blocks[x][y - 1][z] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::BOTTOM);
					}
					if ((z == (CHUNK_SIZE - 1)) || (blocks[x][y][z + 1] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::FRONT);
					}
					if ((z == 0) || (blocks[x][y][z - 1] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::BACK);
					}
					if ((x == (CHUNK_SIZE - 1)) || (blocks[x + 1][y][z] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::RIGHT);
					}
					if ((x == 0) || (blocks[x - 1][y][z] == 0))
					{
						newVoxel.addVoxelFace(vertices, indices, voxelPosition, CubeFace::LEFT);
					}

				}
			}
		}

	}
};