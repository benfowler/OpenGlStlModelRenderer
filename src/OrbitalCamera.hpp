#ifndef ORBITAL_CAMERA_H
#define ORBITAL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Small constant for clamping to avoid gimbal lock at poles
const float EPSILON = 0.001f;

class OrbitalCamera {
public:
    // Spherical coordinates
    float radius;
    float azimuthAngle; // Theta (in radians)
    float polarAngle;   // Phi (in radians)

    // Camera options
    float mouseSensitivity;
    float zoomSensitivity;

    OrbitalCamera(float initRadius = 5.0f, float initAzimuth = 0.0f, float initPolar = 0.0f)
        : radius(initRadius),
          azimuthAngle(initAzimuth),
          polarAngle(initPolar),
          mouseSensitivity(0.005f),
          zoomSensitivity(0.5f) {}

    // Processes mouse movement to change the azimuthal and polar angles
    void ProcessMouseMovement(float xoffset, float yoffset) {
        azimuthAngle += xoffset * mouseSensitivity;
        polarAngle -= yoffset * mouseSensitivity; // Y is inverted

        // Clamp the polar angle to prevent flipping
        // The range is (-PI/2, PI/2)
        if (polarAngle > (M_PI / 2.0f) - EPSILON) {
            polarAngle = (M_PI / 2.0f) - EPSILON;
        }
        if (polarAngle < (-M_PI / 2.0f) + EPSILON) {
            polarAngle = (-M_PI / 2.0f) + EPSILON;
        }
    }

    // Processes mouse scroll to change the radius (zoom)
    void ProcessMouseScroll(float yoffset) {
        radius -= yoffset * zoomSensitivity;
        // Clamp the radius to prevent going inside the model or too far away
        if (radius < 1.0f) {
            radius = 1.0f;
        }
        if (radius > 50.0f) {
            radius = 50.0f;
        }
    }

    // Calculates the Cartesian position of the camera from its spherical coordinates
    glm::vec3 getPosition() const {
        float x = radius * std::cos(polarAngle) * std::cos(azimuthAngle);
        float y = radius * std::sin(polarAngle);
        float z = radius * std::cos(polarAngle) * std::sin(azimuthAngle);
        return glm::vec3(x, y, z);
    }


    // Returns the view matrix calculated from the camera's spherical coordinates
    glm::mat4 GetViewMatrix() const {
        glm::vec3 position = getPosition();
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // Always look at the origin
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);     // World's up direction

        return glm::lookAt(position, target, up);
    }

    // Returns the current zoom level for use in perspective calculation
    float getZoom() const {
        // We can keep a fixed zoom for the perspective matrix, as the "zoom" effect
        // is handled by changing the radius. 45 degrees is a standard field of view.
        return 45.0f;
    }
};

#endif
