#pragma once

#include "../loader/Event.hpp"

namespace cocos2d {
    class CCNode;
}

namespace geode {
    template<class T>
    concept InheritsCCNode = std::is_base_of_v<cocos2d::CCNode, T>;

    // Base class; exists so event listeners can be placed dynamically at runtime
    class GEODE_DLL AEnterLayerEvent : public Event {
    protected:
        std::string m_layerID;
        cocos2d::CCNode* m_layer;
    
    public:
        AEnterLayerEvent(
            std::string const& layerID,
            cocos2d::CCNode* layer
        );

        std::string getID() const;
        cocos2d::CCNode* getLayer() const;
    };

    class GEODE_DLL AEnterLayerEventHandler : public EventHandler<AEnterLayerEvent> {
	public:
		using Consumer = std::function<void(AEnterLayerEvent*)>;
    
    protected:
		Consumer m_consumer;
		std::optional<std::string> m_targetID;
	
	public:
        PassThrough handle(AEnterLayerEvent* event) override;

		AEnterLayerEventHandler(
			std::optional<std::string> const& id,
			Consumer handler
		);
    };

    template<InheritsCCNode T>
    class EnterLayerEvent : public AEnterLayerEvent {
    public:
        EnterLayerEvent(
            std::string const& layerID,
            T* layer
        ) : AEnterLayerEvent(layerID, layer) {}

        T* getLayer() const {
            return static_cast<T*>(m_layer);
        }
    };

    template<class T, class N>
    concept InheritsEnterLayer = std::is_base_of_v<EnterLayerEvent<N>, T>;

    template<class N, InheritsEnterLayer<N> T>
	class EnterLayerEventHandler : public EventHandler<EnterLayerEvent<N>> {
	public:
		using Consumer = std::function<void(T*)>;
        
	protected:
		Consumer m_consumer;
		std::optional<std::string> m_targetID;
	
	public:
        PassThrough handle(EnterLayerEvent<N>* event) override {
            if (m_targetID == event->getID()) {
                m_consumer(static_cast<T*>(event));
            }
			return PassThrough::Propagate;
		}

		EnterLayerEventHandler(
			std::optional<std::string> const& id,
			Consumer handler
		) : m_targetID(id),
            m_consumer(handler) {}
	};
}
