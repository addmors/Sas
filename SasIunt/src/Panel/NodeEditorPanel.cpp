#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>
#include "ssph.h"
#include "NodeEditorPanel.h"
#include "Sas/NodeEditot/Widgets.h"



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

	void NodeEditorPanel::AddPin(Node* node, const PinKind kind, const std::string& name)
	{
		if(kind == PinKind::Input)
			node->Inputs.emplace_back(GetNextId(), name.c_str(), PinType::Flow);
		else
			node->Outputs.emplace_back(GetNextId(), name.c_str(), PinType::Flow);
		BuildNode(node);
	}

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
		m_Context = ed::CreateEditor();
		ed::SetCurrentEditor(m_Context);
		ed::NavigateToContent();
	};

	void NodeEditorPanel::OnDetach() {
		ed::DestroyEditor(m_Context);
	};

	void NodeEditorPanel::OnImGuiRender(int i, ImVec2 size)
	{
		ed::SetCurrentEditor(m_Context);

		if(m_FirstFrame)
			ed::NavigateToContent();

		ed::NodeId contextNodeId = 0;
		ed::LinkId contextLinkId = 0;
		ed::PinId  contextPinId = 0;
		bool createNewNode = false;
		Pin* newNodeLinkPin = nullptr;
		Pin* newLinkPin = nullptr;

		float leftPaneWidth = 400.0f;
		float rightPaneWidth = 800.0f;
		
		
		ImGui::SameLine(0.0f, 12.0f);
		std::string s = "NE" + std::to_string(i);
		ed::Begin(s.c_str(),size);
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
				ed::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID, linkInfo.Color, 2.0f);

			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
				for (auto& link : m_Links)
					ed::Flow(link.ID);
		

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
			if (!createNewNode)
			{
				if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
				{
					auto showLabel = [](const char* label, ImColor color)
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
						auto size = ImGui::CalcTextSize(label);

						auto padding = ImGui::GetStyle().FramePadding;
						auto spacing = ImGui::GetStyle().ItemSpacing;

						ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

						auto rectMin = ImGui::GetCursorScreenPos() - padding;
						auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

						auto drawList = ImGui::GetWindowDrawList();
						drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
						ImGui::TextUnformatted(label);
					};

					ed::PinId startPinId = 0, endPinId = 0;

					if (ed::QueryNewLink(&startPinId, &endPinId))
					{
						auto startPin = FindPin(startPinId);
						auto endPin = FindPin(endPinId);

						newLinkPin = startPin ? startPin : endPin;

						if (startPin->Kind == PinKind::Input)
						{
							std::swap(startPin, endPin);
							std::swap(startPinId, endPinId);
						}

						if (startPin && endPin)
						{
							if (endPin == startPin)
							{
								ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
							}
							else if (endPin->Kind == startPin->Kind)
							{
								showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
								ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
							}
							//else if (endPin->Node == startPin->Node)
							//{
							//    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
							//    ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
							//}
							else if (endPin->Type != startPin->Type)
							{
								showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
								ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
							}
							else
							{
								showLabel("+ Create Link", ImColor(32, 45, 32, 180));
								if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
								{
									m_Links.emplace_back(Link(GetNextId(), startPinId, endPinId));
									m_Links.back().Color = GetIconColor(startPin->Type);
								}
							}
						}
					}

					ed::PinId pinId = 0;
					if (ed::QueryNewNode(&pinId))
					{
						newLinkPin = FindPin(pinId);
						if (newLinkPin)
							showLabel("+ Create Node", ImColor(32, 45, 32, 180));

						if (ed::AcceptNewItem())
						{
							createNewNode = true;
							newNodeLinkPin = FindPin(pinId);
							newLinkPin = nullptr;
							ed::Suspend();
							ImGui::OpenPopup("Create New Node");
							ed::Resume();
						}
					}
				}
				else
					newLinkPin = nullptr;

				ed::EndCreate();

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
			}
			ImGui::SetCursorScreenPos(cursorTopLeft);
		}
		
		if (m_FirstFrame)
			ed::NavigateToContent();
		// End of interaction with editor.
		ed::End();
		 
		ed::SetCurrentEditor(nullptr);

		m_FirstFrame = false;

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

	NodeEditorPanel::Node * NodeEditorPanel::SpawnOutputActionNode()
	{
		m_Nodes.emplace_back(GetNextId(), "OutputAction");
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "Sample", PinType::Float);
		m_Nodes.back().Outputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
		m_Nodes.back().Inputs.emplace_back(GetNextId(), "Event", PinType::Delegate);

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
	}

	NodeEditorPanel::Node* NodeEditorPanel::SpawnEmpty(const std::string& Name)
	{
		m_Nodes.emplace_back(GetNextId(), Name.c_str());
		BuildNode(&m_Nodes.back());
		return &m_Nodes.back();
	};


	EditorCppPaser::EditorCppPaser()
	{
	}

	Ref<NodeEditorPanel> EditorCppPaser::AddNodePanel(Ref<NodeEditorPanel> panel, const std::string& Name)
	{
		m_Panels.push_back(panel);
		m_Names.push_back(Name);
		m_Panels.back()->OnAttach();
		return m_Panels.back();
	}

	Ref<FunctionPaser> EditorCppPaser::AddFunctionPanel(Ref<FunctionPaser> panel, const std::string& Name)
	{
		m_Function.push_back(panel);
		m_NamesFunc.push_back(Name);
		return m_Function.back();
	}

	void EditorCppPaser::OnDetach()
	{
		for (auto& panel : m_Panels)
			panel->OnDetach();
	}

	void EditorCppPaser::LoadFile(const std::string& path)
	{
		CppParser parser;

		m_Compound = parser.parseFile(path);
		if (!m_Compound)
			SS_ASSERT(0, "error parse");

		cppWriter.emit(m_Compound.get(), std::cout); //show to console;
	}

	void EditorCppPaser::OnImGuiRender()
	{
		ImGui::Begin("CppEditor", (bool*)0);
		int i = 0;
			for (auto panel : m_Panels) {
				std::string child_window = "Panel " + std::to_string(i);

				if ( ImGui::CollapsingHeader((m_Names[i] + "###" + std::to_string(i)).c_str()) )
				{
					ImVec2 reg = ImVec2(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y/2) - ImGui::GetWindowSize() / 40;
					ImGui::BeginChild(child_window.c_str(), reg, false);
					panel->OnImGuiRender(i,reg - reg/80);
					ImGui::EndChild();
				}
				i++;
			}
		i = 0;
			for (auto m_Func : m_Function) {
				std::string child_window = "Func " + std::to_string(i); //TEMP + 10
				if (ImGui::CollapsingHeader((m_NamesFunc[i] + "###" + std::to_string(i + 100)).c_str())) {
					ImVec2 reg = ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y) - ImGui::GetWindowSize() / 40;
					ImGui::BeginChild(child_window.c_str(), reg, false);
					m_Func->OnImGuiRender();
					ImGui::EndChild();
				}
			}
		ImGui::End();
	}

	FunctionPaser::FunctionPaser()
	{
	}

	Ref<NodeEditorPanel> FunctionPaser::AddNodePanel(Ref<NodeEditorPanel> panel, const std::string& Name)
	{
		m_Panels.push_back(panel);
		m_Names.push_back(Name);
		m_Panels.back()->OnAttach();
		return m_Panels.back();
	}

	void FunctionPaser::OnDetach()
	{
		for (auto& panel : m_Panels)
			panel->OnDetach();
	}

	void FunctionPaser::OnImGuiRender()
	{
		int i = 0;
		for (auto panel : m_Panels) {
			std::string child_window = "Func " + std::to_string(i);
			// TEMP +1000;
			if (ImGui::CollapsingHeader((m_Names[i] + "###" + std::to_string(i + 100 )).c_str()))
			{
				ImVec2 reg = ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y/2) - ImGui::GetWindowSize() / 40;
				ImGui::BeginChild(child_window.c_str(), reg, false);
				panel->OnImGuiRender(i, reg - reg / 80);
				ImGui::EndChild();
			}
			i++;
		}
	}

}
