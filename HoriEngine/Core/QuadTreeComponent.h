#pragma once

#include "Rect.h"
#include "Transform.h"

#include <memory>
#include <vector>
#include <World.h>
#include <Collider.h>
#include <unordered_map>
#include <map>

namespace Hori
{
	class QuadTreeNode
	{
	public:
		QuadTreeNode(std::vector<Entity> const& nodeObjects, Rect bounds, int level = 0)
			: bounds(bounds),
			level(level)
		{
			objects = nodeObjects;
			for (auto& obj : objects)
			{
				positions[obj] = Ecs::GetInstance().GetComponent<SphereCollider>(obj)->transform.position;
			}

			if (nodeObjects.size() > maxObjects && level < maxLevel)
			{
				Subdivide();
			}
		}

		void Subdivide()
		{
			float halfWidth = bounds.w / 2.0f;
			float halfHeight = bounds.h / 2.0f;

			Rect nwRect{ bounds.x,            bounds.y,            halfWidth, halfHeight };
			Rect neRect{ bounds.x + halfWidth, bounds.y,            halfWidth, halfHeight };
			Rect swRect{ bounds.x,            bounds.y + halfHeight, halfWidth, halfHeight };
			Rect seRect{ bounds.x + halfWidth, bounds.y + halfHeight, halfWidth, halfHeight };

			std::vector<Entity> nwObjects, neObjects, swObjects, seObjects;
			auto& world = Ecs::GetInstance();
			for (auto& object : objects)
			{
				auto position = world.GetComponent<Transform>(object)->position;
				if (nwRect.Contains(position))      nwObjects.push_back(object);
				else if (neRect.Contains(position)) neObjects.push_back(object);
				else if (swRect.Contains(position)) swObjects.push_back(object);
				else if (seRect.Contains(position)) seObjects.push_back(object);
			}

			objects.clear();
			positions.clear();

			if (!nwObjects.empty())
				nw = std::make_unique<QuadTreeNode>(nwObjects, nwRect, level + 1);
			if (!neObjects.empty())
				ne = std::make_unique<QuadTreeNode>(neObjects, neRect, level + 1);
			if (!swObjects.empty())
				sw = std::make_unique<QuadTreeNode>(swObjects, swRect, level + 1);
			if (!seObjects.empty())
				se = std::make_unique<QuadTreeNode>(seObjects, seRect, level + 1);
		}

		std::vector<Entity> QueryRange(const Rect& range)
		{
			std::vector<Entity> found;

			if (!bounds.Intersects(range))
				return found;

			auto& world = Ecs::GetInstance();
			for (auto& obj : objects)
			{
				auto pos = positions[obj];
				if (range.Contains(pos))
				{
					found.push_back(obj);
				}
			}

			if (nw)
			{
				auto subFound = nw->QueryRange(range);
				found.insert(found.end(), subFound.begin(), subFound.end());
			}
			if (ne)
			{
				auto subFound = ne->QueryRange(range);
				found.insert(found.end(), subFound.begin(), subFound.end());
			}
			if (sw)
			{
				auto subFound = sw->QueryRange(range);
				found.insert(found.end(), subFound.begin(), subFound.end());
			}
			if (se)
			{
				auto subFound = se->QueryRange(range);
				found.insert(found.end(), subFound.begin(), subFound.end());
			}

			return found;
		}

		std::vector<Entity> GetNeighbours(Entity entity)
		{
			auto& world = Ecs::GetInstance();

			auto position = world.GetComponent<SphereCollider>(entity)->transform.position;
			auto radius = world.GetComponent<SphereCollider>(entity)->radius;

			Rect queryArea;
			queryArea.x = position.x - 2*radius;
			queryArea.y = position.y - 2*radius;
			queryArea.w = radius * 4.f;
			queryArea.h = radius * 4.f;

			return QueryRange(queryArea);
		}

		Rect bounds{};
		int level = 0;
		int maxLevel = 5;
		int maxObjects = 5;

		std::vector<Entity> objects{};
		std::unordered_map<Entity, glm::vec2> positions{};
		std::unique_ptr<QuadTreeNode> nw{}, ne{}, sw{}, se{};
	};

	struct QuadTreeComponent
	{
		std::shared_ptr<QuadTreeNode> tree;
	};
}
