#include "vertices.h"

std::vector<float> generateVerticesGround() {
    return {
        -3.0f,  0.0f, -3.0f,  0.0f, 1.0f,
         3.0f,  0.0f, -3.0f,  1.0f, 1.0f,
         3.0f,  0.0f,  3.0f,  1.0f, 0.0f,
         3.0f,  0.0f,  3.0f,  1.0f, 0.0f,
        -3.0f,  0.0f,  3.0f,  0.0f, 0.0f,
        -3.0f,  0.0f, -3.0f,  0.0f, 1.0f
    };
}

std::vector<float> generateVerticesCar(){
    return {
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
}
