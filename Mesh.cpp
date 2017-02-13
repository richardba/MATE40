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
  int i;
  if(unordered_points)
  {
    for (i = 0; i < points.size() - SLICES - 1; i++)
    {
      if ((i + 1) % (SLICES + 1) != 0)
      {
        //UPPER TRIANGLE
        const vec3 UL0 = vec3(points[i][0], points[i][1], points[i][2]);
        const vec3 UR0 = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
        const vec3 LL0 = vec3(points[i+ SLICES+1][0], points[i+SLICES+1][1], points[i+SLICES+1][2]);

        //LOWER TRIANGLE
        const vec3 UR1 = vec3(points[i+1][0], points[i+1][1], points[i+1][2]);
        const vec3 LL1 = vec3(points[i + SLICES+1][0], points[i + SLICES+1][1], points[i + SLICES+1][2]);
        const vec3 LR1 = vec3(points[i+ SLICES+2][0], points[i+SLICES+2][1], points[i+SLICES+2][2]);

        vec3 normal0, normal1;
        if(i<=(points.size()/2))
        {
          normal0 = normalize(cross(UL0 - UR0, LL0 - UR0));
          normal1 = normalize( cross( LR1 - LL1, UR1 - LL1 ) );
        } else
        {
          normal0 = normalize( cross( UR0 - LL0, UL0 - LL0 ) );
          normal1 = normalize( cross( LR1 - LL1, UR1 - LL1 ) );
        }

        vertex.push_back( Vertex( LL0, normal0 ) );
        vertex.push_back( Vertex( UR0, normal0 ) );
        vertex.push_back( Vertex( UL0, normal0 ) );
        vertex.push_back( Vertex( LL1, normal1 ) );
        vertex.push_back( Vertex( LR1, normal1 ) );
        vertex.push_back( Vertex( UR1, normal1 ) );
      }
    }

    for (; i < points.size() - 1; i++)
    {
      const vec3 UL0 = vec3(points[i][0], points[i][1], points[i][2]);
      const vec3 UR0 = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
      const vec3 LL0 = vec3(points[i+ SLICES+1 - points.size()][0], points[i+SLICES+1 - points.size()][1], points[i+SLICES+1 - points.size()][2]);


      const vec3 UR1 = vec3(points[i+1][0], points[i+1][1], points[i+1][2]);
      const vec3 LL1 = vec3(points[i + SLICES+1 - points.size()][0], points[i + SLICES+1 - points.size()][1], points[i + SLICES+1 - points.size()][2]);
      const vec3 LR1 = vec3(points[i+ SLICES+2 - points.size()][0], points[i+SLICES+2 - points.size()][1], points[i+SLICES+2 - points.size()][2]);

      vec3 normal0, normal1;
      if(i<=(points.size()/2))
      {
        normal0 = normalize( cross( UL0 - LL0, UR0 - LL0) );
        normal1 = normalize( cross( UR1 - LL1, LR1 - LL1) );
      } else
      {
        normal0 = normalize( cross( UR0 - LL0, UL0 - LL0 ) );
        normal1 = normalize( cross( LR1 - LL1, UR1 - LL1 ) );
      }

      vertex.push_back( Vertex( LL0, normal0 ) );
      vertex.push_back( Vertex( UR0, normal0 ) );
      vertex.push_back( Vertex( UL0, normal0 ) );
      vertex.push_back( Vertex( LL1, normal1 ) );
      vertex.push_back( Vertex( LR1, normal1 ) );
      vertex.push_back( Vertex( UR1, normal1 ) );

    }
    unordered_points=0;
  }
  glColor3ub(0.0, 128, 64);
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY );
  glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &vertex[0].position );
  glNormalPointer( GL_FLOAT, sizeof(Vertex), &vertex[0].normal );
  glDrawArrays( GL_TRIANGLES, 0, vertex.size() );
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );

}

Mesh::~Mesh()
{

}

Mesh::Mesh()
{

}
