module;

#include <glm/glm.hpp>

export module gumlib:config;
import std;

export namespace gumlib
{
	using entity			= std::uint32_t;
	using system_type		= std::uint32_t;
	using sparse_type		= std::uint16_t;
	using component_type	= std::uint8_t;

	constexpr inline component_type max_components = 255;
	constexpr inline entity max_entities = 65535;

	using signature = std::bitset<max_components>;

	constexpr sparse_type null_index = std::numeric_limits<sparse_type>::max();
} // gumlib