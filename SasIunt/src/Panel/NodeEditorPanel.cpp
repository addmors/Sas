#include <imgui/imgui_internal.h>
#include <imgui\imgui.h>
#include "ssph.h"
#include "NodeEditorPanel.h"
#include "Sas/NodeEditot/Widgets.h"

#define IMGUI_DEFINE_MATH_OPERATORS


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
	
	ImColor NodeEditorPanel::GetIconColor(PinType type)
	{
		{
			switch (type)
			{
			default:
			case PinType::Flow:     return ImColor(255, 255, 255);
			case PinType::Bool:     return ImColor(220, 48, 48);
			case PinType::Int:      return ImColor(68, 201, 156);
			case PinType::Float:    return ImColor(147, 226, 74);
			case PinType::String:   return ImColor(124, 21, 153);
			case PinType::Object:   return ImColor(51, 150, 215);
			case PinType::Function: return ImColor(218, 0, 183);
			case PinType::Delegate: return ImColor(255, 48, 48);
			}
		};
	}

	void NodeEditorPanel::DrawPinIcon(const Pin& pin, bool connected, int alpha)
	{
			Drawing::IconType iconType;
			ImColor  color = GetIconColor(pin.Type);
			color.Value.w = alpha / 255.0f;
			
			switch (pin.Type)
			{
			case PinType::Flow:     iconType = Drawing::IconType::Flow;   break;
			case PinType::Bool:     iconType = Drawing::IconType::Circle; break;
			case PinType::Int:      iconType = Drawing::IconType::Circle; break;
			case PinType::Float:    iconType = Drawing::IconType::Circle; break;
			case PinType::String:   iconType = Drawing::IconType::Circle; break;
			case PinType::Object:   iconType = Drawing::IconType::Circle; break;
			case PinType::Function: iconType = Drawing::IconType::Circle; break;
			case PinType::Delegate: iconType = Drawing::IconType::Square; break;
			default:
				return;
			}

			Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
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
		ed::SetCurrentEditor(m_Context);

		Node* node;
		node = SpawnInputActionNode();      ed::SetNodePosition(node->ID, ImVec2(-252, 220));
		node = SpawnBranchNode();           ed::SetNodePosition(node->ID, ImVec2(-300, 351));

		ed::NavigateToContent();	
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

		ed::NodeId contextNodeId = 0;
		ed::LinkId contextLinkId = 0;
		ed::PinId  contextPinId = 0;
		bool createNewNode = false;
		Pin* newNodeLinkPin = nullptr;
		Pin* newLinkPin = nullptr;

		float leftPaneWidth = 400.0f;
		float rightPaneWidth = 800.0f;
		
		
		ImGui::SameLine(0.0f, 12.0f);

		ed::Begin("Node editor");
		{
			auto cursorTopLeft = ImGui::GetCursorScreenPos();
			for (auto& node : m_Nodes)
			{
				if (node.Type != NodeType::Blueprint && node.Type != NodeType::Simple)
					continue;

				const auto isSimple = node.Type == NodeType::Simple;

				bool hasOutputDelegates = false;
				for (auto& output : node.Outputs)
					if (output.Type == PinType::Delegate)
						hasOutputDelegates = true;

				m_BuilderNode->Begin(node.ID);
				if (!isSimple)
				{
					m_BuilderNode->Header(node.Color);
					ImGui::Spring(0);
					ImGui::TextUnformatted(node.Name.c_str());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					if (hasOutputDelegates)
					{
						ImGui::BeginVertical("delegates", ImVec2(0, 28));
						ImGui::Spring(1, 0);
						for (auto& output : node.Outputs)
						{
							if (output.Type != PinType::Delegate)
								continue;

							auto alpha = ImGui::GetStyle().Alpha;
							if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
								alpha = alpha * (48.0f / 255.0f);

							ed::BeginPin(output.ID, ed::PinKind::Output);
							ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
							ed::PinPivotSize(ImVec2(0, 0));
							ImGui::BeginHorizontal(output.ID.AsPointer());
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
							if (!output.Name.empty())
							{
								ImGui::TextUnformatted(output.Name.c_str());
								ImGui::Spring(0);
							}
							DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
							ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
							ImGui::EndHorizontal();
							ImGui::PopStyleVar();
							ed::EndPin();

							//DrawItemRect(ImColor(255, 0, 0));
						}
						ImGui::Spring(1, 0);
						ImGui::EndVertical();
						ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
					}
					else
						ImGui::Spring(0);
					m_BuilderNode->EndHeader();
				}

				for (auto& input : node.Inputs)
				{
					auto alpha = ImGui::GetStyle().Alpha;
					if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);

					m_BuilderNode->Input(input.ID);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
					ImGui::Spring(0);
					if (!input.Name.empty())
					{
						ImGui::TextUnformatted(input.Name.c_str());
						ImGui::Spring(0);
					}
					if (input.Type == PinType::Bool)
					{
						ImGui::Button("Hello");
						ImGui::Spring(0);
					}
					ImGui::PopStyleVar();
					m_BuilderNode->EndInput();
				}

				if (isSimple)
				{
					m_BuilderNode->Middle();

					ImGui::Spring(1, 0);
					ImGui::TextUnformatted(node.Name.c_str());
					ImGui::Spring(1, 0);
				}

				for (auto& output : node.Outputs)
				{
					if (!isSimple && output.Type == PinType::Delegate)
						continue;

					auto alpha = ImGui::GetStyle().Alpha;
					if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);

					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					m_BuilderNode->Output(output.ID);
					if (output.Type == PinType::String)
					{
						static char buffer[128] = "Edit Me\nMultiline!";
						static bool wasActive = false;

						ImGui::PushItemWidth(100.0f);
						ImGui::InputText("##edit", buffer, 127);
						ImGui::PopItemWidth();
						if (ImGui::IsItemActive() && !wasActive)
						{
							ed::EnableShortcuts(false);
							wasActive = true;
						}
						else if (!ImGui::IsItemActive() && wasActive)
						{
							ed::EnableShortcuts(true);
							wasActive = false;
						}
						ImGui::Spring(0);
					}
					if (!output.Name.empty())
					{
						ImGui::Spring(0);
						ImGui::TextUnformatted(output.Name.c_str());
					}
					ImGui::Spring(0);
					DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
					ImGui::PopStyleVar();
					m_BuilderNode->EndOutput();
				}

				m_BuilderNode->End();
			}
			// Submit Links
			for (auto& linkInfo : m_Links)
				ed::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID);

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
				for (auto& link : m_Links)
					ed::Flow(link.ID);
		}
		
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
