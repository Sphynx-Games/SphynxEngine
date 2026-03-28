#pragma once

#include <Sphynx.h>


namespace Sphynx
{
	class EditorLayer;
	class OperationManager;

	class EditorApplication : public Application
	{
	public:
		EditorApplication();

	public:
		static EditorApplication* GetInstance();

		virtual void Init(const HashMap<std::string, Array<std::string>>& commandArguments) override;
		virtual void Run() override;
		virtual void Shutdown() override;

		void OpenAssetEditor(const std::string& name, const AssetMetadata& metadata);
		OperationManager* GetOperationManager() const;

	private:
		EditorLayer* m_EditorLayer;

	};
}
