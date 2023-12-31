#pragma once

#include <map>
#include <vector>
#include <string>

class VersionDetect
{
	public:
		static std::string GetVersion();

		std::map<uint32_t, std::string> s_buildMap;
};
