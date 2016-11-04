#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <math.h>

class Camera {

public:
	Camera();
	virtual ~Camera();

	glm::mat4 getView();
	void updateLongitudeRotation(float rad);
	void updateLatitudeRotation(float rad);
	void updatePosition(glm::vec3 value);

private:
	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 centre;

	float longitudeRotRad;
	float latitudeRotRad;
};