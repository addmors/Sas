#pragma once
#include <imgui_node_editor.h>
#include "Sas/NodeEditot/BuilderNode.h"

#include "Sas/Renderer/Texture.h"

#include "../Panel/NodeEditorPanel.h"

#include <cppparser.h>
#include <cppwriter.h>
#include <sstream>

namespace Sas
{
	namespace ed = ax::NodeEditor;

	class Cpp2Node
	{
	public:
		Cpp2Node(const std::string& path);
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

		enum class PinKind
		{
			Output,
			Input
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
	
	private:
		Ref<NodeEditorPanel> AddPanel(const CppObj* varTypeObj);
	private:
		void emit();

		void emit(const CppObj* cppObj);
	    void emitVarList(const CppVarList* varListObj) const;
		void emitEnum(const CppEnum* enmObj)  ;
		void emitTypedef(const CppTypedefName* typedefName) ;
		void emitUsingDecl(const CppUsingDecl* usingDecl) ;
		void emitTypedefList(const CppTypedefList* typedefList) ;
		void emitFwdDecl(const CppFwdClsDecl* fwdClsDeclObj) ;
		void emitMacroCall(const CppMacroCall* macroCallObj) ;
		void emitCompound(const CppCompound* compoundObj) ;
		void emitFunction(const CppFunction* funcObj) ;
		void emitFunctionPtr(const CppFunctionPointer* funcPtrObj) ;
		void emitConstructor(const CppConstructor* ctorObj) ;
		void emitDestructor(const CppDestructor* dtorObj) ;
		void emitTypeConverter(const CppTypeConverter* typeConverterObj) ;
		void emitDocComment(const CppDocComment* docCommentObj) ;

		void emitExpr(const CppExpr* exprObj) ;
		void emitDefine(const CppDefine* defineObj) ;
		void emitUndef(const CppUndef* undefObj) ;
		void emitInclude(const CppInclude* includeObj) ;
		void emitHashIf(const CppHashIf* hashIfObj ) ;
		void emitHashIf(CppHashIf::CondType condType, const std::string& cond ) ;
		void emitEndIf( ) ;
		void emitPragma(const CppPragma* pragmaObj  ) ;
		void emitBlob(const CppBlob* blobObj) ;

		void emitVarType(const CppVarType* varTypeObj);
		void emitParamList(const CppParamVector* paramListObj) ;
		void emitExprAtom(const CppExprAtom& exprObj) ;

		void emitIfBlock(const CppIfBlock* ifBlock) ;
		void emitWhileBlock(const CppWhileBlock* whileBlock) ;
		void emitDoBlock(const CppDoWhileBlock* doBlock) ;
		void emitForBlock(const CppForBlock* forBlock) ;
		void emitSwitchBlock(const CppSwitchBlock* switchBlock) ;

		void emitVar(const CppVar* varObj, bool skipName) ;
		void emitParamList(const CppParamVector* paramListObj, bool skipName) ;
		void emitFunctionPtr(const CppFunctionPointer* funcPtrObj, bool emitNewLine) ;
		void emitFunction(const CppFunction* funcObj, bool skipParamName, bool emitNewLine) ;
		void emitConstructor(const CppConstructor* ctorObj, bool skipParamName) ;
		void emitTemplSpec(const CppTemplateParamList* templSpec) ;

	private:
		CppWriter cppWriter;
		CppCompoundPtr m_Compound;
		CppObj* cppObj;

		ed::EditorContext* m_Context = nullptr;    // Editor context, required to trace a editor state.

		std::map<const CppObj*, Ref<NodeEditorPanel>> m_VarTypePanel;

		static int g_NextID;
		static int g_NextImGuiID;
		
	private:
		static int GetNextID();
		static int GetNexImGuiID();

	private:

		static inline const std::string to_string(CppCompoundType cmpndType)
		{
			switch (cmpndType)
			{
			case CppCompoundType::kNamespace:
				return "namespace";
			case CppCompoundType::kClass:
				return "class";
			case CppCompoundType::kStruct:
				return "struct";
			case CppCompoundType::kUnion:
				return "union";
			default:
				return "compound";
			}
		}

		static inline const std::string emitAttribute(std::uint32_t attr)
		{
			std::string s;
			if (attr & kStatic)
				s = "static ";
			else if (attr & kExtern)
				s = "extern ";
			else if (attr & kExternC)
				s = "extern C ";

			if (attr & kConst)
				s += "const ";
			if (attr & kConstExpr)
				s+= "constexpr ";
			if (attr & kVolatile)
				s+= "volatile ";
			if (attr & kMutable)
				s+= "mutable ";

			return s;
		}

		static inline const std::string emitTypeModifier(const CppTypeModifier& modifier)
		{
			std::stringstream stm;
			std::uint8_t constBit = 0;
			for (constBit = 0; constBit < modifier.ptrLevel_; ++constBit)
			{
				if (modifier.constBits_ & (1 << constBit))
					stm << " const ";
				stm << "*";
			}
			if (modifier.constBits_ & (1 << constBit))
				stm << " const";
			if (modifier.refType_ == CppRefType::kByRef)
				stm << '&';
			else if (modifier.refType_ == CppRefType::kRValRef)
				stm << "&&";

			return stm.str();
		}
	};
}
