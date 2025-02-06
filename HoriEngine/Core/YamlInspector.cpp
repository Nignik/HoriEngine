#include "YamlInspector.h"


namespace Hori
{

	YamlInspector::YamlInspector()
	{
		auto& world = World::GetInstance();

		m_entity = world.CreateEntity();

	}

}