#pragma once
#include <dcore/World/World.hpp>

// NOT USED!!!
namespace dcore::data
{
	/**
	 * @brief Generic seraializer class
	 *
	 * @tparam Self
	 * @tparam FuncType
	 * @tparam Entry
	 * @deprecated
	 */
	template<class Self, typename FuncType, typename Entry>
	class GenericSerializer
	{
	public:
		static Self *Instance();

		void RegisterType(std::type_index type, Entry entry) { Entries_[type] = entry; }

		template<typename T>
		void RegisterType(Entry entry)
		{
			RegisterType(std::type_index(typeid(T)), entry);
		}

	private:
		std::unordered_map<std::type_index, Entry> Entries_;
	};

	class Serializer;
	class DeSerializer;

	/** @deprecated */
	using SerializerFunction = void (*)(Serializer *, void *);
	/** @deprecated */
	using DeSerializerFunction = void (*)(DeSerializer *, void *);
	/** @deprecated */
	struct SerializerEntry
	{
		SerializerFunction func;
	};
	/** @deprecated */
	struct DeSerializerEntry
	{
		DeSerializerFunction func;
		size_t size;
	};

	/** @deprecated */
	class Serializer : public GenericSerializer<Serializer, SerializerFunction, SerializerEntry>
	{
	};
	/** @deprecated */
	class DeSerializer : public GenericSerializer<DeSerializer, DeSerializerFunction, DeSerializerEntry>
	{
	};
} // namespace dcore::data
