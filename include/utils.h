#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#define HEIGHT 600
#define WIDTH 900
//#define M_PI acos(-1.0)
#define SLICES 30
#define THE_NUMBER 9e20f
#define USE_GLFW 0

using namespace glm;
using namespace std;

struct Vertex
{
    Vertex( const vec3& position, const vec3& normal )
        : position( position )
        , normal( normal )
    {}
    vec3 position;
    vec3 normal;
};

extern int count,picked,pickIndex,del,form;
extern vector<Vertex> vertex;
extern vector<vec3> controlPoints;
extern const vec4 clearColor;
extern const vec4 blackColor;
extern double maxCoords[];
extern bool unordered_points,complete;
extern GLFWwindow* window;

extern void boundingLimits(double[],vec3);
extern void initGLFW();
extern void initShaders(GLuint*, GLuint[]);
void mouseCallback(GLFWwindow*, int, int, int);
