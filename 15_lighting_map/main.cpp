#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

int width = 800;
int height = 600;

float last_xpos = (float)width / 2.0f;
float last_ypos = (float)height / 2.0f;

float fov = 45.0f;

float last_frame = 0.0f;
float delta_time = 0.0f;

glm::vec3 cameraInitPos(-1.5f, 0.0f, 7.0f);

Camera camera({
    Camera::Type::fly,
    cameraInitPos,
    glm::vec3(0.0f, 0.0f, -1.0f),
    -75.0f,
    0.0f,
    delta_time
});

bool right_mouse_btn_pressed = false;

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height) {
    width = new_width;
    height = new_height;
    glViewport(0, 0, width, height);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!right_mouse_btn_pressed) {
        last_xpos = xpos;
        last_ypos = ypos;
        
        return;
    }
        
    float x_offset = last_xpos - xpos;
    float y_offset = ypos - last_ypos;
    
    last_xpos = xpos;
    last_ypos = ypos;
    
    const float sensetive = 0.1f * fov / 45.0f;
    
    x_offset *= sensetive;
    y_offset *= sensetive;
    
    camera.addYaw(x_offset);
    camera.addPitch(y_offset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        right_mouse_btn_pressed = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        right_mouse_btn_pressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void handle_key(GLFWwindow* window, int key, void (*callback)()) {
    static double last_call_time = glfwGetTime();
    
    double time = glfwGetTime();
    double diff = time - last_call_time;
    
    if (glfwGetKey(window, key) == GLFW_PRESS && diff > 0.2) {
        callback();
        last_call_time = time;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= yoffset;
    
    if (fov < 1.0f) fov = 1.0f;
    else if (fov > 45.0f) fov = 45.0f;
}

void process_camera_move(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveBackwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveBackwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveForwardRight();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveForwardLeft();
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward();
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward();
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft();
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight();
    }
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    process_camera_move(window);
}

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))) {
        std::cout << "Failed to initilize glad" << std::endl;
        glfwTerminate();
        return -2;
    }
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition(1.2f, 0.0f, 2.0f);
    
    unsigned int VAO, VBO, lightVAO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Texture boxTexture("./assets/container2.png", Texture::png);
    Texture boxSpecTex("./assets/container2_specular.png", Texture::png);
    Texture boxEmission("./assets/matrix.jpg", Texture::jpg);
    Texture boxEmissionMap("./assets/emission_map.jpg", Texture::jpg);
    
    Shader cubeProgram("./shaders/cube-vs.glsl", "./shaders/cube-fs.glsl");
    Shader lightProgram("./shaders/light-vs.glsl", "./shaders/light-fs.glsl");
    
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        
        float time = glfwGetTime();
        
        delta_time = time - last_frame;
        last_frame = time;
        
        camera.setDeltaTime(delta_time);
        
        float l_arcLength = 2.5f * delta_time;
        glm::vec3 l_radiusVec = lightPosition - cubePosition;
        float l_angle = (360 * l_arcLength) / (2 * glm::pi<float>() * glm::length(l_radiusVec));
        glm::mat4 l_rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(l_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 l_rotatedRadiusVec(l_rotateMat * glm::vec4(l_radiusVec, 1.0f));
        lightPosition = cubePosition + l_rotatedRadiusVec;

        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition);

        glm::mat4 view = camera.view();
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
        
        cubeProgram.use();
        
        cubeProgram.setValue("model", model);
        cubeProgram.setValue("view", view);
        cubeProgram.setValue("projection", projection);
        cubeProgram.setValue("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        cubeProgram.setValue("lightColor", glm::vec3(1.0f));
        cubeProgram.setValue("material.diffuse", 0);
        cubeProgram.setValue("material.specular", 1);
        cubeProgram.setValue("material.emission", 2);
        cubeProgram.setValue("material.emissionMap", 3);
        cubeProgram.setValue("material.shininess", 32.0f);
        cubeProgram.setValue("lightPos", lightPosition);
        cubeProgram.setValue("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        cubeProgram.setValue("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        cubeProgram.setValue("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        
        boxTexture.bind(GL_TEXTURE0);
        boxSpecTex.bind(GL_TEXTURE1);
        boxEmission.bind(GL_TEXTURE2);
        boxEmissionMap.bind(GL_TEXTURE3);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightProgram.use();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));
        
        lightProgram.setValue("view", view);
        lightProgram.setValue("projection", projection);
        lightProgram.setValue("model", model);
        lightProgram.setValue("lightColor", glm::vec3(1.0f));
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    return 0;
}
