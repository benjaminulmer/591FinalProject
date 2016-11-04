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

	float angleDeg = longitudeRotRad * M_PI/180;
	glm::vec3 eyeTemp = glm::rotateY(eye, angleDeg);
	//glm::rotate()

	return glm::lookAt(eyeTemp, centre, up);
}

void Camera::updateLongitudeRotation(float rad) {
	longitudeRotRad += rad;
}

void Camera::updateLatitudeRotation(float rad) {
	latitudeRotRad += rad;
}

void Camera::updatePosition(glm::vec3 value) {
	eye += value;
}
