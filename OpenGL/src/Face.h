#include <glm/glm.hpp>
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

class Face {
	
public:
	Face(const glm::vec3& pos0, const glm::vec3&  pos1, 
		const glm::vec3& pos2, const glm::vec3&  pos3, 
		const std::string& path);
	~Face();

private:
	Texture texture_;
	float* positions_;

	// Tells the program how to draw the triangles forming the face
	unsigned int indices_[6] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexBufferLayout layout_;
	VertexArray va_;
	VertexBuffer* vb_;
	


	
};