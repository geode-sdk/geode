//Code by Absolute, have no idea if im allowed to use it but hope i can :)
//https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19#file-versiondetect-cpp
/*
#include "VersionDetect.h"
#include <Windows.h>

std::map<uint32_t, std::string> VersionDetect::s_buildMap =
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

std::string VersionDetect::GetVersion()
{
	HMODULE hMod = GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(hMod);

	if (dos_header->e_magic == IMAGE_DOS_SIGNATURE)
	{
		PIMAGE_NT_HEADERS nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(hMod) + dos_header->e_lfanew);

		if (nt_header->Signature == IMAGE_NT_SIGNATURE)
		{
			auto it = s_buildMap.find(nt_header->FileHeader.TimeDateStamp);

			if (it != s_buildMap.end())
				return it->second;
		}
	}

	return "";
}*/
