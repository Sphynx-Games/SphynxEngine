#pragma once

#include <Sphynx.h>
#include <Core/EntryPoint.h>


class EditorApplication : public Sphynx::Application
{
public:
	EditorApplication();

public:
	virtual void Init() override;
	virtual void Run() override;
	virtual void Shutdown() override;

};
