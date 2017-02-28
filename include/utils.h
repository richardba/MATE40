#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "Controls.h"
#include "Shader.h"
#include "Texture.h"

#define HEIGHT 600
#define WIDTH 900
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif // M_PI
//#define M_PI acos(-1.0)
#define THE_NUMBER 9e20f
#define USE_GLFW 0
#define SLICES 30.0f
#define M_DIST 0.00015f
#define GL_TYPE_3D 3

using namespace glm;
using namespace std;

class Mesh;
extern GLuint count,picked,pickIndex,form;
extern const float slices;
extern vector<vec3> controlPoints,
                    sample,
                    normals,
                    vertex;
extern const vec4 clearColor,
                  blackColor;
extern const vec3 delBackground,
                  regColor,
                  editBackground;
extern vec3 currentColor;
extern double maxCoords[];
extern bool unordered_points, complete, del, shift;
extern GLFWwindow* window;
extern vector<vec2> uvs;

extern vec3 calcCasteljau(double, vector<vec3>);
extern void surfaceRevolution( const vector<vec3>& pts, unsigned int segments = 32);
extern void boundingLimits(double[],vec3);
extern void computeBezier();
extern void draw(GLuint, GLuint, GLuint, GLuint, GLuint, vec3, vector<vec3>);
extern void drawBezier();
extern void drawCircle(int, double, double, double);
extern void drawLine(vec3, vec3);
extern void drawSurface(GLuint buffer,
                        GLuint colorUniform,
                        GLuint shader,
                        vec3 color,
                        vector<vec3> data,
                        vector<vec3> normal,
                        vector<vec2> uv,
                        GLuint arrayAtribSize=GL_TYPE_3D,
                        GLuint drawType=GL_TRIANGLES);
extern void glfwDrawCurve(GLuint,GLuint);
extern void initGLFW();
extern void initShaders(GLuint*, GLuint[]);
extern void keyCallback(GLFWwindow*, int, int, int, int);
extern void mouseCallback(GLFWwindow*, int, int, int);
extern void positionCallback(GLFWwindow *, double, double);
extern void surfaceRevolution(mat4 mvp);
extern vec2 calcUV(vec3);
