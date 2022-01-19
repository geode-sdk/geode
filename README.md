# CacaoSDK
Cacao is a revolutionary Geometry Dash modding framework for MacOS users. Cacao requires CMake so make sure you have that. To start, run the `create` file and specify a location

Cacao uses [a modified version of Lilac Core](https://github.com/altalk23/lilac-core/) for function hooking.

## Prerequisites

The projects created with the `./create` are normally in the current working directory. To override this you can add a `CACAO_PROJECT_FOLDER` environment variable. You can do this by adding `export CACAO_PROJECT_FOLDER="path/to/project/folder"` to your .bash_profile file.

## How to use

TODO: Make this properly documented  

### Cacao

Cacao comes with a neat little way to hook very quickly. This let's you automatically hook functions without needing to manually find the address each time. It works by subclassing some of the Cacao classes and overriding some of the methods. Cacao classes are just like normal GD classes but prefixed with a `$`. To easily subclass these classes there is a macro called `$()`. To use it I give the macro the class I want to hook. Example:
```cpp
#include <Cacao>
#include <iostream>

class $(EditorUI) {
    void undoLastAction(CCObject* p0) {
        std::cout << "Undo!" << std::endl;
    }
};
```

If you want to call the original function, there is also an easy way to do that as well:
```cpp
#include <Cacao>
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
#include <Cacao>
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

Since the Cacao classes subclass the GD classes, we can use the members and functions like we would in a normal class.
```cpp
#include <Cacao>
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
#include <Cacao>
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

If there's a function, class, or member you want to be added to the Cacao catalog consider creating a pull request on [CacaoData](https://github.com/altalk23/CacaoData/).

For Cacao, (almost) all class variables are accessed via functions and prefixed with an underscore, e.g `GameManager::sharedState()->_playLayer()`. You can find a full list of these inside the Header.hpp header file.

For a full list of helper functions, look at the helpers/Cacao.hpp header file.
