#pragma once

#include "../loader/Event.hpp"

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
            std::string const& layerID,
            cocos2d::CCNode* layer
        );
    };

    class GEODE_DLL AEnterLayerFilter : public EventFilter<AEnterLayerEvent> {
	public:
		using Callback = void(AEnterLayerEvent*);
    
    protected:
		std::optional<std::string> m_targetID;
	
	public:
        ListenerResult handle(utils::MiniFunction<Callback> fn, AEnterLayerEvent* event);

		AEnterLayerFilter(
			std::optional<std::string> const& id
		);
        AEnterLayerFilter(AEnterLayerFilter const&) = default;
    };

    template<InheritsCCNode T>
    class EnterLayerEvent : public AEnterLayerEvent {
    public:
        EnterLayerEvent(
            std::string const& layerID,
            T* layer
        ) : AEnterLayerEvent(layerID, layer) {}

        T* getLayer() const {
            return static_cast<T*>(this->layer);
        }
    };

    template<class T, class N>
    concept InheritsEnterLayer = std::is_base_of_v<EnterLayerEvent<N>, T>;

    template<class N, InheritsEnterLayer<N> T = EnterLayerEvent<N>>
	class EnterLayerFilter : public EventFilter<EnterLayerEvent<N>> {
	public:
		using Callback = void(T*);
        
	protected:
		std::optional<std::string> m_targetID;
	
	public:
        ListenerResult handle(utils::MiniFunction<Callback> fn, EnterLayerEvent<N>* event) {
            if (m_targetID == event->getID()) {
                fn(static_cast<T*>(event));
            }
			return ListenerResult::Propagate;
		}

		EnterLayerFilter(
			std::optional<std::string> const& id
		) : m_targetID(id) {}
        EnterLayerFilter(EnterLayerFilter const&) = default;
	};
}
