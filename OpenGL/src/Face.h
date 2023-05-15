#include <glm/glm.hpp>
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "glm/gtc/matrix_transform.hpp"

class Face {
	
public:
	Face(const glm::vec3& pos0, const glm::vec3&  pos1, 
		const glm::vec3& pos2, const glm::vec3&  pos3, 
		const std::string& path, const std::string& shader_path);
	~Face();
;
	glm::vec3* getPos();
	glm::vec3* getVecPos();
	float* getPositions();
	unsigned int* getIndices();
	void updatePos(const glm::vec3& newPos);
	Texture getTexture();
	Shader getShader();

private:
	Texture texture_;
	float* positions_;
	Shader shader_;

	// Tells the program how to draw the triangles forming the face
	unsigned int indices_[6] = {
		0, 1, 2,
		2, 3, 0
	};

	glm::vec3 pos_{ 0,0,0 };	

	glm::vec3* vecPos_;
};