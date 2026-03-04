export module gumlib:registry;

import std;
import :config;
import :entity;
import :component;
import :system;
import :view;

export namespace gumlib
{
	class Registry
	{
		std::unique_ptr<EntityManager> entity_manager_ = std::make_unique<EntityManager>();
		std::unique_ptr<SystemManager> system_manager_ = std::make_unique<SystemManager>();

		std::vector<std::unique_ptr<IPool>> pools_;

		template <typename Component>
		component_type get_component_type()
		{
			return ComponentId<Component>::value();
		}

		template <typename Component>
		const Pool<Component>& get_pool()
		{
			return *static_cast<Pool<Component>*>(pools_[get_component_type<Component>()].get());
		}

		template <typename Component>
		std::pair<size_t, IPool*> get_pool_info()
		{
			auto pool = pools_[ComponentId<Component>::value()];
			return std::pair<size_t, std::unique_ptr<IPool>>(pool->get_size(), pool.get());
		}

	public:
		Registry()
		{
			pools_.resize(max_components);
		}

		// ReSharper disable once CppMemberFunctionMayBeConst - calls non-const function
		void update(const float dt)
		{
			system_manager_->update(dt);
		}

		// ReSharper disable once CppMemberFunctionMayBeConst - calls non-const function
		void render(const float dt)
		{
			system_manager_->render(dt);
		}

		// ReSharper disable once CppMemberFunctionMayBeConst - calls non-const function
		entity create_entity()
		{
			return entity_manager_->create_entity();
		}

		// ReSharper disable once CppMemberFunctionMayBeConst - calls non-const function
		void destroy_entity(const entity e)
		{
			system_manager_->entity_destroyed(e);
			entity_manager_->destroy_entity(e);

			for (const auto& pool : pools_)
			{
				pool->entity_destroyed(e);
			}
		}

		template <typename Component>
		void register_component()
		{
			pools_[ComponentId<Component>::value()] = std::make_unique<Pool<Component>>;
		}

		template <typename System, typename... Args>
		System& register_system(Args&&... args)
		{
			std::unique_ptr<System> system = std::make_unique<System>(std::forward<Args>(args)...);
			system_manager_->register_system(std::move(system));
			return system_manager_->get_systems().back();
		}

		template <typename Component>
		void add_component(entity e, Component c)
		{
			get_pool<Component>().add_component(e, c);
			entity_manager_->set_signature_bit(e, get_component_type<Component>(), true);
			system_manager_->entity_signature_changed(e, entity_manager_->get_signature(e));
		}

		template <typename Component>
		void remove_component(const entity e)
		{
			get_pool<Component>().remove(e);
			entity_manager_->set_signature_bit(e, get_component_type<Component>(), false);
			system_manager_->entity_signature_changed(e, entity_manager_->get_signature(e));
		}

		template <typename... Components>
		View view()
		{
			// set signature created by Components
			signature mask;
			(mask.set(get_component_type<Components>()), ...);

			// fill vector with info of each Ts (components) pool
			std::vector<std::pair<size_t, IPool*>> pool_info = {
				get_pool_info<Components>()...
			};

			// find the pool with the least amount of tracked entities
			const std::pair<size_t, IPool*> min_pool = std::min_element(pool_info.begin(), pool_info.end(),
			                                                            [](const auto& a, const auto& b) { a.first < b.first; });

			return View(min_pool.second->get_entities(), mask, *entity_manager_);
		}
	};
} // gumlib