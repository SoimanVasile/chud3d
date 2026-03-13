#include "../lib/glad/include/glad/gl.h"
#include <GLFW/glfw3.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include <stdlib.h>
#include "cglm/cglm.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

vec3 cameraPos = {-2.5f, 0.0f, 5.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};

vec3 cameraUp = {0.0f, 1.0f, 0.0f};

float deltaTime = 0.0f;

float lastFrame = 0.0f;

int firstMouse = 1;

float yaw = -90.0f;

float pitch = 0.0f;
float lastX = 400.0f;

float lastY = 300.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 front;
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));                     
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    
    glm_vec3_normalize_to(front, cameraFront);
}


char* read_from_files(const char* file_name){
  FILE* file = fopen(file_name, "r");

  if (!file){
    printf("Coultnd open the file!!!");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* content = (char*)malloc((length+1) * sizeof(char));
  fread(content, 1, length, file);

  content[length] = '\0';

  fclose(file);
  return content;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    float cameraSpeed = 5.0f * deltaTime;

    vec3 temp;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glm_vec3_scale(cameraFront, cameraSpeed, temp);
        glm_vec3_add(cameraPos, temp, cameraPos);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        glm_vec3_scale(cameraFront, cameraSpeed, temp);
        glm_vec3_sub(cameraPos, temp, cameraPos);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_cross(cameraFront, cameraUp, temp);
        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, cameraSpeed, temp);
        glm_vec3_sub(cameraPos, temp, cameraPos);
    }


    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        glm_vec3_cross(cameraFront, cameraUp, temp);

        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, cameraSpeed, temp);
        glm_vec3_add(cameraPos, temp, cameraPos);
    }
}

int run (){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);    

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Unemployed", NULL, NULL);

    if (window == NULL){
        printf("The window failed to open");
        glfwTerminate();
        return 1;
    }

    GLfloat vertices[] = {
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
    0,  1,  2,  2,  3,  0, // Front
         4,  5,  6,  6,  7,  4, // Back
         8,  9, 10, 10, 11,  8, // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom;
    };

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version){
        printf("Couldnt initialize glad!");
        return -1;
    }

    glViewport(0,0, 1920, 1080);
    glEnable(GL_DEPTH_TEST);
    
    char* vertexShaderSource = read_from_files("src/shader/shader.vert");

    char* fragmentShaderSource = read_from_files("src/shader/shader.frag");

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load("src/shader/wood.jpg", &width, &height, &nrChannels, 0);

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        printf("Failed to load texture!\n");
    }

    stbi_image_free(data);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    while (!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        mat4 model, view, projection;
        glm_mat4_identity(model);
        glm_mat4_identity(view);
        glm_mat4_identity(projection);

        vec3 roomSize = {5.0f, 5.0f, 5.0f};

        glm_scale(model, roomSize);

        glm_perspective(glm_rad(67.0f), 800.0f/600.0f, 0.1f, 100.0f, projection);

        vec3 target;
        glm_vec3_add(cameraPos, cameraFront, target);

        glm_lookat(cameraPos, target, cameraUp, view);

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const GLfloat*)projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
