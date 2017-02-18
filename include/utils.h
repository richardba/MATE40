#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#define HEIGHT 600
#define WIDTH 900
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif // M_PI
//#define M_PI acos(-1.0)
#define THE_NUMBER 9e20f
#define USE_GLFW 0
#define SLICES 30.0f

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

extern int count,picked,pickIndex,form;
extern const float slices;
extern vector<Vertex> vertex;
extern vector<vec3> controlPoints;
extern const vec4 clearColor,
                  blackColor;
extern const vec3 delDot,
                  regDot,
                  editDot;
extern double maxCoords[];
extern bool unordered_points, complete, del, shift;
extern GLFWwindow* window;
extern vec3 sample[(int)SLICES];


extern void boundingLimits(double[],vec3);
extern void initGLFW();
extern void initShaders(GLuint*, GLuint[]);
extern void glfwDrawCurve();
extern vec3 calcCasteljau(double, vector<vec3>);
extern void computeBezier();
extern void drawCircle(int, double, double, double);
extern void drawLine(vec3, vec3);
extern void drawBezier();
extern void mouseCallback(GLFWwindow*, int, int, int);
extern void keyCallback(GLFWwindow*, int, int, int, int);
extern void positionCallback(GLFWwindow *, double, double);
