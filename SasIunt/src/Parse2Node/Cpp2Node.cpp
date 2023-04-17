#include <ssph.h>
#include "Cpp2Node.h"
#include <iostream>

#include <cppindent.h>
#include <cppcompound-info-accessor.h>
#include <cppfunc-info-accessor.h>
#include <cppobj-info-accessor.h>
#include <cppvar-info-accessor.h>

#define STEP 50;

namespace Sas {
    

	int Cpp2Node::g_NextID = 1;
	int Cpp2Node::g_NextImGuiID = 1;

	Cpp2Node::Cpp2Node(const std::string& path)
	{
		CppParser parser;

		m_Compound = parser.parseFile(path);
		if (!m_Compound)
			SS_ASSERT(0, "error parse");

		cppWriter.emit(m_Compound.get(), std::cout); //show to console;
		emit();
	}

    Ref<NodeEditorPanel> Cpp2Node::AddPanel(const CppObj* varTypeObj)
    {
        if (!m_VarTypePanel.contains(varTypeObj))
            m_VarTypePanel[varTypeObj] = CreateRef<NodeEditorPanel>();

        return m_VarTypePanel[varTypeObj];
    }

    void Cpp2Node::emit(const CppObj* cppObj)
	{
       switch (cppObj->objType_)
        {
        case CppObjType::kHashDefine:
            return emitDefine((CppDefine*)cppObj);
        case CppObjType::kHashUndef:
            return emitUndef((CppUndef*)cppObj);
        case CppObjType::kHashInclude:
            return emitInclude((CppInclude*)cppObj);
        case CppObjType::kHashIf:
            return emitHashIf((CppHashIf*)cppObj);
        case CppObjType::kHashPragma:
            return emitPragma((CppPragma*)cppObj);

        case CppObjType::kVarType:
            return emitVarType((CppVarType*)cppObj);
        case CppObjType::kVar:
            return emitVar((CppVar*)cppObj, true);
        case CppObjType::kVarList:
            return emitVarList((CppVarList*)cppObj);
        case CppObjType::kEnum:
            return emitEnum((CppEnum*)cppObj);
        case CppObjType::kDocComment:
            return emitDocComment((CppDocComment*)cppObj);
        case CppObjType::kUsingDecl:
            return emitUsingDecl((CppUsingDecl*)cppObj);
        case CppObjType::kTypedefName:
            return emitTypedef((CppTypedefName*)cppObj);
        case CppObjType::kTypedefNameList:
            return emitTypedefList((CppTypedefList*)cppObj);
        case CppObjType::kCompound:
            return emitCompound((CppCompound*)cppObj);
        case CppObjType::kFwdClsDecl:
            return emitFwdDecl((CppFwdClsDecl*)cppObj);
        case CppObjType::kFunction:
            return emitFunction((CppFunction*)cppObj);
        case CppObjType::kConstructor:
            return emitConstructor((CppConstructor*)cppObj);
        case CppObjType::kDestructor:
            return emitDestructor((CppDestructor*)cppObj);
        case CppObjType::kTypeConverter:
            return emitTypeConverter((CppTypeConverter*)cppObj);
        case CppObjType::kFunctionPtr:
            return emitFunctionPtr((CppFunctionPointer*)cppObj);
        case CppObjType::kIfBlock:
            return emitIfBlock((CppIfBlock*)cppObj);
        case CppObjType::kWhileBlock:
            return emitWhileBlock((CppWhileBlock*)cppObj);
        case CppObjType::kDoWhileBlock:
            return emitDoBlock((CppDoWhileBlock*)cppObj);
        case CppObjType::kForBlock:
            return emitForBlock((CppForBlock*)cppObj);
        case CppObjType::kExpression:
            return emitExpr((CppExpr*)cppObj);
        case CppObjType::kSwitchBlock:
            return emitSwitchBlock(static_cast<const CppSwitchBlock*>(cppObj));
        case CppObjType::kMacroCall:
            return emitMacroCall(static_cast<const CppMacroCall*>(cppObj));

        case CppObjType::kBlob:
            return emitBlob((CppBlob*)cppObj);

        default:
            break;
       }

	}

    void Cpp2Node::emit()
    {
        emit(m_Compound.get());
    }

    void Cpp2Node::emitCompound(const CppCompound* compoundObj)
    {
        if (isNamespaceLike(compoundObj))
            if (compoundObj->templateParamList()) 
            {
                std::string header = to_string(compoundObj->compoundType())
                    + compoundObj->apidecor()
                    + ' ' + compoundObj->name()
                    + (compoundObj->hasAttr(kFinal) ? "final" : "")
                    + " ###" + std::to_string(GetNexImGuiID());

                if (ImGui::CollapsingHeader(header.c_str()))
                    emitTemplSpec(compoundObj->templateParamList());
            }
    }

	int Cpp2Node::GetNextID()
	{
		return g_NextID++;
	}

    int Cpp2Node::GetNexImGuiID()
    {
        return g_NextImGuiID++;
    }

    void Cpp2Node::emitFunctionPtr(const CppFunctionPointer* funcPtrObj, bool emitNewLine) 
    {

        auto panel = AddPanel(funcPtrObj);
    }
    
    void Cpp2Node::emitTemplSpec(const CppTemplateParamList* templSpec)
    {
        std::string templ_text = "template ###" + std::to_string(GetNexImGuiID());

        if (templSpec) {
            if (ImGui::CollapsingHeader(templ_text.c_str()))
            {
                const char* sep = "";
                for (auto& param : *templSpec)
                {
                    templ_text += sep;
                    if (param->paramType_)
                    {
                        if (param->paramType_->objType_ == CppVarType::kObjectType)
                            emitVarType(static_cast<const CppVarType*>(param->paramType_.get()));
                        else
                            emitFunctionPtr(static_cast<const CppFunctionPointer*>(param->paramType_.get()), false);
                    }
                    /*else
                    {
                        stm << "typename ";
                    }
                    stm << param->paramName_;
                    if (param->defaultArg())
                    {
                        stm << " = ";
                        emit(param->defaultArg(), stm, CppIndent(), true);
                    }
                    sep = ", ";*/
                }
            }
        }
    }

    void Cpp2Node::emitVarType(const CppVarType* varTypeObj)
    {
        auto panel = AddPanel(varTypeObj);

        const auto attr = varTypeObj->typeAttr() | (isConst(varTypeObj) ? CppIdentifierAttrib::kConst : 0);
        
        std::string s = emitAttribute(attr);

        if (varTypeObj->compound())
            emit(varTypeObj->compound());
        //else
            //stm << varTypeObj->baseType();
        const auto& origTypeModifier = varTypeObj->typeModifier();
        const CppTypeModifier typeModifier{
          origTypeModifier.refType_, origTypeModifier.ptrLevel_, origTypeModifier.constBits_ & ~1 };
        //emitTypeModifier(typeModifier, stm);
        //if (varTypeObj->paramPack_)
            //stm << "...";
    }

}