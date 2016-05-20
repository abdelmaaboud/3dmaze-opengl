#ifndef Renderer_h__
#define Renderer_h__

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <memory>


#include "Shaders/shader.hpp"
#include "Texture/texture.h"
#include "Model/Model.h"
#include "EulerCamera/EulerCamera.h"

class Renderer
{
	enum RenderingMode
	{
		NO_TEXTURE,
		TEXTURE_ONLY,
		BLEND
	};

	GLuint programID;

	// Transformation
	GLuint MatrixID;
	int maze[20][30];
	std::unique_ptr<Model> myTriangle;
	std::unique_ptr<Model> mycube;
   
	

	glm::vec3 ambientLight;
	glm::vec3 specularlight;
	glm::vec3 Diffuselight;
	glm::vec3 lightPosition;

	// Transformation
	
	GLuint ModelMatrixID;
	GLuint AmbientLightID;

	GLuint DissfuseLightID;
	GLuint SpecularLightID;

	GLuint LightPositionID;
	GLuint EyePositionID;
	GLuint AttID;

	GLuint MaterialDiffuseColorID;
	GLuint MaterialAmbientColorID;
	GLuint MaterialSpecularColorID;


	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	std::unique_ptr<EulerCamera> myCamera;

	std::unique_ptr<Texture> mTexture1;
	std::unique_ptr<Texture> mTexture2;
	std::unique_ptr<Texture> mTexture3;
	std::unique_ptr<Texture> mTexture4;
	std::unique_ptr<Texture> mTexture5;
	std::unique_ptr<Texture> mTexture6;
	std::unique_ptr<Texture> mTexture7;
	
	GLuint mRenderingModeID;
	RenderingMode mRenderingMode;


	std::unique_ptr<Model> cube;
	std::unique_ptr<Model> cube1;
	
	glm::mat4 cubeM;
	glm::mat4 sqmskybox;
	glm::mat4 sqmhero;
	glm::mat4 sqmenemy;
	glm::mat4 sqmenemy2;
	glm::mat4 final;
	int counter;
	glm::mat4 floorM;
	
	
	float hero_x, hero_z, enemy1x, enemy2x, enemy1z, enemy2z;
	float min_x, min_z, max_x, max_z;
	// bool random;
	int score;
	int camera_type,camera_scale;
	float look_at_x,look_at_z;
	int direction;

public:
	Renderer();
	~Renderer();

	void Initialize();
	void Draw();
	void HandleKeyboardInput(int key);
	void HandleMouse(double deltaX, double deltaY);

	
	void Update(double deltaTime);
	void Cleanup();

	void move(char direction,char sign);

	void initCameraPosition();
	void initTextcure();
	void createcube(std::unique_ptr<Model> &cube);
	void Initializemaze();
	
};

#endif // Renderer_h__

