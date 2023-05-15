#include "Face.h"
#include <iostream>

Face::Face(const glm::vec3& pos0, const glm::vec3& pos1,
	const glm::vec3& pos2, const glm::vec3& pos3,
	const std::string& path, const std::string& shader_path) :
	positions_{ nullptr }, texture_{Texture(path)},
	shader_{shader_path} {

	/*Here we will construct our positions float */
	positions_ = new float[16] {
			pos0.x, pos0.y, pos0.z, 1.0f,  // 0.0f, 0.0f,
			pos1.x, pos1.y, pos1.z, 1.0f,  // 1.0f, 0.0f,
			pos2.x, pos2.y, pos2.z, 1.0f,  // 1.0f, 1.0f,
			pos3.x, pos3.y, pos3.z, 1.0f,  // 0.0f, 1.0f,
	};

	vecPos_ = new glm::vec3[4]{ pos0, pos1, pos2, pos3 };
}


Face::~Face() {
	delete[] positions_;
}

glm::vec3* Face::getPos() {
	return &(pos_);
}

glm::vec3* Face::getVecPos(){
	return vecPos_;
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

Shader Face::getShader() {
	return shader_;
}

void Face::updatePos(const glm::vec3& newPos) {
	glm::vec3 pos_ = newPos;
}