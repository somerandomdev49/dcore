#pragma once
#include <dcore/World/World.hpp>

namespace dcore::data
{
    template<class Self, typename FuncType, typename Entry>
    class GenericSerializer
    {
    public:
        static Self *Instance();
        
        void RegisterType(std::type_index type, Entry entry)
        { Entries_[type] = entry; }

        template<typename T>
        void RegisterType(Entry entry)
        { RegisterType(std::type_index(typeid(T)), entry); }
    
    private:
        std::unordered_map<std::type_index, Entry> Entries_; 
    };

    class Serializer;
    class DeSerializer;
    
    using SerializerFunction = void (*)(Serializer*, void*);
    using DeSerializerFunction = void (*)(DeSerializer*, void*);
    struct SerializerEntry { SerializerFunction func; };
    struct DeSerializerEntry { DeSerializerFunction func; size_t size; };

    class Serializer : public GenericSerializer<Serializer, SerializerFunction, SerializerEntry> {};
    class DeSerializer : public GenericSerializer<DeSerializer, DeSerializerFunction, DeSerializerEntry> {};
}
