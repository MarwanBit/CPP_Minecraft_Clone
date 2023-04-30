#include <glm/glm.hpp>
#include "Face.h"

class Block {

public:
	Block(glm::vec3* Faces);
	~Block();


private:
	
	/*create an array of face objects*/
	Face* faces_;

	
};