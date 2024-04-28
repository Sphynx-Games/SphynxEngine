#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Renderer/RendererTypes.h"

class b2World;

namespace Sphynx
{
	enum RigidbodyType
	{
		STATIC, DYNAMIC, KINEMATIC
	};

	struct SPHYNX_API RigidbodyComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(RigidbodyComponent);

	public:
		RigidbodyComponent() : RigidbodyType(RigidbodyType::STATIC), Simulate(true) {}
		
		RigidbodyComponent(RigidbodyType rigidbodyType) : RigidbodyType(rigidbodyType), Simulate(true) {}

	public:
		RigidbodyType RigidbodyType;
		bool Simulate;
	};
}
