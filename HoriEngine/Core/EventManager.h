#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>

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
		}

	private:
		EventManager() = default;

		std::unordered_map<std::type_index, std::vector<std::unique_ptr<IEvent>>> m_eventQueues;
	};
}