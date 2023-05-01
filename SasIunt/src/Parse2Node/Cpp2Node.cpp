#include <ssph.h>
#include "Cpp2Node.h"
#include <iostream>

#include <cppindent.h>
#include <cppcompound-info-accessor.h>
#include <cppfunc-info-accessor.h>
#include <cppobj-info-accessor.h>
#include <cppvar-info-accessor.h>

#include "ListGenerator.h"

#define STEP 50;
#define CASE_OPERATOR(op) case(op): {return #op;};

static const std::string get_str_fromOperator(CppOperator op)
{
    switch (op)
    {
        CASE_OPERATOR(kNone)

            CASE_OPERATOR(kUnariPrefixOperatorStart)
            CASE_OPERATOR(kUnaryMinus)
            CASE_OPERATOR(kBitToggle)    // ~
            CASE_OPERATOR(kLogNot)       // ! (Logical not)
            CASE_OPERATOR(kDerefer)      // *
            CASE_OPERATOR(kRefer)        // & as in &x;
            CASE_OPERATOR(kPreIncrement) // ++i
            CASE_OPERATOR(kPreDecrement) // --i
            CASE_OPERATOR(kUnariSufixOperatorStart)
            CASE_OPERATOR(kPostIncrement) // i++
            CASE_OPERATOR(kPostDecrement) // i--

            CASE_OPERATOR(kBinaryOperatorStart)
            CASE_OPERATOR(kPlus)  // +
            CASE_OPERATOR(kMinus) // -
            CASE_OPERATOR(kMul)   // *
            CASE_OPERATOR(kDiv)   // /
            CASE_OPERATOR(kPercent)
            CASE_OPERATOR(kAnd)
            CASE_OPERATOR(kOr)
            CASE_OPERATOR(kBitAnd) // &
            CASE_OPERATOR(kBitOr)  // |
            CASE_OPERATOR(kXor)
            CASE_OPERATOR(kEqual)   // =
            CASE_OPERATOR(kLess)    // <
            CASE_OPERATOR(kGreater) // >
            CASE_OPERATOR(kPlusEqual)
            CASE_OPERATOR(kMinusEqual)
            CASE_OPERATOR(kMulEqual)
            CASE_OPERATOR(kDivEqual)
            CASE_OPERATOR(kPerEqual)
            CASE_OPERATOR(kXorEqual)
            CASE_OPERATOR(kAndEqual)
            CASE_OPERATOR(kOrEqual)
            CASE_OPERATOR(kLeftShift) // <<
            CASE_OPERATOR(kRightShift) // >>
            CASE_OPERATOR(kLShiftEqual) // <<=
            CASE_OPERATOR(kRShiftEqual) // >>=
            CASE_OPERATOR(kCmpEqual)    // ==
            CASE_OPERATOR(kNotEqual)    // ==
            CASE_OPERATOR(kLessEqual)   // <=
            CASE_OPERATOR(kGreaterEqual)
            CASE_OPERATOR(k3WayCmp)
            CASE_OPERATOR(kComma)
            CASE_OPERATOR(kBinaryOperatorEnd)

            CASE_OPERATOR(kDerefOperatorStart)
            CASE_OPERATOR(kDot)   // .
            CASE_OPERATOR(kArrow) // ->
            CASE_OPERATOR(kArrowStar)

            CASE_OPERATOR(kSpecialOperations)
            CASE_OPERATOR(kFunctionCall)
            CASE_OPERATOR(kUniformInitCall)
            CASE_OPERATOR(kArrayElem) // x[5]
            CASE_OPERATOR(kCStyleCast)
            CASE_OPERATOR(kFunctionStyleCast)
            CASE_OPERATOR(kConstCast)
            CASE_OPERATOR(kStaticCast)
            CASE_OPERATOR(kDynamicCast)
            CASE_OPERATOR(kReinterpretCast)
            CASE_OPERATOR(kPlacementNew)

            CASE_OPERATOR(kTertiaryOperator)
    }
}

namespace Sas 
{
    static ListGenerator g_list;

    void Cpp2Node::emit(const CppObj* cppObj, const Ref<EmptyCompoundImgui>& compound)
    {
        //g_list >> 1;

        switch (cppObj->objType_)
        {
        case CppObjType::kHashDefine:
            return emitDefine((CppDefine*)cppObj, compound);
        case CppObjType::kHashUndef:
            return emitUndef((CppUndef*)cppObj, compound);
        case CppObjType::kHashInclude:
            return emitInclude((CppInclude*)cppObj, compound);
        case CppObjType::kHashIf:
            return emitHashIf((CppHashIf*)cppObj, compound);
        case CppObjType::kHashPragma:
            return emitPragma((CppPragma*)cppObj, compound);
        case CppObjType::kVarType:
            return emitVarType((CppVarType*)cppObj, compound);
        case CppObjType::kVar:
            return emitVar((CppVar*)cppObj, false, compound);
        case CppObjType::kVarList:
            return emitVarList((CppVarList*)cppObj, compound);
        case CppObjType::kEnum:
            return emitEnum((CppEnum*)cppObj, compound);
        case CppObjType::kDocComment:
            return emitDocComment((CppDocComment*)cppObj, compound);
        case CppObjType::kUsingDecl:
            return emitUsingDecl((CppUsingDecl*)cppObj, compound);
        case CppObjType::kTypedefName:
            return emitTypedef((CppTypedefName*)cppObj, compound);
        case CppObjType::kTypedefNameList:
            return emitTypedefList((CppTypedefList*)cppObj, compound);
        case CppObjType::kCompound:
            return emitCompound((CppCompound*)cppObj, compound);
        case CppObjType::kFwdClsDecl:
            return emitFwdDecl((CppFwdClsDecl*)cppObj, compound);
        case CppObjType::kFunction:
            return emitFunction((CppFunction*)cppObj, compound);
        case CppObjType::kConstructor:
            return emitConstructor((CppConstructor*)cppObj, compound);
        case CppObjType::kDestructor:
            return emitDestructor((CppDestructor*)cppObj, compound);  
        case CppObjType::kTypeConverter:
            return emitTypeConverter((CppTypeConverter*)cppObj, compound);
        case CppObjType::kFunctionPtr:
            return emitFunctionPtr((CppFunctionPointer*)cppObj, compound);
        case CppObjType::kIfBlock:
            return emitIfBlock((CppIfBlock*)cppObj, compound);
        case CppObjType::kWhileBlock:
            return emitWhileBlock((CppWhileBlock*)cppObj, compound);
        case CppObjType::kDoWhileBlock:
            return emitDoBlock((CppDoWhileBlock*)cppObj, compound);
        case CppObjType::kForBlock:
            return emitForBlock((CppForBlock*)cppObj, compound);
        case CppObjType::kExpression:
            return emitExpr((CppExpr*)cppObj, compound);
        case CppObjType::kSwitchBlock:
            return emitSwitchBlock(static_cast<const CppSwitchBlock*>(cppObj), compound);
        case CppObjType::kMacroCall:
            return emitMacroCall(static_cast<const CppMacroCall*>(cppObj), compound);
        case CppObjType::kBlob:
            emitBlob((CppBlob*)cppObj, compound);
        default:
            assert(0);
        }
    }

    void Cpp2Node::emitEnum(const CppEnum* enmObj, const Ref<EmptyCompoundImgui>& compound) 
    {
        g_list++;
        Ref<EnumImGui> enum_ref = CreateRef<EnumImGui>(enmObj->name_, enmObj->isClass_ ? "class" : "", enmObj->underlyingType_);
        compound->AddChild(enum_ref);
        enum_ref->SetPos(g_list);
        enum_ref->SetAccess(enmObj->accessType_);

        if (enmObj->itemList_)
        {
            const bool isEnumBodyBlob = !enmObj->itemList_->empty() && enmObj->itemList_->front()->val_
                && (enmObj->itemList_->front()->val_->objType_ == CppBlob::kObjectType);
            if (isEnumBodyBlob)
            {
                enum_ref->SetBlob(static_cast<CppBlob*>(enmObj->itemList_->front()->val_.get())->blob_);
            }
            else
            {
                g_list >> 1;
                for (auto enmItem : *(enmObj->itemList_))
                {

                    if (enmItem->name_.empty())
                    {
                        emit(enmItem->val_.get(), enum_ref);
                    }
                    else
                    {
                        if (enmItem->val_ && isExpr(enmItem->val_.get()))
                        {
                            g_list++;
                            Ref<NameAndEqualImGui> m_NameEql = CreateRef<NameAndEqualImGui>();
                            enum_ref->AddChild(m_NameEql);
                            m_NameEql->SetPos(g_list);

                            g_list >> 1;
                            
                            g_list++;
                            Ref<EmptyCompoundToDrawImGui> name = CreateRef<EmptyCompoundToDrawImGui>(enmItem->name_, enmItem->name_);
                            name->SetPos(g_list);
                            m_NameEql->SetName(name);

                            auto* expr = static_cast<CppExpr*>(enmItem->val_.get());
                            Ref<CompoundWrapperImGui> equal = CreateRef<CompoundWrapperImGui>();
                            m_NameEql->GetEqual() = equal;
                            emitExpr(expr, m_NameEql->GetEqual());
                            
                            g_list << 1;
                        }
                        else
                        {
                            g_list++;
                            Ref<EmptyCompoundToDrawImGui> name = CreateRef<EmptyCompoundToDrawImGui>(enmItem->name_, enmItem->name_);
                            name->SetPos(g_list);
                            enum_ref->AddChild(name);
                        }
                    }
                }
                g_list << 1;
            }
        }
    }

    void Cpp2Node::emitExpr(const CppExpr* exprObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<ExpressionImGui> expression_ref = CreateRef<ExpressionImGui>(exprObj->oper_, exprObj->flags_);
        compound->AddChild(expression_ref);
        expression_ref->SetPos(g_list);
        expression_ref->SetAccess(exprObj->accessType_);

        g_list >> 1;
        if (exprObj->oper_ == kNone)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);
        }
        else if (exprObj->oper_ > kUnariPrefixOperatorStart && exprObj->oper_ < kUnariSufixOperatorStart)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);
        }
        else if (exprObj->oper_ > kUnariSufixOperatorStart && exprObj->oper_ < kBinaryOperatorStart)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);
        }
        else if (exprObj->oper_ > kBinaryOperatorStart && exprObj->oper_ < kDerefOperatorStart)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ > kDerefOperatorStart && exprObj->oper_ < kSpecialOperations)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ == kFunctionCall)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ == kUniformInitCall)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ == kArrayElem)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ == kCStyleCast)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ >= kConstCast && exprObj->oper_ <= kReinterpretCast)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        else if (exprObj->oper_ == kTertiaryOperator)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        
            Ref<CompoundWrapperImGui> expr3 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression3(expr3);
            emitExprAtom(exprObj->expr1_, expr3);
        }
        else if (exprObj->oper_ == kPlacementNew)
        {
            Ref<CompoundWrapperImGui> expr1 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression1(expr1);
            emitExprAtom(exprObj->expr1_, expr1);

            Ref<CompoundWrapperImGui> expr2 = CreateRef<CompoundWrapperImGui>();
            expression_ref->SetExpression2(expr2);
            emitExprAtom(exprObj->expr2_, expr2);
        }
        g_list << 1;
    }

    void Cpp2Node::emitDefine(const CppDefine* defineObj, const Ref<EmptyCompoundImgui>& ref)
    {

        g_list++;

        std::stringstream stm;
        if (!defineObj->defn_.empty())
        {
            const auto firstNonSpaceCharPos =
                std::find_if(defineObj->defn_.begin(), defineObj->defn_.end(), [](char c) { return !std::isspace(c); });
            if (firstNonSpaceCharPos != defineObj->defn_.end())
            {
                if (*firstNonSpaceCharPos != '(')
                    stm << '\t';
                stm << defineObj->defn_;
            }
        }
        Ref<EmptyCompoundImgui> define_ref = ref->AddChild(CreateRef<DefineImgui>(defineObj->name_, stm.str()));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitVarList(const CppVarList* varListObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<VarTypeListImGui> var_list = CreateRef<VarTypeListImGui>("Var list");
        compound->AddChild(var_list);
        var_list->SetPos(g_list);
        var_list->SetAccess(varListObj->accessType_);
        
        g_list >> 1;
        Ref<CompoundWrapperImGui> firstVar = CreateRef<CompoundWrapperImGui>();
        var_list->SetFirstVar(firstVar);
        emitVar(varListObj->firstVar().get(),false, firstVar);
        
        auto& varDeclList = varListObj->varDeclList();
        for (size_t i = 0; i < varDeclList.size(); ++i)
        {
            const auto& decl = varDeclList[i];
            var_list->SetModiferChild(emitTypeModifier(decl));
            emitVarDecl(decl, var_list);
        }

        g_list << 1;

    }

    void Cpp2Node::emitVar(const CppVar* varObj, bool skipName, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<VarImGui> var = CreateRef<VarImGui>("Var");
        compound->AddChild(var);
        var->SetPos(g_list);
        var->SetAccess(varObj->accessType_);

        Ref<CompoundWrapperImGui> var_type = CreateRef<CompoundWrapperImGui>();
        Ref<CompoundWrapperImGui> var_decl = CreateRef<CompoundWrapperImGui>();
        
        var->SetCompoundType(var_type);
        var->SetCompoundDecl(var_decl);
        var->SetApiDecor(varObj->apidecor());

        g_list >> 1;
        emitVarType(varObj->varType(), var_type);
        emitVarDecl(varObj->varDecl(), var_decl);
        g_list << 1;
    
    }

    void Cpp2Node::emitUndef(const CppUndef* undefObj, const Ref<EmptyCompoundImgui>& ref)
    {
        g_list++;
        Ref<EmptyCompoundImgui> define_ref = ref->AddChild(CreateRef<UndefImgui>(undefObj->name_));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitMacroCall(const CppMacroCall* macroCallObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundImgui> define_ref = compound->AddChild(CreateRef<MacroCallImGui>(macroCallObj->macroCall_));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitBlob(const CppBlob* blobObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundImgui> define_ref = compound->AddChild(CreateRef<BlobImGui>(blobObj->blob_));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitDocComment(const CppDocComment* docCommentObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundImgui> comment_ref = compound->AddChild(CreateRef<UndefImgui>(docCommentObj->doc_));
        comment_ref->SetPos(g_list);
    }

    void Cpp2Node::emitUsingDecl(const CppUsingDecl* usingDecl, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        auto usingDec_ref = CreateRef<UsingDeclImGui>(usingDecl->name_);
        compound->AddChild(usingDec_ref);
        usingDec_ref->SetPos(g_list);
        usingDec_ref->SetAccess(usingDecl->accessType_);

        g_list >> 1;
        if (usingDecl->templateParamList()) {
        
            auto templ_spec = CreateRef<CompoundWrapperImGui>();
            usingDec_ref->SetTempltSpec(templ_spec);
            emitTemplSpec(usingDecl->templateParamList(), templ_spec);
        }
        if (usingDecl->cppObj_)
        {
            auto m_Compound = CreateRef<CompoundWrapperImGui>();
            usingDec_ref->SetCompound(m_Compound);
            emit(usingDecl->cppObj_.get(), m_Compound);
        }
        g_list << 1;
    }
    
    void Cpp2Node::emitTypedef(const CppTypedefName* typedefName, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        auto typedef_ref = CreateRef<TypedefImGui>();
        compound->AddChild(typedef_ref);
        typedef_ref->SetPos(g_list);
        typedef_ref->SetAccess(typedefName->accessType_);
        
        g_list >> 1;
        auto var_ref = CreateRef<CompoundWrapperImGui>();
        typedef_ref->SetVar(var_ref);
        emitVar(typedefName->var_.get(), false, var_ref);
        g_list << 1;
    }

    void Cpp2Node::emitTypedefList(const CppTypedefList* typedefList, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<TypedefListImGui> typdeflist_ref = CreateRef<TypedefListImGui>();
        compound->AddChild(typdeflist_ref);
        typdeflist_ref->SetPos(g_list);
        typdeflist_ref->SetAccess(typedefList->accessType_);

        g_list >> 1;
        auto varList_ref = CreateRef<CompoundWrapperImGui>();
        typdeflist_ref->SetVarList(varList_ref);
        emitVarList(typedefList->varList_.get(), typdeflist_ref);
        g_list << 1;
    }

    void Cpp2Node::emitFwdDecl(const CppFwdClsDecl* fwdDeclObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        auto fwddecl_ref = CreateRef<FwdDeclImGui>(fwdDeclObj->name_, fwdDeclObj->attr() & kFriend);
        compound->AddChild(fwddecl_ref);
        fwddecl_ref->SetPos(g_list);
        fwddecl_ref->SetAccess(fwdDeclObj->accessType_);
        g_list >> 1;

        if (fwdDeclObj->templateParamList())
        {
            auto templ_spec = CreateRef<CompoundWrapperImGui>();
            fwddecl_ref->SetTempltSpec(templ_spec);
            emitTemplSpec(fwdDeclObj->templateParamList(), templ_spec);
        }
        if (fwdDeclObj->cmpType_ != CppCompoundType::kUnknownCompound)
            fwddecl_ref->SetCompoundType(to_string(fwdDeclObj->cmpType_));
        if (!fwdDeclObj->apidecor_.empty())
            fwddecl_ref->SetApiDecor(fwdDeclObj->apidecor_);
        g_list << 1;
    }

    void Cpp2Node::emitInclude(const CppInclude* includeObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundImgui> define_ref = compound->AddChild(CreateRef<IncludeImgui>(includeObj->name_));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitHashIf(const CppHashIf* hashIfObj, const Ref<EmptyCompoundImgui>& compound) 
    {
        g_list++;

        Ref<HashIfImgui> defined_ref =  CreateRef<HashIfImgui>("name", hashIfObj->cond_);

        compound->AddChild(defined_ref);

        defined_ref->SetPos(g_list);

        switch (hashIfObj->condType_)
        { 
        case CppHashIf::kIf:
            defined_ref->SetName("#if");
            break;

        case CppHashIf::kIfDef:
            defined_ref->SetName("#ifdef");
            break;

        case CppHashIf::kIfNDef:
            defined_ref->SetName("#ifndef");
            break;

        case CppHashIf::kElIf:
            defined_ref->SetName("#elif");
            break;

        case CppHashIf::kElse:
            defined_ref->SetName("#else");
            break;

        case CppHashIf::kEndIf:
            defined_ref->SetName("#endif");
            break;
        }
        
    }
    
    void Cpp2Node::emitPragma(const CppPragma* pragmaObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundImgui> define_ref = compound->AddChild(CreateRef<PragmaImGui>(pragmaObj->defn_));
        define_ref->SetPos(g_list);
    }

    void Cpp2Node::emitVarType(const CppVarType* varTypeObj, const Ref<EmptyCompoundImgui>& compound) 
    {
        g_list++;

        const auto attr = varTypeObj->typeAttr() | (isConst(varTypeObj) ? CppIdentifierAttrib::kConst : 0);
        std::string attr_str = emitAttribute(attr);
        
 
        const auto& origTypeModifier = varTypeObj->typeModifier();
        const CppTypeModifier typeModifier{ origTypeModifier.refType_, origTypeModifier.ptrLevel_, origTypeModifier.constBits_ & ~1 };
        std::string typeModifier_str = emitTypeModifier(typeModifier) + ((varTypeObj->paramPack_) ? "..." : "");
        
        Ref<VarTypeImGui> var_type = CreateRef<VarTypeImGui>("compound var type" + typeModifier_str + ":", attr_str);
        compound->AddChild(var_type);
        var_type->SetPos(g_list);
        var_type->SetAccess(varTypeObj->accessType_);

        if (varTypeObj->compound()) {
            g_list >> 1;
            emit(varTypeObj->compound(), var_type);
            g_list << 1;
        }
        else
            var_type->SetName(varTypeObj->baseType());

    }

    void Cpp2Node::emitExprAtom(const CppExprAtom& exprObj, const Ref<EmptyCompoundImgui>& compound)
    {
        switch (exprObj.type)
        {
        case CppExprAtom::kAtom:
            emitAtom(*(exprObj.atom), compound);
            break;
        case CppExprAtom::kExpr:
            emitExpr(exprObj.expr, compound);
            break;
        case CppExprAtom::kVarType:
            emitVarType(exprObj.varType, compound);
        default:
            break;
        }
    }

    void Cpp2Node::emitCompound(const CppCompound* compoundObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;

        Ref<NEmptyCompoundImGui> nempty_compound = CreateRef<NEmptyCompoundImGui>(compoundObj->name(), "");
        compound->AddChild(nempty_compound);
        nempty_compound->SetPos(g_list);
        nempty_compound->SetAccess(compoundObj->accessType_);
        
        g_list >> 1;
        bool namespaseLike = nempty_compound->SetNamespaceLike(isNamespaceLike(compoundObj));
        nempty_compound->SetClassLike(isClassLike(compoundObj));

        if (namespaseLike)
        {
            if (compoundObj->templateParamList())
            {
                emitTemplSpec(compoundObj->templateParamList(), nempty_compound);
            }
            nempty_compound->SetCompoundType(to_string(compoundObj->compoundType()));
            if (!compoundObj->apidecor().empty())
                nempty_compound->SetApiDecor(compoundObj->apidecor());
            nempty_compound->SetFinal(compoundObj->hasAttr(kFinal));
        }
        
        if (compoundObj->inheritanceList())
        {
            nempty_compound->SetInheritanceList(compoundObj->inheritanceList());
        }
        nempty_compound->SetExternBlook(compoundObj->compoundType() == CppCompoundType::kExternCBlock);

        CppAccessType lastAccessType = CppAccessType::kUnknown;
        forEachMember(compoundObj, [&](const CppObj* memObj) {
            emit(memObj, nempty_compound);
            return false;
            });
        g_list << 1;
    }

    void Cpp2Node::emitTemplSpec(const CppTemplateParamList* templSpec, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<TemplSpecImgui> temp_compound = CreateRef<TemplSpecImgui>();
        temp_compound->SetPos(g_list);
        compound->AddChild(temp_compound);

        g_list >> 1;
        if (templSpec)
        {
            const char* sep = "";
            for (auto& param : *templSpec)
            {
                if (param->paramType_)
                {
                    if (param->paramType_->objType_ == CppVarType::kObjectType) {
                        emitVarType(static_cast<const CppVarType*>(param->paramType_.get()), temp_compound);
                        temp_compound->AddTempParam(CreateRef<TemplSpecImgui::TempParam>(param->paramName_, param->defaultArg() != nullptr));
                    }
                    else
                    {
                        //emitFunctionPtr(static_cast<const CppFunctionPointer*>(param->paramType_.get()), temp_compound);
                        temp_compound->AddTempParam(CreateRef<TemplSpecImgui::TempParam>(param->paramName_, param->defaultArg() != nullptr));
                    }
                }
                else
                {
                    g_list++;
                    Ref<EmptyCompoundToDrawImGui> empty = CreateRef<EmptyCompoundToDrawImGui>("typename", param->paramName_);
                    empty->SetPos(g_list);
                    temp_compound->AddChild(empty);
                    temp_compound->AddTempParam(CreateRef<TemplSpecImgui::TempParam>(param->paramName_, param->defaultArg() != nullptr));
                }
                if (param->defaultArg())
                    emit(param->defaultArg(),temp_compound);
            }
        }
        g_list << 1;
    }

    void Cpp2Node::emitVarDecl(const CppVarDecl& varDecl, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<VarDeclImGui> varDecl_compound = CreateRef<VarDeclImGui>(varDecl.name());
        varDecl_compound->SetPos(g_list);
        compound->AddChild(varDecl_compound);
        g_list >> 1;

        for (const auto& arrSize : varDecl.arraySizes())
        {
            if (arrSize)
                emitExpr(arrSize.get(), varDecl_compound);
        }
        if (varDecl.assignType() == AssignType::kUsingEqual)
        {
            Ref<CompoundWrapperImGui> wrapp = CreateRef<CompoundWrapperImGui>();
            varDecl_compound->SetDefaultValue("kUsingEqual");
            varDecl_compound->SetCompound(wrapp);
            emit(varDecl.assignValue(), wrapp);
        }
        else if (varDecl.assignType() == AssignType::kUsingBracket)
        {
            if (varDecl.assignValue()) 
            {
                Ref<CompoundWrapperImGui> wrapp = CreateRef<CompoundWrapperImGui>();
                varDecl_compound->SetCompound(wrapp);
                varDecl_compound->SetDefaultValue("kUsingBracket");
                emit(varDecl.assignValue(), wrapp);
            }
        }
        else if (varDecl.assignType() == AssignType::kUsingBraces)
        {
            if (varDecl.assignValue()) 
            {
                Ref<CompoundWrapperImGui> wrapp = CreateRef<CompoundWrapperImGui>();
                varDecl_compound->SetDefaultValue("kUsingBraces");
                varDecl_compound->SetCompound(wrapp);
                emit(varDecl.assignValue(), wrapp);
            }
        }
        g_list << 1;
    }

    void Cpp2Node::emitAtom(const std::string& atom, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<EmptyCompoundToDrawImGui> atomImGui = CreateRef<EmptyCompoundToDrawImGui>("Atom: " + atom, atom);
        compound->AddChild(atomImGui);
        atomImGui->SetPos(g_list);
    }

    void Cpp2Node::emitFunction(const CppFunction* funcObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<FunctionImGui> function_ref = CreateRef<FunctionImGui>
       (
            funcObj->name_,
            funcObj->decor1(),
            funcObj->decor2(), 
            funcObj->attr(),
            funcObj->hasAttr(kTypedef),
            funcObj->objType_ == CppObjType::kFunctionPtr
       );
        
        function_ref->SetPos(g_list);
        function_ref->SetAccess(funcObj->accessType_);
        compound->AddChild(function_ref);
        
        g_list >> 1;

        if (funcObj->templateParamList())
        {
            Ref<CompoundWrapperImGui> tempt_ref = CreateRef<CompoundWrapperImGui>();
            function_ref->SetTempltSpec(tempt_ref);
            emitTemplSpec(funcObj->templateParamList(), tempt_ref);
        }

        Ref<CompoundWrapperImGui> varType_ref = CreateRef<CompoundWrapperImGui>();
        function_ref->SetVarType(varType_ref, funcObj->hasAttr(kTrailingRet));
        emitVarType(funcObj->retType_.get(), varType_ref);

        if (funcObj->params()) {
            Ref<CompoundWrapperImGui> paramList_ref = CreateRef<CompoundWrapperImGui>();
            function_ref->SetParamList(paramList_ref);
            emitParamList(funcObj->params(), paramList_ref);
        }

        
        if (funcObj->defn())
        {
            const auto defn = funcObj->defn();
            if (defn->hasASingleBlobMember())
            {
                function_ref->SetBlob(static_cast<CppBlob*>(defn->members().front().get())->blob_);
            }
            else
            {
                Ref<CompoundWrapperImGui> Compound_ref = CreateRef<CompoundWrapperImGui>();
                function_ref->SetCompound(Compound_ref);
                emitCompound(funcObj->defn(),Compound_ref);
            }
        }
        g_list << 1;
    }
    
    void Cpp2Node::emitFunctionPtr(const CppFunctionPointer* funcPtrObj, const Ref<EmptyCompoundImgui>& compound)
    {
        emitFunction(funcPtrObj, compound);
    }

    void Cpp2Node::emitParamList(const CppParamVector* paramListObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<ParamListImGui> paramList_ref = CreateRef<ParamListImGui>();
        paramList_ref->SetPos(g_list);
        compound->AddChild(paramList_ref);

        g_list >> 1;
        for (auto prmItr = paramListObj->begin(); prmItr != paramListObj->end(); ++prmItr)
        {
            auto& param = *prmItr;
            Ref<CompoundWrapperImGui> wrap_ref = CreateRef<CompoundWrapperImGui>();
            paramList_ref->AddChild(wrap_ref);
            switch (param->objType_)
            {
            case CppObjType::kVar:
                emitVar(static_cast<const CppVar*>(param.get()), false, wrap_ref);
                break;
            case CppObjType::kFunctionPtr:
                emitFunctionPtr(static_cast<const CppFunctionPointer*>(param.get()), wrap_ref);
                break;
            default:
                assert(false);
            }
        }
        g_list << 1;
    }
    
    void Cpp2Node::emitConstructor(const CppConstructor* ctorObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<ConstructorImGui> ctor_ref = CreateRef<ConstructorImGui>(ctorObj->name_, ctorObj->decor1(), ctorObj->attr(), isDeleted(ctorObj));
        compound->AddChild(ctor_ref);
        ctor_ref->SetPos(g_list);
        ctor_ref->SetAccess(ctorObj->accessType_);

        g_list >> 1;
        if (ctorObj->templateParamList())
        {
            auto templ_spec = CreateRef<CompoundWrapperImGui>();
            ctor_ref->SetTempltSpec(templ_spec);
            emitTemplSpec(ctorObj->templateParamList(), templ_spec);
        }

        if (ctorObj->params()) {
            auto param_list = CreateRef<CompoundWrapperImGui>();
            ctor_ref->SetParamList(param_list);
            emitParamList(ctorObj->params(), param_list);
        }
        
        if (ctorObj->memInits_.memInitList)
        {
            if (ctorObj->memInits_.memInitListIsABlob_)
            {
                ctor_ref->SetBlob(ctorObj->memInits_.blob->blob_);
            }
            else
            {
                if (!ctorObj->memInits_.memInitList->empty())
                {
                    g_list++;
                    Ref<InitializerListImGui> init_list = CreateRef<InitializerListImGui>();
                    ctor_ref->SetInitList(init_list);
                    init_list->SetPos(g_list);
                    g_list >> 1;

                    for (const auto& memInit : *(ctorObj->memInits_.memInitList))
                    {

                        g_list++;
                        Ref<NameAndEqualImGui> m_NameEql = CreateRef<NameAndEqualImGui>();
                        init_list->AddChild(m_NameEql);
                        m_NameEql->SetPos(g_list);

                        g_list >> 1;
                        g_list++;
                        Ref<EmptyCompoundToDrawImGui> name = CreateRef<EmptyCompoundToDrawImGui>(memInit.first, memInit.first);
                        name->SetPos(g_list);
                        m_NameEql->SetName(name);

                        Ref<CompoundWrapperImGui> equal = CreateRef<CompoundWrapperImGui>();
                        m_NameEql->GetEqual() = equal;
                        emitExpr(memInit.second, equal);

                        g_list << 1;
                    }
                    g_list << 1;
                }
            }
        }

        if (ctorObj->defn())
        {
            auto compound_ref = CreateRef<CompoundWrapperImGui>();
            ctor_ref->SetCompound(compound_ref);
            emitCompound(ctorObj->defn(), compound_ref);
        }
        g_list << 1;

    }

    void Cpp2Node::emitDestructor(const CppDestructor* dtorObj, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<DestructorImGui> dtor_ref = CreateRef<DestructorImGui>(dtorObj->name_, dtorObj->decor1(), dtorObj->attr());
        compound->AddChild(dtor_ref);
        dtor_ref->SetPos(g_list);
        dtor_ref->SetAccess(dtor_ref->accessType_);
        
        g_list >> 1;
        if (dtorObj->templateParamList())
        {
            auto templ_spec = CreateRef<CompoundWrapperImGui>();
            dtor_ref->SetTempltSpec(templ_spec);
            emitTemplSpec(dtorObj->templateParamList(), templ_spec);

        }
        if (dtorObj->defn())
        {
            auto compound_ref = CreateRef<CompoundWrapperImGui>();
            dtor_ref->SetCompound(compound_ref);
            emitCompound(dtorObj->defn(), compound_ref);
        }
        g_list << 1;
    }
    
    void Cpp2Node::emitIfBlock(const CppIfBlock* ifBlock, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<IfBlockImGui> ifbloc_ref = CreateRef<IfBlockImGui>();
        ifbloc_ref->SetPos(g_list);
        ifbloc_ref->SetAccess(ifBlock->accessType_);
        compound->AddChild(ifbloc_ref);
        g_list >> 1;

        g_list++;
        Ref<IfWrapperImGui> if_wrap = CreateRef<IfWrapperImGui>();
        ifbloc_ref->SetIF(if_wrap);
        ifbloc_ref->SetPos(g_list);
        
        auto wrap = CreateRef<CompoundWrapperImGui>();
        if_wrap->SetExpr(wrap);
        
        g_list >> 1;
        emit(ifBlock->cond_.get(), wrap);

        if (ifBlock->body_)
        {
            auto wrap_bloc = CreateRef<CompoundWrapperImGui>();

            if_wrap->SetBlock(wrap_bloc);
            emit(ifBlock->body_.get(), wrap_bloc);
        }
        g_list << 1;

        
        if (ifBlock->elsePart())
        {
            g_list++;
            Ref<ElseWrapperImGui> else_wrap = CreateRef<ElseWrapperImGui>();
            ifbloc_ref->SetElse(else_wrap);
            else_wrap->SetPos(g_list);

            auto wrap_block = CreateRef<CompoundWrapperImGui>();
            else_wrap->SetBlock(wrap_block);
            
            g_list >> 1;
            emit(ifBlock->elsePart(), wrap_block);
            g_list << 1;
        }
        g_list << 1;
    }

    void Cpp2Node::emitWhileBlock(const CppWhileBlock* whileBlock, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<WhileImGui> while_ref = CreateRef<WhileImGui>();
        while_ref->SetPos(g_list);
        while_ref->SetAccess(whileBlock->accessType_);
        compound->AddChild(while_ref);

        g_list >> 1;

        auto expr_block = CreateRef<CompoundWrapperImGui>();
        while_ref->SetExpr(expr_block);
        emit(whileBlock->cond_.get(), expr_block);
        
        if (whileBlock->body_) {
            auto block = CreateRef<CompoundWrapperImGui>();
            while_ref->SetBlock(block);
            emit(whileBlock->body_.get(), block);
        }

        g_list << 1;
    }

    void Cpp2Node::emitDoBlock(const CppDoWhileBlock* doBlock, const Ref<EmptyCompoundImgui>& compound) 
    {
        g_list++;
        Ref<WhileImGui> while_ref = CreateRef<WhileImGui>();
        while_ref->SetPos(g_list);
        while_ref->SetAccess(doBlock->accessType_);
        compound->AddChild(while_ref);

        g_list >> 1;

        if (doBlock->body_) 
        {
            auto block = CreateRef<CompoundWrapperImGui>();
            while_ref->SetBlock(block);
            emit(doBlock->body_.get(), block);
        }
        
        auto expr_block = CreateRef<CompoundWrapperImGui>();
        while_ref->SetExpr(expr_block);
        emit(doBlock->cond_.get(), expr_block);

        g_list << 1;
    }

    void Cpp2Node::emitForBlock(const CppForBlock* forBlock, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<ForImGui> for_ref = CreateRef<ForImGui>();
        for_ref->SetPos(g_list);
        for_ref->SetAccess(forBlock->accessType_);
        compound->AddChild(for_ref);

        g_list >> 1;

        if (forBlock->start_.get())
        {
            auto block = CreateRef<CompoundWrapperImGui>();
            for_ref->SetExpr1(block);
            emit(forBlock->start_.get(), block);
        }

        if (forBlock->stop_)
        {
            auto block = CreateRef<CompoundWrapperImGui>();
            for_ref->SetExpr2(block);
            emitExpr(forBlock->stop_.get(), block);
        }
        
        if (forBlock->step_)
        {
            auto block = CreateRef<CompoundWrapperImGui>();
            for_ref->SetExpr3(block);
            emitExpr(forBlock->step_.get(), block);
        }
        
        if (forBlock->body_)
        {
            auto block = CreateRef<CompoundWrapperImGui>();
            for_ref->SetBlock(block);
            emit(forBlock->body_.get(), block);
        }
        g_list << 1;
    }

    void Cpp2Node::emitSwitchBlock(const CppSwitchBlock* switchBlock, const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<SwitchImGui> swithc_ref = CreateRef<SwitchImGui>();
        swithc_ref->SetPos(g_list);
        swithc_ref->SetAccess(switchBlock->accessType_);
        compound->AddChild(swithc_ref);
        g_list >> 1;
        auto expr = CreateRef<CompoundWrapperImGui>();
        swithc_ref->SetExpr(expr);
        emitExpr(switchBlock->cond_.get(), expr);
        
        for (const auto& caseStmt : *(switchBlock->body_))
        {
            g_list++;
            auto expr = CreateRef<CaseImGui>();
            expr->SetPos(g_list);
            swithc_ref->AddChild(expr);

            if (caseStmt.case_)
            {
                auto expr_wrap = CreateRef<CompoundWrapperImGui>();
                expr->SetExpr(expr_wrap);
                emitExpr(caseStmt.case_.get(), expr_wrap);
            }
            else
            {
                auto expr_wrap = CreateRef<CompoundWrapperImGui>();
                expr->SetExpr(expr_wrap);
                g_list++;
                auto default_var = CreateRef<EmptyCompoundToDrawImGui>("default:", "default:");
                expr_wrap->AddChild(default_var);
            }

            if (caseStmt.body_) {
                auto compound_wrap = CreateRef<CompoundWrapperImGui>();
                expr->SetCompound(compound_wrap);
                emitCompound(caseStmt.body_.get(), compound_wrap);
            }
        }
        g_list << 1;
    }

    void Cpp2Node::emitTypeConverter(const CppTypeConverter* typeConverterObj,const Ref<EmptyCompoundImgui>& compound)
    {
        g_list++;
        Ref<TypeConventorImGui> typeconv_ref = CreateRef<TypeConventorImGui>( typeConverterObj->attr());
        compound->AddChild(typeconv_ref);
        typeconv_ref->SetPos(g_list);
        typeconv_ref->SetAccess(typeConverterObj->accessType_);

        g_list >> 1;
        if (typeConverterObj->templateParamList()) {
            auto templ_spec = CreateRef<CompoundWrapperImGui>();
            typeconv_ref->SetTempltSpec(templ_spec);
            emitTemplSpec(typeConverterObj->templateParamList(), templ_spec);
        }

        auto var_type = CreateRef<CompoundWrapperImGui>();
        typeconv_ref->SetVarType(var_type);
        emitVarType(typeConverterObj->to_.get(), var_type);

        if (typeConverterObj->defn())
        {
            auto compound_ref = CreateRef<CompoundWrapperImGui>();
            typeconv_ref->SetCompound(compound_ref);
            emitCompound(typeConverterObj->defn(), compound_ref);
        }
        g_list << 1;
    }

    void EmptyCompoundImgui::OnImGuiRender()
    {
        ChandeColorStyle();
        switch (m_ColorStyle)
        {
        case(Green):
        {
            break;
        }
        case(Blue):
        {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15, 0.19, 1.00, 0.54));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.03, 0.24, 0.57, 0.65));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.36, 0.40, 0.95, 0.00));
            break;
        }
        case(Yellow):
        {
            ImGui::PushStyleColor(ImGuiCol_Header,ImVec4(0.84, 0.68, 0, 0.75));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(0.84, 0.68, 0, 0.90));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive,ImVec4(0.92, 0.77, 0, 1.00));
            break;
        }
        case(Red):
        {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.00, 0.15, 0.29, 1.00));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.00, 0.15, 0.29, 0.75));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.00, 0.15, 0.29, 1.00));
            break;
        }
        default:
            break;
        }
        //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(spasing, 10.0));
    }

    void EmptyCompoundImgui::OnImGuiRenderEnd()
    {
        switch (m_ColorStyle)
        {
        case(Green):
        {
            break;
        }
        case(Blue):
        {
            ImGui::PopStyleColor(3);
            break;
        }
        case(Yellow):
        {
            ImGui::PopStyleColor(3);
            break;
        }
        case(Red):
        {
            ImGui::PopStyleColor(3);
            break;
        }
        default:
            break;
        }

        //ImGui::PopStyleVar();
    }

    void DefineImgui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if ( ImGui::TreeNodeEx(("#define ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed) )
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            if(!m_Defined.empty())
                ImGui::Text(m_Defined.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void UndefImgui::OnImGuiRender()
    {
        
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("#undef ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void MacroCallImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("#macro call ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void BlobImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Blob ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void IncludeImgui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("#include ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void HashIfImgui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("#hashif ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            if (!m_Hash.empty())
                ImGui::Text(m_Hash.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void PragmaImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("#pragma ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void VarTypeImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Var Type ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text((attr + name).c_str());
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void VarTypeImGui::OnImGuiRenderChild()
    {
        for (auto& child : m_Child)
            child->OnImGuiRender();
    }

    void NEmptyCompoundImGui::PrintInheritanceList()
    {
        if (inh_beg != inh_end)
        {
            ImGui::Text("Inheritance List: ");
            char sep = ':';
            for (CppInheritanceList::const_iterator inhItr = inh_beg; inhItr != inh_end; ++inhItr)
            {
                ImGui::Text("\t%c %s %s", sep, Cpp2Node::to_string(inhItr->inhType), inhItr->baseName);
                sep = ',';
            }
        }
    }

    void NEmptyCompoundImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Compound: " + name + ((m_Final) ? "final": "") + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            
            if (!m_CompoundType.empty())
                ImGui::Text("%s : %s", "Compound Type: ", m_CompoundType.c_str());
            if (!m_apiDecor.empty())
                ImGui::Text("%s : %s", "Compound ApiDecor: ", m_apiDecor.c_str());
            if (!name.empty())
                ImGui::Text("%s : %s", "Compound Name: ", name.c_str());
            if (m_Extern)

            PrintInheritanceList();

            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void NEmptyCompoundImGui::OnImGuiRenderChild()
    {
        for (auto& child : m_Child)
        {
            if (child->accessType_ != CppAccessType::kUnknown)
                ImGui::Text("%s : %s", "Access Type:", Cpp2Node::emitAccessType(child->accessType_));
            child->OnImGuiRender();
        }
    }

    void TemplSpecImgui::OnImGuiRenderChild()
    {
        for (int i = 0; i < m_TemplChild.size(); ++i)
        {
            m_Child[i]->OnImGuiRender();

            ImGui::Text(m_TemplChild[i]->m_ParamName.c_str());

            if (m_TemplChild[i]->m_defaultArg) {
                i++;
                ImGui::Text("With Default Agrs: ");
                m_Child[i]->OnImGuiRender();
            }
        }
    }

    void TemplSpecImgui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("template###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void EmptyCompoundToDrawImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx((name + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(text.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void VarDeclImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
     
        if (m_DefValue.empty())
        {
            if (ImGui::TreeNodeEx(("Var Decl:" + name+ "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
            {
                PopUpMenuItem();
                ImGui::Text(name.c_str());
                OnImGuiRenderChild();
                ImGui::TreePop();
            }
            else
            {
                PopUpMenuItem();
            }
        }
        else
        {
            if (ImGui::TreeNodeEx(("Var Decl: "+ m_DefValue + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
            {
                PopUpMenuItem();
                ImGui::Text(name.c_str());
                OnImGuiRenderChild();
                ImGui::TreePop();
            }
            else
            {
                PopUpMenuItem();
            }
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();

    }

    void VarDeclImGui::OnImGuiRenderChild()
    {
        for (int i = 0; i < m_Expression.size();) {
            ImGui::Text("[");
            m_Expression[i]->OnImGuiRender();
            ImGui::Text("]");
        }
        if(m_Compound)
            m_Compound->OnImGuiRender();
    }

    void VarTypeListImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Var List###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            m_FirstVar->OnImGuiRender();
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void VarTypeListImGui::OnImGuiRenderChild()
    {
        for (int i = 0; i < m_Child.size(); i++)
        {
            if(!m_chlidTypeModifer[i].empty())
                ImGui::Text("%s : %s", "Modifer Type: ", m_chlidTypeModifer[i].c_str());
            m_Child[i]->OnImGuiRender();
        }
    }

    void VarImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Var###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (!m_ApiDecor.empty())
                ImGui::Text("%s : %s", "ApiDecor: ", m_ApiDecor.c_str());

            if(m_CompoundType) m_CompoundType->OnImGuiRender();
            if(m_CompoundDecl) m_CompoundDecl->OnImGuiRender();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }
    
    void ExpressionImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Expr: " + get_str_fromOperator(operator_)+ "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if(!Specific_arg.empty())
                ImGui::Text("%s : %s", "Specific Expression word: ",Specific_arg.c_str());

            if (expr1_)
                expr1_->OnImGuiRender();
            if (expr2_)
                expr2_->OnImGuiRender();
            if (expr3_)
                expr3_->OnImGuiRender();

            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }
    
    void EnumImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("enum " + name + ' ' + type + ' ' + underlyingType + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (!m_blob.empty())
                ImGui::Text(m_blob.c_str());
            else
                OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void EnumImGui::OnImGuiRenderChild()
    {
        for (auto& child : m_Child)
            child->OnImGuiRender();
    }

    void NameAndEqualImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Named Expr###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void NameAndEqualImGui::OnImGuiRenderChild()
    {
        m_Name->OnImGuiRender();
        ImGui::Text(" = ");
        m_Equal->OnImGuiRender();
    }

    void FunctionImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Function: " + name + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();

            if (!decor1.empty())
                ImGui::Text("%s : %s", "Initial decorative parameters", decor1.c_str());
            
            if(!begin_attribute.empty())
                ImGui::Text("%s : %s", "Begin Attribute", begin_attribute.c_str());

            if (!is_RetTypeOnEnd)
                m_VarType->OnImGuiRender();

            if (!decor2.empty())
                ImGui::Text("%s : %s", "Secondary decorative parameters", decor2.c_str());
            
            if (m_ParamList) m_ParamList->OnImGuiRender();

            if (!end_attribute.empty())
                ImGui::Text("%s : %s", "End Attribute", end_attribute.c_str());

            if(is_RetTypeOnEnd)
                m_VarType->OnImGuiRender();

            if (!m_Blob.empty())
            {
                ImGui::Text("%s : %s", "Blob", m_Blob.c_str());
            }
            else
            {
                if (m_Compound) m_Compound->OnImGuiRender();
            }

            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void ParamListImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Parametr List###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void ParamListImGui::OnImGuiRenderChild()
    {
        for (auto& child : m_Child)
            child->OnImGuiRender();
    }

    void ConstructorImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Constructor: " + name + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();

            if (!decor1.empty())
                ImGui::Text("%s : %s", "Initial decorative parameters", decor1.c_str());

            if (!begin_attribute.empty())
                ImGui::Text("%s : %s", "Begin Attribute", begin_attribute.c_str());
            
            if (m_ParamList) m_ParamList->OnImGuiRender();

            if (!m_Blob.empty())
            {
                ImGui::Text("%s : %s", "Blob", m_Blob.c_str());
            }
            else
            {
                if (m_InitList) m_InitList->OnImGuiRender();
            }

            if (!is_deleted) 
            {
                if (m_Compound) m_Compound->OnImGuiRender();
            }
            else 
                ImGui::Text("Constructor is deleted");

            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void InitializerListImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Initializer List###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void InitializerListImGui::OnImGuiRenderChild()
    {
        for (auto& child : m_Expression)
            child->OnImGuiRender();
    }

    void DestructorImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Destructor: " + name + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();

            if (!decor.empty())
                ImGui::Text("%s : %s", "Initial decorative parameters", decor.c_str());

            if (!begin_attribute.empty())
                ImGui::Text("%s : %s", "Begin Attribute", begin_attribute.c_str());

            if (m_Compound) m_Compound->OnImGuiRender();
            ImGui::TreePop();
        }
    }

    void DocComment::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Comment: ###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            ImGui::Text(m_Comment.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }
    
    void UsingDeclImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("UsingDecl: " + name + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();
            if (m_Compound) m_Compound->OnImGuiRender();

            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void TypedefImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Typedef:###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Var) m_Var->OnImGuiRender();
            ImGui::TreePop();
        }

        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void TypedefListImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Typedef List:###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_VarList) m_VarList->OnImGuiRender();
            ImGui::TreePop();
        }

        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void FwdDeclImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Forwad Declaration: " + name + ((is_friend) ? "friend" : "") + "###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();
            
            if (!m_CompoundType.empty())
                ImGui::Text("%s : %s", "Forwad Type: ", m_CompoundType.c_str());
            if (!m_apiDecor.empty())
                ImGui::Text("%s : %s", "Forwad ApiDecor: ", m_apiDecor.c_str());
            if (!name.empty())
                ImGui::Text("%s : %s", "Forwad Name: ", name.c_str());
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void TypeConventorImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Type Conventor(operator T()):###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_TemplSpec) m_TemplSpec->OnImGuiRender();
            
            if (m_VarType) m_VarType->OnImGuiRender();
            
            if (!begin_attr.empty())
                ImGui::Text("%s : %s", "Attribute: ", begin_attr.c_str());
            if (m_Compound) m_Compound->OnImGuiRender();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void IfBlockImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("If Block###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_If) m_If->OnImGuiRender();
            if (m_Else) m_Else->OnImGuiRender();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void IfWrapperImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("If###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Expr) 
            {
                ImGui::Text("Expression: ");
                m_Expr->OnImGuiRender();
            }
            if (m_Block)
            {

                ImGui::Text("Block: ");
                m_Block->OnImGuiRender();
            }
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void ElseWrapperImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Else###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Block) m_Block->OnImGuiRender();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void WhileImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("While Block###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Expr)
            {
                ImGui::Text("Expression: ");
                m_Expr->OnImGuiRender();
            }
            if (m_Block)
            {

                ImGui::Text("Block: ");
                m_Block->OnImGuiRender();
            }
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void DoWhileImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Do While Block###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Block)
            {

                ImGui::Text("Block: ");
                m_Block->OnImGuiRender();
            }
            if (m_Expr)
            {
                ImGui::Text("Expression: ");
                m_Expr->OnImGuiRender();
            }
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void ForImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("For Block###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Expr1)
            {
                ImGui::Text("Start: ");
                m_Expr1->OnImGuiRender();
            }
            if (m_Expr2)
            {
                ImGui::Text("Stop: ");
                m_Expr2->OnImGuiRender();
            }
            if (m_Expr3)
            {
                ImGui::Text("Step: ");
                m_Expr3->OnImGuiRender();
            }
            if (m_Block)
            {

                ImGui::Text("Block: ");
                m_Block->OnImGuiRender();
            }
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void CaseImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Case###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Expr) m_Expr->OnImGuiRender();
            if (m_Compound) m_Compound->OnImGuiRender();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }

    void SwitchImGui::OnImGuiRenderChild()
    {
        for (auto case_ : m_Cases)
            case_->OnImGuiRender();
    }

    void SwitchImGui::OnImGuiRender()
    {
        EmptyCompoundImgui::OnImGuiRender();
        if (ImGui::TreeNodeEx(("Switch Block###" + m_Pos).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed))
        {
            PopUpMenuItem();
            if (m_Expr) m_Expr->OnImGuiRender();
            ImGui::Text("Cases:");
            OnImGuiRenderChild();
            ImGui::TreePop();
        }
        else
        {
            PopUpMenuItem();
        }
        EmptyCompoundImgui::OnImGuiRenderEnd();
    }
}