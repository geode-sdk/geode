#pragma once

#include "../loader/Event.hpp"

namespace cocos2d {
    class CCNode;
}

namespace geode {
    template<class T>
    concept InheritsCCNode = std::is_base_of_v<cocos2d::CCNode, T>;

    template<InheritsCCNode T>
    class EnterLayerEvent : public Event {
    protected:
        std::string m_layerID;
        T* m_layer;
    
    public:
        EnterLayerEvent(
            std::string const& layerID,
            T* layer
        ) : m_layerID(layerID),
            m_layer(layer) {}

        std::string getID() const {
            return m_layerID;
        }

        T* getLayer() const {
            return m_layer;
        }
    };

    template<class T, class N>
    concept InheritsEnterLayer = std::is_base_of_v<EnterLayerEvent<N>, T>;

    template<class N, InheritsEnterLayer<N> T>
	class EnterLayerEventHandler : public EventHandler<EnterLayerEvent<N>> {
	public:
		using Consumer = void(*)(T*);
        
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
