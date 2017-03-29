#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#define HEIGHT 600
#define WIDTH 900

extern glm::mat4 getViewMatrix();
extern glm::mat4 getProjectionMatrix();
extern void computeMatricesFromInputs();

#endif // CONTROLS_H_INCLUDED
