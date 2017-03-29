#include <vector>
#ifndef MESH_H
#define MESH_H

using namespace std;

class Mesh
{
  public:
    Mesh();
    virtual ~Mesh();
    void clearPoints();
    void addPoint(vector<double> vec);
    void setPoints();
    void computeFaces();
    void setFaces();
    void calcEdge();
    void drawFaces();

  protected:

  private:
  int pointsNo, facesNo, edgesNo;
  vector<vector<double> > points;
  vector<int> noFacePoints;
  vector<vector<int> > faces;
};

#endif // MESH_H
