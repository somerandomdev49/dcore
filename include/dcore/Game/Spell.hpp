#pragma once
#include <dcore/Core/Type.hpp>

namespace dg
{
	class ActorComponent;

	/**
	 * @brief Abstract base class for any spell.
	 */
	class Spell
	{
	public:
		dstd::USize GetId() const;
		virtual void Cast(ActorComponent *caster, ActorComponent *target) = 0;
	private:
		dstd::USize Id_;
	};
}
