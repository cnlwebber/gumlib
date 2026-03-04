export module game:asteroid;

import gumlib;
import std;

export namespace game
{
	enum class size
	{
		large, medium, small
	};

	struct AsteroidC
	{
		size asteroid_size = size::large;
	};


	class AsteroidSystem final : public gumlib::ISystem
	{
	public:
		AsteroidSystem()
		{
			require_component<AsteroidC>();
		}

		void update(const float dt, gumlib::Registry& reg) override
		{
			for (const auto e : entities_)
			{
				switch (auto& [asteroid_size] = reg.get_component<AsteroidC>(e); asteroid_size)
				{
				case size::large:
					asteroid_size = size::medium;
					continue;
				case size::medium:
					asteroid_size = size::small;
					continue;
				case size::small:
					reg.remove_component<AsteroidC>(e);
					std::cout << "component removed" << "\n";
				}
			}
		}
	};
}