#include "ObjectFactory.h"

namespace GameEngine
{
    ObjectFactoryFunctionLink::ObjectFactoryFunctionLink (const std::string& objectTypeName, ObjectFactoryFunction function) :
        m_objectTypeName (objectTypeName), m_objectFactoryFunction (function)
    {
        ObjectFactory::RegisterFunction (m_objectTypeName, m_objectFactoryFunction);
    }

    ObjectFactoryFunctionLink::~ObjectFactoryFunctionLink ()
    {
        ObjectFactory::UnregisterFunction (m_objectTypeName);
    }

    ObjectFactory::ObjectFactory ()
    {

    }

    ObjectFactory::~ObjectFactory()
    {
        m_functionMap.clear ();
    }

    ObjectFactory& ObjectFactory::GetInstance ()
    {
        static ObjectFactory instance;
        return instance;
    }

    void ObjectFactory::RegisterFunction (const std::string& objectTypeName, ObjectFactoryFunction function)
    {
        ObjectFactory& instance = GetInstance ();
        ObjectFactoryFunctionMap& functionMap = instance.m_functionMap;

        if (functionMap.find (objectTypeName) == functionMap.end ())
        {
            functionMap.emplace (objectTypeName, function);
        }
    }

    void ObjectFactory::UnregisterFunction (const std::string& objectTypeName)
    {
        ObjectFactory& instance = GetInstance ();
        ObjectFactoryFunctionMap& functionMap = instance.m_functionMap;

        if (functionMap.find (objectTypeName) != functionMap.end ())
        {
            functionMap.erase (objectTypeName);
        }
    }

    ObjectFactoryFunction ObjectFactory::GetFunction (const std::string& objectTypeName)
    {
        ObjectFactoryFunction function = nullptr;
        ObjectFactory& instance = GetInstance ();
        ObjectFactoryFunctionMap& functionMap = instance.m_functionMap;
        auto found = functionMap.find (objectTypeName);

        if (found != functionMap.end ())
        {
            function = found->second;
        }

        return function;
    }
}