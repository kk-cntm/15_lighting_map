#include "lookAt.h"

glm::mat4 lookAt(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
    glm::vec3 z = glm::normalize(position - direction);
    glm::vec3 x = glm::normalize(glm::cross(glm::normalize(up), z));
    glm::vec3 y = glm::cross(z, x);
    
    glm::mat4 rotate(1.0f);
    rotate[0][0] = x.x;
    rotate[1][0] = x.y;
    rotate[2][0] = x.z;
    rotate[0][1] = y.x;
    rotate[1][1] = y.y;
    rotate[2][1] = y.z;
    rotate[0][2] = z.x;
    rotate[1][2] = z.y;
    rotate[2][2] = z.z;
    
    glm::mat4 translate(1.0f);
    translate[3][0] = -position.x;
    translate[3][1] = -position.y;
    translate[3][2] = -position.z;
    
    return rotate * translate;
}
