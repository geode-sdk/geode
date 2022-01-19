# History of Interface Classes

### 1a07998694824abc492eb6b9d98591d0c0dc26e5
This version had the problem that you could only hook a single class inside a mod container, and it only supported virtual functions  
Also the first version of the interface / cackit classes
```cpp
class: public $EditorUI {
	void undoLastAction() override {
		std::cout << "We have " << cac_this->getSelectedObjects()->count() << " objects elected\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;
```

### 781465559ec779e295d1b25a511bbe1d277dd702
This version allowed for multiple classes, though the classes still needed to be inside the same source file and between the two macros `CAC_HOOKS` and `END_CAC_HOOKS`  
My personal second least favorite
```cpp
CAC_HOOKS
class: public $EditorUI {
	void undoLastAction() override {
		std::cout << "Undo!\n";
	}
} MyEditorUIHook;
END_CAC_HOOKS
```

### e577a57317681332d4974e1e299159c2f6e307af
This version is the first one that utilized the CRTP system, which eventually became the heart of the interface classes
```cpp
CAC_HOOKS
class EditorUIHook: public $EditorUI<EditorUIHook> {
	void undoLastAction() override {
		std::cout << "Undo!\n";
	}
} MyEditorUIHook;
END_CAC_HOOKS
```

### 4c10edc67ac8893136dbc97df30fe04996d69001
This version ditched the the macros that put the hooks inside an anonymous function for a static global variable approach. `APPLY_HOOKS` macro is exactly that.
```cpp
class EditorUIHook: public $EditorUI<EditorUIHook> {
public:
	void undoLastAction() override {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

APPLY_HOOKS();
```

### c9f090d539b6b74742f2bab793c4cc88991f981a
This version removed the CRTP system, not needing to get the address of the virtual with the derived
```cpp
class: public $EditorUI {
public:
	void undoLastAction() override {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

APPLY_HOOKS();
```

### 37c7ec8df248a02254de6d84daecca444a484384
This version removed the need for the public storage access, for the same reason as above
```cpp
class: public $EditorUI {
	void undoLastAction() override {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

APPLY_HOOKS();
```

### de5eee71b18aa50c225a76667861610049c00b21
This version brang back both CRTP system (but this time wrapped inside a macro) and the public access. Now it also supported all types of functions, and not just virtual functions
```cpp
class REDIRECT($EditorUI) {
public:
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

APPLY_HOOKS();
```

### f18c2f73ae1e1cc1662dbcbe54f36e657a699723
This version renamed the macros to the more Cacao familiar syntax with the $ prefixes
```cpp
class $redirect(EditorUI) {
public:
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

$apply();
```

### 4027f1e82a1461fad30ef6e51b6dfc1b887479a8
This version ditched redirect for implement because I was dumb and I misinterpreted something and all  
IIRC it was related to the constructors  
My personal least favorite
```cpp
class $implement(EditorUI, MyEditorUIHook) {
public:
    void undoLastAction(CCObject* p0) {
        std::cout << "Undo!" << std::endl;
        $EditorUI::undoLastAction(p0);
    }
} endImplement(MyEditorUIHook);

$apply();
```

### b32b9f0783b9079269685260ca7188d557f9307e
Thankfully this version reverted that while fixing the problem too
```cpp
class $redirect(EditorUI) {
public:
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;

$apply();
```

### 7531b2ae915447df8eb9ce825ee7755303396812
This version removed the need for the `$apply` macro
```cpp
class $redirect(EditorUI) {
public:
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
} MyEditorUIHook;
```

### e6ec14774b94f8b5cdc30a246ed48d46674bce95
This version removed the need for both the variable initializer and the public specifier, using some macro stuff  
My personal second favorite
```cpp
class $redirect(EditorUI) {
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
};
```

### f361adca95c6e3c07e0fde654fd7c8c1a59d3b2f
This version combined the two macros to create the ultimate macro `$`  
My personal favorite
```cpp
class $(EditorUI) {
	void undoLastAction() {
		std::cout << "Undo!\n";
		$EditorUI::undoLastAction();
	}
};
```

### de74c1ffa4a3a7f72ac2394fbfb4a1262fbe470b
This version allowed for the creation of fields, which let you effectively add extra fields to classes
You can access them with the special c++ operator ->*
```cpp
class $(EditorUI) {
	member_t<int> number_of_undos; 
	void undoLastAction() {
		std::cout << "You have pressed undo " << ++this->*number_of_undos << " times!\n";
		$EditorUI::undoLastAction();
	}
};
```

### 5f5bead640bc2a52c087f41c49c3354073809f46
This version added the ability to use the `$orig` macro to call the original function, instead of needing to type it out manually.
```cpp
class $(EditorUI) {
	void undoLastAction() {
		std::cout << "Undo!\n";
		$orig();
	}
};
```