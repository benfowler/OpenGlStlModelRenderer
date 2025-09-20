#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include "glm/glm.hpp"

// We need to include the STLLoader directly.
// This is a bit of a hack for a simple test setup, but works.
#include "../src/STLLoader.hpp"

// Test fixture for creating temporary files
class STLLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary ASCII STL file for testing
        ascii_filename = "temp_ascii_test.stl";
        std::ofstream ascii_file(ascii_filename);
        ascii_file << "solid test_shape\n";
        ascii_file << "  facet normal 0.0 0.0 1.0\n";
        ascii_file << "    outer loop\n";
        ascii_file << "      vertex 0.0 0.0 0.0\n";
        ascii_file << "      vertex 1.0 0.0 0.0\n";
        ascii_file << "      vertex 0.0 1.0 0.0\n";
        ascii_file << "    endloop\n";
        ascii_file << "  endfacet\n";
        ascii_file << "endsolid test_shape\n";
        ascii_file.close();

        // Create a temporary Binary STL file for testing
        binary_filename = "temp_binary_test.stl";
        std::ofstream binary_file(binary_filename, std::ios::binary);
        char header[80] = {0}; // 80-byte header
        binary_file.write(header, 80);
        uint32_t num_triangles = 1;
        binary_file.write(reinterpret_cast<const char*>(&num_triangles), sizeof(uint32_t));
        // Triangle 1
        float normal[3] = {0.0f, 0.0f, 1.0f};
        float v1[3] = {0.0f, 0.0f, 0.0f};
        float v2[3] = {1.0f, 0.0f, 0.0f};
        float v3[3] = {0.0f, 1.0f, 0.0f};
        uint16_t attr = 0;
        binary_file.write(reinterpret_cast<const char*>(normal), 12);
        binary_file.write(reinterpret_cast<const char*>(v1), 12);
        binary_file.write(reinterpret_cast<const char*>(v2), 12);
        binary_file.write(reinterpret_cast<const char*>(v3), 12);
        binary_file.write(reinterpret_cast<const char*>(&attr), 2);
        binary_file.close();
    }

    void TearDown() override {
        std::remove(ascii_filename.c_str());
        std::remove(binary_filename.c_str());
    }

    std::string ascii_filename;
    std::string binary_filename;
};

TEST_F(STLLoaderTest, CanLoadAsciiStl) {
    std::vector<float> vertices;
    bool success = loadSTL(ascii_filename, vertices);
    ASSERT_TRUE(success);

    // 1 triangle * 3 vertices/triangle * 6 floats/vertex (3 pos + 3 normal)
    ASSERT_EQ(vertices.size(), 1 * 3 * 6);

    // Check first vertex data
    EXPECT_FLOAT_EQ(vertices[0], 0.0f); // v1.x
    EXPECT_FLOAT_EQ(vertices[1], 0.0f); // v1.y
    EXPECT_FLOAT_EQ(vertices[2], 0.0f); // v1.z
    EXPECT_FLOAT_EQ(vertices[3], 0.0f); // n.x
    EXPECT_FLOAT_EQ(vertices[4], 0.0f); // n.y
    EXPECT_FLOAT_EQ(vertices[5], 1.0f); // n.z

    // Check second vertex data
    EXPECT_FLOAT_EQ(vertices[6], 1.0f); // v2.x
}

TEST_F(STLLoaderTest, CanLoadBinaryStl) {
    std::vector<float> vertices;
    bool success = loadSTL(binary_filename, vertices);
    ASSERT_TRUE(success);

    // 1 triangle * 3 vertices/triangle * 6 floats/vertex (3 pos + 3 normal)
    ASSERT_EQ(vertices.size(), 1 * 3 * 6);

    // Check first vertex data
    EXPECT_FLOAT_EQ(vertices[0], 0.0f); // v1.x
    EXPECT_FLOAT_EQ(vertices[1], 0.0f); // v1.y
    EXPECT_FLOAT_EQ(vertices[2], 0.0f); // v1.z
    EXPECT_FLOAT_EQ(vertices[3], 0.0f); // n.x
    EXPECT_FLOAT_EQ(vertices[4], 0.0f); // n.y
    EXPECT_FLOAT_EQ(vertices[5], 1.0f); // n.z

    // Check third vertex data
    EXPECT_FLOAT_EQ(vertices[12], 0.0f); // v3.x
    EXPECT_FLOAT_EQ(vertices[13], 1.0f); // v3.y
}

TEST_F(STLLoaderTest, FailsOnNonExistentFile) {
    std::vector<float> vertices;
    bool success = loadSTL("non_existent_file.stl", vertices);
    ASSERT_FALSE(success);
}
