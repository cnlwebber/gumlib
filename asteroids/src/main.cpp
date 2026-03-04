import std;
import gumlib;
import game;

// tests the ecs by registering the asteroid system which just removes any AsteroidC component attached to any entities, used for debugging the ecs for now
int main()
{
	auto registry = gumlib::Registry();

	registry.register_component<game::AsteroidC>();
	registry.register_system<game::AsteroidSystem>();

	const auto asteroid_e = registry.create_entity();

	registry.add_component(asteroid_e, game::AsteroidC());

	int n = 3;
	while (n > 0)
	{
		constexpr float dt = 1.0f;
		registry.update(dt);
		--n;
	}
	std::cin >> n;
}