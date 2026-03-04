module;

#include "glm/vec3.hpp"

export module gumlib:context;

import std;
import :config;

// TODO: read context from config file
export namespace gumlib
{
	struct CollisionPair;

	struct InputContext
	{
		std::array<bool, max_keyboard_buttons> keys = {false};
		std::array<bool, max_keyboard_buttons> prev_keys = {false};

		std::array<bool, max_mouse_buttons> mouse_buttons = {false};
		std::array<bool, max_mouse_buttons> prev_mouse_mutons = {false};

		float mouse_sensitivity = 5.0f;
		float mouse_delta_x = 0.0f;
		float mouse_delta_y = 0.0f;
		float mouse_x = 0.0f;
		float mouse_y = 0.0f;

		bool first_mouse = true;

		inline void set_keyboard_key(const int key, const bool pressed)
		{
			keys[key] = pressed;
		}

		inline void set_mouse_button(const int button, const bool pressed)
		{
			mouse_buttons[button] = pressed;
		}

		inline bool is_key_pressed(const int key) const
		{
			return keys[key] && !prev_keys[key];
		}

		inline bool is_key_held(const int key) const
		{
			return keys[key];
		}

		inline bool is_button_pressed(const int button) const
		{
			return mouse_buttons[button] && !prev_mouse_mutons[button];
		}

		inline bool is_button_held(const int button) const
		{
			return mouse_buttons[button];
		}

		inline void update_mouse(const float x_pos, const float y_pos)
		{
			if (first_mouse)
			{
				mouse_x = x_pos;
				mouse_y = y_pos;
				first_mouse = false;
			}

			mouse_delta_x = (x_pos - mouse_x);
			mouse_delta_y = (mouse_y - y_pos);

			mouse_x = x_pos;
			mouse_y = y_pos;
		}

		inline void prepareForNextFrame()
		{
			mouse_delta_x = 0.0f;
			mouse_delta_y = 0.0f;

			prev_mouse_mutons = mouse_buttons;
			prev_keys = keys;
		}
	};

	//struct ShaderContext
	//{
	//	std::unordered_map<std::string, Shader> shaders;

	//	Shader& getShader(const std::string name)
	//	{
	//		return shaders.at(name);
	//	}
	//};

	struct TimeContext
	{
		float delta_time = 0.0f;
		float total_time = 0.0f;
		float last_frame = 0.0f;
		float time_scale = 1.0f;

		unsigned int frame_count = 0;

		inline void update(const float current_frame)
		{
			delta_time = (current_frame - last_frame) * time_scale;
			last_frame = current_frame;
			total_time = current_frame;
			frame_count++;
		}
	};

	struct WindowContext
	{
		// SEE TODO
		int width = default_width, height = default_height;

		bool is_fullscreen = false;
		bool vsync_enabled = false;
		float aspect_ratio = 800.0f / 600.0f;
	};

	struct AudioContext
	{
		float master_volume = 0.5f;
		float music_volume = 0.5f;
		float sfx_volume = 0.5f;
		bool is_muted = false;
	};

	struct PhysicsContext
	{
		glm::vec3 gravity = gravity_vec;
		int solver_iterations = 6; // accuracy vs performance setting (six default for box2d/physics)
		// SEE TODO
		//std::vector<CollisionManifold> activeCollisions;
	};

	struct RandomContext
	{
		std::mt19937 generator;
		std::uint32_t seed = 0;
	};

	struct GameContext
	{
		enum class state { menu, playing, paused, game_over } current = state::menu;

		int score = 0;
		int current_level = 0;
	};

	class ContextManager
	{
		std::unique_ptr<InputContext> input_context_ = std::make_unique<InputContext>();
		std::unique_ptr<RandomContext> random_context_ = std::make_unique<RandomContext>();
		std::unique_ptr<TimeContext> time_context_ = std::make_unique<TimeContext>();
		std::unique_ptr<WindowContext> window_context_ = std::make_unique<WindowContext>();
		std::unique_ptr<GameContext> game_context_ = std::make_unique<GameContext>();
		std::unique_ptr<PhysicsContext> physics_context_ = std::make_unique<PhysicsContext>();
		std::unique_ptr<AudioContext> audio_context_ = std::make_unique<AudioContext>();
		//std::unique_ptr<ShaderContext> m_shaderContext = std::make_unique<ShaderContext>();

	public:
		InputContext& get_input_context() const
		{
			return *input_context_.get();
		}

		RandomContext& get_random_context() const
		{
			return *random_context_.get();
		}

		TimeContext& get_time_context() const
		{
			return *time_context_.get();
		}

		WindowContext& get_window_context() const
		{
			return *window_context_.get();
		}

		GameContext& get_game_context() const
		{
			return *game_context_.get();
		}

		PhysicsContext& get_physics_context() const
		{
			return *physics_context_.get();
		}

		AudioContext& get_audio_context() const
		{
			return *audio_context_.get();
		}

		//ShaderContext& getShaderContext()
		//{
		//	return *m_shaderContext.get();
		//}
	};
} // gumlib
