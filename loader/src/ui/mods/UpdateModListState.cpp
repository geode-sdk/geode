#include "UpdateModListState.hpp"

UpdateModListStateEvent::UpdateModListStateEvent(UpdateState&& target) : target(target) {}

ListenerResult UpdateModListStateFilter::handle(std::function<Callback> fn, UpdateModListStateEvent* event) {
    if (
        // If the listener wants to hear all state updates then let it
        std::holds_alternative<UpdateWholeState>(m_target) ||
        // If the event is update everything then update everything
        std::holds_alternative<UpdateWholeState>(event->target) ||
        // Otherwise only run if the event is what is asked for
        m_target == event->target
    ) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

UpdateModListStateFilter::UpdateModListStateFilter() : m_target(UpdateWholeState()) {}
UpdateModListStateFilter::UpdateModListStateFilter(UpdateState&& target) : m_target(target) {}
