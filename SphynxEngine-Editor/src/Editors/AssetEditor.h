#pragma once

#include "Base/Editor.h"
#include <Container/Map.h>
#include <Container/Pair.h>
#include <Asset/Asset.h>


namespace Sphynx
{
	class EditorLayer;
	struct AssetMetadata;

	class AssetEditor : public Editor
	{
	public:
		AssetEditor();
		virtual ~AssetEditor() = default;

		void OpenAssetEditor(const std::string& name, const AssetMetadata& metadata, EditorLayer* editorLayer);

	protected:
		virtual void RenderGUI() override;

		// Menu bar
		virtual bool HasMenuBar() const override { return false; }
		virtual void RenderMenuBar() override;

	private:
		HashMap<AssetHandle, Pair<Editor*, std::string>> m_AssetEditors;
	};
}