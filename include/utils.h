#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>

#define HEIGHT 600
#define WIDTH 900
#define M_PI acos(-1.0)
#define SLICES 30

using namespace glm;
using namespace std;

extern bool unordered_points;

struct Vertex
{
    Vertex( const vec3& position, const vec3& normal )
        : position( position )
        , normal( normal )
    {}
    vec3 position;
    vec3 normal;
};

extern vector<Vertex> vertex;
