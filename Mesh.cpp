#include "include/Mesh.h"
#include "include/utils.h"
#include <iostream>
#include <climits>

using namespace std;
double maxX=LLONG_MIN , maxY=LLONG_MIN, maxZ=LLONG_MIN, minX=LLONG_MAX, minY=LLONG_MAX, minZ=LLONG_MAX;
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


        const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
        const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
        const vec3 LL = vec3(points[i+ SLICES+1][0], points[i+SLICES+1][1], points[i+SLICES+1][2]);
        const vec3 LR = vec3(points[i+ SLICES+2][0], points[i+SLICES+2][1], points[i+SLICES+2][2]);

        const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
        const vec3 normal1 = normalize(cross(LR - LL, UL - LL));

        vertex.push_back( Vertex( LL, normal0 ) );
        vertex.push_back( Vertex( UR, normal0 ) );
        vertex.push_back( Vertex( UL, normal0 ) );
        vertex.push_back( Vertex( LL, normal1 ) );
        vertex.push_back( Vertex( LR, normal1 ) );
        vertex.push_back( Vertex( UR, normal1 ) );
        if(UL.x>maxX)
        {
          maxX=UL.x;
        }
        else if(UR.x>maxX)
        {
          maxX=UR.x;
        }
        else if(LL.x>maxX)
        {
          maxX=LL.x;
        }
        else if(LR.x>maxX)
        {
          maxX=LR.x;
        }

        if(UL.y>maxY)
        {
          maxY=UL.y;
        }
        else if(UR.y>maxY)
        {
          maxY=UR.y;
        }
        else if(LL.y>maxY)
        {
          maxY=LL.y;
        }
        else if(LR.y>maxY)
        {
          maxY=LR.y;
        }

        if(UL.z>maxZ)
        {
          maxZ=UL.z;
        }
        else if(UR.z>maxZ)
        {
          maxZ=UR.z;
        }
        else if(LL.z>maxZ)
        {
          maxZ=LL.z;
        }
        else if(LR.z>maxZ)
        {
          maxZ=LR.z;
        }
      }
    }

    for (; i < points.size() - 1; i++)
    {
      const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
      const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
      const vec3 LL = vec3(points[i+ SLICES+1 - points.size()][0], points[i+SLICES+1 - points.size()][1], points[i+SLICES+1 - points.size()][2]);
      const vec3 LR = vec3(points[i+ SLICES+2 - points.size()][0], points[i+SLICES+2 - points.size()][1], points[i+SLICES+2 - points.size()][2]);

      const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
      const vec3 normal1 = normalize(cross(LR - LL, UL - LL));

      vertex.push_back( Vertex( LL, normal0 ) );
      vertex.push_back( Vertex( UR, normal0 ) );
      vertex.push_back( Vertex( UL, normal0 ) );
      vertex.push_back( Vertex( LL, normal1 ) );
      vertex.push_back( Vertex( LR, normal1 ) );
      vertex.push_back( Vertex( UR, normal1 ) );

    }
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
