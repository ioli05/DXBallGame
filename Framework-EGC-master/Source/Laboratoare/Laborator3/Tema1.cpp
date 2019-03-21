#include "Tema1.h"

#include <vector>
#include <iostream>

#include <sstream>  // for string streams 
#include <string>  // for string 

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <vector>
#include<math.h>
#include <Core/Engine.h>

using namespace std;

struct caramida{
	int posX, posY;
	glm::vec3 corner;
	bool show;
} ;
float rotateangle = 0; 
float speed = 5.0f;
bool mouse = false;
float tx; //position of the mouse
float positionXBall, positionYBall; //position of the ball
float positionXPlatform, positionYPlatform; //position of the platform
caramida arrayBricks[50]; //all the bricks
float resolutionY = 720, resolutionX = 1280; //resolution of the screen
bool flagDrawAgain = false; //flag used when a life is lost
bool flagPowerUp1 = false, flagPowerUp2 = false, flagPowerUp3 = false; //flags used for powerUps
int Life = 3; //Initial number of Lives
float deplasarePowerUp1 = 4, deplasarePowerUp2 = 4, deplasarePowerUp3 = 4; //Move of every PowerUp
float deplasareX = 0, deplasareY = -4; //Move of the Ball
float radius = 10; //Radius of the ball
Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	//Set left side red
	glm::vec3 corner = glm::vec3(0, 100, 0);

	float height = resolutionY;
	float width = 30 ;

	Mesh* rectangle1 = Object2D::CreateRectangle("rectangle1", corner, width, height, glm::vec3(1, 0, 0), true);
	AddMeshToList(rectangle1);

	//Set up side red
	corner = glm::vec3(0, resolutionY - 30, 0);

	width = resolutionX;
	height = 30;

	Mesh* rectangle2 = Object2D::CreateRectangle("rectangle2", corner, width, height, glm::vec3(1, 0, 0), true);
	AddMeshToList(rectangle2);

	//Set right side red
	corner = glm::vec3(resolutionX - 30, 100, 0);

	height = resolutionY;
	width = 30;

	Mesh* rectangle3 = Object2D::CreateRectangle("rectangle3", corner, width, height, glm::vec3(1, 0, 0), true);
	AddMeshToList(rectangle3);

	//Set the platform
	corner = glm::vec3(0,40,0); 

	height = 20;
	width = 200;

	Mesh* platform = Object2D::CreateRectangle("platform", corner, width, height, glm::vec3(0, 1, 0), true);
	AddMeshToList(platform);

	//Set the ball
	
	float radius = 10;

	corner = glm::vec3(100, 40 + radius, 0);

	Mesh* circle = Object2D::CreateCircle("circle", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(circle);

	//Set bricks
	int x = 245, y = 400; //Coordinates of first brick
	for (int i = 0; i < 50; i++) {
		arrayBricks[i].posX = x + (i % 10) * 80; 
		arrayBricks[i].posY = y + (i / 10) * 40;
		arrayBricks[i].show = true; //at first, every brick is shown on the screen

		//compute different names for every brick 
		ostringstream str1;
		str1 << i;
		string name = str1.str();
		corner = glm::vec3(arrayBricks[i].posX, arrayBricks[i].posY, 0);
		glm::vec3 color;
		//different color for those bricks who hide powerUps for an easy identification
		if (i == 7 || i == 21 || i == 36) {
			color = glm::vec3(0, 1, 1);
		}
		else {
			color = glm::vec3(1, 0, 0);
		}
		Mesh* caramida = Object2D::CreateRectangle(name, corner, 70, 30, color, true);
		AddMeshToList(caramida);
	}

	
	//Place Life1
	corner = glm::vec3(10, 20, 0);

	Mesh* life1 = Object2D::CreateCircle("life1", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(life1);

	//Place Life2
	corner = glm::vec3(50, 20, 0);

	Mesh* life2 = Object2D::CreateCircle("life2", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(life2);

	//Place Life3
	corner = glm::vec3(90, 20, 0);

	Mesh* life3 = Object2D::CreateCircle("life3", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(life3);

	//Life 4 and 5 are created and will be rendered only if the powerUp life will be activated
	//Place Life4
	corner = glm::vec3(130, 20, 0);

	Mesh* life4 = Object2D::CreateCircle("life4", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(life4);

	//Place Life5
	corner = glm::vec3(170, 20, 0);

	Mesh* life5 = Object2D::CreateCircle("life5", corner, radius, glm::vec3(1, 1, 0), true);
	AddMeshToList(life5);


	//Place PowerUp1
	corner = glm::vec3(0, 0, 0); 

	Mesh* powerUp1 = Object2D::CreateSquare("powerUp1", corner, 10, glm::vec3(0.6f, 0.4f, 0.8f), true);
	AddMeshToList(powerUp1);
	deplasarePowerUp1 = 410; //Y coordinate of the powerUp1

	//Place PoweUp2
	corner = glm::vec3(0, 0, 0); 

	Mesh* powerUp2 = Object2D::CreateSquare("powerUp2", corner, 10, glm::vec3(0.9f, 0.9f, 0.8f), true);
	AddMeshToList(powerUp2);
	deplasarePowerUp2 = 490; //Y coordinate of the powerUp2
	
	//PlacePowerUp3
	corner = glm::vec3(0, 0, 0); 

	Mesh* powerUp3 = Object2D::CreateSquare("powerUp3", corner, 10, glm::vec3(0.2f, 0.5f, 0.3f), true);
	AddMeshToList(powerUp3);
	deplasarePowerUp3 = 525; //Y coordinate of the powerUp3
}
void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}
void Tema1::Update(float deltaTimeSeconds)
{

	modelMatrix = glm::mat3(1);
	//Render left up and right side of the screen, the red part
	RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["rectangle3"], shaders["VertexColor"], modelMatrix);

	//Check wether or not one of the PowerUps was activated. If so, it will translate and
		//rotate until it hits the botttom of the screen
	if (flagPowerUp1 == true) { 
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(835, deplasarePowerUp1 -= 2);
		modelMatrix *= Transform2D::Rotate(rotateangle -= (deltaTimeSeconds));

		RenderMesh2D(meshes["powerUp1"], shaders["VertexColor"], modelMatrix); 
		if (deplasarePowerUp1 < 0) {
			flagPowerUp1 = false;
		}
	}
	if (flagPowerUp2 == true) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(345, deplasarePowerUp2 -= 2);
		modelMatrix *= Transform2D::Rotate(rotateangle -= (deltaTimeSeconds));

		RenderMesh2D(meshes["powerUp2"], shaders["VertexColor"], modelMatrix); 
		if (deplasarePowerUp2 < 0) {
			flagPowerUp2 = false;
		}
	}
	if (flagPowerUp3 == true) { 
		modelMatrix = glm::mat3(1);
		deplasarePowerUp2 -= 2;
		modelMatrix *= Transform2D::Translate(735, deplasarePowerUp2 -= 2);
		modelMatrix *= Transform2D::Rotate(rotateangle -= (deltaTimeSeconds));

		RenderMesh2D(meshes["powerUp3"], shaders["VertexColor"], modelMatrix); 
		if (deplasarePowerUp3 < 0) {
			flagPowerUp3 = false;
		}
	}

	
	//Draw all 50 bricks. If it is needed to draw the scene again because of a lost life, then all the
		//bricks will be showed on the screen
	modelMatrix = glm::mat3(1);
	for (int i = 0; i < 50; i++) {
		if (flagDrawAgain == true && arrayBricks[i].show == false) {
			arrayBricks[i].show = true;
		}
		if (arrayBricks[i].show == true) {
			ostringstream str1;
			str1 << i;
			string name = str1.str();
			RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
		}
	}
	//Draw on the left side of the screen all the lives that the player has 
	modelMatrix = glm::mat3(1);
	for (int i = 0; i <= Life; i++) {
		//Compute a different name for every life : example ("life2" for second life)
		string life = "life";
		ostringstream str1;
		str1 << life << i;
		string name = str1.str();
		string result = life + name;
		
		RenderMesh2D(meshes[name], shaders["VertexColor"], modelMatrix);
	}
	//Translate position of the platform according to the position of the mouse
	positionXPlatform = tx;
	modelMatrix *= Transform2D::Translate(tx, 20);
	RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);

	//If the button is not pressed or the scene needed to be draw again the ball and
		//the platform will move according to the mouse position
	if (mouse == false || flagDrawAgain == true) {

		positionXBall = positionXPlatform;
		positionYBall = 50; //40 is the Y position of the platform and the ball will 
			//be 10 pixels highr than the platform

		if (Life != 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(positionXPlatform, positionYBall);
			RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
		}

		if (Life == 0) {
			//The player will no longer be able to press the mouse and start the game
				//with a new ball because he has no more lives
			mouse = false;
		}
	}

	if (mouse == true) {
		flagDrawAgain = false;

		if ((positionYBall + deplasareY) < (40 - radius)) {
			Life--;
			flagDrawAgain = true;
			mouse = false;
		}
		else
		if (((positionXBall + radius) > (positionXPlatform - 100)) && ((positionXBall + radius) < (positionXPlatform + 100)) && ((positionYBall + deplasareY) < 40)
			&& (positionYBall + deplasareY > (40 - radius))) {
			//100 is coming from the middle of the X coordinate of the platform
			//40 is coming from the Y position of the platform and 30 is the left up position of Y platform - radius

			float deplasare = (positionXBall - (float)positionXPlatform) / 100; //100 = the middle of the width of the platform

			if (deplasare > 0.85f) { deplasare = 0.85f; } 
			if (deplasare < -0.85f) { deplasare = -0.85f; }

			//Right part of the platform => the ball will move on the right
			if (deplasare > 0) {
				deplasareY = (1 - deplasare) * speed;
				deplasareX = deplasare * speed;
			}
			//Left part of the platform => the ball will move on the left
			else if (deplasare < 0) {
				deplasareY = (1 - fabs(deplasare)) * speed;
				deplasareX = -fabs(deplasare) * speed;
			}
			//Middle => the ball will modify only its Y value
			else {
				deplasareY *= -1;
			}
		}
		else {
			//Collusion with the top wall
			if ((positionYBall + deplasareY) > ((resolutionY - 80))) {
				//80 = 30 the height of the wall, 10 the radius and 40 is the 
					//Y of the ball at the initial time
				deplasareY *= -1;
			}

			//Collusion with right wall
			if ((positionXBall + deplasareX) > resolutionX - 130) {
				//130 = 30 is the width of the wall and 100 is from the 
					//X of the ball at the initial time
				deplasareX *= -1;
			}

			//Collusion with right wall
			if ((positionXBall + deplasareX) < -70) {
				//-70 = 30 is the width of the wall and -100 is from the
					//X of the ball at the initial time
				deplasareX *= -1;
			}

			//Collusion with bricks
			
			//135 is the X position of the first brick - 100 the initial X position of the ball and 945 is the position of the last brick - 100
			//340, 380, 420, 460, 500 are the Y positions of the ball where it will detect the brick

			if (positionXBall >= 135 && positionXBall <= 955) {
				int x = -1;
				x = CheckCollusionBrick(positionXBall, positionYBall, deplasareY, deplasareX);
				if (x != -1 && arrayBricks[x].show == true) {
					CheckPowerUp(x);
					deplasareY *= -1;
					arrayBricks[x].show = false;
				}
			}
		}
		positionXBall += deplasareX;
		positionYBall += deplasareY;

		modelMatrix = glm::mat3(1);
		
		modelMatrix *= Transform2D::Translate(positionXBall, positionYBall);
		RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
	}
}
void Tema1::CheckPowerUp(int x) {
	//If the brick hit is one that contains a powerUp then the flag for that powerUp is activated
	if (x == 7) { flagPowerUp1 = true;  Life++; }
	if (x == 21) { flagPowerUp2 = true;  Life++; }
	if (x == 36) {
		flagPowerUp3 = true;
		for (int i = 0; i < 10; i++) {
			arrayBricks[i + 30].show = false;
		}
	}
}
int Tema1::CheckCollusionBrick(int xBall, int yBall, float deplasareY, float deplasareX) {
	//It depends on the sign of the move because the space between the bricks is 10, 
		//and the radius of the ball is also 10. By checking the sign we assure that the brick will
		//dissapear when it will be hit and not when the center of the bal hits the brick
	int x = -1, y = -1;
	if (deplasareY > 0) {
		if (yBall >= 340 && yBall < 380) { y = 0; }
		if (yBall >= 380 && yBall < 420) { y = 1; }
		if (yBall >= 420 && yBall < 460) { y = 2; }
		if (yBall >= 460 && yBall < 500) { y = 3; }
		if (yBall >= 500 && yBall < 540) { y = 4; }
	}
	else {
		if (yBall >= 350 && yBall < 390) { y = 0; }
		if (yBall >= 390 && yBall < 430) { y = 1; }
		if (yBall >= 430 && yBall < 470) { y = 2; }
		if (yBall >= 470 && yBall < 510) { y = 3; }
		if (yBall >= 510 && yBall < 550) { y = 4; }
	}
	if (deplasareX > 0) {
		if (xBall >= 135 && xBall < 225) { x = 0; }
		if (xBall >= 225 && xBall < 305) { x = 1; }
		if (xBall >= 305 && xBall < 385) { x = 2; }
		if (xBall >= 385 && xBall < 465) { x = 3; }
		if (xBall >= 465 && xBall < 545) { x = 4; }
		if (xBall >= 545 && xBall < 625) { x = 5; }
		if (xBall >= 625 && xBall < 705) { x = 6; }
		if (xBall >= 705 && xBall < 785) { x = 7; }
		if (xBall >= 785 && xBall < 865) { x = 8; }
		if (xBall >= 865 && xBall <= 945) { x = 9; }
	}
	else {
		if (xBall >= 145 && xBall < 235) { x = 0; }
		if (xBall >= 235 && xBall < 315) { x = 1; }
		if (xBall >= 315 && xBall < 395) { x = 2; }
		if (xBall >= 395 && xBall < 475) { x = 3; }
		if (xBall >= 475 && xBall < 555) { x = 4; }
		if (xBall >= 555 && xBall < 635) { x = 5; }
		if (xBall >= 635 && xBall < 715) { x = 6; }
		if (xBall >= 715 && xBall < 795) { x = 7; }
		if (xBall >= 795 && xBall < 875) { x = 8; }
		if (xBall >= 875 && xBall <= 955) { x = 9; }
	}

	if (x == -1 || y == -1) { return -1; }
	return x + y * 10;
	
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	tx = mouseX;
	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	//press button => calculate the direction
	mouse = true;

}