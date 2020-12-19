#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Type.h"
#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
    class Object;

    using ObjectFactoryFunction = std::shared_ptr<Object> (*) ();
    using ObjectFactoryFunctionMap = std::unordered_map<std::string, ObjectFactoryFunction>;

    class ObjectFactoryFunctionLink final
    {
    public:
        ENGINE_CORE_API ObjectFactoryFunctionLink (const std::string& objectTypeName, ObjectFactoryFunction function);
        ENGINE_CORE_API ~ObjectFactoryFunctionLink ();

    private:
        std::string m_objectTypeName;
        ObjectFactoryFunction m_objectFactoryFunction;
    };

    class ObjectFactory final
    {
    private:
        ObjectFactory ();
        ~ObjectFactory();

        ENGINE_CORE_API static ObjectFactory& GetInstance ();
        
    public:
        ObjectFactory (const ObjectFactory&) = delete;
        ObjectFactory& operator= (const ObjectFactory&) = delete;

        ENGINE_CORE_API static void RegisterFunction (const std::string& objectTypeName, ObjectFactoryFunction function);
        ENGINE_CORE_API static void UnregisterFunction (const std::string& objectTypeName);
        ENGINE_CORE_API static ObjectFactoryFunction GetFunction (const std::string& objectTypeName);

    private:
        ObjectFactoryFunctionMap m_functionMap;
    };
}

#define REGISTER_OBJECT_FACTORY(OBJECT_CLASS_NAME)\
static ::std::shared_ptr<::GameEngine::Object> CreateObjectFunction##OBJECT_CLASS_NAME ()\
{\
    static_assert (::GameEngine::IsBaseOf<::GameEngine::Object, OBJECT_CLASS_NAME>::Result, "You can register class derived from object only!");\
    return ::std::make_shared<OBJECT_CLASS_NAME> ();\
}\
\
static ::GameEngine::ObjectFactoryFunctionLink ObjectFactoryFunctionLink##OBJECT_CLASS_NAME  (#OBJECT_CLASS_NAME, CreateObjectFunction##OBJECT_CLASS_NAME);
