#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// default camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  45.0f;
const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
	private:
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 worldUp; 
		glm::vec3 up;
		glm::vec3 right;

		// angles
		float yaw;
		float pitch;

		// camera control 
		float zoom;
		float sensitivity; 
		float speed;

	public:
		/// <summary>
		/// camera needs to take in the position, target, pitch, and yaw. Then it needs to set 
		/// the zoom, sensitivity, speed, up and right. The up and right need to be calculated from 
		/// cross products. 
		///		note:
		/// </summary>
		Camera(glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 camWorldUp = WORLDUP,
			float camPitch = PITCH, float camYaw = YAW, float camSpeed = SPEED, float camSensitivity = SENSITIVITY, float camZoom = ZOOM);

		///
		///
		/// What else do we need?
		///		- function to update position
		///			- we need to move forward back left right. We just need to take in the 
		///			  delta time from the main and some sort of var that tells us what dir we are moving 
		///		- function to update mouse look / camera direction
		///			- mouse look depends on the x/yposition of the mouse on the screen. In the tutorial 
		///			  it shows how to get the x and y offset. We can pass those to this function and then 
		///			  just do what it says to do in that function. We make 
		///		- function to update the target/directoin we are looking in. 
		///			- the direction or target we are looking at is dependent on the pitch and yaw. We create 
		///			  the vector for the target from the calculations of the target vector. So we can just move 
		///			  those calculations to this function. When this takes place we also have to update the up (not world) 
		///			  vector since that will have changed and right vector. 
		///			- something to note:
		///				- I've already seen the code BUT I didn't realize why this function would be called initially instead 
		///				  of just initializing the right and up to default values, these operations are done over and over 
		///				  again each time we look and it makes no difference if we just call a function that does that for us. 
		///				- it also helps with code reduction 
		///				- worldUp can just straight up be initialized I don't think there's a downside and if there is I can change ltr
		///		- function to give us a look at matrix 
		///			this will only require a positoin, position + target, worldUp? I think 
		
		void updatePosition(int movemnetDir, float deltaTime); // handles frame data / delta time from main
		void handleMouseUpdate(float xoffset, float yoffset); // is called when mouse changes. takes in mouse information from main 
		void handleZoomUpdate(float yoffset); // called by main offset 
		void updateTarget(); // this should be called by constructor and handle mouse input. 
		glm::mat4 getLookAtMatrix(); // this is the view matrix 

		// getters 
		float getZoom();
};

#endif