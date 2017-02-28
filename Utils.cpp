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
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  {
    glClearColor(blackColor.x, blackColor.y, blackColor.z, blackColor.w);
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

void surfaceRevolution(mat4 mvp)
{
  mat4 mv = mat4(mvp);
  for (int i = 0; i < 360; i += 5)
  {
    mv = glm::rotate(mv, (float)i, vec3(0,1,0));
    for (int j = 0; j < SLICES + 1; j++)
    {
      float xp = mv[0].x * sample[j].x + mv[0].y * sample[j].y + mv[0].z * sample[j].z + mv[0].w;
      float yp = mv[1].x * sample[j].x + mv[1].y * sample[j].y + mv[1].z * sample[j].z + mv[1].w;
      float zp = mv[2].x * sample[j].x + mv[2].y * sample[j].y + mv[2].z * sample[j].z + mv[2].w;
      float wp = mv[3].x * sample[j].x + mv[3].y * sample[j].y + mv[3].z * sample[j].z + mv[3].w;
      wp *= wp;

      xp /= wp;
      yp /= wp;
      zp /= wp;

      vertex.push_back(vec3(xp,yp,zp));
    }
  }
}

void surfaceRevolution( const vector<vec3>& pts, unsigned int segments )
{
  vector<vec2> circlePts;
  for( unsigned int i = 0; i <= segments; ++i )
  {
    float angle = (i / (float)segments) * M_PI * 2.0f;
    circlePts.push_back( vec2( cos( angle ), sin( angle ) ) );
  }

  typedef vector<vec3> Layer;
  typedef vector<Layer> Layers;
  Layers layers( pts.size(), Layer( circlePts.size() ) );
  for( size_t i = 0; i < pts.size(); ++i )
  {
    for( unsigned int j = 0; j < circlePts.size(); ++j )
    {
        layers[i][j] = vec3( circlePts[j] * pts[i].x, pts[i].y );
    }
  }
  for( size_t i = 1; i < layers.size(); ++i )
  {
    const Layer& prvLayer = layers[ i-1 ];
    const Layer& curLayer = layers[ i-0 ];
    for( size_t j = 1; j < circlePts.size(); ++j )
    {
      const vec3& LL = prvLayer[ j-1 ];
      const vec3& LR = prvLayer[ j-0 ];
      const vec3& UL = curLayer[ j-1 ];
      const vec3& UR = curLayer[ j-0 ];

      const vec3 normal0 = normalize( cross( UR - LL, UL - LL ) );

      const vec3 normal1 = normalize( cross( LR - LL, UL - LL ) );

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
  }
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
          GLuint shader,
          vec3 color,
          vector<vec3> data)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), &data[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

  glDrawArrays(drawType, 0, data.size());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glUseProgram(0);

}

void drawSurface(GLuint buffer,
          GLuint colorUniform,
          GLuint shader,
          vec3 color,
          vector<vec3> data,
          vector<vec3> normal,
          vector<vec2> uv,
          GLuint arrayAtribSize,
          GLuint drawType)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3), &data[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

  glDrawArrays(drawType, 0, data.size());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glUseProgram(0);

}


