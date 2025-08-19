#pragma once

#include <glm/vec3.hpp>
#include "WorldSettings.h"
#include "PerlinNoise.h"
#include "Voxel.h"
#include "Renderer/Buffer.h"

namespace Enxel
{
	class Chunk
	{
	public:
		Chunk(int x, int y, int z, const WorldSettings& settings, const PerlinNoise& perlinNoise);

		void draw(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

		inline void setVertexBuffer(VertexBuffer* vb) { m_VertexBuffer = vb; }
		inline void setIndexBuffer(IndexBuffer* ib) { m_IndexBuffer = ib; }
		inline VertexBuffer* getVertexBuffer() const { return m_VertexBuffer; }
		inline IndexBuffer* getIndexBuffer() const { return m_IndexBuffer; }
		
	private:
		const WorldSettings& m_Settings;

		glm::vec3 m_WorldSpacePosition; 
		std::vector<Voxel> voxels; 

		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
	
	// x,y,z -> index
	inline int toIndex(int x, int y, int z) {
		return x | (y << m_Settings.ChunkShiftY) | (z << m_Settings.ChunkShiftZ);
	}

	// index -> x,y,z
	inline void toCoords(int idx, int& x, int& y, int& z) {
		x = idx & m_Settings.ChunkMask;
		y = (idx >> m_Settings.ChunkShiftY) & m_Settings.ChunkMask;
		z = idx >> m_Settings.ChunkShiftZ;
	}

	};
}
