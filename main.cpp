#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <Shader.h>
#include <iostream>
#include <vertices.h>

// Funções para compilar shaders
void checkShaderCompileStatus(GLuint shader);
void checkProgramLinkStatus(GLuint program);
// Função para carregar textura
unsigned int loadTexture(const char* path);
// Processar eventos de teclado
void processInput(GLFWwindow *window, float *x, float *y, float *z);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Função para carregar as regras de movimentação do carro
void moveCar(int *curva, float *carPosX, float *carPosZ, float *rotation);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Dados dos objetos (chao e carro)
float groundVertices[] = {
    -3.0f,  0.0f, -3.0f,  0.0f, 1.0f,
     3.0f,  0.0f, -3.0f,  1.0f, 1.0f,
     3.0f,  0.0f,  3.0f,  1.0f, 0.0f,
     3.0f,  0.0f,  3.0f,  1.0f, 0.0f,
    -3.0f,  0.0f,  3.0f,  0.0f, 0.0f,
    -3.0f,  0.0f, -3.0f,  0.0f, 1.0f
};

float cubeVertices[] = {
    // Frontal
    -1.0f,  0.0f,  0.5f,  0.0f,  0.0f,
    -1.0f,  0.5f,  0.5f,  0.0f,  0.25f,
    -1.0f,  0.5f,  1.5f,  0.25f, 0.25f,
    -1.0f,  0.0f,  0.5f,  0.0f,  0.0f,
    -1.0f,  0.5f,  1.5f,  0.25f, 0.25f,
    -1.0f,  0.0f,  1.5f,  0.25f, 0.0f,

    // Traseira
     1.0f,  0.0f,  0.5f,  0.25f, 0.0f,
     1.0f,  0.5f,  0.5f,  0.25f, 0.25f,
     1.0f,  0.5f,  1.5f,  0.5f,  0.25f,
     1.0f,  0.0f,  0.5f,  0.25f, 0.0f,
     1.0f,  0.5f,  1.5f,  0.5f,  0.25f,
     1.0f,  0.0f,  1.5f,  0.5f,  0.0f,

    // Lateral Direita
    -1.0f,  0.0f,  0.5f,  0.5f,  0.0f,
    -1.0f,  0.5f,  0.5f,  0.5f,  0.25f,
     1.0f,  0.5f,  0.5f,  1.0f,  0.25f,
    -1.0f,  0.0f,  0.5f,  0.5f,  0.0f,
     1.0f,  0.5f,  0.5f,  1.0f,  0.25f,
     1.0f,  0.0f,  0.5f,  1.0f,  0.0f,

    // Lateral Esquerda
    -1.0f,  0.0f,  1.5f,  0.5f,  0.0f,
    -1.0f,  0.5f,  1.5f,  0.5f,  0.25f,
     1.0f,  0.5f,  1.5f,  1.0f,  0.25f,
    -1.0f,  0.0f,  1.5f,  0.5f,  0.0f,
     1.0f,  0.5f,  1.5f,  1.0f,  0.25f,
     1.0f,  0.0f,  1.5f,  1.0f,  0.0f,

    // Inferior
    -1.0f,  0.0f,  0.5f,  0.5f,  0.5f,
     1.0f,  0.0f,  0.5f,  1.0f,  0.5f,
     1.0f,  0.0f,  1.5f,  1.0f,  1.0f,
    -1.0f,  0.0f,  0.5f,  0.5f,  0.5f,
     1.0f,  0.0f,  1.5f,  1.0f,  1.0f,
    -1.0f,  0.0f,  1.5f,  1.0f,  0.5f,

    // Superior
    -1.0f,  0.5f,  1.5f,  0.0f,  0.25f,
    -1.0f,  0.5f,  0.5f,  0.0f,  0.5f,
     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,
    -1.0f,  0.5f,  1.5f,  0.0f,  0.25f,
     1.0f,  0.5f,  0.5f,  0.5f,  0.5f,
     1.0f,  0.5f,  1.5f,  0.5f,  0.25f
};

float factor = 0.0005f;
float carPosX = 0.0f;
float carPosZ = 0.0f;
float rotation = 0.0f;

int curva = 1;

bool visaoAerea = true;

int main() {
   float x = 0.0f;
   float y = 0.0f;
   float z = -5.0f;

    // Inicializar GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Falha ao inicializar GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pista de corrida 3D", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!window) {
        fprintf(stderr, "Falha ao criar a janela GLFW\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Falha ao inicializar GLEW\n");
        return -1;
    }

    // Configurar buffers de vértices para cada objeto
    GLuint VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // Solo
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atribuir coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cubo
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atribuir coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader groundShader("vertex.glsl", "fragment.glsl");
    Shader carShader("vertex.glsl", "fragment.glsl");

    unsigned int
        groundTexture = loadTexture("res/images/pista3.png"),
        carTexture = loadTexture("res/images/kombi.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        // Atender os eventos
        processInput(window, &x, &y, &z);

        // Limpar a tela
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.5f, 0.0f));
        if(visaoAerea){
            model       = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view        = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
            projection  = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        } else {
            model       = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.5f, 0.0f));
            view        = glm::translate(view, glm::vec3(0.5f, -1.75f, -6.35f));
            projection  = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);        }

        // Renderizar cada objeto com seu shader
        // Solo
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        groundShader.use();
        unsigned int modelLoc = glGetUniformLocation(groundShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(groundShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        groundShader.setMat4("projection", projection);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Carro
        // Bind da textura e shader
        glBindTexture(GL_TEXTURE_2D, carTexture);
        carShader.use();
        // Mover o carro
        moveCar(&curva, &carPosX, &carPosZ, &rotation);
        model = glm::translate(model, glm::vec3(carPosX, 0.0f, carPosZ));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        //Aplicar escala de 20% no tamanho do carro
        model = glm::scale(model, glm::vec3(0.2f));
        // Configuração das matrizes no shader
        modelLoc = glGetUniformLocation(carShader.ID, "model");
        viewLoc  = glGetUniformLocation(carShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        carShader.setMat4("projection", projection);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Trocar os buffers da janela
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Funções auxiliares
void checkShaderCompileStatus(GLuint shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Erro de compilação de shader: %s\n", infoLog);
    }
}

void checkProgramLinkStatus(GLuint program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Erro ao linkar programa: %s\n", infoLog);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float *x, float *y, float *z) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS) {
        *x+=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }

    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        *x-=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }


    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        *y+=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }

    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        *y-=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }


    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
        *z+=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }


    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        *z-=0.1f;
        printf("(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        *x = 0.0f;
        *y = 0.0f;
        *z = -5.0f;
        printf("RESET\n(%.3f,%.3f,%.3f\n", *x, *y, *z);
    }
}

void moveCar(int *curvaOriginal, float *carPosXOriginal, float *carPosZOriginal, float *rotationOriginal) {
    int curva = *curvaOriginal;
    float carPosX = *carPosXOriginal, carPosZ = *carPosZOriginal, rotation = *rotationOriginal;

    //std::cout << "X: " << carPosX * factor << " Z: " << carPosZ << std::endl;
    switch (curva) {
        case 1:
            carPosX -= 1.0f * factor;
            if (carPosX <= -2.5f) {
                rotation -= 90.0f;
                curva = 2;
            }
            break;

        case 2:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= -2.405f) {
                rotation -= 90.0f;
                curva = 3;
            }
            break;

        case 3:
            carPosX += 1.0f * factor;
            if (carPosX >= -1.5f) {
                rotation -= 90.0f;
                curva = 4;
            }
            break;

        case 4:
            carPosZ += 1.0f * factor;
            if (carPosZ >= -1.925f) {
                rotation -= 25.0f;
                curva = 5;
            }
            break;

        case 5:
            carPosZ += 1.0f * factor;
            carPosX -= 0.5f * factor;
            if (carPosX <= -1.895f && carPosZ >= -1.135f) {
                rotation += 90.0f;
                curva = 6;
            }
            break;

        case 6:
            carPosZ += 0.5f * factor;
            carPosX += 1.0f * factor;
            if (carPosX >= -1.185f && carPosZ >= -0.78f) {
                rotation += 90.0f;
                curva = 7;
            }
            break;

        case 7:
            carPosZ -= 1.0f * factor;
            carPosX += 0.5f * factor;
            if (carPosX >= -0.495f && carPosZ <= -2.16f) {
                rotation -= 65.0f;
                curva = 8;
            }
            break;

        case 8:
            carPosX += 1.0f * factor;
            if (carPosX >= 0.05f) {
                rotation -= 90.0f;
                curva = 9;
            }
            break;

        case 9:
            carPosZ += 1.0f * factor;
            if (carPosZ >= -0.85f) {
                rotation += 90.0f;
                curva = 10;
            }
            break;

        case 10:
            carPosX += 1.0f * factor;
            if (carPosX >= 1.05f) {
                rotation += 90.0f;
                curva = 11;
            }
            break;

        case 11:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= -2.05f) {
                rotation -= 90.0f;
                curva = 12;
            }
            break;

        case 12:
            carPosX += 1.0f * factor;
            if (carPosX >= 1.85f) {
                rotation -= 90.0f;
                curva = 13;
            }
            break;

        case 13:
            carPosZ += 1.0f * factor;
            if (carPosZ >= 1.5f) {
                rotation -= 90.0f;
                curva = 14;
            }
            break;

        case 14:
            carPosX -= 1.0f * factor;
            if (carPosX <= 1.075f) {
                rotation -= 90.0f;
                curva = 15;
            }
            break;

        case 15:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= 0.0f) {
                rotation = 0;
                curva = 1;
            }
            break;
    }

    *curvaOriginal = curva;
    *carPosXOriginal = carPosX, *carPosZOriginal = carPosZ, *rotationOriginal = rotation;
}

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}

