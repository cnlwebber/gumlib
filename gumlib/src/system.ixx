// ReSharper disable CppMemberFunctionMayBeConst - functions call non-const member(s)
module;

#include <cassert>

export module gumlib:system;

import std;
import :config;
import :component;

export namespace gumlib
{
	class Registry;

	class ISystem
	{
	protected:
		signature mask_;
		std::vector<entity> entities_;
		std::vector<sparse_type> sparse_;

	public:
		ISystem() : sparse_(std::vector(max_entities, null_index)) {}
		virtual ~ISystem() = default;
		virtual void update(const float dt, Registry& reg) = 0;
		virtual void render(const float dt, Registry& reg) {}

		const std::vector<entity>& get_entities() const
		{
			return entities_;
		}

		const signature& get_mask() const
		{
			return mask_;
		}

		bool has_entity(const entity e) const
		{
			return e < max_entities && sparse_[e] != null_index;
		}

		void add_entity(const entity e)
		{
			assert(e < max_entities && "Max entities reached");

			if (sparse_[e] != null_index) return;
			entities_.emplace_back(e);
			sparse_[e] = entities_.size() - 1;
		}

		void remove_entity(const entity e)
		{
			assert(e < max_entities && "Out of entity bounds");
			if (sparse_[e] == null_index) return;

			const sparse_type index_to_remove = sparse_[e];
			// swap and pop for O(1) removal
			if (const size_t last_index = entities_.size() - 1; index_to_remove != last_index)
			{
				const entity last_entity = entities_[last_index];
				sparse_[last_entity] = index_to_remove;
				entities_[index_to_remove] = last_entity;
			}
			sparse_[e] = null_index;
			entities_.pop_back();
		}

		template <typename Component>
		void require_component()
		{
			mask_.set(ComponentId<Component>::value(), true);
		}
	};

	class SystemManager
	{
		std::vector<std::unique_ptr<ISystem>> systems_;

	public:
		void update(const float dt, Registry& reg)
		{
			for (const auto& system : systems_)
			{
				system->update(dt, reg);
			}
		}

		void render(const float dt, Registry& reg)
		{
			for (const auto& system : systems_)
			{
				system->render(dt, reg);
			}
		}

		void register_system(std::unique_ptr<ISystem> system)
		{
			systems_.push_back(std::move(system));
		}

		void entity_destroyed(const entity e)
		{
			for (const auto& system : systems_)
			{
				system->remove_entity(e);
			}
		}

		void entity_signature_changed(const entity e, const signature& s)
		{
			for (const auto& system : systems_)
			{
				const signature mask = system->get_mask();
				if (mask.none()) continue;

				const bool matches = (mask & s) == mask;
				if (const bool in_system = system->has_entity(e); matches && !in_system)
				{
					system->add_entity(e);
				}
				else if (!matches && in_system)
				{
					system->remove_entity(e);
				}
			}
		}

		const std::vector<std::unique_ptr<ISystem>>& get_systems()
		{
			return systems_;
		}
	};
} // gumlib