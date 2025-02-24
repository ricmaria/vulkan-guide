
#include <vk_types.h>
#include <SDL_events.h>

class Camera
{
public:
	glm::mat4 get_view_matrix();
	glm::mat4 get_rotation_matrix();

	inline const glm::vec3& get_position() { return _position; }

	void init(const glm::vec3& position, const glm::vec3& velocity, float pitch, float yaw);

	void process_sdl_event(SDL_Event& e);

	void update();

private:
	glm::vec3 _position;
	glm::vec3 _velocity;
	// vertical rotation
	float _pitch{ 0.f };
	// horizontal rotation
	float _yaw{ 0.f };
};