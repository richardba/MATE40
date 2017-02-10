#include "include/Mesh.h"
#include "include/utils.h"
#include <iostream>

using namespace std;
void Mesh::clearPoints()
{
  points.clear();
}

void Mesh::addPoint(vector<double> vec)
{
  points.push_back(vec);
}

void Mesh::setPoints()
{
  pointsNo = points.size();
}

void Mesh::computeFaces()
{
  noFacePoints.clear();
  faces.clear();
  vector<int> face;
  int i;

  for (i = 0; i < points.size() - SLICES - 1; i++)
  {
    if ((i + 1) % (SLICES + 1) != 0)
    {
      noFacePoints.push_back(4);
      face.push_back(i);
      face.push_back(i + 1);
      face.push_back(i + SLICES + 2);
      face.push_back(i + SLICES + 1);
      faces.push_back(face);
      face.pop_back();
      face.pop_back();
      face.pop_back();
      face.pop_back();
    }
  }

  for (; i < points.size() - 1; i++)
  {
    noFacePoints.push_back(4);
    face.push_back(i);
    face.push_back(i + 1);
    face.push_back(i + SLICES + 2 - points.size());
    face.push_back(i + SLICES + 1 - points.size());
    faces.push_back(face);
    face.pop_back();
    face.pop_back();
    face.pop_back();
    face.pop_back();
  }
}

void Mesh::setFaces()
{
  facesNo = faces.size();
}

void Mesh::calcEdge()
{
  edgesNo = pointsNo + facesNo - 2;
}

void Mesh::drawFaces()
{
  vector<int> face;
  int i;
  for (i = 0; i < points.size() - SLICES - 1; i++)
  {
    if ((i + 1) % (SLICES + 1) != 0)
    {
      //std:cout << i/(points.size()-SLICES-2.0f) << endl;
      glColor3d(.0, .0, i/(points.size()-SLICES-2.0f));
      glPointSize(3);
      glBegin(GL_POINTS);
      glVertex3d(points[i+1][0], points[i+1][1], points[i+1][2]);
      glEnd();
      glColor3d(0.0, .5, .25);
      glBegin(GL_TRIANGLES);
      glVertex3d(points[i][0], points[i][1], points[i][2]);
      glVertex3d(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
      glVertex3d(points[i+ SLICES+1][0], points[i+SLICES+1][1], points[i+SLICES+1][2]);
      glEnd();
      glColor3d(.5, .25, 0.0);
      glBegin(GL_TRIANGLES);
      glVertex3d(points[i+1][0], points[i+1][1], points[i+1][2]);
      glVertex3d(points[i + SLICES+1][0], points[i + SLICES+1][1], points[i + SLICES+1][2]);
      glVertex3d(points[i+ SLICES+2][0], points[i+SLICES+2][1], points[i+SLICES+2][2]);
      glEnd();
    }
  }

  for (; i < points.size() - 1; i++)
  {
    glColor3d(0.0, .5, .25);
    glBegin(GL_LINES);
    glVertex3d(points[i][0], points[i][1], points[i][2]);
    glVertex3d(points[i + SLICES + 1 - points.size()][0], points[i + SLICES + 1 - points.size()][1], points[i + SLICES + 1 - points.size()][2]);
    glVertex3d(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
    glVertex3d(points[i + SLICES + 2 - points.size()][0], points[i + SLICES + 2 - points.size()][1], points[i + SLICES + 2 - points.size()][2]);
    glEnd();
    glColor3d(.5, .25, 0.0);
    glBegin(GL_LINES);
    glVertex3d(points[i][0], points[i][1], points[i][2]);
    glVertex3d(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
    glVertex3d(points[i + SLICES + 1 - points.size()][0], points[i + SLICES + 1 - points.size()][1], points[i + SLICES + 1 - points.size()][2]);
    glVertex3d(points[i + SLICES + 2 - points.size()][0], points[i + SLICES + 2 - points.size()][1], points[i + SLICES + 2 - points.size()][2]);
    glEnd();
  }
}

Mesh::~Mesh()
{

}

Mesh::Mesh()
{

}
