# GeodeSDK
Geode is a revolutionary Geometry Dash modding framework. Geode is built using CMake so make sure it is installed before building. To start, run the `create` file and specify a location

## Prerequisites

The projects created with the `./create` are normally in the current working directory. To override this you can add a `GEODE_PROJECT_FOLDER` environment variable. You can do this by adding `export GEODE_PROJECT_FOLDER="path/to/project/folder"` to your .bash_profile file.

## How to use

TODO: Make this properly documented

Make sure you include the flag `-T host=x64` if you are building for Windows, that way you avoid out of heap space errors.

### Geode

Geode comes with a neat little way to hook very quickly. This let's you automatically hook functions without needing to manually find the address each time. It works by subclassing some of the Geode classes and overriding some of the methods. Geode classes are just like normal GD classes but prefixed with a `$`. To easily subclass these classes there is a macro called `$()`. To use it I give the macro the class I want to hook. Example:
```cpp
#include <Geode>
#include <iostream>

class $(EditorUI) {
    void undoLastAction(CCObject* p0) {
        std::cout << "Undo!" << std::endl;
    }
};
```

If you want to call the original function, there is also an easy way to do that as well:
```cpp
#include <Geode>
#include <iostream>

class $(EditorUI) {
    void undoLastAction(CCObject* p0) {
        std::cout << "Undo!" << std::endl;
        $EditorUI::undoLastAction(p0);
    }
};
```

If you need the name of the hook class you can use the `$()` macro with 2 parameters:
```cpp
#include <Geode>
#include <iostream>

class $(EditorUI, EditorUIHook) {
	void callback(CCObject*) {
		std::cout << "Called from EditorUIHook!" << std::endl;
	}
    void undoLastAction(CCObject* p0) {
    	auto func = &EditorUIHook::callback;
        (this->*func)(p0); // c++ syntax moment
        $EditorUI::undoLastAction(p0);
    }
};
```

Since the Geode classes subclass the GD classes, we can use the members and functions like we would in a normal class.
```cpp
#include <Geode>
#include <iostream>

class $(EditorUI) {
    void undoLastAction(CCObject* p0) {
        std::cout << "We have " << getSelectedObjects()->count() << " objects selected" << std::endl;
        $EditorUI::undoLastAction(p0);
    }
};
```

If you want, you can also use a function with the name `inject` to run code after the mod is loaded.
```cpp
#include <Geode>
#include <iostream>

class $(EditorUI) {
    void undoLastAction(CCObject* p0) {
        std::cout << "We have " << getSelectedObjects()->count() << " objects selected" << std::endl;
        $EditorUI::undoLastAction(p0);
    }
};

void inject() {
    std::cout << "Hello!" << std::endl;
}
```

If there's a function, class, or member you want to be added to the Geode catalog consider creating a pull request on [GeodeData](https://github.com/altalk23/GeodeData/).

For Geode, (almost) all class variables are accessed via functions and prefixed with an underscore, e.g `GameManager::sharedState()->_playLayer()`. You can find a full list of these inside the Header.hpp header file.

For a full list of helper functions, look at the helpers/Geode.hpp header file.
