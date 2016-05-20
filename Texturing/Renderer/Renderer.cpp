#include "Renderer.h"
#include <gl/glew.h>
#include <gtc/matrix_transform.hpp> 
#include <gtx/transform.hpp>

using namespace glm;
Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

int Level = 0;
void Renderer::Initialize()
{
	myCamera = std::unique_ptr<EulerCamera>(new EulerCamera());
	score = 0;


	//For Start Game.
	if (Level == 0) {
		//Creat Cube For Floor.
		createcube(mycube);
		floorM = glm::translate(-5.0f, -1.0f, -5.0f)*glm::scale(30.0f, 1.0f, 40.0f);

		//Creat Cub For SkyBox.
		createcube(cube);
		sqmskybox = glm::translate(-3.0f, -1.0f, -3.0f)*glm::scale(25.0f, 50.0f, 35.0f);


		//Initialize Texture
		initTextcure();

		//Initialize Position_Camera.
		initCameraPosition();
	}
	else {

		//Initialize Maze (2D Array).
		Initializemaze();

		//Creat Cube For Floor.
		createcube(mycube);
		floorM = glm::translate(-5.0f, -1.0f, -5.0f)*glm::scale(30.0f, 1.0f, 40.0f);

		//Creat Cub For SkyBox.
		createcube(cube);
		sqmskybox = glm::translate(-3.0f, -1.0f, -3.0f)*glm::scale(25.0f, 50.0f, 35.0f);

		//Creat Cub For Hero
		createcube(cube);
		sqmhero = glm::translate(+1.0f, 0.0f, 0.0f);

		//Creat Cub For Down Enemy
		createcube(cube);
		sqmenemy = glm::translate(+3.0f, 0.0f, +19.0f);

		//Creat Cub For Up Enemy
		createcube(cube);
		sqmenemy2 = glm::translate(+17.0f, 0.0f, +12.0f);

		//Creat Cub For Final_State
		createcube(cube);
		final = glm::translate(+16.0f, -0.8f, +29.0f)*glm::scale(3.0f, 1.0f, 3.0f);


		//Initialize Texture
		initTextcure();


		camera_type = 0;
		hero_x = .5, hero_z = 0, enemy1x = 3, enemy2x = 17, enemy1z = 19, enemy2z = 12;
		min_x = .5, min_z = 0, max_x = 17, max_z = 30;
		camera_scale = 20;
		look_at_x = hero_x, look_at_z = 10;
		counter = 0;
		direction = 2;
	}

}
double speed1 = 0.06;
double speed2 = 0.06;
void Renderer::Update(double deltaTime)
{
	if (Level == 0) {
	}
	else {
		glm::mat4 enemy1trans = glm::translate(0.0f, 0.0f, (float)speed1);
		sqmenemy = sqmenemy * enemy1trans;
		enemy1z += speed1;
		if (enemy1z < 16 || enemy1z >= 28)
		{
			speed1 = -speed1;
		}

		glm::mat4 enemy2trans = glm::translate(0.0f, 0.0f, (float)speed2);

		sqmenemy2 = sqmenemy2 * enemy2trans;

		enemy2z += speed2;
		if (enemy2z < 11 || enemy2z >= 20)
		{
			speed2 = -speed2;
		}

		if (hero_x >= 15.5&& hero_z >= 28) {
			Level++;
			Initialize();
		}
		if ((hero_x >= enemy2x - 2 && hero_x <= enemy2x) && (hero_z >= enemy2z - 0.5 && hero_z <= enemy2z + 0.5)) {
			Initialize();
		}
		if ((hero_x >= enemy1x - 2 && hero_x <= enemy1x) && (hero_z >= enemy1z - 0.5 && hero_z <= enemy1z + 0.5)) {
			Initialize();
		}
		//calculate look at
		if (direction == 1) {
			look_at_x = hero_x + 4;
			look_at_z = hero_z;
		}
		else  if (direction == 2) {
			look_at_x = hero_x;
			look_at_z = hero_z + 4;
		}
		else  if (direction == 3) {
			look_at_x = hero_x - 4;
			look_at_z = hero_z;
		}
		else  if (direction == 4) {
			look_at_x = hero_x;
			look_at_z = hero_z - 4;
		}
		if (camera_type % 3 == 0) {
			myCamera->Reset(0.0, 40.0, 15.0,
				10, 1, 15,
				0, 2, 0);
		}
		else if (camera_type % 3 == 1) {
			myCamera->Reset(hero_x, camera_scale, hero_z,
				hero_x, 1, hero_z + 3,
				0, 2, 0);
		}

		else if (camera_type % 3 == 2) {
			myCamera->Reset(hero_x + 0.5, 2, hero_z + 0.5,
				look_at_x, 0.5, look_at_z,
				0, 1, 0);
		}

		glm::mat4 VP = myCamera->GetProjectionMatrix() * myCamera->GetViewMatrix();

		int hx, hz;
		hx = int(hero_x + 0.25);
		hz = int(hero_z + 0.25);
		if (maze[hx][hz] == 2) {
			maze[hx][hz] = 0;
			score++;
			printf("score = %d \n", score);
		}
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 30; j++) {
				if (maze[i][j] == 2) {
					cubeM = glm::translate(i + 1.0f, 0.0f, j + 0.5f) *glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(0.5f, 0.5f, 0.5f);
					mTexture6->Bind();
					glm::mat4 cubeMVP = VP * cubeM;
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cubeMVP[0][0]);
					cube->Draw();
				}
			}
		}
		lightPosition = glm::vec3(hero_x, 10.0f, hero_z);
		glUniform3fv(LightPositionID, 1, &lightPosition[0]);
	}
}

void Renderer::move(char direction, char sign){
	glm::mat4 herotrans;
	int x = int(hero_x);
	int z = int(hero_z);
	if (direction == 'x'&&sign == '+'){
		x = int(hero_x + 1.0);
		if (hero_x <= max_x && (maze[x + 1][z] != 1 || z<0)){
			herotrans = glm::translate(1.0f, 0.0f, 0.0f);
			hero_x += 1.0;
		}
	}
	if (direction == 'x'&&sign == '-'){
		if (hero_x > min_x && (maze[x][z] != 1 || x<1 || z<0)){
			herotrans = glm::translate(-1.0f, 0.0f, 0.0f);
			hero_x -= 1.0;
		}
	}
	if (direction == 'z'&&sign == '+'){
		x = int(hero_x + 1);

		if (hero_z <= max_z && (maze[x][z + 1] != 1 || z<-1)){
			herotrans = glm::translate(0.0f, 0.0f, 1.0f);
			hero_z += 1.0;
		}
	}
	if (direction == 'z'&&sign == '-'){

		x = int(hero_x + 1);
		z = int(hero_z );
		if (hero_z > min_z && (maze[x][z - 1] != 1 || z<1)){
			herotrans = glm::translate(0.0f, 0.0f, -1.0f);
			hero_z -= 1.0;
		}
	}
	sqmhero *= herotrans;
}

void Renderer::Draw()
{
	// Use our shader
	glUseProgram(programID);

	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");

	glm::mat4 VP = myCamera->GetProjectionMatrix() * myCamera->GetViewMatrix();

	if (Level == 0 || Level == 4) {
		mTexture3->Bind();
		glm::mat4 sqMVP = VP * sqmskybox;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVP[0][0]);
		cube->Draw();

		if (Level == 0)
			mTexture2->Bind();
		else
			mTexture4->Bind();

		glm::mat4 floorMVP = VP * floorM;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &floorMVP[0][0]);
		mycube->Draw();

	}
	else {
		//the floor
		mTexture2->Bind();
		glm::mat4 floorMVP = VP * floorM;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &floorMVP[0][0]);
		mycube->Draw();

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 30; j++) {
				if (maze[i][j] == 1) {
					cubeM = glm::translate(i + 0.0f, 0.0f, j + 0.0f) *glm::scale(1.0f, 1.0f, 1.0f);
					mTexture1->Bind();
					glm::mat4 cubeMVP = VP * cubeM;
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cubeMVP[0][0]);
					cube->Draw();

				}
				else if (maze[i][j] == 2) {
					cubeM = glm::translate(i + 1.0f, 0.0f, j + 0.5f) *glm::scale(0.5f, 0.5f, 0.5f)*glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
					mTexture6->Bind();
					glm::mat4 cubeMVP = VP * cubeM;
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &cubeMVP[0][0]);
					cube->Draw();
				}
			}
		}
		mTexture3->Bind();
		glm::mat4 sqMVP = VP * sqmskybox;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVP[0][0]);
		cube->Draw();

		mTexture5->Bind();
		glm::mat4 sqMVPhero = VP * sqmhero;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVPhero[0][0]);
		cube->Draw();

		mTexture4->Bind();
		glm::mat4 sqMVPenemy = VP * sqmenemy;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVPenemy[0][0]);
		cube->Draw();

		mTexture4->Bind();
		glm::mat4 sqMVPenemy2 = VP * sqmenemy2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVPenemy2[0][0]);
		cube->Draw();

		mTexture7->Bind();
		glm::mat4 sqMVPfinal = VP * final;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sqMVPfinal[0][0]);
		cube->Draw();
	}
}

void Renderer::HandleKeyboardInput(int key)
{
	if (Level == 0) {
		if (key == GLFW_KEY_SPACE)
			Level++;
	}
	else if (Level == 4) {
		Level = 4;
	}
	else {
		counter++;
		printf("Renderer::KEY = %d \n", key);
		if (counter % 2) {

			switch (key)
			{
				//Moving forward
			case GLFW_KEY_UP:
			case GLFW_KEY_W:
				if (camera_type % 3 == 0) {
					move('x', '+');
				}
				else if (camera_type % 3 == 1) {
					move('z', '+');
				}
				else {
					if (direction == 1) {
						move('x', '+');
					}
					else if (direction == 2) {
						move('z', '+');
					}
					else if (direction == 3) {
						move('x', '-');
					}
					else if (direction == 4) {
						move('z', '-');
					}
				}
				break;

				//Moving backword
			case GLFW_KEY_DOWN:
			case GLFW_KEY_S:
				if (camera_type % 3 == 0) {
					move('x', '-');
				}
				else if (camera_type % 3 == 1) {
					move('z', '-');
				}
				else {
					if (direction == 1) {
						move('x', '-');
						direction = 3;
					}
					else if (direction == 2) {
						move('z', '-');
						direction = 4;
					}
					else if (direction == 3) {
						move('x', '+');
						direction = 1;
					}
					else if (direction == 4) {
						move('z', '+');
						direction = 2;
					}
				}
				break;

				// Moving right
			case GLFW_KEY_RIGHT:
			case GLFW_KEY_D:
				if (camera_type % 3 == 0) {
					move('z', '+');
				}
				else if (camera_type % 3 == 1) {
					move('x', '-');
				}
				else {
					if (direction == 1) {
						move('z', '+');
						direction = 2;
					}
					else if (direction == 2) {
						move('x', '-');
						direction = 3;
					}
					else if (direction == 3) {
						move('z', '-');
						direction = 4;
					}
					else if (direction == 4) {
						move('x', '+');
						direction = 1;
					}
				}
				break;
				// Moving left
			case GLFW_KEY_LEFT:
			case GLFW_KEY_A:
				if (camera_type % 3 == 0) {
					move('z', '-');
				}
				else if (camera_type % 3 == 1) {
					move('x', '+');
				}
				else {
					if (direction == 1) {
						move('z', '-');
						direction = 4;
					}
					else if (direction == 2) {
						move('x', '+');
						direction = 1;
					}
					else if (direction == 3) {
						move('z', '+');
						direction = 2;
					}
					else if (direction == 4) {
						move('x', '-');
						direction = 3;
					}
				}
				break;
			case GLFW_KEY_X:
				camera_scale -= 2;
				if (camera_scale < 6)
					camera_scale += 36;
				break;
			case GLFW_KEY_Z:
				camera_scale += 2;
				if (camera_scale > 44)
					camera_scale -= 36;
				break;
			case GLFW_KEY_C:
				camera_type++;
				printf("camera_type=%d", camera_type);
				break;
				//
			case GLFW_KEY_L:
				ambientLight += 0.1;
				glUniform3fv(AmbientLightID, 1, &ambientLight[0]);
				break;
			case GLFW_KEY_K:
				ambientLight -= 0.1;
				glUniform3fv(AmbientLightID, 1, &ambientLight[0]);
				break;

			default:
				break;
			}
		}
	}
}

void Renderer::HandleMouse(double deltaX, double deltaY)
{
	//	printf("Renderer::MOUSE = %f , %f \n",deltaX,deltaY);
	myCamera->Yaw(deltaX);
	myCamera->Pitch(deltaY);
	myCamera->UpdateViewMatrix();

}


void Renderer::createcube(std::unique_ptr<Model> &cube)
{
	cube = std::unique_ptr<Model>(new Model());

	// back    (0,1,2,3)
	cube->VertexData.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	cube->UVData.push_back(glm::vec2(0.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(0.0f, 1.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 1.0f));

	glm::vec3 CubeNormal = glm::vec3(0.0, 0.0, -1.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	//first triangle.
	cube->IndicesData.push_back(0);
	cube->IndicesData.push_back(1);
	cube->IndicesData.push_back(3);

	//second triangle.
	cube->IndicesData.push_back(0);
	cube->IndicesData.push_back(2);
	cube->IndicesData.push_back(3);

	// front    (4,5,6,7)
	cube->VertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	cube->UVData.push_back(glm::vec2(0.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(0.0f, 1.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 1.0f));

	CubeNormal = glm::vec3(0.0, 0.0, 1.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	//first triangle.
	cube->IndicesData.push_back(4);
	cube->IndicesData.push_back(5);
	cube->IndicesData.push_back(7);

	//second triangle.
	cube->IndicesData.push_back(4);
	cube->IndicesData.push_back(6);
	cube->IndicesData.push_back(7);

	// right    (8,9,10,11)
	cube->VertexData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	cube->UVData.push_back(glm::vec2(0.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(0.0f, 1.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 1.0f));

	CubeNormal = glm::vec3(-1.0, 0.0, 0.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	//first triangle.
	cube->IndicesData.push_back(8);
	cube->IndicesData.push_back(9);
	cube->IndicesData.push_back(11);

	//second triangle.
	cube->IndicesData.push_back(8);
	cube->IndicesData.push_back(10);
	cube->IndicesData.push_back(11);

	// left    (12,13,14,15)
	cube->VertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 1.0f));

	cube->UVData.push_back(glm::vec2(0.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(0.0f, 1.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 1.0f));

	CubeNormal = glm::vec3(1.0, 0.0, 0.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	//first triangle.
	cube->IndicesData.push_back(12);
	cube->IndicesData.push_back(13);
	cube->IndicesData.push_back(15);

	//second triangle.
	cube->IndicesData.push_back(12);
	cube->IndicesData.push_back(14);
	cube->IndicesData.push_back(15);

	CubeNormal = glm::vec3(1.0, 0.0, 0.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	// up    (16,17,18,19)
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	cube->VertexData.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
	cube->VertexData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	cube->UVData.push_back(glm::vec2(0.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 0.0f));
	cube->UVData.push_back(glm::vec2(0.0f, 1.0f));
	cube->UVData.push_back(glm::vec2(1.0f, 1.0f));

	CubeNormal = glm::vec3(0.0, -1.0, 0.0);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);
	cube->NormalsData.push_back(CubeNormal);

	//first triangle.
	cube->IndicesData.push_back(16);
	cube->IndicesData.push_back(17);
	cube->IndicesData.push_back(19);

	//second triangle.
	cube->IndicesData.push_back(16);
	cube->IndicesData.push_back(18);
	cube->IndicesData.push_back(19);

	cube->Initialize();
}

void Renderer::initTextcure()
{
	/////////////////////////////////////////////////////////////////////// 
	// loading and initalizing textures.
	if (Level == 0 || Level == 4) {
		mTexture2 = std::unique_ptr<Texture>(new Texture("start.jpg", 0));
		mTexture3 = std::unique_ptr<Texture>(new Texture("1.jpg", 1));
		mTexture4 = std::unique_ptr<Texture>(new Texture("end.jpg", 2));

	}
	else if (Level==1){
		mTexture1 = std::unique_ptr<Texture>(new Texture("3.jpg", 0));
		mTexture2 = std::unique_ptr<Texture>(new Texture("2.jpg", 1));
		mTexture3 = std::unique_ptr<Texture>(new Texture("1.jpg", 2));
		mTexture4 = std::unique_ptr<Texture>(new Texture("fire.jpg", 3));
		mTexture5 = std::unique_ptr<Texture>(new Texture("bfire.jpg", 4));
		mTexture6 = std::unique_ptr<Texture>(new Texture("gold_tex.jpg", 5));
		mTexture7 = std::unique_ptr<Texture>(new Texture("final.jpg", 6));
	}
	else if(Level==2){
		mTexture1 = std::unique_ptr<Texture>(new Texture("wood1.jpg", 0));
		mTexture2 = std::unique_ptr<Texture>(new Texture("abstract.jpg", 1));
		mTexture3 = std::unique_ptr<Texture>(new Texture("sky.jpg", 2));
		mTexture4 = std::unique_ptr<Texture>(new Texture("fire.jpg", 3));
		mTexture5 = std::unique_ptr<Texture>(new Texture("bfire.jpg", 4));
		mTexture6 = std::unique_ptr<Texture>(new Texture("gold_tex.jpg", 5));
		mTexture7 = std::unique_ptr<Texture>(new Texture("final.jpg", 6));
	}
	else if (Level == 3) {
		mTexture1 = std::unique_ptr<Texture>(new Texture("colorful t.jpg", 0));
		mTexture2 = std::unique_ptr<Texture>(new Texture("colorful t.jpg", 1));
		mTexture3 = std::unique_ptr<Texture>(new Texture("g5.jpg", 2));
		mTexture4 = std::unique_ptr<Texture>(new Texture("fire.jpg", 3));
		mTexture5 = std::unique_ptr<Texture>(new Texture("bfire.jpg", 4));
		mTexture6 = std::unique_ptr<Texture>(new Texture("color.jpg", 5));
		mTexture7 = std::unique_ptr<Texture>(new Texture("final.jpg", 6));
	}
	//////////////////////////////////////////////////////////////////////////
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Use our shader
	glUseProgram(programID);

	LightPositionID = glGetUniformLocation(programID, "LightPosition_worldspace");
	AmbientLightID = glGetUniformLocation(programID, "ambientLight");
	DissfuseLightID = glGetUniformLocation(programID, "diffuseLight");
	SpecularLightID = glGetUniformLocation(programID, "specularLight");
	EyePositionID = glGetUniformLocation(programID, "EyePosition_worldspace");
	//Step 2 => Initilization
	lightPosition = glm::vec3(10.0f, 10.0f, 10.0f);
	ambientLight = glm::vec3(1, 1, 1);
	Diffuselight = glm::vec3(0, 1, 0);
	specularlight = glm::vec3(1, 1, 1);
	//Step 3 => send values to shader
	glUseProgram(programID);
	glUniform3fv(LightPositionID, 1, &lightPosition[0]);
	glUniform3fv(AmbientLightID, 1, &ambientLight[0]);
	glUniform3fv(DissfuseLightID, 1, &Diffuselight[0]);
	glUniform3fv(SpecularLightID, 1, &specularlight[0]);

	//////////////////////////////////////////////////////////////////////////

	glUniform3fv(EyePositionID, 1, &myCamera->GetEyePosition()[0]);

}

void Renderer::initCameraPosition()
{
	//projection matrix.
	myCamera->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	// View matrix : 
	myCamera->Reset(0.0, 40.0, 15.0,
		10, 1, 15,
		0, 2, 0);

}

void Renderer::Cleanup()
{
	glDeleteProgram(programID);
}

void Renderer::Initializemaze()
{

	//initialize 2D Array with 0.
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++)
		{
			maze[i][j] = 0;
		}
	}
	//Left Wall.
	for (int i = 3; i < 20; i++)
		maze[i][0] = 1;

	//Right Wall.
	for (int i = 0; i < 16; i++)
		maze[i][29] = 1;

	//Up And Down Wall.
	for (int j = 0; j < 30; j++) {
		maze[0][j] = 1;
		maze[19][j] = 1;
	}

	//Level 1 Maze.
	if (Level == 1) {
		for (int i = 0; i < 15; i++)
			maze[i][5] = 1;

		for (int i = 5; i < 19; i++)
			maze[i][10] = 1;

		for (int i = 0; i < 14; i++)
			maze[i][15] = 1;

		for (int i = 7; i < 19; i++)
			maze[i][21] = 1;

		//Golds
		for (int i = 1; i < 19; i += 3) {
			for (int j = 3; j < 27; j += 5) {
				if (maze[i][j] != 1)
				{
					maze[i][j] = 2;
				}
			}
		}

	}
	//Level 2 Maze.
	else if (Level == 2) {
		for (int i = 0; i < 20; i++) {
			maze[i][5] = 1;
			maze[i][10] = 1;
			maze[i][15] = 1;
			maze[i][21] = 1;
		}
		maze[9][5] = 0; maze[10][5] = 0; maze[11][5] = 0;
		maze[1][10] = 0; maze[2][10] = 0;
		maze[15][15] = 0; maze[16][15] = 0; maze[17][15] = 0; maze[18][15] = 0;
		maze[1][21] = 0; maze[2][21] = 0; maze[3][21] = 0; maze[4][21] = 0; maze[5][21] = 0;


		//Golds
		for (int i = 1; i < 19; i += 3) {
			for (int j = 3; j < 27; j += 5) {
				if (maze[i][j] != 1)
				{
					maze[i][j] = 2;
				}
			}
		}
	}
	//Level 3 Maze.
	else if (Level == 3) {
		for (int i = 5; i < 15; i++) {
			maze[i][4] = 1;
		}
		for (int i = 4; i < 11; i++) {
			maze[15][i] = 1;
		}
		for (int i = 15; i < 20; i++) {
			maze[i][11] = 1;
		}
		for (int i = 7; i <11; i++) {
			maze[3][i] = 1;
		}


		for (int i = 11; i <17; i++) {
			maze[11][i] = 1;
		}
		for (int i = 1; i <11; i++) {
			maze[i][11] = 1;
		}
		for (int i = 9; i <20; i++) {
			maze[i][21] = 1;
		}
		for (int i = 4; i <9; i++) {
			maze[i][17] = 1;
		}
		for (int i = 17; i <22; i++) {
			maze[8][i] = 1;
		}
		maze[6][25] = 1; maze[6][26] = 1; maze[6][27] = 1; maze[6][28] = 1;


		//Golds
		for (int i = 1; i < 19; i += 3) {
			for (int j = 2; j < 27; j += 5) {
				if (maze[i][j] != 1)
				{
					maze[i][j] = 2;
				}
			}
		}
	}

}
