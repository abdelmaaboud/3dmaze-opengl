#ifndef Model_h__
#define Model_h__

#include <glm/glm.hpp>
#include <vector>
#include <gl\glew.h>
class Model
{



	//VAO
	GLuint mVertexArrayObjectID;

	//VBOs
	GLuint mVertexDataBufferID;
	GLuint mColorDataBufferID;
	GLuint mIndicesDataBufferID;
	GLuint mUVDataBufferID;
	GLuint mNormalsBufferID;
	/*std::vector<glm::vec3> mNormalData;
	std::vector<glm::vec2> mUVData;*/
public:
	Model();
	~Model();

	std::vector<glm::vec3> VertexData;
	std::vector<glm::vec3> ColorData;
	std::vector<unsigned short> IndicesData;
	std::vector<glm::vec2> UVData;
	std::vector<glm::vec3> NormalsData;
	void Initialize();
	void Draw();
	void Cleanup();

};
#endif // Model_h__

