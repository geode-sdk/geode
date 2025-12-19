#pragma once

#include "../loader/Event.hpp"
#include <optional>

namespace cocos2d {
    class CCNode;
}

namespace geode {
    template<class T>
    concept InheritsCCNode = std::is_base_of_v<cocos2d::CCNode, T>;

    // Base class; exists so event listeners can be placed dynamically at runtime
    struct GEODE_DLL AEnterLayerEvent : public Event {
        const std::string layerID;
        cocos2d::CCNode* layer;

        AEnterLayerEvent(
            std::string layerID,
            cocos2d::CCNode* layer
        );
    };

    class GEODE_DLL AEnterLayerFilter : public EventFilter<AEnterLayerEvent> {
	public:
		using Callback = void(AEnterLayerEvent*);

    protected:
		std::optional<std::string> m_targetID;

	public:
        ListenerResult handle(geode::Function<Callback>& fn, AEnterLayerEvent* event);

		AEnterLayerFilter(
			std::optional<std::string> id
		);
        AEnterLayerFilter(AEnterLayerFilter const&) = default;
    };

    template<InheritsCCNode T>
    class EnterLayerEvent final : public AEnterLayerEvent {
    public:
        EnterLayerEvent(
            std::string layerID,
            T* layer
        ) : AEnterLayerEvent(std::move(layerID), layer) {}

        T* getLayer() const {
            return static_cast<T*>(this->layer);
        }
        std::string_view getID() const {
            return this->layerID;
        }
        EventListenerPool* getPool() const override {
            return DefaultEventListenerPool::getForEvent<EnterLayerEvent<T>>();
        }
    };

    template<class T, class N>
    concept InheritsEnterLayer = std::is_base_of_v<EnterLayerEvent<N>, T>;

    template<class N, InheritsEnterLayer<N> T = EnterLayerEvent<N>>
	class EnterLayerFilter final : public EventFilter<EnterLayerEvent<N>> {
	public:
		using Callback = void(T*);

	protected:
		std::optional<std::string> m_targetID;

	public:
        ListenerResult handle(geode::Function<Callback> fn, EnterLayerEvent<N>* event) {
            if (m_targetID.has_value()) {
                if (m_targetID.value() == event->getID()) {
                    fn(static_cast<T*>(event));
                }
            }
            else {
                fn(static_cast<T*>(event));
            }
			return ListenerResult::Propagate;
		}

		EnterLayerFilter(
			std::optional<std::string> id
		) : m_targetID(std::move(id)) {}
        EnterLayerFilter(EnterLayerFilter const&) = default;
        EventListenerPool* getPool() const override {
            return DefaultEventListenerPool::getForEvent<EnterLayerEvent<N>>();
        }
	};
}
