#include "include/Mesh.h"
#include "include/Utils.h"
#include <iostream>
#include <climits>

using namespace std;
double maxCoords[]= {-THE_NUMBER , -THE_NUMBER, -THE_NUMBER}, minCoords[]= {THE_NUMBER, THE_NUMBER, THE_NUMBER};
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

  for (i = 0; i < points.size() - slices - 1; i++)
  {
    if ((i + 1) % ((int)slices + 1) != 0)
    {
      noFacePoints.push_back(4);
      face.push_back(i);
      face.push_back(i + 1);
      face.push_back(i + slices + 2);
      face.push_back(i + slices + 1);
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
    face.push_back(i + slices + 2 - points.size());
    face.push_back(i + slices + 1 - points.size());
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
    for (i = 0; i < points.size() - slices - 1; i++)
    {
      if ((i + 1) % ((int)slices + 1) != 0)
      {
        //UPPER TRIANGLE


        const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
        const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
        const vec3 LL = vec3(points[i+ slices+1][0], points[i+slices+1][1], points[i+slices+1][2]);
        const vec3 LR = vec3(points[i+ slices+2][0], points[i+slices+2][1], points[i+slices+2][2]);

        const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
        const vec3 normal1 = normalize(cross(LR - LL, UL - LL));

        vertex.push_back( Vertex( LL, normal0 ) );
        vertex.push_back( Vertex( UR, normal0 ) );
        vertex.push_back( Vertex( UL, normal0 ) );
        vertex.push_back( Vertex( LL, normal1 ) );
        vertex.push_back( Vertex( LR, normal1 ) );
        vertex.push_back( Vertex( UR, normal1 ) );

      }
    }

    for (; i < points.size() - 1; i++)
    {
      const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
      const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
      const vec3 LL = vec3(points[i+ slices+1 - points.size()][0], points[i+slices+1 - points.size()][1], points[i+slices+1 - points.size()][2]);
      const vec3 LR = vec3(points[i+ slices+2 - points.size()][0], points[i+slices+2 - points.size()][1], points[i+slices+2 - points.size()][2]);

      const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
      const vec3 normal1 = normalize(cross(LR - LL, UL - LL));

      vertex.push_back( Vertex( LL, normal0 ) );
      vertex.push_back( Vertex( UR, normal0 ) );
      vertex.push_back( Vertex( UL, normal0 ) );
      vertex.push_back( Vertex( LL, normal1 ) );
      vertex.push_back( Vertex( LR, normal1 ) );
      vertex.push_back( Vertex( UR, normal1 ) );
      boundingLimits(maxCoords,UR);
      boundingLimits(maxCoords,UL);
      boundingLimits(maxCoords,LL);
      boundingLimits(maxCoords,LR);

    }
//
//  cout << maxCoords[0] << "\n"
//  << maxCoords[1] << "\n"
//  << maxCoords[2] << "\n"
//  << maxCoords[3] << "\n"
//  << maxCoords[4] << "\n"
//  << maxCoords[5] << endl;
    points.clear();
    unordered_points=0;
  }


  glPolygonMode( GL_FRONT, GL_FILL );
  glPolygonMode( GL_BACK, GL_FILL );


  glColor3ub(0.0, 128, 64);
  glEnable(GL_DEPTH_TEST);
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
