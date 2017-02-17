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
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
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

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      del = 1;
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
      if(controlPoints.size()>1)
      {
        controlPoints.erase(controlPoints.begin()+pickIndex);
        ::count--;
        picked = 0;
        pickIndex = -1;
      }
    }
    if(!controlPoints.empty())
    {
      form = 1;
    }
  }
//NAO ESQUECER
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !complete)
  {
    if (picked)
    {
      if(y<0)
        y=0;
      else if(y>600)
        y=600;
      if(x<0)
        x=0;
      else if(x>900)
      controlPoints[pickIndex].x = x, controlPoints[pickIndex].y = HEIGHT - y;
      picked = 0;
    }
  }
}

void positionCallback(GLFWwindow *window, double x, double y)
{
  if (picked)
  {
    cout << x << endl;
    controlPoints[pickIndex].x = x, controlPoints[pickIndex].y = HEIGHT - y;
  }
}

vec3 interpolation(vec3 a, vec3 b, double t)
{
  vec3 c;
  c.x = (1 - t)*a.x + t*b.x;
  c.y = (1 - t)*a.y + t*b.y;
  c.z = (1 - t)*a.z + t*b.z;
  return c;
}


vec3 calcCasteljau(double t, vector<vec3> points)
{
  if(points.size()==1)
    return points[0];
  else
  {
    vector<vec3> tmp;
    for(int inc=0; inc<points.size()-1; inc++)
      tmp.push_back(interpolation(points[inc], points[inc+1], t));
    return calcCasteljau(t,tmp);
  }
}

void computeBezier()
{
  double step = 1.0 / SLICES;
  int inc = 0;

  for (double t = 0; t <= 1 && inc <= SLICES; t += step, inc++)
  {
    sample[inc] = calcCasteljau(t,controlPoints);
  }
}

void drawCircle(double xc, double yc, double radius)
{
  glPushMatrix();

  glTranslated(xc, yc, 0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBegin(GL_POLYGON);
  int sides = (2 * M_PI * radius) / 0.01;
  for (double i = 0; i < 2 * M_PI; i += M_PI / sides)
    glVertex3d(cos(i) * radius, sin(i) * radius, 0.0);
  glEnd();

  glPopMatrix();
}

void drawLine(vec3 a, vec3 b)
{
  glColor3f(0.0, 1.0, 1.0);

  glBegin(GL_LINES);
  glVertex3d(a.x, a.y, a.z);
  glVertex3d(b.x, b.y, b.z);
  glEnd();
}


void drawBezier()
{
  for (int inc = 0; inc < SLICES; inc++)
  {
    drawLine(sample[inc], sample[inc + 1]);
  }
}

void glfwDraw()
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 900.0, 0.0, 600.0, -900.0, 900.0);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  for (int inc = 0; inc < ::count; inc++)
  {
    if((pickIndex == inc) && picked)
      glColor3f(0.0, 0.7, 0.0);
    else
      glColor3f(1.0, 0.0, 0.0);

    drawCircle(controlPoints[inc].x, controlPoints[inc].y, 6);
    glColor3f(0.0, 0.7, 0.0);
    glBegin(GL_POINTS);
    glVertex2d(controlPoints[inc].x, controlPoints[inc].y);
    glEnd();
  }

  if (form)
  {
    computeBezier();
    drawBezier();
  }
}




