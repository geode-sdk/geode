// Only a single objc++ file is used because since pch doesnt work, each file adds a lot to the compile times
#include <Geode/DefaultInclude.hpp>

USE_GEODE_NAMESPACE();

#if defined(GEODE_IS_MACOS)

#include "mac/crashlog.mm"
#include "mac/FileWatcher.mm"
#include "mac/util.mm"

#elif defined(GEODE_IS_IOS)

#include "ios/FileWatcher.mm"
#include "ios/util.mm"

#endif