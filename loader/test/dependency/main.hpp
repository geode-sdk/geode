#pragma once

#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
    #ifdef EXPORTING_MOD
        #define GEODE_TESTDEP_DLL __declspec(dllexport)
    #else
        #define GEODE_TESTDEP_DLL __declspec(dllimport)
    #endif
#else
    #define GEODE_TESTDEP_DLL
#endif

class GEODE_TESTDEP_DLL TestEvent : public Event {
protected:
    std::string data;

public:
    std::string getData() const;
    TestEvent(std::string const& data);
};

class GEODE_TESTDEP_DLL TestEventFilter : public EventFilter<TestEvent> {
public:
    using Callback = void(TestEvent*);

    ListenerResult handle(utils::MiniFunction<Callback> fn, TestEvent* event);
    TestEventFilter();
    TestEventFilter(TestEventFilter const&) = default;
};
