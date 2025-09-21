#ifndef STL_LOADER_H
#define STL_LOADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Reads a 3D vector from a binary stream
inline glm::vec3 read_vec3(std::ifstream& in) {
    float x, y, z;
    in.read(reinterpret_cast<char*>(&x), sizeof(float));
    in.read(reinterpret_cast<char*>(&y), sizeof(float));
    in.read(reinterpret_cast<char*>(&z), sizeof(float));
    return glm::vec3(x, y, z);
}

// Loads an STL file (binary or ASCII) into a vector of floats (pos, normal)
// Returns true on success
inline bool loadSTL(const std::string& filename, std::vector<float>& vertices) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "ERROR::STL_LOADER::COULD_NOT_OPEN_FILE: " << filename << std::endl;
        return false;
    }

    // Check for ASCII or binary
    char header[80];
    file.read(header, 80);
    std::string headerStr(header, 80);

    if (headerStr.find("solid") != std::string::npos && headerStr.find("\n") != std::string::npos) {
        // --- ASCII STL ---
        file.close();
        file.open(filename);
        if(!file) return false;

        std::string line;
        glm::vec3 normal;
        std::vector<glm::vec3> face_vertices;

        // Skip 'solid <name>'
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "endsolid") {
                break;
            }
            if (token == "facet") {
                ss >> token; // "normal"
                ss >> normal.x >> normal.y >> normal.z;
                face_vertices.clear();
            } else if (token == "vertex") {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                face_vertices.push_back(vertex);
            } else if (token == "endfacet") {
                if (face_vertices.size() == 3) {
                    for(int i = 0; i < 3; ++i) {
                        vertices.push_back(face_vertices[i].x);
                        vertices.push_back(face_vertices[i].y);
                        vertices.push_back(face_vertices[i].z);
                        vertices.push_back(normal.x);
                        vertices.push_back(normal.y);
                        vertices.push_back(normal.z);
                    }
                }
            }
        }
    } else {
        // --- Binary STL ---
        uint32_t num_triangles;
        file.read(reinterpret_cast<char*>(&num_triangles), sizeof(uint32_t));

        vertices.reserve(num_triangles * 3 * 6); // 3 vertices per triangle, 6 floats per vertex

        for (uint32_t i = 0; i < num_triangles; ++i) {
            glm::vec3 normal = read_vec3(file);
            glm::vec3 v1 = read_vec3(file);
            glm::vec3 v2 = read_vec3(file);
            glm::vec3 v3 = read_vec3(file);
            uint16_t attribute_byte_count;
            file.read(reinterpret_cast<char*>(&attribute_byte_count), sizeof(uint16_t));

            // Vertex 1
            vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z);
            vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
            // Vertex 2
            vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
            vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
            // Vertex 3
            vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
            vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
        }
    }

    file.close();
    return true;
}

#endif
