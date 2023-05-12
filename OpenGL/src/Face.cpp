#include "Face.h"
#include <iostream>

Face::Face(const glm::vec3& pos0, const glm::vec3& pos1,
	const glm::vec3& pos2, const glm::vec3& pos3,
	const std::string& path) :
	positions_{ nullptr }, texture_{Texture(path)} {

	/*Here we will construct our positions float */
	positions_ = new float[16] {
			pos0.x, pos0.y, 0.0f, 0.0f,
			pos1.x, pos1.y, 1.0f, 0.0f,
			pos2.x, pos2.y, 1.0f, 1.0f,
			pos3.x, pos3.y, 0.0f, 1.0f,
	};
}


Face::~Face() {
	delete[] positions_;
}

glm::vec3* Face::getPos() {
	return &(pos_);
}

float* Face::getPositions() {
	return positions_;
}

unsigned int* Face::getIndices() {
	return indices_;
}

Texture Face::getTexture() {
	return texture_;
}

void Face::updatePos(const glm::vec3& newPos) {
	glm::vec3 pos_ = newPos;
}