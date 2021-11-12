#pragma once
#include <dcore/Data/FileOutput.hpp>
#include <typeindex>

namespace dcore::world
{
	using Entity = uint32_t;
	
	struct System
	{
		using StartFunction = void (*)(Entity*);
		using UpdateFunction = void (*)(Entity*);
		using EndFunction = void (*)(Entity*);
		using SaveFunction = void (*)(Entity*, data::Json&);
		using LoadFunction = void (*)(Entity*, data::Json&);

		StartFunction StartFunction;
		UpdateFunction UpdateFunction;
		EndFunction EndFunction;
		SaveFunction SaveFunction;
		LoadFunction LoadFunction;
	};

	class ECS
	{
	public:
		template<typename ComponentType>
		void RegisterSystem(System &&system);
	
	private:
		struct ComponentHandle { Entity *entity; void *component; };
		std::unordered_map<std::type_index, std::vector<ComponentHandle>> Components_;
		std::unordered_map<std::type_index, System> Systems_;
	};
};
