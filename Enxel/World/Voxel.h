#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include "Vulkan/Vertex.h"


enum CubeFace {
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
	BACK,
	FRONT
};

enum VoxelType {
	NONE = 0,
	GRASS = 1,
	DIRT = 2,
	STONE = 3,
	SAND = 4,
};

struct Voxel{
	VoxelType Type;

	
	void addVoxelFace(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 position, CubeFace cubeFace, float voxelSize = 1.0);
	

};

