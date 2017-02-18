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

vector<vec3> controlPoints;
vector<Vertex> vertex;

vec3 sample[(int)SLICES];

Mesh mesh;





void glutDisplay()
{
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

  if (!complete)
  {

    glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 900.0, 0.0, 600.0, -900.0, 900.0);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (int inc = 0; inc < ::count; inc++)
    {
      drawCircle(inc, controlPoints[inc].x, controlPoints[inc].y, 6);
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
  if(complete)
  {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

    glMatrixMode( GL_MODELVIEW );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-900.0, 900.0, -200.0, 700.0, -800.0, 1200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(eyeX, eyeY, eyeZ, pickObjX, pickObjY, pickObjZ, 0,1,0);
    if(unordered_points)
    {
      vector<double> temp;
      mesh.clearPoints();
      for (int i = 0; i < 360; i += 5)
      {
        glPushMatrix();
        glRotated(i, 0.0, 1.0, 0.0);
        float mv[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mv);
        for (int j = 0; j < slices + 1; j++)
        {
          float xp = mv[0] * sample[j].x + mv[4] * sample[j].y + mv[8] * sample[j].z + mv[12];
          float yp = mv[1] * sample[j].x + mv[5] * sample[j].y + mv[9] * sample[j].z + mv[13];
          float zp = mv[2] * sample[j].x + mv[6] * sample[j].y + mv[10] * sample[j].z + mv[14];
          float wp = mv[3] * sample[j].x + mv[7] * sample[j].y + mv[11] * sample[j].z + mv[15];
          wp *= wp;

          xp /= wp;
          yp /= wp;
          zp /= wp;
          temp.push_back(xp);
          temp.push_back(yp);
          temp.push_back(zp);
          mesh.addPoint(temp);
          temp.pop_back();
          temp.pop_back();
          temp.pop_back();
        }
        glPopMatrix();
      }
      mesh.setPoints();
      mesh.computeFaces();
      mesh.setFaces();
      mesh.calcEdge();
    }
    mesh.drawFaces();
  }
  glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
  if(key==27)
    exit(0);
  if(key=='D' || key=='d')
    del = 1;
}

void mouse(int button, int state, int x, int y )
{
//  código para rotação da view
  started=0;
  if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && complete )
  {
    started=1;

    oldX = x;
    oldY = y;

    glutPostRedisplay();
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !complete)
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

  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !complete)
  {
    if (picked)
    {
      controlPoints[pickIndex].x = x,controlPoints[pickIndex].y = HEIGHT - y;
      picked = 0;
    }
  }

  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
      complete = 1;
    }
  glutPostRedisplay();
}

void activeMotion(int x, int y)
{
  if (picked)
  {
    controlPoints[pickIndex].x = x, controlPoints[pickIndex].y = HEIGHT - y;
  }

  ///rotação da camera
  if (complete)
  {
    theta += (x-oldX)*0.01f;
    phi   += (y-oldY)*0.01f;

    eyeX = pickObjX + radius*cos(phi)*sin(theta);
    eyeY = pickObjY + radius*sin(phi)*sin(theta);
    eyeZ = pickObjZ + radius*cos(theta);

    oldX = x;
    oldY = y;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  GLuint VertexArrayID;
  GLuint shaders[2];
  GLuint pointsBuffer;
  if(!USE_GLFW)
  {
    initGLFW();
    initShaders(&VertexArrayID, shaders);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, positionCallback);

    glGenBuffers(1, &pointsBuffer);

    do
    {
      glClear( GL_COLOR_BUFFER_BIT );
      if(!complete)
        glfwDrawCurve(shaders[0],pointsBuffer);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
  }
  else
  {
    glutInit(&argc, argv);

    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(25, 25);
    glutCreateWindow("Surface of Revolution");
    glViewport(0, 0, 900, 600);

    glutMouseFunc(mouse);
    glutDisplayFunc(glutDisplay);
    //glutIdleFunc(glutDisplay);
    glutMotionFunc(activeMotion);
    glutKeyboardFunc(processNormalKeys);
    glutIgnoreKeyRepeat(1);

      // set up lighting
      glShadeModel( GL_SMOOTH );
      glEnable( GL_COLOR_MATERIAL );
      glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
      glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
      glEnable( GL_LIGHTING );

    glEnable( GL_LIGHT0 );
    GLfloat position[] = { 0, 0, 1, 0 };
    glLightfv( GL_LIGHT0, GL_POSITION, position );

    glutMainLoop();
  }
  glDeleteBuffers(1, &pointsBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(shaders[0]);
	glDeleteProgram(shaders[1]);
  return 0;
}
