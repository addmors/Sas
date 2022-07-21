#include "ssph.h"
#include "NodeEditorPanel.h"

# define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui\imgui.h>
# include <imgui/imgui_internal.h>

namespace Sas {
	int NodeEditorPanel::s_NextId = 1;
	
	static void ImGuiEx_BeginColumn()
	{
		ImGui::BeginGroup();
	}

	static void ImGuiEx_NextColumn()
	{
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
	}
	static void ImGuiEx_EndColumn()
	{
		ImGui::EndGroup();
	}

	void NodeEditorPanel::BuildNode(NodeEditorPanel::Node* node)
	{
		for (auto& input : node->Inputs)
		{
			input.Node = node;
			input.Kind = PinKind::Input;
		}

		for (auto& output : node->Outputs)
		{
			output.Node = node;
			output.Kind = PinKind::Output;
		}
	};

	NodeEditorPanel::Node* NodeEditorPanel::FindNode(ed::NodeId id)
	{
		for (auto& node : m_Nodes)
			if (node.ID == id)
				return &node;

		return nullptr;
	}

	NodeEditorPanel::Link* NodeEditorPanel::FindLink(ed::LinkId id)
	{
		for (auto& link : m_Links)
			if (link.ID == id)
				return &link;

		return nullptr;
	}

	NodeEditorPanel::Pin* NodeEditorPanel::FindPin(ed::PinId id)
	{
		if (!id)
			return nullptr;

		for (auto& node : m_Nodes)
		{
			for (auto& pin : node.Inputs)
				if (pin.ID == id)
					return &pin;

			for (auto& pin : node.Outputs)
				if (pin.ID == id)
					return &pin;
		}

		return nullptr;
	}

	bool NodeEditorPanel::IsPinLinked(ed::PinId id)
	{
		if (!id)
			return false;

		for (auto& link : m_Links)
			if (link.StartPinID == id || link.EndPinID == id)
				return true;

		return false;
	};
	
	bool NodeEditorPanel::CanCreateLink(Pin* a, Pin* b)
	{
		if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
			return false;

		return true;
	}

	NodeEditorPanel::NodeEditorPanel()
	{
		m_Background = Texture2D::Create("Resources/Icons/NodeEditor/BlueprintBackground.png");
		m_BuilderNode = CreateRef<BuilderNode>((ImTextureID)m_Background->GetRendererID(),m_Background->GetWidth(), m_Background->GetHeight());
	}

	void NodeEditorPanel::OnAttach() {
		ed::Config config;
		config.SettingsFile = "Simple.json"; //TODO: What is it?
		m_Context = ed::CreateEditor(&config);
	};

	void NodeEditorPanel::OnDetach() {
		ed::DestroyEditor(m_Context);
	};

	void NodeEditorPanel::OnImGuiRender()
	{
		ImGui::Begin("Node Editor");

		
		auto& io = ImGui::GetIO();

		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

		ImGui::Separator();

		ed::SetCurrentEditor(m_Context);

		// Start interaction with editor.
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));

		int uniqueId = 1;

		//
		// 1) Commit known data to editor
		//

		// Submit Node A
		ed::NodeId nodeA_Id = uniqueId++;
		ed::PinId  nodeA_InputPinId = uniqueId++;
		ed::PinId  nodeA_OutputPinId = uniqueId++;

		if (m_FirstFrame)
			ed::SetNodePosition(nodeA_Id, ImVec2(10, 10));
		ed::BeginNode(nodeA_Id);
		ImGui::Text("Node A");
		ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
		ImGui::Text("-> In");
		ed::EndPin();
		ImGui::SameLine();
		ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
		ImGui::Text("Out ->");
		ed::EndPin();
		ed::EndNode();

		// Submit Node B
		ed::NodeId nodeB_Id = uniqueId++;
		ed::PinId  nodeB_InputPinId1 = uniqueId++;
		ed::PinId  nodeB_InputPinId2 = uniqueId++;
		ed::PinId  nodeB_OutputPinId = uniqueId++;

		if (m_FirstFrame)
			ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));
		ed::BeginNode(nodeB_Id);
		ImGui::Text("Node B");
		ImGuiEx_BeginColumn();
		ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
		ImGui::Text("-> In1");
		ed::EndPin();
		ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
		ImGui::Text("-> In2");
		ed::EndPin();
		ImGuiEx_NextColumn();
		ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
		ImGui::Text("Out ->");
		ed::EndPin();
		ImGuiEx_EndColumn();
		ed::EndNode();

		// Submit Links
		for (auto& linkInfo : m_Links)
			ed::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID);

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
			for (auto& link : m_Links)
				ed::Flow(link.ID);

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate())
		{
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId))
			{
				// QueryNewLink returns true if editor want to create new link between pins.
				//
				// Link can be created only for two valid pins, it is up to you to
				// validate if connection make sense. Editor is happy to make any.
				//
				// Link always goes from input to output. User may choose to drag
				// link from output pin or input pin. This determine which pin ids
				// are valid and which are not:
				//   * input valid, output invalid - user started to drag new ling from input pin
				//   * input invalid, output valid - user started to drag new ling from output pin
				//   * input valid, output valid   - user dragged link over other pin, can be validated

				if (inputPinId && outputPinId) // both are valid, let's accept link
				{
					// ed::AcceptNewItem() return true when user release mouse button.
					if (ed::AcceptNewItem())
					{
						// Since we accepted new link, lets add one to our list of links.
						m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ed::Link(m_Links.back().ID, m_Links.back().StartPinID, m_Links.back().EndPinID);
					}

					// You may choose to reject connection between these nodes
					// by calling ed::RejectNewItem(). This will allow editor to give
					// visual feedback by changing link thickness and color.
				}
			}
		}
		ed::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ed::BeginDelete())
		{
			ed::LinkId linkId = 0;
			while (ed::QueryDeletedLink(&linkId))
			{
				if (ed::AcceptDeletedItem())
				{
					auto id = std::find_if(m_Links.begin(), m_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
					if (id != m_Links.end())
						m_Links.erase(id);
				}
			}

			ed::NodeId nodeId = 0;
			while (ed::QueryDeletedNode(&nodeId))
			{
				if (ed::AcceptDeletedItem())
				{
					auto id = std::find_if(m_Nodes.begin(), m_Nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
					if (id != m_Nodes.end())
						m_Nodes.erase(id);
				}
			}
		}
		ed::EndDelete();



		// End of interaction with editor.
		ed::End();

		if (m_FirstFrame)
			ed::NavigateToContent(0.0f);

		ed::SetCurrentEditor(nullptr);

		m_FirstFrame = false;

		ImGui::End();
	}
	NodeEditorPanel::Node* NodeEditorPanel::SpawnInputActionNode()
	{
		m_Nodes.emplace_back(GetNextId(), "InputAction Fire", ImColor(255, 128, 128));
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Delegate);
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "Pressed", PinType::Flow);
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "Released", PinType::Flow);

		BuildNode(&m_Nodes.back());

		return &m_Nodes.back();
	}

	NodeEditorPanel::Node* NodeEditorPanel::SpawnBranchNode()
	{
		m_Nodes.emplace_back(GetNextId(), "Branch");
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "True", PinType::Flow);
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "False", PinType::Flow);

		BuildNode(&m_Nodes.back());

		return &m_Nodes.back();
	};

}
