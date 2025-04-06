#pragma once

#include <Geode/platform/cplatform.h>

bool safeModeCheck();
int geodeEntry(void* platformData);

#ifdef GEODE_IS_MACOS
void checkPermissionPlist();
#endif