#pragma once
#include <imgui_node_editor.h>
#include "Sas/NodeEditot/BuilderNode.h"

#include "Sas/Renderer/Texture.h"

#include "../Panel/NodeEditorPanel.h"

#include "ListGenerator.h"
#include <cppparser.h>
#include <cppwriter.h>
#include <sstream>

namespace Sas
{
	namespace ed = ax::NodeEditor;

	enum ColorStyle
	{
		Green,
		Blue,
		Yellow,
		Red,
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class EmptyCompoundImgui
	{
	public:
		void SetPos(const ListGenerator& generator) { m_Pos = generator.GetLevelInString(); };
		void SetStyle(ColorStyle style) { m_ColorStyle = style; };
		void SetAccess(CppAccessType type) { accessType_ = type; };
		virtual void OnImGuiRender();
		virtual void OnImGuiRenderEnd();
		void PushStyle();
		void PopStyle();


		CppAccessType accessType_ = CppAccessType::kUnknown;
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) { return nullptr; };
	protected:
		void PopUpMenuItem()
		{
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Green")) {
					m_ColorStyleChange = ColorStyle::Green;
					chandedColor = true;
				}
				if (ImGui::MenuItem("Blue")) {
					m_ColorStyleChange = ColorStyle::Blue;
					chandedColor = true;
				}
				if (ImGui::MenuItem("Yellow")) {
					m_ColorStyleChange = ColorStyle::Yellow;
					chandedColor = true;
				}
				if (ImGui::MenuItem("Red")) {
					m_ColorStyleChange = ColorStyle::Red;
					chandedColor = true;
				}
				ImGui::EndPopup();
			}

		}
		void ChandeColorStyle()
		{
			if (chandedColor)
			{
				m_ColorStyle = m_ColorStyleChange;
				chandedColor = false;
			}
		};

		std::string m_Pos;
		ColorStyle m_ColorStyle = ColorStyle::Green;
		ColorStyle m_ColorStyleChange = ColorStyle::Green;
		bool chandedColor = false;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class CompoundWrapperImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() { m_Child->OnImGuiRender(); };
		virtual void OnImGuiRenderEnd() { m_Child->OnImGuiRenderEnd(); };

		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) { m_Child = ref; return m_Child; };
	private:
		Ref<EmptyCompoundImgui> m_Child;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class EmptyCompoundToDrawImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		EmptyCompoundToDrawImGui(const std::string& name, const std::string& text) : name(name), text(text) {};
	private:
		std::string name;
		std::string text;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class DocComment
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		DocComment(const std::string& Comment) : m_Comment(Comment ){};
	private:
		std::string m_Comment;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class UsingDeclImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		UsingDeclImGui(const std::string& name): name(name) {};
	public:
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref) { m_TemplSpec = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref) { m_Compound = ref; };
	private:
		std::string name;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		Ref<CompoundWrapperImGui> m_Compound;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class DefineImgui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		DefineImgui(const std::string& name, const std::string& Defined) : m_Defined(Defined), name(name) {};
	private:
		std::string name;
		std::string m_Defined;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class UndefImgui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		UndefImgui(const std::string& name) : name(name) {};
	private:
		std::string name;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class MacroCallImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		MacroCallImGui(const std::string& name) : name(name) {};
	private:
		std::string name;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class BlobImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		BlobImGui(const std::string& name) : name(name) {};
	private:
		std::string name;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class HashIfImgui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		HashIfImgui(const std::string& name, const std::string& Hash) : m_Hash(Hash), name(name) {};
		void SetName(const std::string& name) { this->name = name; };
	private:
		std::string m_Hash;
		std::string name;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class IncludeImgui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		IncludeImgui(const std::string& name) : name(name) {};
	private:
		std::string name;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class PragmaImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		PragmaImGui(const std::string& name) : name(name) {};
		std::string name;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class VarTypeImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void OnImGuiRenderChild();

		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Child.push_back(ref); return m_Child.back(); };
	public:
		VarTypeImGui(const std::string& name, const std::string& attr_type) :attr(attr_type), name(name) {};
		void SetName(const std::string& name) { this->name = name; };

	private:
		std::string name;
		std::string attr;
		std::vector<Ref<EmptyCompoundImgui>> m_Child;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class ExpressionImGui
		:public EmptyCompoundImgui
	{
	public:
		ExpressionImGui(const CppOperator _operator, const short _flags)
			:operator_(_operator)
			,flags_(_flags)
		{
			std::stringstream stm;

			if (flags_ & CppExpr::kReturn)
				stm << "return ";
			if (flags_ & CppExpr::kThrow)
				stm << "throw ";
			if (flags_ & CppExpr::kInitializer)
				stm << "Initializer ";
			if (flags_ & CppExpr::kBracketed)
				stm << "Bracketed ";
			if (flags_ & CppExpr::kNew)
				stm << "new ";
			if (flags_ & CppExpr::kSizeOf)
				stm << "sizeof( ";
			else if (flags_ & CppExpr::kDelete)
				stm << "delete ";
			else if (flags_ & CppExpr::kDeleteArray)
				stm << "delete[] ";			
			Specific_arg = std::move(stm.str());
		};

	public:
		virtual void OnImGuiRender() override;
		void SetExpression1(Ref<CompoundWrapperImGui> expr1) { expr1_ = expr1; };
		void SetExpression2(Ref<CompoundWrapperImGui> expr2) { expr2_ = expr2; };
		void SetExpression3(Ref<CompoundWrapperImGui> expr3) { expr3_ = expr3; };
	private:
		Ref<CompoundWrapperImGui> expr1_;
		Ref<CompoundWrapperImGui> expr2_;
		Ref<CompoundWrapperImGui> expr3_;
		const CppOperator operator_;
		const short flags_;
		std::string Specific_arg = "";
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class IfWrapperImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetBlock(const Ref<CompoundWrapperImGui>& ref) { m_Block = ref; };
		void SetExpr(const Ref<CompoundWrapperImGui>& ref) { m_Expr = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Expr;
		Ref<CompoundWrapperImGui> m_Block;
	};

	class ElseWrapperImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetBlock(const Ref<CompoundWrapperImGui>& ref) { m_Block = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Block;
	};

	class IfBlockImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetIF(const Ref<IfWrapperImGui>& ref) { m_If = ref; };
		void SetElse(const Ref<ElseWrapperImGui>& ref) { m_Else = ref; };

	private:
		Ref<IfWrapperImGui> m_If;
		Ref<ElseWrapperImGui> m_Else;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class DoWhileImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetBlock(const Ref<CompoundWrapperImGui>& ref) { m_Block = ref; };
		void SetExpr(const Ref<CompoundWrapperImGui>& ref) { m_Expr = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Expr;
		Ref<CompoundWrapperImGui> m_Block;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class WhileImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetBlock(const Ref<CompoundWrapperImGui>& ref) { m_Block = ref; };
		void SetExpr(const Ref<CompoundWrapperImGui>& ref) { m_Expr = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Expr;
		Ref<CompoundWrapperImGui> m_Block;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class ForImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void SetBlock(const Ref<CompoundWrapperImGui>& ref) { m_Block = ref; };
		void SetExpr1(const Ref<CompoundWrapperImGui>& ref) { m_Expr1 = ref; };
		void SetExpr2(const Ref<CompoundWrapperImGui>& ref) { m_Expr2 = ref; };
		void SetExpr3(const Ref<CompoundWrapperImGui>& ref) { m_Expr3 = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Expr1;
		Ref<CompoundWrapperImGui> m_Expr2;
		Ref<CompoundWrapperImGui> m_Expr3;

		Ref<CompoundWrapperImGui> m_Block;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class CaseImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		void SetExpr(const Ref<CompoundWrapperImGui>& ref) { m_Expr = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref) { m_Compound = ref; };

	private:
		Ref<CompoundWrapperImGui> m_Expr;
		Ref<CompoundWrapperImGui> m_Compound;
	};

	class SwitchImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void OnImGuiRenderChild();

		void SetExpr(const Ref<CompoundWrapperImGui>& ref) { m_Expr = ref; };
		void AddChild(const Ref<CaseImGui>& ref) { m_Cases.push_back(ref);};
	private:
		Ref<CompoundWrapperImGui> m_Expr;
		std::vector<Ref<CaseImGui>> m_Cases;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class VarDeclImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void OnImGuiRenderChild();

		VarDeclImGui(const std::string& name) : name(name) {};
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Expression.push_back(ref); return m_Expression.back(); };
		void SetCompound(const Ref<CompoundWrapperImGui> ref) { m_Compound = ref; };
		void SetDefaultValue(const std::string& def) { m_DefValue = def; };
	private:
		std::string name;
		std::vector<Ref<EmptyCompoundImgui>> m_Expression;
		Ref<CompoundWrapperImGui> m_Compound;
		std::string m_DefValue;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class VarImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;

		VarImGui(const std::string& name) : name(name) {};
		void SetApiDecor(const std::string& apidecor) { m_ApiDecor = apidecor; };
		void SetCompoundType(const Ref<CompoundWrapperImGui> ref) { m_CompoundType = ref; };
		void SetCompoundDecl(const Ref<CompoundWrapperImGui> ref) { m_CompoundDecl = ref; };
	private:
		std::string name;
		std::string m_ApiDecor;

		Ref<CompoundWrapperImGui> m_CompoundType;
		Ref<CompoundWrapperImGui> m_CompoundDecl;

	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class VarTypeListImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void OnImGuiRenderChild();
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Child.push_back(ref); return m_Child.back(); };
	public:
		VarTypeListImGui(const std::string& name) :  name(name) {};
		void SetName(const std::string& name) { this->name = name; };
		void SetModiferChild(const std::string& name) { m_chlidTypeModifer.push_back(name); };
		void SetFirstVar(const Ref<CompoundWrapperImGui> first) { m_FirstVar = first; };
	private:
		std::string name;
		std::vector<Ref<EmptyCompoundImgui>> m_Child;
		std::vector<std::string> m_chlidTypeModifer;
		Ref<CompoundWrapperImGui> m_FirstVar;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class TypedefImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		void SetVar(const Ref<CompoundWrapperImGui>& ref) { m_Var = ref; };
	private:
		Ref<CompoundWrapperImGui> m_Var;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class TypedefListImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		void SetVarList(const Ref<CompoundWrapperImGui>& ref) { m_VarList = ref; };
	private:
		Ref<CompoundWrapperImGui> m_VarList;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class FwdDeclImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		FwdDeclImGui(const std::string& name, const bool is_friend) : name(name), is_friend(is_friend) {};
	public:
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref) { m_TemplSpec = ref; };
		void SetApiDecor(const std::string& apidecor) { m_apiDecor = apidecor; };
		void SetCompoundType(const std::string& compoundType) { m_CompoundType = compoundType; };
	private:
		std::string name;
		std::string m_apiDecor;
		std::string m_CompoundType;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		bool is_friend;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class NEmptyCompoundImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		void OnImGuiRenderChild();
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Child.push_back(ref); return m_Child.back();};
	public:
		NEmptyCompoundImGui(const std::string& name, const std::string& namespace_str) :m_NamespaceStr(namespace_str), name(name) {};
		void SetApiDecor(const std::string& apidecor) { m_apiDecor = apidecor; };
		void SetCompoundType(const std::string& compoundType) { m_CompoundType = compoundType; };
		bool SetFinal(bool _final) { m_Final = _final; return m_Final; };
		bool SetNamespaceLike(bool namespaceLike) { m_NamespaceLike = namespaceLike; return m_NamespaceLike; };
		bool SetExternBlook(bool extern_) { m_Extern = extern_; return m_Extern;};
		bool SetClassLike(bool classLike) { m_ClassLike = classLike; return classLike; }
		void SetInheritanceList(const CppInheritanceListPtr& InheritanceList) 
		{
			inh_beg = InheritanceList->begin();
			inh_end = InheritanceList->end();
		};
		
	private:
		void PrintInheritanceList();
	private:
		std::string name;
		std::string m_CompoundType;
		std::string m_NamespaceStr;
		std::string m_apiDecor;
		bool m_Final = false, m_NamespaceLike = false, m_Extern = false, m_ClassLike = false;
		CppInheritanceList::const_iterator inh_beg;
		CppInheritanceList::const_iterator inh_end;
		std::vector<Ref<EmptyCompoundImgui>> m_Child;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class TemplSpecImgui
		:public EmptyCompoundImgui
	{
	public:
		struct TempParam
		{
			std::string m_ParamName;
			bool m_defaultArg;
			TempParam(const std::string& paramName, const bool defaultArg)
				:m_ParamName(paramName)
				, m_defaultArg(defaultArg) {};
		};
	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRenderChild();
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Child.push_back(ref); return m_Child.back(); };
		void AddTempParam(const Ref<TempParam>& param) { m_TemplChild.push_back(param);};
	public:
		TemplSpecImgui(){};
	private:
		std::vector<Ref<TempParam>> m_TemplChild;
		std::vector<Ref<EmptyCompoundImgui>> m_Child;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class NameAndEqualImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRenderChild();
	public:
		void SetName(const Ref<EmptyCompoundToDrawImGui>& ref) { m_Name = ref; };
		Ref<CompoundWrapperImGui>& GetEqual() { return m_Equal;};
	private:
		Ref<EmptyCompoundToDrawImGui> m_Name;
		Ref<CompoundWrapperImGui> m_Equal;
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class EnumImGui
		:public EmptyCompoundImgui
	{
	public:
		EnumImGui(const std::string& name, const std::string& type, const std::string& underlyingType) : name(name), type(type), underlyingType(underlyingType){};
	public:
		void SetBlob(const std::string& blob) { m_blob = blob; };

	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRenderChild();
		virtual Ref<EmptyCompoundImgui> AddChild(const Ref<EmptyCompoundImgui>& ref) override { m_Child.push_back(ref); return m_Child.back(); };
	private:
		std::string name;
		std::string type;
		std::string underlyingType;
		std::string m_blob;
		std::vector<Ref<EmptyCompoundImgui>> m_Child;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class ParamListImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRenderChild();
		
		void AddChild(const Ref<CompoundWrapperImGui>& ref) { m_Child.push_back(ref); };
	private:
		std::vector<Ref<CompoundWrapperImGui>> m_Child;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class FunctionImGui
		:public EmptyCompoundImgui
	{
	public:
		FunctionImGui(const std::string& name, const std::string& decor1, const std::string& decor2, const uint32_t attr, const bool is_typedef, const bool is_ptr)
			:name(name)
			,decor1(decor1)
			,decor2(decor2)
			,attr_(attr)
			,is_Typedef(is_typedef)
			,is_Ptr(is_ptr)
		{
			if (is_Ptr)
				this->name = '(' + name + "*)";

			std::stringstream stm;
			if (hasAttr(kTypedef))
				stm << "typedef";
			if (hasAttr(kStatic))
				stm << "static ";
			else if (hasAttr(kExtern))
				stm << "extern ";
			else if (hasAttr(kVirtual) && !(hasAttr(kOverride) || hasAttr(kFinal)))
				stm << "virtual ";
			else if (hasAttr(kInline))
				stm << "inline ";
			else if (hasAttr(kExplicit))
				stm << "explicit ";
			else if (hasAttr(kFriend))
				stm << "friend ";
			if (hasAttr(kConstExpr))
				stm << "constexpr ";
			if (hasAttr(kTrailingRet))
				stm << "auto";

			begin_attribute = std::move(stm.str());
			stm.clear();

			if (hasAttr(kConst))
				stm << " const";
			if (hasAttr(kPureVirtual))
				stm << " = 0";
			else if (hasAttr(kOverride))
				stm << " override";
			else if (hasAttr(kFinal) )
				stm << " final";

			end_attribute = std::move(stm.str());
		};
	public:
		virtual void OnImGuiRender() override;
	public:
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref)              { m_TemplSpec = ref; };
		void SetVarType(const Ref<CompoundWrapperImGui>& ref, const bool end) { m_VarType = ref; is_RetTypeOnEnd = end; };
		void SetParamList(const Ref<CompoundWrapperImGui>& ref)               { m_ParamList = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref)                { m_Compound = ref; };
		void SetBlob(const std::string& blob)                                 { m_Blob = blob; };
	private:
		bool hasAttr(std::uint32_t _attr) const { return ((attr_ & _attr) == _attr); }
	private:
		std::string name;
		std::string decor1;
		std::string decor2;
		std::string begin_attribute;
		std::string end_attribute;
		std::string m_Blob;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		Ref<CompoundWrapperImGui> m_VarType;
		Ref<CompoundWrapperImGui> m_ParamList;
		Ref<CompoundWrapperImGui> m_Compound;
		uint32_t attr_;
		bool is_Typedef;
		bool is_Ptr;
		bool is_RetTypeOnEnd;

	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class InitializerListImGui
		: public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRenderChild();
		Ref<NameAndEqualImGui> AddChild(const Ref<NameAndEqualImGui>& ref) { m_Expression.push_back(ref); return m_Expression.back(); };
	private:
		std::vector<Ref<NameAndEqualImGui>> m_Expression;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////

	class ConstructorImGui
		:public EmptyCompoundImgui
	{
	public:
		ConstructorImGui(const std::string& name,const std::string& decor1, uint32_t _attr, const bool is_deleted)
			:name(name)
			,decor1(decor1)
			,attr_(_attr)
			,is_deleted(is_deleted)
		{
			std::stringstream stm;

			if (hasAttr(kInline))
				stm << "inline ";
			if (hasAttr(kExplicit))
				stm << "explicit ";

			begin_attribute = std::move(stm.str());
		}
	public:
		virtual void OnImGuiRender() override;
	public:
		void SetParamList(const Ref<CompoundWrapperImGui>& ref) { m_ParamList = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref) { m_Compound = ref; };
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref) { m_TemplSpec = ref; };
		void SetInitList(const Ref<InitializerListImGui>& ref) { m_InitList = ref; };

		void SetBlob(const std::string& blob) { m_Blob = blob; };
	private:
		bool hasAttr(std::uint32_t _attr) const { return ((attr_ & _attr) == _attr); }
	private:
		std::string name;
		std::string decor1;
		std::string begin_attribute;
		std::string m_Blob;
		uint32_t attr_;
		Ref<InitializerListImGui> m_InitList;
		Ref<CompoundWrapperImGui> m_Compound;
		Ref<CompoundWrapperImGui> m_ParamList;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		bool is_deleted;
	};
	
	class DestructorImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		DestructorImGui(const std::string& name, const std::string& decor, uint32_t _attr)
			:name(name)
			,decor(decor)
			,attr_(_attr)
		{
			std::stringstream stm;

			if (hasAttr(kInline))
				stm << "inline ";
			else if (hasAttr(kExplicit))
				stm << "explicit ";
			else if (hasAttr(kVirtual))
				stm << "virtual ";

			begin_attribute = std::move(stm.str());
		}
	public:
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref) { m_TemplSpec = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref) { m_Compound = ref; };
	private:
		bool hasAttr(std::uint32_t _attr) const { return ((attr_ & _attr) == _attr); }
	private:
		std::string decor;
		std::string name;
		std::string begin_attribute;
		uint32_t attr_;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		Ref<CompoundWrapperImGui> m_Compound;
	};
	
	class TypeConventorImGui
		:public EmptyCompoundImgui
	{
	public:
		virtual void OnImGuiRender() override;
	public:
		TypeConventorImGui(const uint32_t _attr)
			:attr_(_attr)
		{
			std::stringstream stm;
			if (hasAttr(kConst))
				stm << " const";
			if (hasAttr(kConstExpr))
				stm << " constexpr";

			begin_attr = std::move(stm.str()); 
		};
	public:
		void SetTempltSpec(const Ref<CompoundWrapperImGui>& ref) { m_TemplSpec = ref; };
		void SetCompound(const Ref<CompoundWrapperImGui>& ref) { m_Compound = ref; };
		void SetVarType(const Ref<CompoundWrapperImGui>& ref) { m_VarType = ref; };
	private:
		bool hasAttr(std::uint32_t _attr) const { return ((attr_ & _attr) == _attr); }
	private:
		std::string begin_attr;
		uint32_t attr_;
		Ref<CompoundWrapperImGui> m_VarType;
		Ref<CompoundWrapperImGui> m_TemplSpec;
		Ref<CompoundWrapperImGui> m_Compound;
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class Cpp2Node
	{
	public:
		void LoadFile(const std::string& path)
		{
			if (path.empty()) return;
			m_NEmptyCompoundImgui = CreateRef<CompoundWrapperImGui>();
			CppParser parser;

			m_Compound = parser.parseFile(path);
			if (!m_Compound)
				SS_ASSERT(0, "error parse");

			emit(m_Compound.get(), m_NEmptyCompoundImgui); //show to console;
			loaded = true;
		}
	public:
		void OnImGuiRender() { 
			if (loaded)
			{
				ImGui::Begin("CppToNode");
				m_NEmptyCompoundImgui->OnImGuiRender();
				ImGui::End();
			};
		};
	public:
		void emit(const CppObj* cppObj, const Ref<EmptyCompoundImgui>& compound);
	private:
	    void emitVarList(const CppVarList* varListObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitEnum(const CppEnum* enmObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitTypedef(const CppTypedefName* typedefName,const Ref<EmptyCompoundImgui>& compound) ;
		void emitUsingDecl(const CppUsingDecl* usingDecl,const Ref<EmptyCompoundImgui>& compound) ;
		void emitTypedefList(const CppTypedefList* typedefList,const Ref<EmptyCompoundImgui>& compound) ;
		void emitFwdDecl(const CppFwdClsDecl* fwdClsDeclObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitMacroCall(const CppMacroCall* macroCallObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitCompound(const CppCompound* compoundObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitFunction(const CppFunction* funcObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitFunctionPtr(const CppFunctionPointer* funcPtrObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitConstructor(const CppConstructor* ctorObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitDestructor(const CppDestructor* dtorObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitTypeConverter(const CppTypeConverter* typeConverterObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitDocComment(const CppDocComment* docCommentObj,const Ref<EmptyCompoundImgui>& compound) ;

		void emitExpr(const CppExpr* exprObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitDefine(const CppDefine* defineObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitUndef(const CppUndef* undefObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitInclude(const CppInclude* includeObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitHashIf(const CppHashIf* hashIfObj ,const Ref<EmptyCompoundImgui>& compound) ;
		void emitPragma(const CppPragma* pragmaObj ,const Ref<EmptyCompoundImgui>& compound) ;
		void emitBlob(const CppBlob* blobObj,const Ref<EmptyCompoundImgui>& compound) ;

		void emitVarType(const CppVarType* varTypeObj,const Ref<EmptyCompoundImgui>& compound);
		void emitParamList(const CppParamVector* paramListObj,const Ref<EmptyCompoundImgui>& compound) ;
		void emitExprAtom(const CppExprAtom& exprObj,const Ref<EmptyCompoundImgui>& compound) ;
		
		void emitIfBlock(const CppIfBlock* ifBlock,const Ref<EmptyCompoundImgui>& compound) ;
		void emitWhileBlock(const CppWhileBlock* whileBlock,const Ref<EmptyCompoundImgui>& compound) ;
		void emitDoBlock(const CppDoWhileBlock* doBlock,const Ref<EmptyCompoundImgui>& compound) ;
		void emitForBlock(const CppForBlock* forBlock,const Ref<EmptyCompoundImgui>& compound) ;
		void emitSwitchBlock(const CppSwitchBlock* switchBlock,const Ref<EmptyCompoundImgui>& compound) ;

		void emitVar(const CppVar* varObj, bool skipName,const Ref<EmptyCompoundImgui>& compound) ;
		void emitParamList(const CppParamVector* paramListObj, bool skipName,const Ref<EmptyCompoundImgui>& compound) ;
		void emitFunction(const CppFunction* funcObj, bool skipParamName, bool emitNewLine,const Ref<EmptyCompoundImgui>& compound) ;
		void emitTemplSpec(const CppTemplateParamList* templSpec, const Ref<EmptyCompoundImgui>& compound) ;
		void emitVarDecl(const CppVarDecl& varDecl, const Ref<EmptyCompoundImgui>& compound);
		
		void emitAtom(const std::string& atom, const Ref<EmptyCompoundImgui>& compound);

	private:
		Ref<CompoundWrapperImGui> m_NEmptyCompoundImgui;
		CppWriter cppWriter;
		CppCompoundPtr m_Compound;
		CppObj* cppObj;

		ed::EditorContext* m_Context = nullptr;    // Editor context, required to trace a editor state.

		std::map<const CppObj*, Ref<NodeEditorPanel>> m_VarTypePanel;
		bool loaded = false; 

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

	public:
		static inline const std::string to_string(CppAccessType accessType)
		{
			std::stringstream stm;
			switch (accessType)
			{
			case CppAccessType::kPublic:
				stm << "public";
				break;
			case CppAccessType::kProtected:
				stm << "protected";
				break;
			case CppAccessType::kPrivate:
				stm << "private";
				break;

			default:
				break;
			}
			return stm.str();
		}
		static inline const std::string emitOperator(const CppOperator op)
		{
			std::stringstream stm;
			switch (op)
			{
			case kUnaryMinus:
				stm << '-';
				break;
			case kBitToggle:
				stm << '~';
				break;
			case kLogNot:
				stm << '!';
				break;
			case kDerefer:
				stm << '*';
				break;
			case kRefer:
				stm << '&';
				break;
			case kPreIncrement:
			case kPostIncrement:
				stm << "++";
				break;
			case kPreDecrement:
			case kPostDecrement:
				stm << "--";
				break;
			case kPlus:
				stm << '+';
				break;
			case kMinus:
				stm << '-';
				break;
			case kMul:
				stm << '*';
				break;
			case kDiv:
				stm << '/';
				break;
			case kPercent:
				stm << '%';
				break;
			case kAnd:
				stm << "&&";
				break;
			case kOr:
				stm << "||";
				break;
			case kBitAnd:
				stm << '&';
				break;
			case kBitOr:
				stm << '|';
				break;
			case kXor:
				stm << '^';
				break;
			case kEqual:
				stm << '=';
				break;
			case kLess:
				stm << '<';
				break;
			case kGreater:
				stm << '>';
				break;
			case kPlusEqual:
				stm << "+=";
				break;
			case kMinusEqual:
				stm << "-=";
				break;
			case kMulEqual:
				stm << "*=";
				break;
			case kDivEqual:
				stm << "/=";
				break;
			case kPerEqual:
				stm << "%=";
				break;
			case kXorEqual:
				stm << "^=";
				break;
			case kAndEqual:
				stm << "&=";
				break;
			case kOrEqual:
				stm << "|=";
				break;
			case kLeftShift:
				stm << "<<";
				break;
			case kRightShift:
				stm << ">>";
				break;
			case kLShiftEqual:
				stm << "<<=";
				break;
			case kRShiftEqual:
				stm << ">>=";
				break;
			case kCmpEqual:
				stm << "==";
				break;
			case kNotEqual:
				stm << "!=";
				break;
			case kLessEqual:
				stm << "<=";
				break;
			case kGreaterEqual:
				stm << ">=";
				break;
			case k3WayCmp:
				stm << "<=>";
				break;
			case kComma:
				stm << ',';
				break;
			case kDot:
				stm << '.';
				break;
			case kArrow:
				stm << "->";
				break;
			case kArrowStar:
				stm << "->*";
				break;
			default:
				break;
			}
			return stm.str();
		};
		static inline const char* emitAccessType(const CppAccessType type)
		{
			switch (type)
			{
			case CppAccessType::kPublic:
				return "public";
			case CppAccessType::kProtected:
				return "protected";
			case CppAccessType::kPrivate:
				return "private";
			default:
				return "";
			}
		};

	};
}
