#pragma once
#include <imgui_node_editor.h>
#include "Sas/NodeEditot/BuilderNode.h"

#include "Sas/Renderer/Texture.h"

#include <cppparser.h>
#include <cppwriter.h>

namespace Sas {
	namespace ed = ax::NodeEditor;

	class NodeEditorPanel
	{
	public:
		enum class PinKind
		{
			Output,
			Input
		};
	private:
		enum class PinType
		{
			Flow,
			Bool,
			Int,
			Float,
			String,
			Object,
			Function,
			Delegate,
		};

		

		enum class NodeType
		{
			Blueprint,
			Simple,
			Tree,
			Comment,
			Houdini
		};

	public:
		struct Node;

		struct Pin
		{
			ed::PinId   ID;
			Node* Node;
			std::string Name;
			PinType     Type;
			PinKind     Kind;

			Pin(int id, const char* name, PinType type) :
				ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
			{
			}
		};

		struct Node
		{
			ed::NodeId ID;
			std::string Name;
			std::vector<Pin> Inputs;
			std::vector<Pin> Outputs;
			ImColor Color;
			NodeType Type;
			ImVec2 Size;

			std::string State;
			std::string SavedState;

			Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
				ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
			{
			}
		};

		struct Link
		{
			ed::LinkId ID;

			ed::PinId StartPinID;
			ed::PinId EndPinID;

			ImColor Color;

			Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) :
				ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
			{
			}
		};
	public:
		NodeEditorPanel();

		void OnAttach();
		void OnDetach();
		void OnImGuiRender(int i, ImVec2 size);

		Node* SpawnInputActionNode();
		Node* SpawnOutputActionNode();
		Node* SpawnBranchNode();
		Node* SpawnEmpty(const std::string& Name);

		void AddPin(Node* node, const PinKind kind, const std::string& name);
		Node* FindNode(ed::NodeId id);
		Link* FindLink(ed::LinkId id);
		Pin* FindPin(ed::PinId id);

		bool IsPinLinked(ed::PinId id);
		bool CanCreateLink(Pin* a, Pin* b);



	private:
		ed::EditorContext*   m_Context = nullptr;    // Editor context, required to trace a editor state.
		ed::EditorContext*   m_Context2 = nullptr;    // Editor context, required to trace a editor state.

		bool                 m_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
		std::vector<Link>    m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		std::vector<Node>    m_Nodes;
		int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
		
		Ref<Texture> m_Background;
		Ref<BuilderNode> m_BuilderNode;
	
	private:

		void DrawPinIcon(const Pin& pin, bool connected, int alpha);

		ImColor GetIconColor(PinType type);
	private:
		static const int s_PinIconSize = 24;
		static int s_NextId;
		static int GetNextId() {return s_NextId++;};

	public:
		static void BuildNode(Node* node);
	};


	class FunctionPaser
	{
	public:
		FunctionPaser();
		Ref<NodeEditorPanel> AddNodePanel(Ref<NodeEditorPanel> panel, const std::string& Name);
		void OnDetach();
		void OnImGuiRender();
	private:
		std::vector<std::string> m_Names;
		std::vector<Ref<NodeEditorPanel>> m_Panels;
	};

	class EditorCppPaser
	{
	public:
		EditorCppPaser();
		Ref<NodeEditorPanel> AddNodePanel(Ref<NodeEditorPanel> panel, const std::string& Name);

		Ref<FunctionPaser> AddFunctionPanel(Ref<FunctionPaser> panel, const std::string& Name);
		void OnDetach();
		void OnImGuiRender();
	private:
		std::vector<std::string> m_Names;
		std::vector<Ref<NodeEditorPanel>> m_Panels;
		std::vector<std::string> m_NamesFunc;
		std::vector<Ref<FunctionPaser>> m_Function;
	};
}
