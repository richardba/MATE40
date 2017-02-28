#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "include/Utils.h"
#include "include/Mesh.h"

using namespace std;
using namespace glm;

bool complete = 0,
     started=0,
     unordered_points=1,
     del = 0;

GLuint picked,
       btn,
       oldX=0,
       oldY=0,
       radius=200,
       pickIndex,
       count = 0,
       form = 0;

const float slices=SLICES;

double eyeX=0,
       eyeY=4,
       eyeZ=25,
       pickObjX=0,
       pickObjY=5,
       pickObjZ=0,
       theta=0,
       phi=0;

vector<vec3> controlPoints, sample;
vector<Vertex> vertex;

Mesh mesh;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		cout << "Erro ao inicializar o GLFW!" << endl;
		cin >> form;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Open a window and create its OpenGL context
	window = glfwCreateWindow( WIDTH, HEIGHT, "Surface of Revolution", NULL, NULL);
	if( window == NULL ){
		cout << "Erro ao inicializar o GLFW!" << endl;
		cin >> form;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouseCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetCursorPosCallback(window, positionCallback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
  {
		cout << "Erro ao inicializar o GLEW!" << endl;
		cin >> form;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint shaders[] = {LoadShaders( "2d.vert", "2d.frag" ), LoadShaders( "3d.vert", "3d.frag" )};
  GLuint pointsBuffer,
         lineBuffer,
         surfaceBuffer;
  glGenBuffers(1, &pointsBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3), &controlPoints[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &lineBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, lineBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3), &sample[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &surfaceBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, surfaceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &vertex[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint color = glGetUniformLocation(shaders[0], "elementColor");

	do
  {
    glClear( GL_COLOR_BUFFER_BIT );
    if(!complete && controlPoints.size())
    {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_PROGRAM_POINT_SIZE);
      glPointSize(7);
      draw(GL_TYPE_3D, pointsBuffer, color, GL_POINTS, shaders[0], currentColor, controlPoints);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_PROGRAM_POINT_SIZE);

      draw(GL_TYPE_3D, lineBuffer, color, GL_LINE_STRIP, shaders[0], vec3(1), sample);
    } else if(complete)
    {
      computeMatricesFromInputs();
      mat4 ProjectionMatrix = getProjectionMatrix();
      mat4 ViewMatrix = getViewMatrix();
      mat4 ModelMatrix = glm::mat4(1.0);
      mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
      mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
      mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
      drawSurface(surfaceBuffer,
          color,
          shaders[0],
          vec3(0),
          vertex);
    }


		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
  vertex.clear();

	// Cleanup VBO
	glDeleteBuffers(1, &pointsBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shaders[0]);
	glDeleteProgram(shaders[1]);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

