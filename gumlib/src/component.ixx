module;

#include <cassert>

export module gumlib:component;

import std;
import :config;

struct ComponentCounter
{
	static inline gumlib::component_type count = 0;

	static inline gumlib::component_type next()
	{
		return count++;
	}
};

template <typename Component>
struct ComponentId
{
	static inline gumlib::component_type value()
	{
		static gumlib::component_type id = ComponentCounter::next();
		return id;
	}
};

export namespace gumlib
{
	class IPool
	{
	protected:
		// entity to index
		std::vector<sparse_type> sparse_;
		// index to entity
		std::vector<entity> dense_;

	public:
		IPool() : sparse_(std::vector(max_entities, null_index)) {}
		virtual ~IPool() = default;
		virtual void entity_destroyed(const entity e) = 0;

		bool has(const entity e) const
		{
			return e < dense_.size() && sparse_[e] != null_index;
		}

		const std::vector<entity>& get_entities() const
		{
			return dense_;
		}

		size_t get_size() const
		{
			return dense_.size();
		}
	};

	template <typename Component>
	class Pool final : public IPool
	{
		std::vector<Component> components_;

	public:
		explicit Pool(const size_t reserve_size = 100)
		{
			components_.reserve(reserve_size);
			dense_.reserve(reserve_size);
		}

		void add_component(const entity e, const Component c)
		{
			assert(sparse_[e] == null_index);

			dense_.emplace_back(e);
			components_.emplace_back(c);

			sparse_[e] = components_.size() - 1;
		}

		void remove(const entity e)
		{
			const sparse_type index_to_remove = sparse_[e];
			if (index_to_remove == null_index) return;

			// swap and pop retains O(1) removal from sparse set
			if (const size_t last_index = components_.size() - 1; index_to_remove != last_index)
			{
				const entity last_entity = dense_[last_index];
				sparse_[last_entity] = index_to_remove;
				components_[index_to_remove] = std::move(components_.back());
				dense_[index_to_remove] = last_entity;
			}

			dense_.pop_back();
			components_.pop_back();
			sparse_[e] = null_index;
		}

		Component& get_component(const entity e)
		{
			return components_[sparse_[e]];
		}

		void entity_destroyed(const entity e) override
		{
			if (this->has(e))
			{
				remove(e);
			}
		}
	};
} // gumlib