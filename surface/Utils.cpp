#include "include/Utils.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

GLFWwindow* window;
const vec4 clearColor = vec4(0.5, 1.0, 1.0, 1.0),
           blackColor = vec4(0,0,0,1);
const vec3 editBackground = vec3(0.025, .05, 0.1),
      regColor = vec3(0.0, 0.7, 0.0),
      delBackground = vec3(0.1, 0.0, 0.0),
      delColor = vec3(0.8,0.2,0.05),
      editColor = vec3(0.1,0.3,0.9);
vec3 currentColor;
bool shift=0;

void boundingLimits(double args[], vec3 coord)
{
  if(coord.x>args[0])
  {
    args[0]=coord.x;
  }
  else if(coord.y>args[1])
  {
    args[1]=coord.y;
  }
  else if(coord.z>args[2])
  {
    args[2]=coord.z;
  }
  else if(coord.x<args[3])
  {
    args[3]=coord.x;
  }
  else if(coord.y<args[4])
  {
    args[4]=coord.y;
  }
  else if(coord.z<args[5])
  {
    args[5]=coord.z;
  }
}

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  if ((key == GLFW_KEY_D||key == GLFW_KEY_DELETE) && action == GLFW_PRESS && controlPoints.size()>1)
  {
    if(!del)
    {
      glClearColor(delBackground.x,delBackground.y,delBackground.z,1);
      currentColor = delColor;
    }
    else
    {
      glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
      currentColor = regColor;
    }
    del ^= 1;
  }
//  if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS)
//    shift = 1;
//  if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
//    shift = 0;
//  if((key == GLFW_KEY_KP_ADD || (key == GLFW_KEY_EQUAL && shift)) && action == GLFW_PRESS)
//    increaseSample();
}

void mouseCallback(GLFWwindow* win, int button, int action, int mods)
{
  double x,y;
  glfwGetCursorPos(window,&x,&y);
  x=x/(WIDTH/2)-1;
  y=-(y/(HEIGHT/2)-1);
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !complete)
  {
    for (int inc = 0; inc < ::count; inc++)
    {
      if(pow(controlPoints[inc].x - x,2) + pow(controlPoints[inc].y - y,2)<M_DIST)
      {
        currentColor = editColor;
        picked=1;
        pickIndex=inc;
        glClearColor(editBackground.x, editBackground.y, editBackground.z, 1);
      }
    }

    if (!del && !picked)
    {
      currentColor = regColor;
      vec3 newp;
      newp.x = x;
      newp.y = y, newp.z = 0;
      controlPoints.push_back(newp);
      ::count++;
    }

    if(del && picked)
    {
      del = 0;
      if(controlPoints.size()>1)
      {
        controlPoints.erase(controlPoints.begin()+pickIndex);
        ::count--;
        picked = 0;
        pickIndex = -1;
        glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
      }
    }
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !complete)
  {
    if (picked)
    {
      if(y<-1)
        y=-1;
      else if(y>1)
        y=1;
      if(x<-1)
        x=-1;
      else if(x>1)
        x=1;
      controlPoints[pickIndex].x = x, controlPoints[pickIndex].y = y;
      picked = 0;
      glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
      currentColor = regColor;
    }
  }
  if(controlPoints.size())
  {
    computeBezier();
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !sample.empty())
  {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    complete = 1;
  }
}

void positionCallback(GLFWwindow *window, double x, double y)
{
  double xPos=x/(WIDTH/2)-1, yPos=-(y/(HEIGHT/2)-1);
  if (picked)
  {
    controlPoints[pickIndex].x = xPos, controlPoints[pickIndex].y = yPos;
    computeBezier();
  }
}

vec3 interpolation(vec3 a, vec3 b, double t)
{
  vec3 c;
  c.x = (1 - t)*a.x + t*b.x;
  c.y = (1 - t)*a.y + t*b.y;
  c.z = (1 - t)*a.z + t*b.z;
  return c;
}

vec3 calcCasteljau(double t, vector<vec3> points)
{
  if(points.size()==1)
    return points[0];
  else
  {
    vector<vec3> tmp;
    for(int inc=0; inc<points.size()-1; inc++)
      tmp.push_back(interpolation(points[inc], points[inc+1], t));
    return calcCasteljau(t,tmp);
  }
}

void computeBezier()
{
  double step = 1.0 / slices;
  int inc = 0;
  sample.clear();
  for (double t = 0; t <= 1 && inc <= slices; t += step, inc++)
  {
    sample.push_back(calcCasteljau(t,controlPoints));
  }
}

void surfaceRevolution( const vector<vec3>& pts, unsigned int segments )
{
  int i,j;
  vector<vec3> tmp;
  for(j = 0; j < pts.size();++j)
  {
      const vec2 p = vec2(pts[j].x, pts[j].y);
      for(i =0; i < segments;++i)
      {
          double theta = ((double)(M_PI*2.0)*((double)i))/(double)segments;
          double x = sin(theta)*p.x;
          double z = cos(theta)*p.x;

          tmp.push_back(vec3(x,p.y,z));
      }
  }

  for(i=0; i < tmp.size()-segments-1; i++)
  {
    const unsigned short UL_index = i,
                       UR_index = i+1,
                       LL_index = i+segments,
                       LR_index = i+segments+1;
    const vec3 UL = tmp[UL_index];
    const vec3 UR = tmp[UR_index];
    const vec3 LL = tmp[LL_index];
    const vec3 LR = tmp[LR_index];

    const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
    const vec3 normal1 = normalize(cross(LR - LL, UL - LL));

    vertex.push_back( LL );
    vertex.push_back( UR );
    vertex.push_back( UL );
    normals.push_back(normal0);
    normals.push_back(normal0);
    normals.push_back(normal0);

    vertex.push_back( LL );
    vertex.push_back( LR );
    vertex.push_back( UR );
    normals.push_back(normal1);
    normals.push_back(normal1);
    normals.push_back(normal1);
    uvs.push_back(calcUV(UL));
    uvs.push_back(calcUV(UR));
    uvs.push_back(calcUV(LL));
    uvs.push_back(calcUV(LR));
  }


//  for (; i < tmp.size() - 1; i++)
//  {
//    const unsigned short UL_index = i-1,
//                         UR_index = i,
//                         LL_index = i+segments - tmp.size(),
//                         LR_index = i+segments - tmp.size();
//    const vec3 UL = tmp[UL_index];
//    const vec3 UR = tmp[UR_index];
//    const vec3 LL = tmp[LL_index];
//    const vec3 LR = tmp[LR_index];
//
//    const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
//    const vec3 normal1 = normalize(cross(LR - LL, UL - LL));
//
//    vertex.push_back( LL );
//    vertex.push_back( UR );
//    vertex.push_back( UL );
//    normals.push_back(normal0);
//    normals.push_back(normal0);
//    normals.push_back(normal0);
//
//    vertex.push_back( LL );
//    vertex.push_back( LR );
//    vertex.push_back( UR );
//    normals.push_back(normal1);
//    normals.push_back(normal1);
//    normals.push_back(normal1);
//    uvs.push_back(calcUV(UL));
//    uvs.push_back(calcUV(UR));
//    uvs.push_back(calcUV(LL));
//    uvs.push_back(calcUV(LR));
//  }
  tmp.clear();
}

vec2 calcUV(vec3 point)
{
    point = glm::normalize(point);

    float u = ((glm::atan(point.x, point.z) / M_PI) + 1.0f) * 0.5f;
    float v = (asin(point.y) / M_PI) + 0.5f;

    return glm::vec2(u, v);
}

void draw(GLuint arrayAtribSize,
          GLuint buffer,
          GLuint colorUniform,
          GLuint drawType,
          GLuint bulletUniform,
          GLuint lineFlag,
          GLuint shader,
          vec3 color,
          vector<vec3> data)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), &data[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(shader);

  glEnableClientState(GL_VERTEX_ARRAY);
  glUniform1f(bulletUniform, lineFlag);
  glUniform3f(colorUniform, color.x, color.y, color.z);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    arrayAtribSize,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(drawType, 0, data.size());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glUseProgram(0);

}

const vector<vec3> returnXAxis()
{
  vector<vec3> tmp;
  tmp.push_back(vec3(-1,0,0));
  tmp.push_back(vec3(1,0,0));
  return tmp;
}

const vector<vec3> returnYAxis()
{
  vector<vec3> tmp;
  tmp.push_back(vec3(0,-1,0));
  tmp.push_back(vec3(0,1,0));
  return tmp;
}

void drawAxis(GLuint arrayAtribSize,
              GLuint buffer,
              GLuint drawType,
              GLuint elementSize,
              GLuint shader,
              GLuint colorUniform,
              vec3 color)
{
  glUseProgram(shader);

  glEnableClientState(GL_VERTEX_ARRAY);
  glUniform3f(colorUniform, color.x, color.y, color.z);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    arrayAtribSize,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glDrawArrays(drawType, 0, elementSize);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glUseProgram(0);
}

