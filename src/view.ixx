// ReSharper disable CppMemberFunctionMayBeConst - functions call non-const member(s)
export module gumlib:view;

import std;
import :entity;
import :config;

namespace gumlib
{
	class View
	{
		const std::vector<entity>& entities_;
		const signature view_mask_;
		EntityManager& entity_manager_;

		// iterates over component_type's min_pool by checking each entity against mask consisting of all component_types
		struct Iterator
		{
			std::vector<entity>::const_iterator ptr;
			std::vector<entity>::const_iterator end;
			signature mask;
			EntityManager& entity_manager;

			void find_next_valid()
			{
				while (ptr != end)
				{
					if ((entity_manager.get_signature(*ptr) & mask) == mask) break;
					++ptr;
				}
			}

		public:
			using iterator_category = std::input_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using pointer = std::vector<entity>::const_iterator;
			using reference = entity; // or value_type&

			Iterator(const std::vector<entity>::const_iterator& t_ptr, const std::vector<entity>::const_iterator& t_end, const signature& s, EntityManager& emp) :
				ptr(t_ptr),
				end(t_end),
				mask(s),
				entity_manager(emp)
			{
				find_next_valid();
			}

			reference operator*() const { return *ptr; }
			pointer operator->() const { return ptr; }

			Iterator& operator++()
			{
				++ptr;
				return *this;
			}

			Iterator& operator++(int)
			{
				Iterator& tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
		};

	public:
		View(const std::vector<entity>& entities, const signature& view_mask, EntityManager& entity_manager) :
			entities_(entities),
			view_mask_(view_mask),
			entity_manager_(entity_manager) {}

		Iterator begin()
		{
			return Iterator(entities_.begin(), entities_.end(), view_mask_, entity_manager_);
		}

		Iterator end()
		{
			return Iterator(entities_.end(), entities_.end(), view_mask_, entity_manager_);
		}
	};
}