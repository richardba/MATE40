#include <GL/glut.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "include/utils.h"
#include "include/Mesh.h"

using namespace std;
using namespace glm;

bool complete = 0,
     started=0,
     unordered_points=1;

int picked = 0,
    btn,
    oldX=0,
    oldY=0,
    radius=200,
    pickIndex;

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

vec3 sample[SLICES + 1];

Mesh mesh;


void drawLine(vec3 a, vec3 b)
{
  glColor3f(0.0, 1.0, 1.0);

  glBegin(GL_LINES);
  glVertex3d(a.x, a.y, a.z);
  glVertex3d(b.x, b.y, b.z);
  glEnd();
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

vec3 interpolation(vec3 a, vec3 b, double t)
{
  vec3 c;
  c.x = (1 - t)*a.x + t*b.x;
  c.y = (1 - t)*a.y + t*b.y;
  c.z = (1 - t)*a.z + t*b.z;
  return c;
}

void drawBezier()
{
  for (int inc = 0; inc < SLICES; inc++)
  {
    drawLine(sample[inc], sample[inc + 1]);
  }
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

int count = 0;
int form = 0;
int del = 0;

void glutDisplay()
{
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glClearColor(0.0, 0.0, 0.0, 0.0);

  if (!complete)
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
  if(complete)
  {
    glClearColor(0.5, 1.0, 1.0, 0.0);

    glMatrixMode( GL_MODELVIEW );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-900.0, 900.0, -200.0, 700.0, -800.0, 1200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(eyeX, eyeY, eyeZ, pickObjX, pickObjY, pickObjZ, 0,1,0);

    vector<double> temp;
    mesh.clearPoints();
    for (int i = 0; i < 360; i += 5)
    {
      glPushMatrix();
      glRotated(i, 0.0, 1.0, 0.0);
      float mv[16];
      glGetFloatv(GL_MODELVIEW_MATRIX, mv);
      for (int j = 0; j < SLICES + 1; j++)
      {
        float xp = mv[0] * sample[j].x + mv[4] * sample[j].y + mv[8] * sample[j].z + mv[12];
        float yp = mv[1] * sample[j].x + mv[5] * sample[j].y + mv[9] * sample[j].z + mv[13];
        float zp = mv[2] * sample[j].x + mv[6] * sample[j].y + mv[10] * sample[j].z + mv[14];
        float wp = mv[3] * sample[j].x + mv[7] * sample[j].y + mv[11] * sample[j].z + mv[15];

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
    mesh.drawFaces();
  }
  glFlush();
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
  glutInit(&argc, argv);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
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
  return 0;
}
