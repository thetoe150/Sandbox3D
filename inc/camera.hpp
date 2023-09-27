#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const glm::vec3 POSITION(0.f, 0.f, 0.f);
const glm::vec3 FRONT(0.f, 0.f, 1.f);
const glm::vec3 WORLD_UP(0.f, 1.f, 0.f);

const float YAW				   = 90.f;
const float PITCH			   = 0.f;

const float MOVEMENT_SPEED	   = 15.0f;
const float MOUSE_MOVE_SPEED   = 0.1f;
const float MOUSE_SCROLL_SPEED = 10.f;
const float ZOOM			   = 45.f;

class Camera
{
public:
	// constructors
	// -------------------------------------------------------------------------
	Camera(glm::vec3 position = POSITION, glm::vec3 front = FRONT,
		  glm::vec3 up = WORLD_UP, float yaw = YAW, float pitch = PITCH)
		  : m_position(position), m_front(front), m_worldUp(up),
			m_yaw(yaw), m_pitch(pitch), m_zoom(ZOOM)
	{
		updateCameraVectors();
	}

	// Lookat matrix
	// -------------------------------------------------------------------------
	glm::mat4 getLookAtMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	// get zoom 
	// -------------------------------------------------------------------------
	float getZoom()
	{
		return m_zoom;
	}

	// get position 
	// -------------------------------------------------------------------------
	glm::vec3 getPosition()
	{
		return m_position;
	}

	// get front 
	// -------------------------------------------------------------------------
	glm::vec3 getFront()
	{
		return m_front;
	}

	// Keyboard input
	// -------------------------------------------------------------------------
	void processKeyboard(CameraMovement direction, float offset)
	{
		float speed = offset * MOVEMENT_SPEED;		
		switch(direction)
		{
			case CameraMovement::FORWARD:
			{
				m_position += m_front * speed;
				break;
			}
			case CameraMovement::BACKWARD:
			{
				m_position -= m_front * speed;
				break;
			}
			case CameraMovement::UP:
			{
				m_position += m_up * speed;
				break;
			}
			case CameraMovement::DOWN:
			{
				m_position -= m_up * speed;
				break;
			}
			case CameraMovement::RIGHT:
			{
				m_position += m_right * speed;
				break;
			}
			case CameraMovement::LEFT:
			{
				m_position -= m_right * speed;
				break;
			}
		}
	}

	// Mouse Movement input
	// -------------------------------------------------------------------------
	void processMouseMovement(float xoffset, float yoffset, 
						bool constrainPitch = true)
	{
		float delta_yaw = xoffset * MOUSE_MOVE_SPEED;
		float delta_pitch = yoffset * MOUSE_MOVE_SPEED;

		m_yaw += delta_yaw;
		m_pitch += delta_pitch;

		if(constrainPitch)
		{
			if(m_pitch > 89.f)
				m_pitch = 89.f;
			if(m_pitch < -89.f)
				m_pitch = -89.f;
		}

		updateCameraVectors();
	}
	
	// Mouse scroll input
	// -------------------------------------------------------------------------
	void processMouseScroll(float yoffset)
	{
		m_zoom -= MOUSE_SCROLL_SPEED * yoffset;
		if(m_zoom < 1.f)
			m_zoom = 1.f;
		if(m_zoom > 45.f)
			m_zoom = 45.f;
	}
	
	void print()
	{
		std::cout << "m_position x: " << m_position.x << std::endl;
		std::cout << "m_position y: " << m_position.y << std::endl;
		std::cout << "m_position z: " << m_position.z << std::endl;

		std::cout << "m_front x: " << m_front.x << std::endl;
		std::cout << "m_front y: " << m_front.y << std::endl;
		std::cout << "m_front z: " << m_front.z << std::endl;
	}
	
private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.y = glm::sin(glm::radians(m_pitch));
		float xz = glm::cos(glm::radians(m_pitch));
		front.x = xz * glm::cos(glm::radians(m_yaw));
		front.z = xz * glm::sin(glm::radians(m_yaw));

		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

private:
	glm::vec3 m_position;

	glm::vec3 m_front;
	glm::vec3 m_right = glm::vec3(0.f);
	glm::vec3 m_up = glm::vec3(0.f);
	glm::vec3 m_worldUp;
	
	float m_yaw;
	float m_pitch;
	
	float m_zoom;
};

