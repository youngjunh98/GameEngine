#pragma once

#if defined (snprintf)
	#undef snprintf
#endif

#include <single_include/nlohmann/json.hpp>

namespace GameEngine
{
    namespace Json
    {
        using Json = nlohmann::json;
        using JsonIterator = Json::iterator;
        using JsonConstIterator = Json::const_iterator;
    }
}