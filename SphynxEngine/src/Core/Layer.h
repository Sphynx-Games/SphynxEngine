#pragma once

#include "Core.h"
//#include "Events/Event.h"
#include <string>

namespace Sphynx
{
	class SPHYNX_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void Attach() {}
		virtual void Detach() {}
		virtual void Update() {}
		//virtual void HandleEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}