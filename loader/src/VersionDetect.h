#pragma once

#include <map>
#include <vector>
#include <string>

namespace VersionDetect
{
	std::string GetVersion();

	extern std::map<uint32_t, std::string> s_buildMap;
}
