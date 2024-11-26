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
    void processInput(GLFWwindow *window);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // Dados dos objetos (chao e carro)
    std::vector<float> groundVertices = generateVerticesGround();
    std::vector<float> cubeVertices = generateVerticesCar();

    float carSpeed = 0.0005f;
    float rotateSpeed = 0.05f;

    float carPosX = 1.05f;
    float carPosZ = 1.85f;
    float rotation = 0.0f;

    int visao = 1;

    int main() {
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
        glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(groundVertices), groundVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Atribuir coordenadas de textura
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Cubo
        glBindVertexArray(VAOs[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(cubeVertices), cubeVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Atribuir coordenadas de textura
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        Shader groundShader("vertex.glsl", "fragment.glsl");
        Shader carShader("vertex.glsl", "fragment.glsl");

        unsigned int
            groundTexture = loadTexture("res/images/pista-quadrada.png"),
            carTexture = loadTexture("res/images/kombi.png");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        // Loop de renderização
        while (!glfwWindowShouldClose(window)) {
            // Atender os eventos
            processInput(window);

            // Limpar a tela
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model         = glm::mat4(1.0f);
            glm::mat4 view          = glm::mat4(1.0f);
            glm::mat4 projection    = glm::mat4(1.0f);
            if(visao == 0){             //Visão de cima
                model       = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                view        = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
                projection  = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            } else if (visao == 1) {    //Visão em diagonal
                model       = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.5f, 0.0f));
                view        = glm::translate(view, glm::vec3(0.5f, -1.75f, -7.5f));
                projection  = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            } else if (visao = 3) {     //Visão seguindo o carro
                glm::vec3 cameraOffset(0.0f, 1.5f, 4.0f);
                glm::vec3 cameraPosition = glm::vec3(
                    carPosX - cameraOffset.z * -cos(glm::radians(rotation)),
                    cameraOffset.y,
                    carPosZ - cameraOffset.z * sin(glm::radians(rotation))
                );
                glm::vec3 cameraTarget = glm::vec3(carPosX, 0.2f, carPosZ);
                glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

                view = glm::lookAt(cameraPosition, cameraTarget, upDirection);
                projection = glm::perspective(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            }

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
            model = glm::translate(model, glm::vec3(carPosX, 0.0f, carPosZ));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2f));
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

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Movimento do carro
        bool keyPress = false;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            carPosZ += carSpeed * sin(glm::radians(rotation));
            carPosX -= carSpeed * cos(glm::radians(rotation));
            keyPress = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            carPosZ -= carSpeed * sin(glm::radians(rotation));
            carPosX += carSpeed * cos(glm::radians(rotation));
            keyPress = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotation += rotateSpeed; // Gira para a esquerda
            if (rotation > 360.0f) rotation = 0.0f;
            keyPress = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotation -= rotateSpeed; // Gira para a direita
            if (rotation < 0.0f) rotation = 360.0f;
            keyPress = true;
        }

        if(keyPress) {
            std::cout << "Angulo: " << rotation << ", PosX: " << carPosX << ", PosZ: " << carPosZ << std::endl;
            keyPress = false;
        }

        //Ajuste da velocidade e rotação
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            carSpeed += 0.00001f;
            keyPress = true;
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (carSpeed > 0.0f){
                carSpeed -= 0.00001f;
                keyPress = true;
            } else {
                carSpeed = 0.0f;
            }
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            rotateSpeed += 0.001f;
            keyPress = true;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            if (rotateSpeed > 0.0f){
                rotateSpeed -= 0.001f;
                keyPress = true;
            } else {
                rotateSpeed = 0.0f;
            }
        }

        if(keyPress) {
            std::cout << "Veolcidade do movimento: " << carSpeed << ", Velocidade da rotação: " << rotateSpeed << std::endl;
            keyPress = false;
        }

        //Definição do tipo de visão
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            visao = 0;
        } else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            visao = 1;
        } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            visao = 2;
        }
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

