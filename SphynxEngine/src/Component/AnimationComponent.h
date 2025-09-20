#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Container/Array.h"
#include "Asset/Asset.h"


namespace Sphynx
{
	struct SPHYNX_API AnimationComponent
	{
		SPX_REFLECT_GENERATED_BODY();
		COMPONENT_COMMON_BODY(AnimationComponent);

	public:
		AnimationComponent(AssetHandle animation, float duration, bool loop = false) :
			Animation(animation), Duration(duration), Loop(loop) {}

	public:
		AssetHandle Animation;
		float Duration;
		bool Loop;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::AnimationComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(Animation)
SPX_REFLECT_PROPERTY(Duration)
SPX_REFLECT_PROPERTY(Loop)

SPX_REFLECT_STRUCT_END(Sphynx::AnimationComponent, SPHYNX_API)