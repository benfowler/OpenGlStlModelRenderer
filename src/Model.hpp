#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "STLLoader.hpp" // Needs the STL loading function

class Model {
public:
    // OpenGL render data
    unsigned int VAO, VBO;
    int vertexCount;

    // Model transformation properties in world space
    glm::vec3 position;
    glm::mat4 rotation;
    glm::vec3 scale;

    // Constructor that loads the model from a given STL file path
    Model(const std::string& path)
        : position(0.0f, 0.0f, 0.0f),
          rotation(1.0f),
          scale(1.0f, 1.0f, 1.0f),
          VAO(0), VBO(0), vertexCount(0)
    {
        std::vector<float> vertices;
        if (!loadSTL(path, vertices)) {
            std::cerr << "ERROR::MODEL::Failed to load model from path: " << path << std::endl;
            return;
        }

        // Each vertex has a position (3) and a normal (3), so 6 floats total
        vertexCount = vertices.size() / 6;

        // Create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        // Load data into vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        // Unbind the VAO
        glBindVertexArray(0);
    }

    // Destructor to clean up OpenGL resources
    ~Model() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
    }

    // Calculates and returns the final model matrix by combining scale, rotation, and translation
    glm::mat4 getModelMatrix() const {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = modelMatrix * rotation;
        modelMatrix = glm::scale(modelMatrix, scale);
        return modelMatrix;
    }

    // Renders the model
    void draw() const {
        if (VAO != 0) {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
            glBindVertexArray(0);
        }
    }
};

#endif
