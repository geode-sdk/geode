#pragma once

#include <Geode/loader/Event.hpp>
#include "sources/ModSource.hpp"

using namespace geode::prelude;

struct UpdatePageNumberState final {
    bool operator==(UpdatePageNumberState const&) const = default;
};
struct UpdateWholeState final {
    std::optional<std::string> searchByDeveloper;
    UpdateWholeState() = default;
    inline explicit UpdateWholeState(std::optional<std::string> dev) : searchByDeveloper(std::move(dev)) {}
    bool operator==(UpdateWholeState const&) const = default;
};
struct UpdateModState final {
    std::string modID;
    inline explicit UpdateModState(std::string modID) : modID(std::move(modID)) {}
    bool operator==(UpdateModState const&) const = default;
};
using UpdateState = std::variant<UpdatePageNumberState, UpdateWholeState, UpdateModState>;

class UpdateModListStateEvent final : public SimpleEvent<UpdateModListStateEvent, UpdateState> {
public:
    // listener params target
    using SimpleEvent::SimpleEvent;
};