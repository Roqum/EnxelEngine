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

		std::vector<bool> visibleMaskXPos(m_Settings.ChunkVolume, false);
		std::vector<bool> visibleMaskXNeg(m_Settings.ChunkVolume, false);
		std::vector<bool> visibleMaskYPos(m_Settings.ChunkVolume, false);
		std::vector<bool> visibleMaskYNeg(m_Settings.ChunkVolume, false);
		std::vector<bool> visibleMaskZPos(m_Settings.ChunkVolume, false);
		std::vector<bool> visibleMaskZNeg(m_Settings.ChunkVolume, false);

		for (int index = 0; index < m_Settings.ChunkVolume; index++)
		{
			int localX, localY, localZ;
			toCoords(index, localX, localY, localZ);

			VoxelType currentType = voxels[index].Type;



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
				visibleMaskYPos[index] = true;
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::TOP);
			}
			if (localY == 0 || voxels[toIndex(localX, localY - 1, localZ)].Type == VoxelType::NONE)
			{
				visibleMaskYNeg[index] = true;
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::BOTTOM);
			}
			if (localX == (m_Settings.ChunkSize - 1) || voxels[toIndex(localX + 1, localY, localZ)].Type == VoxelType::NONE)
			{
				visibleMaskXPos[index] = true;	
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::RIGHT);
			}
			if (localX == 0 || voxels[toIndex(localX - 1, localY, localZ)].Type == VoxelType::NONE)
			{
				visibleMaskXNeg[index] = true;
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::LEFT);
			}
			if (localZ == (m_Settings.ChunkSize - 1) || voxels[toIndex(localX, localY, localZ + 1)].Type == VoxelType::NONE)
			{
				visibleMaskZPos[index] = true;
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::FRONT);
			}
			if (localZ == 0 || voxels[toIndex(localX, localY, localZ - 1)].Type == VoxelType::NONE)
			{
				visibleMaskZNeg[index] = true;
				//voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, CubeFace::BACK);
			}
			
		}

	}

	void Chunk::Cleanup()
	{
		if (m_VertexBuffer) {
			m_VertexBuffer->Unbind();
			delete m_VertexBuffer;
			m_VertexBuffer = nullptr;
		}
		if (m_IndexBuffer) {
			m_IndexBuffer->Unbind();
			delete m_IndexBuffer;
			m_IndexBuffer = nullptr;
		}
		voxels.clear();
	}
	void Chunk::GreedyMeshing(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::vector<bool>& mask)
	{
		
		int meshedXSize = 0;

		for (int index = 0; index < m_Settings.ChunkVolume; index++)
		{
			if (!mask[index])
			{
				continue;
			}


		}


		// Greedy Mesh X axies
		for (int neighborIndexX = 1; neighborIndexX < (mask); neighborIndexX++)
		{
			if (mask[index + neighborIndexX].Type != currentType)
			{
				break;
			}
			meshedXSize++;
		}

		// Greedy Mesh Z axies
		int meshedZSize = 0;
		for (int meshedX = 0; meshedX < meshedXSize; meshedX++)
		{
			bool zRowMeshSucceded = true;
			for (int neighborIndexZ = 1; neighborIndexZ < (m_Settings.ChunkVolume - localZ); neighborIndexZ++)
			{
				if (voxels[toIndex(localX + meshedX, localY, localZ + neighborIndexZ)].Type != currentType)
				{
					zRowMeshSucceded = false;
					break;
				}
			}
			if (!zRowMeshSucceded)
			{
				break;
			}

			meshedZSize++;
		}

		// Greedy Mesh Y axies
		int meshedYSize = 0;
		for (int meshedX = 0; meshedX < meshedXSize; meshedX++)
		{
			bool yRowMeshSucceded = true;

			for (int meshedZ = 0; meshedZ < meshedZSize; meshedZ++)
			{
				for (int neighborIndexY = 1; neighborIndexY < (m_Settings.ChunkVolume - localY); neighborIndexY++)
				{
					if (voxels[toIndex(localX + meshedX, localY + neighborIndexY, localZ + meshedZ)].Type != currentType)
					{
						yRowMeshSucceded = false;
						break;
					}
				}
				if (!yRowMeshSucceded)
				{
					break;
				}
			}
			if (!yRowMeshSucceded)
			{
				break;
			}
			meshedYSize++;
		}

	}
}
