#include "Camera.h"

Camera::Camera() {
	eye = glm::vec3(0.0f,0.0f, 20.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	centre = glm::vec3(0.0f, 0.0f, 0.0f);

	longitudeRotRad = 0;
	latitudeRotRad = 0;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

glm::mat4 Camera::getView() {
	glm::vec3 eyeTemp = glm::rotateY(eye, -longitudeRotRad);
	eyeTemp = glm::rotate(eyeTemp, latitudeRotRad, glm::cross(eyeTemp, glm::vec3(0.0, 1.0, 0.0)));

	return glm::lookAt(eyeTemp, centre, up);
}

void Camera::updateLongitudeRotation(float rad) {
	longitudeRotRad += rad * M_PI/180;
}

void Camera::updateLatitudeRotation(float rad) {
	latitudeRotRad += rad * M_PI/180;
	if (latitudeRotRad > M_PI/2 - 0.1f) {
		latitudeRotRad = M_PI/2 - 0.1f;
	}
	else if (latitudeRotRad < -M_PI/2 + 0.1f) {
		latitudeRotRad = -M_PI/2 + 0.1f;
	}
}

void Camera::updatePosition(glm::vec3 value) {
	eye += value;
}
