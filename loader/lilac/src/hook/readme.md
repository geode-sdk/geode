#lilac::hook

### Abstract

Currently, the hooking mechanisms used by most mod developers is ugly, inconsistent, error-prone, and incompatible.
The Geode hooking library shall be created in a way that makes it very hard for developers to make common mistakes
and look similar to actual C++ source code, rather than using C function pointers and ugly syntax.
The API should be concise, without requiring extra steps in order to enable a hook after creating it.
In addition, it should allow for easy interoperability between in-game modifications without hook conflicts.

### Design

The Geode hooking library shall be implemented using trap instructions and subroutine concepts: "trap hooking".
The advantage of this approach is that:
- Trap exceptions and subroutining are available on all major CPU architectures, as they are useful for 
debugging and writing maintainable code.
- Trap hooking typically requires less space in code to implement.
- Trap hooking allows for us to read and write to CPU registers easily.
- Trap hooking is not particularly architecture-specific, and does not require a disassembler.
- Trap hooking is much better for multihook management, compared to inline hooking.
- Trap hooking does not require a "function gateway".

The Geode hooking library can also be used for easy midfunction hooks, though this is not the main goal at this time.
(Midfunction hooks break cross-platform concept)

### Implementation

The Geode hooking library shall use this structure (or something similar) for its hook manager:
```cpp
class HookManager {
private:
    // platform dependent: should be in another class, actually
    static constexpr char trap[] = { /* the trap, or something */ };

private:
    class HookChain {
        std::vector<void*> detours = {};
        std::vector<CallFrame*> frames = {};

        char original_bytes[sizeof(trap)] = {};
    }

    class CallFrame {
        const void* address = nullptr;
        HookChain* parent = nullptr;
        char original_bytes[sizeof(trap)] = {};
    }

    static inline std::map<void*, HookChain>
        all_hooks = {};
    static inline std::map<void*, CallFrame> all_frames = {};

public:
    struct Exception {
        const void* address = nullptr;
        const void* return_address = nullptr;
        const void*& instruction_pointer;
    }

    static bool
        handler(Exception& info);
}
```

#### Adding / Removing hooks

When the user request to create a hook, we will:
- Find the address in our `all_hooks` map.
  - Add a new entry to the map at the address, if the address was not found.
  - Save the old bytes at the address, and replace it with our trap instruction.
- Push the detour into our vector of detours in our `HookChain` structure.

#### Handling trap exceptions

During execution, our trap instructions will be hit, and the operating system will send us a
notification for handling. Our handler will:

- Try to find the hook in our `all_hooks` map.
- If it found a match, we will:
  - Create a new `CallFrame` struct.
  - Save the old bytes at the return address of the exception in the `CallFrame` struct, 
  and replace it with our trap instruction.
  - Push the call frame into our `frames` vector in the matching `HookChain` structure.
  - Add a new entry to the `all_frames` map at the return address, if the return address was not found.
    - If it was found, we found a tail call. We will clean up as if we found the exception's address
    in `all_frames`. **NOTE:** This structure is logically inconsistent from actual call hierarchy,
    but does not make much of a difference and does not require storing the last `CallFrame`.
  - Redirect the instruction pointer to `detours[frames.size() - 1]`.
  - Check if we are on the last detour in the chain.
    - If so, we must remove the trap instruction so that the detour can call the original without issue.
    - The instruction will be replaced when the last detour returns.
  - **We should mark that the last detour called the next detour in the chain, for good interop purposes.**

- If it didn't find a match in `all_hooks`, it will then search `all_frames`.
- If it found a match, we will:
  - Check if we are on the last detour in the parent chain.
    - If so, we will replace the trap instruction for future hooks.
  - Restore the old bytes at the frame's address.
  - Remove the call frame from the parent chain's `frames` vector.
  - Remove the entry from the `all_frames` map.
  - **We should check if the detour we're returning from called the next detour, so modules can adjust for
  not being called.**

- If we couldn't find the address in either map, this probably isn't an exception we need to handle.
- We will return `false` from the handler, to signal to the native handler to ignore the exception / terminate the process.

#### Platform / Architecture-specific code

Platform and architecture specific code will be delegated to an interface that developers can easily override to implement
trap hooking on the desired platform. Developers will be required to specify:
- A trap opcode in hex, stored in a char array.
- An OS-native exception handler that will initialize an `Exception` instance for the core to use.
- An interface for writing to virtual memory.
- An initialization method to set up the hooking library for the platform.

### Issues

- Android does not allow for dynamic allocation in signal handlers, and it is not well-defined behavior in the POSIX standard.
  - setcontext() and RtlRestoreContext(). Use a initial setter-upper to handle allocations outside the handler, and then
  swap context to the first detour.
- I can't think of any as of now, which is good. (pie)
