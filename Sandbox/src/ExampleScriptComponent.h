#pragma once

#include <Component/ScriptComponent.h>
#include <Logging/Log.h>
#include "Sandbox.h"
#include "Container/Array.h"
#include "Asset/AssetMetadata.h"


class SANDBOX_API ExampleScriptComponent : public Sphynx::ScriptComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	ExampleScriptComponent() : m_Num(0) {}

public:
	virtual void BeginPlay() override
	{
		SPX_LOG_DEBUG("Start BeginPlay...");
		Sphynx::ScriptComponent::BeginPlay();
	}

	virtual void EndPlay() override
	{
		SPX_LOG_DEBUG("Start EndPlay...");
		Sphynx::ScriptComponent::EndPlay();
	}

	virtual void Update(float deltaTime) override
	{
		//SPX_LOG_DEBUG("Num = {}", m_Num);
		++m_Num;

		Sphynx::ScriptComponent::Update(deltaTime);
	}

private:
	uint32_t m_Num;
	Sphynx::Array<Sphynx::AssetHandle> m_Handles;
};

#include "Reflection/Reflection.h"


SPX_REFLECT_CLASS_BEGIN(ExampleScriptComponent, SANDBOX_API)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(m_Num)
SPX_REFLECT_PROPERTY(m_Handles)

SPX_REFLECT_CLASS_END(ExampleScriptComponent, SANDBOX_API)