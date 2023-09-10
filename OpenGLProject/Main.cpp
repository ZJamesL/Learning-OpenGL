#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <math.h>
#include "Camera/Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Image_loader/stb_image.h"


// functions for the main file
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setup();
void bindBuffers(unsigned int * VBO, unsigned int * VAO, unsigned int * EBO, float* vertices, unsigned int* indices);
void recursive_triangle(unsigned int transformLoc, glm::vec3 translate, glm::mat4 matrix, double time, int depth); // this one is for fun
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int create_texture(const char* filepath, unsigned int sWrapMethod, unsigned int tWrapMethod, unsigned int minFilterMethod, unsigned int magFilterMethod);


// global variables 
GLFWwindow* window;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// perspective matrix variables 
float fov = 45.0f;
float Z_NEAR = 0.1f;
float Z_FAR = 100.0f;

// camera globals
Camera camera = Camera();
float lastX = (float)SCR_WIDTH / 2;
float lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = true;

// time 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera/mouse position variables 

int main() {
    // setup intializes glfw, the window, and glad
    setup();

    // set up shaderProgram and run it 
    Shader shader("./shaders/v_shader.vert", "./shaders/f_shader.frag");
    Shader shader2("./shaders/v_shader.vert", "./shaders/f_shader2.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // vertices            // colors           // UV coordinates
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
                                                   
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
                                                   
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                                                   
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                                                   
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
                                                   
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f

         // coords               // colors           // texCoords
         //-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // bottom left
         // 0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // top right
         // 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // bottom right         
         //-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f   // top left
    };
    unsigned int indices[] = { 
        0, 1, 2,  // first Triangle
        1, 0, 3   // second triangle
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // set up the VBO, VAO 
    unsigned int VBO, VAO, EBO;
    // set up the VBO, VAO ;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // first bind the VAO and set teh vertex buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // link the vertex attributes 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // set texture coordinate attribute 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // here we unbind the VBO from teh array_buffer for safety and it's ok since it has been registered with glVertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // create textures 
    unsigned int texture1, texture2;
    stbi_set_flip_vertically_on_load(true);
    texture1 = create_texture("./images/reimu_fumo.png", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture2 = create_texture("./images/marisa_fumo_2.jpg", GL_NEAREST, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // misc
    const float radius = 10.0f;

    // our render loop 
    while (!glfwWindowShouldClose(window)) {
        // input 
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // get detla time 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // bind textures on the texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        shader.use();

        //set up camera/view matrix
        glm::mat4 view = glm::mat4(1.0f);
        float camX = sin(glfwGetTime() * 2) * radius;
        float camZ = cos(glfwGetTime() * 2) * radius;
        view = camera.getLookAtMatrix();
        shader.setMat4("view", view);

        // set up projection matrix 
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, Z_NEAR, Z_FAR);
        shader.setMat4("projection", projection);

        // visual effect
        unsigned int mixerLoc = glGetUniformLocation(shader.ID, "mixer");
        float mixer = sin(glfwGetTime() * 2) * 0.5 + 0.5;
        glUniform1f(mixerLoc, mixer);

        for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // poll events and swap the buffers 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deallocate everything 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.del();

    // clear all glfw resources 
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xPos = (float) xpos;
    float yPos = (float) ypos;

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    camera.handleMouseUpdate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    printf("%f\n", yoffset);
    camera.handleZoomUpdate((float)yoffset);
}

void recursive_triangle(unsigned int transformLoc, glm::vec3 translate, glm::mat4 matrix, double time, int depth) {
    if (depth < 0) return;

    matrix = glm::translate(matrix, translate);
    matrix = glm::scale(matrix, glm::vec3(0.5f));
    if (depth % 2 == 0) {
        glm::mat4 matrixToUse = glm::rotate(matrix, (float)time, glm::vec3(0.0, 1.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrixToUse));
    } else {
        glm::mat4 matrixToUse = glm::rotate(matrix, (float) 0.0, glm::vec3(0.0, 1.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrixToUse));
    }
    
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // recursive calls
    depth -= 1;
    recursive_triangle(transformLoc, glm::vec3(0.0f, 1.5f, 0.0f), matrix, time, depth);
    recursive_triangle(transformLoc, glm::vec3(-1.0f,-0.5f, 0.0f), matrix,time, depth);
    recursive_triangle(transformLoc, glm::vec3(1.0f, -0.5f, 0.0f), matrix,time, depth);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glEnable(GL_DEPTH_TEST);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) 
        glDisable(GL_DEPTH_TEST);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forwards
        camera.updatePosition(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backwards
        camera.updatePosition(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
        camera.updatePosition(3, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
        camera.updatePosition(4, deltaTime);
}

void setup() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // enable depth testing 
    glEnable(GL_DEPTH_TEST);

    // tell GLFW to hide and capture and cursor 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

unsigned int create_texture(const char* filepath, unsigned int sWrapMethod, unsigned int tWrapMethod,
                            unsigned int minFilterMethod, unsigned int magFilterMethod) 
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping options 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMethod);
    // set filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMethod);
    
    // determine if png or jpg 
    int len = strlen(filepath);
    const char* fileformat = &filepath[len - 4];
    int result;
    if (strcmp(fileformat, ".png") == 0) {
        result = 0;
    } else if (strcmp(fileformat, ".jpg") == 0) {
        result = 1;
    } else {
        std::cerr << "ERROR: unsupported file format" << std::endl;
        exit(-1);
    }

    // load in texture data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        if (result == 0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

void bindBuffers(unsigned int * VBO, unsigned int * VAO, unsigned int * EBO, float* vertices, unsigned int* indices) {
    // set up the VBO, VAO ;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    // first bind the VAO and set teh vertex buffers
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // link the vertex attributes 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // set texture coordinate attribute 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // here we unbind the VBO from teh array_buffer for safety and it's ok since it has been registered with glVertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}