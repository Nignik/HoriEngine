#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <unordered_map>
#include <functional>

namespace Hori
{
	struct IEvent
	{
		virtual ~IEvent() = default;
	};

	class EventManager
	{
	public:
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		static EventManager& GetInstance()
		{
			static EventManager instance;
			return instance;
		}

		template <typename EventType, typename... Args>
		void AddEvents(Args&&... args)
		{
			std::unique_ptr<EventType> event = std::make_unique<EventType>(std::forward<Args>(args)...);
			m_eventQueues[std::type_index(typeid(EventType))].push_back(std::move(event));
		}

		template <typename EventType>
		void Subscribe(std::function<void(const EventType&)> listener)
		{
			auto wrapper = [fn = std::move(listener)](const IEvent& e) {
				fn(static_cast<const EventType&>(e));
				};
			m_subscribers[std::type_index(typeid(EventType))].push_back(std::move(wrapper));
		}

		void DispatchEvents()
		{
			for (auto& [typeIdx, events] : m_eventQueues)
            {
                auto subIter = m_subscribers.find(typeIdx);
                if (subIter != m_subscribers.end())
                {
                    // For each event in that queue
                    for (auto& eventPtr : events)
                    {
                        // Call every subscriber
                        for (auto& subscriberFn : subIter->second)
                        {
                            subscriberFn(*eventPtr);
                        }
                    }
                }
            }
            Clear();
		}

		template<typename EventType>
		std::unique_ptr<EventType> PopEvent()
		{
			auto& vec = m_eventQueues[std::type_index(typeid(EventType))];
			if (vec.empty())
			{
				return nullptr;
			}

			std::unique_ptr<IEvent> event = std::move(vec.back());
			vec.pop_back();

			if (EventType* casted = dynamic_cast<EventType*>(event.get()))
			{
				return std::unique_ptr<EventType>(static_cast<EventType*>(event.release()));
			}

			return nullptr;
		}

		void Clear()
		{
			m_eventQueues.clear();
		}

		/*template <typename EventType>
		std::vector<std::unique_ptr<EventType>> GetEvents()
		{
			auto& baseVec = m_eventQueues[std::type_index(typeid(EventType))];
			std::vector<EventType*> result;
			result.reserve(baseVec.size());

			for (auto& e : baseVec)
			{
				if (auto* casted = dynamic_cast<EventType*>(e.get()))
				{
					result.push_back(casted);
				}
			}
			return result;
		}*/

	private:
		EventManager() = default;
		
		std::unordered_map<std::type_index, std::vector<std::unique_ptr<IEvent>>> m_eventQueues{};
        using SubscriberFn = std::function<void(const IEvent&)>;
		std::unordered_map<std::type_index, std::vector<SubscriberFn>> m_subscribers{};
	};
}