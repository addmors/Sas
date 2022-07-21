#pragma once
# include <imgui_node_editor.h>


namespace Sas {

	class BuilderNode {
	public:
		BuilderNode(ImTextureID texture = nullptr, int textureWidth = 0, int textureHeight = 0);
		void Begin(ax::NodeEditor::NodeId id);
		void End();

		void Header(const ImVec4& color = ImVec4(1, 1, 1, 1));
		void EndHeader();

		void Input(ax::NodeEditor::PinId id);
		void EndInput();
		
		void Middle();

		void Output(ax::NodeEditor::PinId id);
		void EndOutput();

		
	private:
		enum class Stage
		{
			Invalid,
			Begin,
			Header,
			Content,
			Input,
			Output,
			Middle,
			End
		};

		bool SetStage(Stage stage);

		void Pin(ax::NodeEditor::PinId id, ax::NodeEditor::PinKind kind);
		void EndPin();

		ImTextureID HeaderTextureId;
		int         HeaderTextureWidth;
		int         HeaderTextureHeight;
		ax::NodeEditor::NodeId      CurrentNodeId;
		ImU32       HeaderColor;
		ImVec2      NodeMin;
		ImVec2      NodeMax;
		ImVec2      HeaderMin;
		ImVec2      HeaderMax;
		ImVec2      ContentMin;
		ImVec2      ContentMax;
		Stage		CurrentStage;
		bool        HasHeader;
	};
}

