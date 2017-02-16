#include "include/Utils.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

GLFWwindow* window;
const vec4 clearColor = vec4(0.5, 1.0, 1.0, 1.0),
           blackColor = vec4(0,0,0,1);

void boundingLimits(double args[], vec3 coord)
{
  if(coord.x>args[0])
  {
    args[0]=coord.x;
  }
  else if(coord.y>args[1])
  {
    args[1]=coord.y;
  }
  else if(coord.z>args[2])
  {
    args[2]=coord.z;
  }
  else if(coord.x<args[3])
  {
    args[3]=coord.x;
  }
  else if(coord.y<args[4])
  {
    args[4]=coord.y;
  }
  else if(coord.z<args[5])
  {
    args[5]=coord.z;
  }
}

void initGLFW()
{
  int tmp;
  if(!glfwInit())
	{
		cout << "Erro ao inicializar o GLFW!" << endl;
		cin >> tmp;
		exit(EXIT_FAILURE);
	}

  window = glfwCreateWindow( WIDTH, HEIGHT, "Surface of Revolution", NULL, NULL);

  glfwWindowHint(GLFW_SAMPLES, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin/glVertex/glEnd work

  if(window == NULL)
  {
    cout << "Falha ao inicializar o GLFW" << endl;
    cin >> tmp;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwPollEvents();
    glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

  glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
}

void initShaders(GLuint* VertexArrayID, GLuint shaders[])
{
  glewExperimental = true;
	if (glewInit() != GLEW_OK)
  {
		cout << "Erro ao inicializar o GLEW!" << endl;
		int tmp;
		cin >> tmp;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glGenVertexArrays(1, VertexArrayID);
	glBindVertexArray(*VertexArrayID);
}

void mouseCallback(GLFWwindow* win, int button, int action, int mods)
{
  double x,y;
  glfwGetCursorPos(window,&x,&y);

 /* if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && complete )
  {
    oldX = x;
    oldY = y;

    glutPostRedisplay();
  }*/

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !complete)
  {
    for (int inc = 0; inc < ::count; inc++)
    {
      if ((pow((double)(controlPoints[inc].x - x), 2)) + (pow((double)(controlPoints[inc].y - HEIGHT + y), 2)) <= pow((double)10, 2))
      {
        picked = 1;
        pickIndex = inc;
      }
    }

    if (!picked)
    {
      vec3 newp;
      newp.x = x;
      newp.y = HEIGHT-y, newp.z = 0;
      controlPoints.push_back(newp);
      cout<< newp.x <<endl;
      cout<< newp.y <<endl;
      cout<< newp.z <<endl;
      ::count++;
    }

    if(del && picked)
    {
      del = 0;
      controlPoints.erase(controlPoints.begin()+pickIndex);
      ::count--;
      picked = 0;
      pickIndex = -1;
      glutPostRedisplay();
    }
    if(!controlPoints.empty())
    {
      form = 1;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !complete)
  {
    if (picked)
    {
      controlPoints[pickIndex].x = x, controlPoints[pickIndex].y = HEIGHT - y;
      picked = 0;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  {
    complete = 1;
  }
}
