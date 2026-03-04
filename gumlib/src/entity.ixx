export module gumlib:entity;

import std;
import :config;

export namespace gumlib
{
	class EntityManager
	{
		std::queue<entity> available_entities_;

		std::vector<signature> entity_signatures_;

		entity live_entities_ = 0;

	public:
		EntityManager()
		{
			entity_signatures_.resize(max_entities);

			for (entity i = 0; i < max_entities; ++i)
			{
				available_entities_.push(i);
			}
		}

		entity create_entity()
		{
			const entity e = available_entities_.front();
			available_entities_.pop();
			++live_entities_;
			return e;
		}

		void destroy_entity(const entity e)
		{
			available_entities_.push(e);
			live_entities_--;
		}

		void set_signature_bit(const entity e, const component_type bit, const bool value)
		{
			entity_signatures_[e].set(bit, value);
		}

		void set_signature(const entity e, const signature& s)
		{
			entity_signatures_[e] = s;
		}

		const signature& get_signature(const entity e) const
		{
			return entity_signatures_[e];
		}
	};
} // gumlib