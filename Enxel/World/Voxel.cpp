#include "Voxel.h"

void Voxel::addVoxelFace(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 position, CubeFace cubeFace, float voxelSize)
{
		uint32_t baseIndex = static_cast<uint32_t>(vertices.size());
		assert(baseIndex < UINT32_MAX - 3); 

		switch (cubeFace)
		{
		case CubeFace::TOP:
		{
			// TOP face (Y+) - viewed from above, counter-clockwise
			vertices.push_back({ { position[0],     position[1] + 1, position[2]     }, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}); // 0
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2]     }, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} }); // 1
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2] + 1 }, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0],     position[1] + 1, position[2] + 1 }, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 1), static_cast<uint32_t>(baseIndex + 2),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 3)
				});
			break;
		}

		case CubeFace::BOTTOM:
		{
			// BOTTOM face (Y-) - viewed from below, clockwise
			vertices.push_back({ { position[0],     position[1], position[2]     }, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} }); // 0
			vertices.push_back({ { position[0],     position[1], position[2] + 1 }, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }); // 1
			vertices.push_back({ { position[0] + 1, position[1], position[2] + 1 }, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0] + 1, position[1], position[2]     }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 1), static_cast<uint32_t>(baseIndex + 2),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 3)
				});
			break;
		}

		case CubeFace::FRONT:
		{
			// FRONT face (Z+) - facing towards viewer, clockwise
			vertices.push_back({ { position[0],     position[1],     position[2] + 1 }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }); // 0
			vertices.push_back({ { position[0] + 1, position[1],     position[2] + 1 }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }); // 1
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2] + 1 }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0],     position[1] + 1, position[2] + 1 }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 1),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 3), static_cast<uint32_t>(baseIndex + 2)
				});
			break;
		}

		case CubeFace::BACK:
		{
			// BACK face (Z-) - facing away from viewer, counter-clockwise
			vertices.push_back({ { position[0] + 1, position[1],     position[2] }, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f} }); // 0
			vertices.push_back({ { position[0],     position[1],     position[2] }, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f} }); // 1
			vertices.push_back({ { position[0],     position[1] + 1, position[2] }, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2] }, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 1),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 3), static_cast<uint32_t>(baseIndex + 2)
				});
			break;
		}

		case CubeFace::RIGHT:
		{
			// RIGHT face (X+) - right side, counter-clockwise
			vertices.push_back({ { position[0] + 1, position[1],     position[2]     }, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }); // 0
			vertices.push_back({ { position[0] + 1, position[1],     position[2] + 1 }, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }); // 1
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2] + 1 }, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0] + 1, position[1] + 1, position[2]     }, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 1), static_cast<uint32_t>(baseIndex + 2),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 3)
				});
			break;
		}

		case CubeFace::LEFT:
		{
			// LEFT face (X-) - left side, clockwise
			vertices.push_back({ { position[0], position[1],     position[2] + 1 }, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }); // 0
			vertices.push_back({ { position[0], position[1],     position[2]     }, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }); // 1
			vertices.push_back({ { position[0], position[1] + 1, position[2]     }, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }); // 2
			vertices.push_back({ { position[0], position[1] + 1, position[2] + 1 }, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} }); // 3

			indices.insert(indices.end(), {
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 1), static_cast<uint32_t>(baseIndex + 2),
				static_cast<uint32_t>(baseIndex), static_cast<uint32_t>(baseIndex + 2), static_cast<uint32_t>(baseIndex + 3)
				});
			break;
		}
		}

	}