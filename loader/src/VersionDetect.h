#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <string>

class VersionDetect
{
	public:
		std::map<uint32_t, std::string> s_buildMap =
		{
			{ 1419173053, "1.900" },
			{ 1419880840, "1.910" },
			{ 1421745341, "1.920" },
			{ 1440638199, "2.000" },
			{ 1440643927, "2.001" },
			{ 1443053232, "2.010" },
			{ 1443077847, "2.011" },
			{ 1443077847, "2.020" },
			{ 1484612867, "2.100" },
			{ 1484626658, "2.101" },
			{ 1484737207, "2.102" },
			{ 1510526914, "2.110" },
			{ 1510538091, "2.111" },
			{ 1510619253, "2.112" },
			{ 1511220108, "2.113" },
			{ 1702921605, "2.200" },
		};

		static std::string GetVersion()
		{
			HMODULE hMod = GetModuleHandle(NULL);
			PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(hMod);
		
			if (dos_header->e_magic == IMAGE_DOS_SIGNATURE)
			{
				PIMAGE_NT_HEADERS nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(hMod) + dos_header->e_lfanew);
		
				if (nt_header->Signature == IMAGE_NT_SIGNATURE)
				{
					for (int i = 0; i < s_buildMap.size(); i++)
					{
						if (s_buildMap[i].first == (nt_header->FileHeader.TimeDateStamp))
						{
							return s_buildMap[i].second;
						}
					}
				}
			}
		
			return "";
		};
};
