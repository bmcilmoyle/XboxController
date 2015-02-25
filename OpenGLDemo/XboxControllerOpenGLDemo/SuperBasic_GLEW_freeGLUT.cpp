// Xbox controller demo
// Author: Ben McIlmoyle

//Copyright (c) 2015 Ben McIlmoyle
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#include "Utils.h"
#include "CTextureLoaderManager.h"
#include "CGameControllerManager.h"
#include "CGameObject.h"
#include "CTriangle.h" 
#include "CHRTimer.h"

#include <glm/glm.hpp>

#include <iostream>	
#include <sstream>	
#include <map>
#include <vector>


std::vector< CGameObject > g_models;

struct VERTEX
{
	VERTEX(): x(0.0f), y(0.0f), z(0.0f), 
		nx(0.0f), ny(0.0f), nz(0.0f), 
		r(0.0f),  g(0.0f),  b(0.0f), 
		textU(0.0f), textV(0.0f){}

	float x,y,z; //Position
	float nx,ny,nz; //Normals
	float r,g,b; //Colour
	float textU, textV; //Texture UV position
};

std::vector< VERTEX >   g_vecVertices;
std::vector< CTriangle > g_vecTriangles;
const int g_numberOfVertices = 4;
const int g_numberOfTriangles = 2;

void CreateSquare(void);

int CurrentWidth = 800;
int CurrentHeight = 600;
int WindowHandle = 0;

//Create a high resolution timer to get time between updates.
//This is used to update the controller for functions like vibrate.
CHRTimer timer;	

//Controller
CGameControllerManager*		p_GameControllerManager = 0;
CXBoxController*			p_GameController_0 = 0;

// We need to know where the variables are inside the shader.
// These numbers indicate the "slot" (set of 4 floats) location
// You can think of these like the columns in a database table;
//   imagine that we couldn't use the +name+ of the column, but
//   had to use the column number. 
GLuint ProjectionMatrixUniformLocation = 0;
GLuint ViewMatrixUniformLocation = 0;
GLuint ModelMatrixUniformLocation = 0;

// These buffers are where we are placing the vertex and index information
// Note that these aren't the actual buffers, but the "names" of the buffers.
// (in OpenGL, a "name" is always an int, not a string)
// (The {0} is a simple way to clear static array to all zeros. It's a "C" thing, not OpenGL.)
GLuint BufferIds[3] = { 0 };

// Same sort of thing as the "name" of the buffer, but these are the 
// "names" of the shaders we are loading
GLuint ShaderIds[3] = { 0 };

// These "Matrix" variables are defined in the Utils file.
// But they are really just an array of floats. Check it out.
Matrix ProjectionMatrix;
Matrix ViewMatrix;
Matrix ModelMatrix;

float CubeRotation = 0;
clock_t LastTime = 0;

// The freeGLUT "callback" function definitions.
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void DeleteShaders(void);
void keyboardFunctionCallback(unsigned char key, int x, int y);

// These are used to init everything
bool InitWindow(int, char*[]);
bool Initialize(int, char*[]);
bool LoadModelsIntoBuffers(void);
void LoadShaders(void);
void Draw(void);



int main(int argc, char* argv[])
{
	//Set up controller manager
	p_GameControllerManager = CGameControllerManager::getGameControllerManager();
	p_GameController_0 = p_GameControllerManager->getController(0);
	p_GameController_0->setDeadZoneCalculation(p_GameController_0->NO_DEAD_ZONE);

	//Controller will vibrate when program starts to confirm game works
	if ( !(( p_GameController_0 != 0 ) && ( p_GameController_0->bIsConnected() )) )
	{
		std::cout << "Didn't get an ID for the game controller; is there one plugged in?" << std::endl;
		return 1;
	}

	//Init the window
	Initialize(argc, argv);

	//Load the shaders
	LoadShaders();

	//Create a square / quad to place texture on
	CreateSquare();

	//Load verticies and indicies
	if ( !LoadModelsIntoBuffers() ) 
	{
		return false;	// Error
	}

	//Load Textures
	std::map<std::string, int> g_TexturesMap;
	g_TexturesMap["arrow.bmp"] = 0;
	g_TexturesMap["back.bmp"] = 0;
	g_TexturesMap["bumper.bmp"] = 0;
	g_TexturesMap["buttonA.bmp"] = 0;
	g_TexturesMap["buttonB.bmp"] = 0;
	g_TexturesMap["buttonX.bmp"] = 0;
	g_TexturesMap["buttonY.bmp"] = 0;
	g_TexturesMap["controller.bmp"] = 0;
	g_TexturesMap["start.bmp"] = 0;
	g_TexturesMap["stick.bmp"] = 0;
	g_TexturesMap["triggerLeft.bmp"] = 0;
	g_TexturesMap["triggerRight.bmp"] = 0;
	CTextureLoaderManager*  g_pTextureLoaderManager = new CTextureLoaderManager();
	g_pTextureLoaderManager->LoadTextures(&g_TexturesMap);

	CGameObject mainController;
	mainController.scale = 1.5f;
	mainController.visible = true;
	mainController.textureID = 7;
	g_models.push_back( mainController );

	CGameObject leftStick;
	leftStick.scale = 0.18f;
	leftStick.z = 0.1f;
	leftStick.textureID = 9;
	g_models.push_back( leftStick );

	CGameObject rightStick;
	rightStick.scale = 0.18f;
	rightStick.z = 0.1f;
	rightStick.textureID = 9;
	g_models.push_back( rightStick );

	CGameObject buttonA;
	buttonA.scale = 0.085f;
	buttonA.z = 0.1f;
	buttonA.x = 0.515f;
	buttonA.y = 0.125f;
	buttonA.textureID = 3;
	g_models.push_back( buttonA );

	CGameObject buttonB;
	buttonB.scale = 0.085f;
	buttonB.z = 0.1f;
	buttonB.x = 0.655f;
	buttonB.y = 0.26f;
	buttonB.textureID = 4;
	g_models.push_back( buttonB );

	CGameObject buttonX;
	buttonX.scale = 0.085f;
	buttonX.z = 0.1f;
	buttonX.x = 0.375f;
	buttonX.y = 0.26f;
	buttonX.textureID = 5;
	g_models.push_back( buttonX );

	CGameObject buttonY;
	buttonY.scale = 0.085f;
	buttonY.z = 0.1f;
	buttonY.x = 0.515f;
	buttonY.y = 0.395f;
	buttonY.textureID = 6;
	g_models.push_back( buttonY );

	CGameObject buttonBack;
	buttonBack.scale = 0.055f;
	buttonBack.z = 0.1f;
	buttonBack.x = -0.142f;
	buttonBack.y = 0.265f;
	buttonBack.textureID = 1;
	g_models.push_back( buttonBack );

	CGameObject buttonStart;
	buttonStart.scale = 0.055f;
	buttonStart.z = 0.1f;
	buttonStart.x = 0.145f;
	buttonStart.y = 0.265f;
	buttonStart.textureID = 8;
	g_models.push_back( buttonStart );

	CGameObject leftTrigger;
	leftTrigger.scale = 0.2f;
	leftTrigger.z = 0.1f;
	leftTrigger.x = -1.3f;
	leftTrigger.y = 0.4f;
	leftTrigger.textureID = 10;
	g_models.push_back( leftTrigger );

	CGameObject rightTrigger;
	rightTrigger.scale = 0.2f;
	rightTrigger.z = 0.1f;
	rightTrigger.x = 1.3f;
	rightTrigger.y = 0.4f;
	rightTrigger.textureID = 11;
	g_models.push_back( rightTrigger );

	CGameObject leftDPad;
	leftDPad.scale = 0.03f;
	leftDPad.z = 0.1f;
	leftDPad.x = -0.355f;
	leftDPad.y = -0.058f;
	leftDPad.textureID = 0;
	g_models.push_back( leftDPad );

	CGameObject rightDPad;
	rightDPad.scale = 0.03f;
	rightDPad.z = 0.1f;
	rightDPad.x = -0.16f;
	rightDPad.y = -0.058f;
	rightDPad.textureID = 0;
	g_models.push_back( rightDPad );

	CGameObject upDPad;
	upDPad.scale = 0.03f;
	upDPad.z = 0.1f;
	upDPad.x = -0.258f;
	upDPad.y = 0.035f;
	upDPad.textureID = 0;
	g_models.push_back( upDPad );

	CGameObject downDPad;
	downDPad.scale = 0.03f;
	downDPad.z = 0.1f;
	downDPad.x = -0.258f;
	downDPad.y = -0.148f;
	downDPad.textureID = 0;
	g_models.push_back( downDPad );

	CGameObject leftBumper;
	leftBumper.scale = 0.07f;
	leftBumper.z = 0.1f;
	leftBumper.x = -0.55f;
	leftBumper.y = 0.75f;
	leftBumper.textureID = 2;
	g_models.push_back( leftBumper );

	CGameObject rightBumper;
	rightBumper.scale = 0.07f;
	rightBumper.z = 0.1f;
	rightBumper.x = 0.55f;
	rightBumper.y = 0.75f;
	rightBumper.textureID = 2;
	g_models.push_back( rightBumper );

	//Start the timer
	timer.Reset();
	timer.Start();

	glutMainLoop();

	exit(EXIT_SUCCESS);
}

void IdleFunction(void)
{
	glutPostRedisplay();

	//Get the time that has passd since the last call to the timer
	timer.Stop();
	float deltaTime = timer.GetElapsedSeconds();
	timer.Start();

	//Update the controller with the delta time
	p_GameController_0->Update(deltaTime);

	const float leftStickDefaultXAxisLocation = -0.504f;
	const float leftStickDefaultYAxisLocation = 0.255f;
	const float rightStickDefaultXAxisLocation = 0.265f;
	const float rightStickDefaultYAxisLocation = -0.044f;
	const float stickMaxDifference = 0.05f;

	float test = p_GameController_0->getLeftJoystickXAxis();

	//Move left stick
	::g_models[1].x = leftStickDefaultXAxisLocation + p_GameController_0->getLeftJoystickXAxis() * stickMaxDifference;
	::g_models[1].y = leftStickDefaultYAxisLocation + p_GameController_0->getLeftJoystickYAxis() * stickMaxDifference;

	//Left stick click
	if(p_GameController_0->buttonPressed(LEFTSTICKCLICK))
		::g_models[1].scale = 0.19f;
	else
		::g_models[1].scale = 0.18f;

	//Move right stick
	::g_models[2].x = rightStickDefaultXAxisLocation + p_GameController_0->getRightJoystickXAxis() * stickMaxDifference;
	::g_models[2].y = rightStickDefaultYAxisLocation + p_GameController_0->getRightJoystickYAxis() * stickMaxDifference;

	//Right stick click
	if(p_GameController_0->buttonPressed(RIGHTSTICKCLICK))
		::g_models[2].scale = 0.19f;
	else
		::g_models[2].scale = 0.18f;

	//A button
	if(p_GameController_0->buttonPressed(ABUTTON))
		::g_models[3].visible = true;
	else
		::g_models[3].visible = false;

	//B button
	if(p_GameController_0->buttonPressed(BBUTTON))
		::g_models[4].visible = true;
	else
		::g_models[4].visible = false;

	//X button
	if(p_GameController_0->buttonPressed(XBUTTON))
		::g_models[5].visible = true;
	else
		::g_models[5].visible = false;

	//Y button
	if(p_GameController_0->buttonPressed(YBUTTON))
		::g_models[6].visible = true;
	else
		::g_models[6].visible = false;

	//Back button
	if(p_GameController_0->buttonPressed(BACKBUTTON))
		::g_models[7].visible = true;
	else
		::g_models[7].visible = false;

	//Start button
	if(p_GameController_0->buttonPressed(STARTBUTTON))
		::g_models[8].visible = true;
	else
		::g_models[8].visible = false;

	//Left trigger
	if(p_GameController_0->buttonPressed(LEFTTRIGGER))
		::g_models[9].visible = true;
	else
		::g_models[9].visible = false;

	//Right trigger
	if(p_GameController_0->buttonPressed(RIGHTTRIGGER))
		::g_models[10].visible = true;
	else
		::g_models[10].visible = false;

	//Left Bumper
	if(p_GameController_0->buttonPressed(LEFTBUMPER))
		::g_models[15].visible = true;
	else
		::g_models[15].visible = false;

	//Right Bumper
	if(p_GameController_0->buttonPressed(RIGHTBUMPER))
		::g_models[16].visible = true;
	else
		::g_models[16].visible = false;


	//Dpad Left
	if(p_GameController_0->buttonPressed(DPADLEFT))
		::g_models[11].visible = true;
	else
		::g_models[11].visible = false;

	//Dpad Right
	if(p_GameController_0->buttonPressed(DPADRIGHT))
		::g_models[12].visible = true;
	else
		::g_models[12].visible = false;

	//Dpad Up
	if(p_GameController_0->buttonPressed(DPADUP))
		::g_models[13].visible = true;
	else
		::g_models[13].visible = false;

	//Dpad Down
	if(p_GameController_0->buttonPressed(DPADDOWN))
		::g_models[14].visible = true;
	else
		::g_models[14].visible = false;

	return;
}


bool Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	if (! InitWindow(argc, argv) )
	{
		return false;	// Error
	}	

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) 
	{
		std::cout << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glGetError();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	ModelMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;
	TranslateMatrix(&ViewMatrix, 0, 0, -2);

	return true;
}

bool InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow("CONTROLLER DEMO");

	if(WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	// Tells GLUT what functions to call... ("callback" functions)
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
	glutCloseFunc(DeleteShaders);

	glutKeyboardFunc( keyboardFunctionCallback );

	return true;
}

// Called when a key is pressed
void keyboardFunctionCallback(unsigned char key, int x, int y)
{
	switch( key )
	{
	case 'w': case 'W':
		::g_models[0].y += 0.005f;	// Up
		break;
	};

	return;
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	ProjectionMatrix =
		CreateProjectionMatrix(
		60,
		(float)CurrentWidth / CurrentHeight,
		1.0f,
		100.0f
		);

	glUseProgram(ShaderIds[0]);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m);
	glUseProgram(0);

	return;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the models to the screen
	Draw();

	glutSwapBuffers();

	return;
}

void TimerFunction(int Value)
{
	glutTimerFunc(250, TimerFunction, 1);

	return;
}

void LoadShaders(void)
{
	ShaderIds[0] = glCreateProgram();
	ExitOnGLError("ERROR: Could not create the shader program");
	{
		ShaderIds[1] = LoadShader("SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);
		ShaderIds[2] = LoadShader("SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
		glAttachShader(ShaderIds[0], ShaderIds[1]);
		glAttachShader(ShaderIds[0], ShaderIds[2]);
	}
	glLinkProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not link the shader program");

	return;
}


bool LoadModelsIntoBuffers()
{

	glm::vec4 p_arrayVertPositions[4];
	glm::vec4 p_arrayVertColours[4];
	glm::vec2 p_arrayVertTexUV[4];

	// Copy the data to this array
	for ( int vertIndex = 0; vertIndex != g_numberOfVertices; vertIndex++ )
	{
		//Set Vertex Position
		p_arrayVertPositions[vertIndex].x  = ::g_vecVertices[vertIndex].x;
		p_arrayVertPositions[vertIndex].y  = ::g_vecVertices[vertIndex].y;
		p_arrayVertPositions[vertIndex].z  = ::g_vecVertices[vertIndex].z;
		p_arrayVertPositions[vertIndex].w  = 1.0f;

		//Set colour per Vertex
		p_arrayVertColours[vertIndex].r = 1.0f;		// Red
		p_arrayVertColours[vertIndex].g = 1.0f;		// Green
		p_arrayVertColours[vertIndex].b = 1.0f;		// Blue
		p_arrayVertColours[vertIndex].a = 1.0f;		// Alpha

		//Set vertex UV
		p_arrayVertTexUV[vertIndex].x = ::g_vecVertices[vertIndex].textU;		// U
		p_arrayVertTexUV[vertIndex].y = ::g_vecVertices[vertIndex].textV;       // V
	}

	// Each triangle has three corners, so we need 3 X number of triangles == number of indices
	int numberOfIndices = ::g_numberOfTriangles * 3;
	GLuint* p_Indices = new GLuint[ numberOfIndices ];
	for ( int triIndex = 0; triIndex < g_numberOfTriangles; triIndex++ )	// Step by three
	{
		CTriangle tempTri = g_vecTriangles[triIndex];

		int index1 = (triIndex * 3);
		int index2 = (triIndex * 3) + 1;
		int index3 = (triIndex * 3) + 2;

		p_Indices[index1] = (GLuint)tempTri.V1;
		p_Indices[index2] = (GLuint)tempTri.V2;
		p_Indices[index3] = (GLuint)tempTri.V3;
	}


	glGenVertexArrays(1, &BufferIds[0]);
	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glGenBuffers(1, &BufferIds[1]);
	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

	// This does the actual copy of the date from the "client" (CPU side)
	//	to the GPU ("video card") side. 
	int totalBufferSize = sizeof(float) * 4 * g_numberOfVertices 				//POSITION
		+ sizeof(float) * 4 * g_numberOfVertices 				//COLOR
		+ sizeof(float)* 2 * g_numberOfVertices;				//UV


	// Don't copy the data right now, so pass NULL
	glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);

	unsigned int offsetToPositions = 0;
	unsigned int offsetToColours = offsetToPositions + sizeof(float) * 4 * g_numberOfVertices;
	unsigned int offsetToUV = offsetToColours*2;

	glBufferSubData( GL_ARRAY_BUFFER, offsetToPositions, sizeof(float) * 4 * g_numberOfVertices, p_arrayVertPositions );
	glBufferSubData( GL_ARRAY_BUFFER, offsetToColours, sizeof(float) * 4 * g_numberOfVertices, p_arrayVertColours );
	glBufferSubData( GL_ARRAY_BUFFER, offsetToUV, sizeof(float) * 2 * g_numberOfVertices, p_arrayVertTexUV );

	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToPositions ) );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToColours ) );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToUV ) );
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glEnableVertexAttribArray(0);			// Position
	glEnableVertexAttribArray(1);			// Colour
	glEnableVertexAttribArray(2);			// UV;	
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	//Create Index buffer
	glGenBuffers(1, &BufferIds[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	ExitOnGLError("ERROR: Could not set VAO attributes");

	int bytesInIndexArray = numberOfIndices * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, p_Indices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);

	return true;
}

void DeleteShaders()
{
	glDetachShader(ShaderIds[0], ShaderIds[1]);
	glDetachShader(ShaderIds[0], ShaderIds[2]);
	glDeleteShader(ShaderIds[1]);
	glDeleteShader(ShaderIds[2]);
	glDeleteProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not destroy the shaders");

	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}

void Draw(void)
{
	// *** START of DRAW THE OBJECT FROM THE BUFFER ****
	glPolygonMode( GL_FRONT_AND_BACK ,  GL_FILL );

	glUseProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not use the shader program");

	ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
	ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
	ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
	ExitOnGLError("ERROR: Could not get shader uniform locations");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	ExitOnGLError("ERROR: Could not get shader uniform locations");

	// Draw each model in the vector
	for ( int index = 0; index != ::g_models.size(); index++ )
	{
		//Don't draw invisible models
		if(g_models[index].visible == false)
			continue;

		GLuint text1ID = glGetUniformLocation( 1, "texture_0" );
		glUniform1i( text1ID, g_models[index].textureID );

		//Set the matrix needed for the model
		ModelMatrix = IDENTITY_MATRIX;

		ScaleMatrix( &ModelMatrix, g_models[index].scale, 
			g_models[index].scale, g_models[index].scale );

		TranslateMatrix( &ModelMatrix, g_models[index].x, 
			g_models[index].y, g_models[index].z );

		RotateAboutX( &ModelMatrix, g_models[index].rotX );
		RotateAboutY( &ModelMatrix, g_models[index].rotY );
		RotateAboutZ( &ModelMatrix, g_models[index].rotZ );

		glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m);
		glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m);
		ExitOnGLError("ERROR: Could not set the shader uniforms");

		glBindVertexArray(BufferIds[0]);
		ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

		glDrawElements(GL_TRIANGLES,  (g_numberOfTriangles * 3), GL_UNSIGNED_INT, (GLvoid*)0);
		ExitOnGLError("ERROR: Could not draw the cube");
	}

	return;
}


void CreateSquare(void)
{
	VERTEX bottomRight;
	VERTEX bottomLeft;
	VERTEX topRight;
	VERTEX topLeft;

	//Set positions and UV co ords
	bottomLeft.x = -1.0f;
	bottomLeft.y = -1.0f;

	bottomLeft.textU = 0.0f;
	bottomLeft.textV = 0.0f;

	bottomRight.x = 1.0f;
	bottomRight.y = -1.0f;

	bottomRight.textU = 1.0f;
	bottomRight.textV = 0.0f;

	topLeft.x = -1.0f;
	topLeft.y = 1.0f;

	topLeft.textU = 0.0f;
	topLeft.textV = 1.0f;

	topRight.x = 1.0f;
	topRight.y = 1.0f;

	topRight.textU = 1.0f;
	topRight.textV = 1.0f;

	g_vecVertices.push_back( topLeft );
	g_vecVertices.push_back( topRight );
	g_vecVertices.push_back( bottomLeft );
	g_vecVertices.push_back( bottomRight );

	CTriangle triangleOne;
	CTriangle triangleTwo;

	triangleOne.V1 = 2;
	triangleOne.V2 = 1;
	triangleOne.V3 = 0;

	triangleTwo.V1 = 2;
	triangleTwo.V2 = 3;
	triangleTwo.V3 = 1;

	g_vecTriangles.push_back( triangleOne );
	g_vecTriangles.push_back( triangleTwo );

	return;
}
