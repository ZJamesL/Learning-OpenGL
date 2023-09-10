#include "Camera.h"







/*
* Camera constructor: takes in a position, target position, world up (0,1,0), and a pitch and yaw
*		It sets camWorldUp to a default value. Pitch and yaw are set to default arguments that 
*		match up with a default target value (0,1,0). Other variables, like 
*		zoom, sensitivity, speed are also initialized to default values. At the end it calls 
*		updateTarget to initialize the up (not world up), right, and target vectors. if pitch and yaw
*		don't have default values then we can update the target by the call at the end. 
* params:
*	vec3 camPosition - position of the camera
*	vec3 camTarget   - target of the camera 
*/
Camera::Camera(glm::vec3 camPosition, glm::vec3 camTarget, glm::vec3 camWorldUp, float camPitch, float camYaw, 
		float camSpeed, float camSensitivity, float camZoom) {
	position    = camPosition;
	target      = camTarget;
	worldUp     = camWorldUp;
	pitch       = camPitch;
	yaw         = camYaw;
	sensitivity = SENSITIVITY;
	zoom        = ZOOM;
	speed	    = SPEED;
	updateTarget();
}

/*
* updatePosition: updates the position of the camera based off inputted delta time, 
*			   and movement type
* params:
*	- int movementType - int 1-4 for up, down, left, right respectively 
*	- float deltaTime  - delta time between current frame and previous
*/
void Camera::updatePosition(int movementType, float deltaTime) {
	float velocity = speed * deltaTime;
	switch (movementType) {
		case 1: // forwards
			position += target * velocity;
			break;
		case 2: // backwards
			position -= target * velocity;
			break;
		case 3: // right
			position += right * velocity;
			break;
		case 4:
			position -= right * velocity;
			break;
		default:
			break;
	}
}

void Camera::handleMouseUpdate(float xoffset, float yoffset) {
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitch += yoffset;
	yaw   += xoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateTarget();
}


void Camera::updateTarget() {
	// update the target position 
	glm::vec3 newTarget;
	newTarget.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newTarget.y = sin(glm::radians(pitch));
	newTarget.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	target = glm::normalize(newTarget);

	// update the up and right 
	right = glm::normalize(glm::cross(target, worldUp));
	up = glm::normalize(glm::cross(right, target));
}

glm::mat4 Camera::getLookAtMatrix() {
	return glm::lookAt(position, position + target, up);
}

void Camera::handleZoomUpdate(float yoffset) {
	zoom -= yoffset;
	printf("yoffset = %f, zoom = %f\n", yoffset, zoom);
	if (zoom < 1.0f)
		zoom =  1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

float Camera::getZoom() {
	return zoom;
}