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

// Funções para compilar shaders
GLuint compileShader(const char* vertexSource, const char* fragmentSource);
void checkShaderCompileStatus(GLuint shader);
void checkProgramLinkStatus(GLuint program);
// Processar eventos de teclado
void processInput(GLFWwindow *window, float *x, float *y, float *z);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void moveCar(int *curva, float *carPosX, float *carPosZ);

// Shaders (cada objeto terá um shader próprio)
const char* vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec3 position;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
    "}\n";

const char* fragmentShaderSource1 = "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "   color = vec4(1.0, 0.0, 0.0, 0.0); // Vermelho\n"
    "}\n";

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Dados dos objetos (chao e carro)
float groundVertices[] = {
    /*
    -0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
    */
    -3.0f, 0.0f,  -3.0f,  0.0f, 1.0f,
    3.0f, 0.0f, -3.0f,  1.0f, 1.0f,
    3.0f, 0.0f,  3.0f,  1.0f, 0.0f,
    3.0f, 0.0f,  3.0f,  1.0f, 0.0f,
   -3.0f, 0.0f, 3.0f,  0.0f, 0.0f,
    -3.0f, 0.0f, -3.0f,  0.0f, 1.0f
};

float cubeVertices[] = {
    //Traseira
    -0.5f, 0.0f, 0.5f, 0.0f, 0.5f,
     0.5f, 0.0f, 0.5f, 0.25f, 0.5f,
     0.5f, 0.5f, 0.5f, 0.25f, 0.75f,
     0.5f, 0.5f, 0.5f, 0.25f, 0.75f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.75f,
    -0.5f, 0.0f, 0.5f, 0.0f, 0.5f,

    //Frontal
    -0.5f, 0.0f,  1.5f, 0.0f, 0.25f,
     0.5f, 0.0f,  1.5f, 0.25f, 0.25f,
     0.5f, 0.5f,  1.5f, 0.25f, 0.5f,
     0.5f, 0.5f,  1.5f, 0.25f, 0.5f,
    -0.5f, 0.5f,  1.5f, 0.0f, 0.5f,
    -0.5f, 0.0f,  1.5f, 0.0f, 0.25f,

    //Lateral Esquerda
    -0.5f, 0.5f,  1.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.25f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.25f, 0.25f,
    -0.5f, 0.0f, 0.5f, 0.25f, 0.25f,
    -0.5f, 0.0f,  1.5f, 0.0f, 0.25f,
    -0.5f, 0.5f,  1.5f, 0.0f, 0.0f,

    //Lateral Direita
     0.5f, 0.5f, 1.5f, 0.25f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.0f,
     0.5f, 0.0f, 0.5f, 0.5f, 0.25f,
     0.5f, 0.0f, 0.5f, 0.5f, 0.25f,
     0.5f, 0.0f, 1.5f, 0.25f, 0.25f,
     0.5f, 0.5f, 1.5f, 0.25f, 0.0f,

    //Inferior
    -0.5f, 0.0f, 0.5f, 0.25f, 0.5f,
     0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
     0.5f, 0.0f, 1.5f, 0.5f, 0.75f,
     0.5f, 0.0f, 1.5f, 0.5f, 0.75f,
    -0.5f, 0.0f, 1.5f, 0.25f, 0.75f,
    -0.5f, 0.0f, 0.5f, 0.25f, 0.5f,

    //Superior
    -0.5f, 0.5f, 0.5f, 0.25f, 0.25f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.25f,
     0.5f, 0.5f, 1.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 1.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 1.5f, 0.25f, 0.5f,
    -0.5f, 0.5f, 0.5f, 0.25f, 0.25f
};

float factor = 0.001f;
float carPosX = 0.0f;
float carPosZ = 0.0f;

int curva = 1;

bool visaoAerea = false;

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

    // Compilar os shaders para cada objeto
    GLuint shaderCar = compileShader(vertexShaderSource, fragmentShaderSource1);

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

    unsigned int groundTexture, carTexture;

    //Definindo a textura do chão
    glGenTextures(1, &groundTexture);
    glBindTexture(GL_TEXTURE_2D, groundTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("res/images/pista3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Falha ao carregar a textura" << std::endl;
    }
    stbi_image_free(data);

    groundShader.use();
    groundShader.setInt("textureToAplicate", 0);

    //Carro
    glGenTextures(1, &carTexture);
    glBindTexture(GL_TEXTURE_2D, carTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("res/images/carro3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Falha ao carregar a textura" << std::endl;
    }
    stbi_image_free(data);

    carShader.use();
    carShader.setInt("textureToAplicate", 0);

    glEnable(GL_DEPTH_TEST);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {

        // Atender os eventos
        processInput(window, &x, &y, &z);

        // Limpar a tela
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar cada objeto com seu shader
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.5f, 0.0f));
        if(visaoAerea){
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
            projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        } else {
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.5f, 0.0f));
            view  = glm::translate(view, glm::vec3(0.5f, -1.75f, -6.35f));
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        }


        glActiveTexture(GL_TEXTURE0);

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
        moveCar(&curva, &carPosX, &carPosZ);
        model = glm::translate(model, glm::vec3(carPosX, 0.0f, carPosZ));
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
GLuint compileShader(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileStatus(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileStatus(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinkStatus(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

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
void processInput(GLFWwindow *window, float *x, float *y, float *z)
{
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

void moveCar(int *curvaOriginal, float *carPosXOriginal, float *carPosZOriginal){
    int curva = *curvaOriginal;
    float carPosX = *carPosXOriginal, carPosZ = *carPosZOriginal;

    //std::cout << "X: " << carPosX * factor << " Z: " << carPosZ << std::endl;
    switch (curva) {
        case 1:
            carPosX -= 1.0f * factor;
            if (carPosX <= -2.5f) {
                curva = 2;
            }
            break;

        case 2:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= -2.405f) {
                curva = 3;
            }
            break;

        case 3:
            carPosX += 1.0f * factor;
            if (carPosX >= -1.5f) {
                curva = 4;
            }
            break;

        case 4:
            carPosZ += 1.0f * factor;
            if (carPosZ >= -1.925f) {
                curva = 5;
            }
            break;

        case 5:
            carPosZ += 1.0f * factor;
            carPosX -= 0.5f * factor;
            if (carPosX <= -1.895f && carPosZ >= -1.135f) {
                curva = 6;
            }
            break;

        case 6:
            carPosZ += 0.5f * factor;
            carPosX += 1.0f * factor;
            if (carPosX >= -1.185f && carPosZ >= -0.78f) {
                curva = 7;
            }
            break;

        case 7:
            carPosZ -= 1.0f * factor;
            carPosX += 0.5f * factor;
            if (carPosX >= -0.495f && carPosZ <= -2.16f) {
                curva = 8;
            }
            break;

        case 8:
            carPosX += 1.0f * factor;
            if (carPosX >= 0.05f) {
                curva = 9;
            }
            break;

        case 9:
            carPosZ += 1.0f * factor;
            if (carPosZ >= -0.85f) {
                curva = 10;
            }
            break;

        case 10:
            carPosX += 1.0f * factor;
            if (carPosX >= 1.05f) {
                curva = 11;
            }
            break;

        case 11:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= -2.05f) {
                curva = 12;
            }
            break;

        case 12:
            carPosX += 1.0f * factor;
            if (carPosX >= 1.85f) {
                curva = 13;
            }
            break;

        case 13:
            carPosZ += 1.0f * factor;
            if (carPosZ >= 1.5f) {
                curva = 14;
            }
            break;

        case 14:
            carPosX -= 1.0f * factor;
            if (carPosX <= 1.075f) {
                curva = 15;
            }
            break;

        case 15:
            carPosZ -= 1.0f * factor;
            if (carPosZ <= 0.0f) {
                curva = 1;
            }
            break;
    }

    *curvaOriginal = curva;
    *carPosXOriginal = carPosX, *carPosZOriginal = carPosZ;
}
