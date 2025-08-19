#include "Chunk.h"


namespace Enxel
{
	Chunk::Chunk(int x, int y, int z, const WorldSettings& settings, const PerlinNoise& perlinNoise) : m_Settings(settings)
	{
		m_WorldSpacePosition = glm::vec3(
			static_cast<float>(x * m_Settings.ChunkSize * m_Settings.VoxelSize),
			static_cast<float>(y * m_Settings.ChunkSize * m_Settings.VoxelSize),
			static_cast<float>(z * m_Settings.ChunkSize * m_Settings.VoxelSize)
		);

		voxels.resize(m_Settings.ChunkVolume);

		for (int index = 0; index < m_Settings.ChunkVolume; index++)
		{
			int localX, localY, localZ;
			toCoords(index, localX, localY, localZ);
			float noiseValue = perlinNoise.Noise(
				static_cast<float>((m_WorldSpacePosition.x + localX * m_Settings.VoxelSize) * 0.05f),
				static_cast<float>((m_WorldSpacePosition.z + localZ * m_Settings.VoxelSize) * 0.05f)
			);

			int height = static_cast<int>(1 + noiseValue * (m_Settings.MaxHeight - 1));
			if (localY <= height) {
				voxels[index].Type = VoxelType::GRASS; // Solid voxel
			}
			else {
				voxels[index].Type = VoxelType::NONE; // Air voxel
			}
		}

	}
	void Chunk::draw(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertices.clear();
		indices.clear(); 

		for (int index = 0; index < m_Settings.ChunkVolume; index++)
		{
			int localX, localY, localZ;
			toCoords(index, localX, localY, localZ);

			glm::vec3 voxelWorldPosition = {
				m_WorldSpacePosition.x + localX * m_Settings.VoxelSize,
				m_WorldSpacePosition.y + localY * m_Settings.VoxelSize,
				m_WorldSpacePosition.z + localZ * m_Settings.VoxelSize
			};

			if (voxels[index].Type == VoxelType::NONE)
			{
				continue;
			}
			if (localY == (m_Settings.ChunkSize - 1) || voxels[toIndex(localX, localY + 1, localZ)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::TOP);
			}
			if (localY == 0 || voxels[toIndex(localX, localY - 1, localZ)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::BOTTOM);
			}
			if (localX == (m_Settings.ChunkSize - 1) || voxels[toIndex(localX + 1, localY, localZ)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::RIGHT);
			}
			if (localX == 0 || voxels[toIndex(localX - 1, localY, localZ)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::LEFT);
			}
			if (localZ == (m_Settings.ChunkSize - 1) || voxels[toIndex(localX, localY, localZ + 1)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::FRONT);
			}
			if (localZ == 0 || voxels[toIndex(localX, localY, localZ - 1)].Type == VoxelType::NONE)
			{
				voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::BACK);
			}
			
		}
	}
}
