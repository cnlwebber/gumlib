// ReSharper disable CppMissingSpace
module;

#include <glm/glm.hpp>

export module gumlib:config;
import std;

export namespace gumlib
{
	using entity			= std::uint16_t;
	using sparse_type		= entity;
	using component_type	= std::uint8_t;

	constexpr inline component_type max_components = 64;
	// 65534 entities
	constexpr inline entity max_entities = std::numeric_limits<entity>::max() - 1;

	using signature = std::bitset<max_components>;

	constexpr sparse_type null_index = std::numeric_limits<sparse_type>::max();

	inline constexpr int max_mouse_buttons = 8;
	inline constexpr int max_keyboard_buttons = 1024;
	inline constexpr int max_key_pressed_queue = 16;
	inline constexpr int max_char_pressed_queue = 16;

	inline constexpr int default_width = 800;
	inline constexpr int default_height = 600;

	inline constexpr glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	inline constexpr glm::vec3 gravity_vec = glm::vec3(0.0f, -9.81f, 0.0f);

} // gumlib