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
const glm::vec3 FRONT(0.f, 1.f, 0.f);
const glm::vec3 WORLD_UP(0.f, 1.f, 0.f);

const float YEW				   = 0.f;
const float PITCH			   = 0.f;

const float MOVEMENT_SPEED	   = 10.f;
const float MOUSE_MOVE_SPEED   = 10.f;
const float MOUSE_SCROLL_SPEED = 10.f;
const float ZOOM			   = 45.f;

class Camera
{
public:
	// constructors
	// -------------------------------------------------------------------------
	Camera(glm::vec3 position = POSITION, glm::vec3 front = FRONT,
		  glm::vec3 up = WORLD_UP, float yew = YEW, float pitch = PITCH)
		  : m_position(position), m_front(front), m_worldUp(up),
			m_yew(YEW), m_pitch(PITCH)
	{
		glm::vec3 hehe(0.1f);	

	}

	// Lookat matrix
	// -------------------------------------------------------------------------
	glm::mat3 getLookAtMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	// Keyboard input
	// -------------------------------------------------------------------------
	void processKeyBoard(CameraMovement direction, float offset)
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
				m_position += m_front * speed;
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
		float delta_yew = xoffset * MOUSE_MOVE_SPEED;
		float delta_pitch = yoffset * MOUSE_MOVE_SPEED;

		m_yew += delta_yew;
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
	
private:
	glm::vec3 m_position;

	glm::vec3 m_front;
	glm::vec3 m_right = glm::vec3(0.f);
	glm::vec3 m_up = glm::vec3(0.f);
	glm::vec3 m_worldUp;
	
	float m_yew;
	float m_pitch;
	
	float m_zoom;

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.y = glm::sin(glm::radians(m_pitch));
		float xz = glm::cos(glm::radians(m_pitch));
		front.x = xz * glm::cos(glm::radians(m_yew));
		front.z = xz * glm::sin(glm::radians(m_yew));

		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
};

