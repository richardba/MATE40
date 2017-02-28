//#include "include/Mesh.h"
//#include "include/Utils.h"
//#include <iostream>
//#include <climits>
//
//using namespace std;
//double maxCoords[]= {-THE_NUMBER , -THE_NUMBER, -THE_NUMBER}, minCoords[]= {THE_NUMBER, THE_NUMBER, THE_NUMBER};
//void Mesh::clearPoints()
//{
//  points.clear();
//}
//
//void Mesh::addPoint(vec3 point)
//{
//  points.push_back(point);
//}
//
//void Mesh::setPoints()
//{
//  pointsNo = points.size();
//}
//
//void Mesh::computeFaces()
//{
//  noFacePoints.clear();
//  faces.clear();
//  vector<int> face;
//  int i;
//
//  for (i = 0; i < points.size() - slices - 1; i++)
//  {
//    if ((i + 1) % ((int)slices + 1) != 0)
//    {
//      noFacePoints.push_back(4);
//      face.push_back(i);
//      face.push_back(i + 1);
//      face.push_back(i + slices + 2);
//      face.push_back(i + slices + 1);
//      faces.push_back(face);
//      face.pop_back();
//      face.pop_back();
//      face.pop_back();
//      face.pop_back();
//    }
//  }
//
//  for (; i < points.size() - 1; i++)
//  {
//    noFacePoints.push_back(4);
//    face.push_back(i);
//    face.push_back(i + 1);
//    face.push_back(i + slices + 2 - points.size());
//    face.push_back(i + slices + 1 - points.size());
//    faces.push_back(face);
//    face.pop_back();
//    face.pop_back();
//    face.pop_back();
//    face.pop_back();
//  }
//}
//
//void Mesh::setFaces()
//{
//  facesNo = faces.size();
//}
//
//void Mesh::calcEdge()
//{
//  edgesNo = pointsNo + facesNo - 2;
//}
//
//void Mesh::drawFaces()
//{
//  int i;
//  if(unordered_points)
//  {
//    for (i = 0; i < points.size() - slices - 1; i++)
//    {
//      if ((i + 1) % ((int)slices + 1) != 0)
//      {
//        const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
//        const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
//        const vec3 LL = vec3(points[i+ slices+1][0], points[i+slices+1][1], points[i+slices+1][2]);
//        const vec3 LR = vec3(points[i+ slices+2][0], points[i+slices+2][1], points[i+slices+2][2]);
//
//        const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
//        const vec3 normal1 = normalize(cross(LR - LL, UL - LL));
//
//        vertex.push_back( LL );
//        vertex.push_back( UR );
//        vertex.push_back( UL );
//        normals.push_back(normal0);
//        normals.push_back(normal0);
//        normals.push_back(normal0);
//
//        vertex.push_back( LL );
//        vertex.push_back( LR );
//        vertex.push_back( UR );
//        normals.push_back(normal1);
//        normals.push_back(normal1);
//        normals.push_back(normal1);
//
//        uvs.push_back(calcUV(UL));
//        uvs.push_back(calcUV(UR));
//        uvs.push_back(calcUV(LL));
//        uvs.push_back(calcUV(LR));
//      }
//    }
//
//    for (; i < points.size() - 1; i++)
//    {
//      const vec3 UL = vec3(points[i][0], points[i][1], points[i][2]);
//      const vec3 UR = vec3(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
//      const vec3 LL = vec3(points[i+ slices+1 - points.size()][0], points[i+slices+1 - points.size()][1], points[i+slices+1 - points.size()][2]);
//      const vec3 LR = vec3(points[i+ slices+2 - points.size()][0], points[i+slices+2 - points.size()][1], points[i+slices+2 - points.size()][2]);
//
//      const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
//      const vec3 normal1 = normalize(cross(LR - LL, UL - LL));
//
//        vertex.push_back( LL );
//        vertex.push_back( UR );
//        vertex.push_back( UL );
//        normals.push_back(normal0);
//        normals.push_back(normal0);
//        normals.push_back(normal0);
//
//        vertex.push_back( LL );
//        vertex.push_back( LR );
//        vertex.push_back( UR );
//        normals.push_back(normal1);
//        normals.push_back(normal1);
//        normals.push_back(normal1);
//
//        uvs.push_back(calcUV(UL));
//        uvs.push_back(calcUV(UR));
//        uvs.push_back(calcUV(LL));
//        uvs.push_back(calcUV(LR));
//
//    }
//
//    points.clear();
//    unordered_points=0;
//  }
//
//
////  glPolygonMode( GL_FRONT, GL_FILL );
////  glPolygonMode( GL_BACK, GL_FILL );
////
////
////  glColor3ub(0.0, 128, 64);
////  glEnable(GL_DEPTH_TEST);
////  glEnableClientState( GL_VERTEX_ARRAY );
////  glEnableClientState( GL_NORMAL_ARRAY );
////  glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &vertex[0].position );
////  glNormalPointer( GL_FLOAT, sizeof(Vertex), &vertex[0].normal );
////  glDrawArrays( GL_TRIANGLES, 0, vertex.size() );
////  glDisableClientState( GL_VERTEX_ARRAY );
////  glDisableClientState( GL_NORMAL_ARRAY );
//}
//
//Mesh::~Mesh()
//{
//
//}
//
//Mesh::Mesh()
//{
//
//}
