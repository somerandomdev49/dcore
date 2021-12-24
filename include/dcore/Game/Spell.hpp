#pragma once
#include <dcore/Core/Type.hpp>

namespace dg
{
	class ActorComponent;

	/**
	 * @brief Virtual spell class 
	 * 
	 */
	class Spell
	{
	public:
		dstd::USize GetId() const;
		void Cast(ActorComponent *caster, ActorComponent *target);
	private:
		dstd::USize Id_;
	};
}
