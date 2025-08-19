#pragma once

#include "Voxel.h"
#include "Vertex.h"
#include <vector>
#include <array>
#include <Buffer.h>

namespace Enxel
{
	constexpr int CHUNK_SIZE = 32;
	constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
	constexpr int CHUNK_MASK = CHUNK_SIZE - 1;
	constexpr int CHUNK_SHIFT_Y = 5; // log2(32)
	constexpr int CHUNK_SHIFT_Z = 10; // log2(32*32)

	/* struct Voxel {
		uint8_t type;
	};
	*/

	struct Chunk {
		std::array<Voxel, CHUNK_VOLUME> voxels;


		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;

		static const int CHUNK_SIZE = 32;

		uint8_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

		// x,y,z -> index

		inline int toIndex(int x, int y, int z) {
			return x | (y << CHUNK_SHIFT_Y) | (z << CHUNK_SHIFT_Z);
		}

		// index -> x,y,z
		inline void toCoords(int idx, int& x, int& y, int& z) {
			x = idx & CHUNK_MASK;
			y = (idx >> CHUNK_SHIFT_Y) & CHUNK_MASK;
			z = idx >> CHUNK_SHIFT_Z;
		}

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
}