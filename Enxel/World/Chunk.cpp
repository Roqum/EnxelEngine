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

		GreedyMeshing(vertices, indices, visibleMaskYPos, CubeFace::TOP);
		GreedyMeshing(vertices, indices, visibleMaskYNeg, CubeFace::BOTTOM);
		GreedyMeshing(vertices, indices, visibleMaskXPos, CubeFace::RIGHT);
		GreedyMeshing(vertices, indices, visibleMaskXNeg, CubeFace::LEFT);
		GreedyMeshing(vertices, indices, visibleMaskZPos, CubeFace::FRONT);
		GreedyMeshing(vertices, indices, visibleMaskZNeg, CubeFace::BACK);


	}
	static int chooseRightIncrementorFirstAxies(const CubeFace& faceSide, const WorldSettings& settings)
	{
		switch (faceSide)
		{
		case CubeFace::TOP:
		case CubeFace::BOTTOM:
			return 1; // X direction incrementor
		case CubeFace::LEFT:
		case CubeFace::RIGHT:
			return 1 << settings.ChunkShiftZ; // Z direction incrementor
		case CubeFace::FRONT:
		case CubeFace::BACK:
			return 1; // X direction incrementor
		default:
			return 1; // Error
		}
	}

	static int chooseRightIncrementorSecondAxies(const CubeFace& faceSide, const WorldSettings& settings)
	{
		switch (faceSide)
		{
		case CubeFace::TOP:
		case CubeFace::BOTTOM:
			return  1 << settings.ChunkShiftZ; // Z direction incrementor
		case CubeFace::LEFT:
		case CubeFace::RIGHT:
			return 1 << settings.ChunkShiftY; // Y direction incrementor
		case CubeFace::FRONT:
		case CubeFace::BACK:
			return 1 << settings.ChunkShiftY; // Y direction incrementor
		default:
			return 1; // Error
		}
	}
	static bool CheckAxiesBoundFirstAxies(int index, const CubeFace& faceSide, const WorldSettings& settings)
	{
		switch (faceSide)
		{
		case CubeFace::TOP:
		case CubeFace::BOTTOM:
			return  (index & settings.ChunkMask) < (settings.ChunkSize - 1); // X direction bound
		case CubeFace::LEFT:
		case CubeFace::RIGHT:
			return  ((index >> settings.ChunkShiftZ) & settings.ChunkMask) < (settings.ChunkSize - 1); // Z direction bound
		case CubeFace::FRONT:
		case CubeFace::BACK:
			return  (index & settings.ChunkMask) < (settings.ChunkSize - 1); // X direction bound
		default:
			return 1; // Error
		}
	}
	static bool CheckAxiesBoundSecondAxies(int index, const CubeFace& faceSide, const WorldSettings& settings)
	{
		switch (faceSide)
		{
		case CubeFace::TOP:
		case CubeFace::BOTTOM:
			return  ((index >> settings.ChunkShiftZ) & settings.ChunkMask) < (settings.ChunkSize - 1); // Z direction bound
		case CubeFace::LEFT:
		case CubeFace::RIGHT:
			return  ((index >> settings.ChunkShiftY) & settings.ChunkMask) < (settings.ChunkSize - 1); // Y direction bound
		case CubeFace::FRONT:
		case CubeFace::BACK:
			return  ((index >> settings.ChunkShiftY) & settings.ChunkMask) < (settings.ChunkSize - 1); // Y direction bound
		default:
			return 1; // Error
		}
	}

	void Chunk::GreedyMeshing(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<bool> mask, const CubeFace& faceSide)
	{
		// +X -> right, -X -> left 
		// +Y -> top, -Y -> bottom
		// +Z -> front, -Z -> back
		

		int firstDirectionIncremental = chooseRightIncrementorFirstAxies(faceSide, m_Settings);
		int secondDirectionIncremental = chooseRightIncrementorSecondAxies(faceSide, m_Settings);

		for (int index = 0; index < m_Settings.ChunkVolume; index++)
		{

			int meshedFirstDirectionSize = 1;
			int meshedSecondDirectionSize = 1;

			int localX, localY, localZ;
			toCoords(index, localX, localY, localZ);

			glm::vec3 voxelWorldPosition = {
				m_WorldSpacePosition.x + localX * m_Settings.VoxelSize,
				m_WorldSpacePosition.y + localY * m_Settings.VoxelSize,
				m_WorldSpacePosition.z + localZ * m_Settings.VoxelSize
			};

			if (!mask[index])
			{
				continue;
			}
			VoxelType meshedType = voxels[index].Type;

			// Greedy Mesh first axies
			for (int firstDirectionNeighbor = index + firstDirectionIncremental; CheckAxiesBoundFirstAxies(firstDirectionNeighbor, faceSide, m_Settings); firstDirectionNeighbor += firstDirectionIncremental)
			{

				if (!mask[firstDirectionNeighbor] || voxels[firstDirectionNeighbor].Type != meshedType)
				{
					break;
				}
				meshedFirstDirectionSize++;

			}

			// Greedy Mesh second axies
			for (int secondDirectionNeighbor = index + secondDirectionIncremental; CheckAxiesBoundSecondAxies(secondDirectionNeighbor, faceSide, m_Settings); secondDirectionNeighbor += secondDirectionIncremental)
			{
				bool xRowMeshSucceded = true;

				for (int meshedXRowIndex = 0; meshedXRowIndex < meshedFirstDirectionSize; meshedXRowIndex++)
				{
					int currentIndex = secondDirectionNeighbor + meshedXRowIndex * firstDirectionIncremental; 
					if (!mask[currentIndex] || voxels[currentIndex].Type != meshedType)
					{
						xRowMeshSucceded = false;
						break;
					}
				}
				if (!xRowMeshSucceded)
				{
					break;
				}
				meshedSecondDirectionSize++;
			}


			for (int meshedX = 0; meshedX < meshedFirstDirectionSize; meshedX++)
			{
				for (int meshedY = 0; meshedY < meshedSecondDirectionSize; meshedY++)
				{
					int currentIndex = index + meshedX * firstDirectionIncremental + meshedY * secondDirectionIncremental;
					mask[currentIndex] = false;
				}
			}

			voxels[index].addVoxelFace(vertices, indices, voxelWorldPosition, faceSide, glm::vec2(meshedFirstDirectionSize * m_Settings.VoxelSize, meshedSecondDirectionSize * m_Settings.VoxelSize));

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

}