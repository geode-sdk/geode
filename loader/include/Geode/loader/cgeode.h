#pragma once

#include <Geode/platform/cplatform.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void GEODE_C_DLL geode_mod_log(void* mod, const char* message);

bool GEODE_C_DLL geode_mod_add_hook(void* mod, void* address, void* detour);

bool GEODE_C_DLL geode_get_last_error(char* buffer, size_t bufferSize);

#ifdef __cplusplus
}
#endif
