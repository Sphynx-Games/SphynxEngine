#pragma once

#include <Sphynx.h>


namespace Sphynx
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication();

	public:
		virtual void Init(const HashMap<CommandArgument, Array<std::string>>& options) override;
		virtual void Run() override;
		virtual void Shutdown() override;

	private:
		class EditorLayer* m_EditorLayer;

	};
}
