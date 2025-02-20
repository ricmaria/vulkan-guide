#include <camera.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Camera::init(const glm::vec3& position, const glm::vec3& velocity, float pitch, float yaw)
{
	_position = position;
	_velocity = velocity;
	_pitch = pitch;
	_yaw = yaw;
}

void Camera::update()
{
	glm::mat4 cameraRotation = get_rotation_matrix();
	_position += glm::vec3(cameraRotation * glm::vec4(_velocity * 0.5f, 0.f));
}

void Camera::process_sdl_event(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_w) { _velocity.z = -1; }
		if (e.key.keysym.sym == SDLK_s) { _velocity.z = 1; }
		if (e.key.keysym.sym == SDLK_a) { _velocity.x = -1; }
		if (e.key.keysym.sym == SDLK_d) { _velocity.x = 1; }
		if (e.key.keysym.sym == SDLK_q) { _velocity.y = -1; }
		if (e.key.keysym.sym == SDLK_e) { _velocity.y = 1; }
	}

	if (e.type == SDL_KEYUP)
	{
		if (e.key.keysym.sym == SDLK_w) { _velocity.z = 0; }
		if (e.key.keysym.sym == SDLK_s) { _velocity.z = 0; }
		if (e.key.keysym.sym == SDLK_a) { _velocity.x = 0; }
		if (e.key.keysym.sym == SDLK_d) { _velocity.x = 0; }
		if (e.key.keysym.sym == SDLK_q) { _velocity.y = 0; }
		if (e.key.keysym.sym == SDLK_e) { _velocity.y = 0; }
	}

	if (e.type == SDL_MOUSEMOTION)
	{
		_yaw += (float)e.motion.xrel / 300.f;
		_pitch -= (float)e.motion.yrel / 300.f;
	}
}

glm::mat4 Camera::get_view_matrix()
{
	// to create a correct model view, we need to move the world in opposite
	// direction to the camera
	//  so we will create the camera model matrix and invert
	glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.f), _position);
	glm::mat4 cameraRotation = get_rotation_matrix();
	return glm::inverse(cameraTranslation * cameraRotation);
}

glm::mat4 Camera::get_rotation_matrix()
{
	// fairly typical FPS style camera. we join the pitch and yaw rotations into
	// the final rotation matrix

	glm::quat pitchRotation = glm::angleAxis(_pitch, glm::vec3{ 1.f, 0.f, 0.f });
	glm::quat yawRotation = glm::angleAxis(_yaw, glm::vec3{ 0.f, -1.f, 0.f });

	return glm::toMat4(yawRotation) * glm::toMat4(pitchRotation);
}