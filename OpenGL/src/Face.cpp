#include "Face.h"
#include <iostream>

Face::Face(const glm::vec3& pos0, const glm::vec3& pos1,
	const glm::vec3& pos2, const glm::vec3& pos3,
	const std::string& path) :
	positions_{ nullptr }, texture_{Texture(path)} {

	/*Here we will construct our positions float */
	positions_ = new float[20] {
			pos0.x, pos0.y, pos0.z, 0.0f, 0.0f,
			pos1.x, pos1.y, pos1.z, 1.0f, 0.0f,
			pos2.x, pos2.y, pos2.z, 1.0f, 1.0f,
			pos3.x, pos3.y, pos3.z, 0.0f, 1.0f,
	};

	//Generate the layout (so we read x,y,z vectors in position and then the texture coordinates)
	layout_.Push<float>(3);
	layout_.Push<float>(2);

	vb_ = new VertexBuffer(positions_, 5 * 4 * sizeof(float));
	va_.AddBuffer(*vb_, layout_);
}


Face::~Face() {
	delete[] positions_;
	delete vb_;
}

