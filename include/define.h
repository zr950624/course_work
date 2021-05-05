#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

#define ALLTYPE(V) \
	V(kProgram) \
	V(kReservedNonModifiers) \
	V(kSemiReserved) \
	V(kIdentifierPlus) \
	V(kTopStatementList) \
	V(kNamespaceName) \
	V(kName) \
	V(kTopStatement) \
	V(kUseType) \
	V(kGroupUseDeclaration) \
	V(kMixedGroupUseDeclaration) \
	V(kPossibleComma) \
	V(kInlineUseDeclarations) \
	V(kUnprefixedUseDeclarations) \
	V(kUseDeclarations) \
	V(kInlineUseDeclaration) \
	V(kUnprefixedUseDeclaration) \
	V(kUseDeclaration) \
	V(kConstList) \
	V(kInnerStatementList) \
	V(kInnerStatement) \
	V(kStatement) \
	V(kCatchList) \
	V(kCatchNameList) \
	V(kFinallyStatement) \
	V(kUnsetVariables) \
	V(kUnsetVariable) \
	V(kFunctionDeclarationStatement) \
	V(kFunctionDeclaration) \
	V(kIsReference) \
	V(kIsVariadic) \
	V(kClassDeclarationStatement) \
	V(kClassDeclaration) \
	V(kClassBody) \
	V(kClassModifiers) \
	V(kClassModifier) \
	V(kTraitDeclarationStatement) \
	V(kInterfaceDeclarationStatement) \
	V(kExtendsFrom) \
	V(kInterfaceExtendsList) \
	V(kImplementsList) \
	V(kForeachVariable) \
	V(kForStatement) \
	V(kForeachStatement) \
	V(kDeclareStatement) \
	V(kSwitchCaseList) \
	V(kCaseList) \
	V(kCaseSeparator) \
	V(kWhileStatement) \
	V(kIfStmtWithoutElse) \
	V(kIfStmt) \
	V(kAltIfStmtWithoutElse) \
	V(kAltIfStmt) \
	V(kParameterList) \
	V(kNonEmptyParameterList) \
	V(kParameter) \
	V(kOptionalType) \
	V(kTypeExpr) \
	V(kType) \
	V(kUnionType2) \
	V(kReturnType) \
	V(kArgumentList) \
	V(kNonEmptyArgumentList) \
	V(kArgument) \
	V(kGlobalVarList) \
	V(kGlobalVar) \
	V(kStaticVarList) \
	V(kStaticVar) \
	V(kClassStatementList) \
	V(kClassStatement) \
	V(kClassMethodDefinition) \
	V(kSimpleVarMember) \
	V(kClassNameList) \
	V(kTraitAdaptations) \
	V(kTraitAdaptationList) \
	V(kTraitAdaptation) \
	V(kTraitPrecedence) \
	V(kTraitAlias) \
	V(kTraitMethodReference) \
	V(kAbsoluteTraitMethodReference) \
	V(kMethodBody) \
	V(kVariableModifiers) \
	V(kMethodModifiers) \
	V(kNonEmptyMemberModifiers) \
	V(kMemberModifier) \
	V(kPropertyList) \
	V(kProperty) \
	V(kClassConstList) \
	V(kClassConstDecl) \
	V(kConstDecl) \
	V(kEchoExprList) \
	V(kEchoExpr) \
	V(kForExprs) \
	V(kNonEmptyForExprs) \
	V(kAnonymousClass) \
	V(kNewExpr) \
	V(kClassConstructor) \
	V(kMightDefineExpr) \
	V(kExpr) \
	V(kInlineFunction) \
	V(kFn) \
	V(kFunction) \
	V(kBackupDocComment) \
	V(kBackupFnFlags) \
	V(kBackupLexPos) \
	V(kReturnsRef) \
	V(kLexicalVars) \
	V(kLexicalVarList) \
	V(kLexicalVar) \
	V(kFunctionCallme) \
	V(kClassName) \
	V(kClassNameReference) \
	V(kExitExpr) \
	V(kBackticksExpr) \
	V(kCtorArguments) \
	V(kDereferencableScalar) \
	V(kScalar) \
	V(kConstant) \
	V(kOptionalExpr) \
	V(kVariableClassName) \
	V(kDereferencable) \
	V(kCallableExpr) \
	V(kCallableVariable) \
	V(kVariable) \
	V(kSimpleVariable) \
	V(kStaticMember) \
	V(kNewVariable) \
	V(kMemberName) \
	V(kPropertyName) \
	V(kArrayPairList) \
	V(kPossibleArrayPair) \
	V(kNonEmptyArrayPairList) \
	V(kArrayPair) \
	V(kEncapsList) \
	V(kStringVarname) \
	V(kEncapsVar) \
	V(kEncapsVarOffset) \
	V(kInternalFunctionsInYacc) \
	V(kIssetVariables) \
	V(kIssetVariable) \
	V(kIdentifier) \
	V(kDollarVariable) \
	V(kNumLiteral) \
	V(kFloatLiteral) \
	V(kIntLiteral) \
	V(kStringLiteral) \
	V(kUnknown) \


#define ALLCLASS(V) \
	V(Program) \
	V(ReservedNonModifiers) \
	V(SemiReserved) \
	V(IdentifierPlus) \
	V(TopStatementList) \
	V(NamespaceName) \
	V(Name) \
	V(TopStatement) \
	V(UseType) \
	V(GroupUseDeclaration) \
	V(MixedGroupUseDeclaration) \
	V(PossibleComma) \
	V(InlineUseDeclarations) \
	V(UnprefixedUseDeclarations) \
	V(UseDeclarations) \
	V(InlineUseDeclaration) \
	V(UnprefixedUseDeclaration) \
	V(UseDeclaration) \
	V(ConstList) \
	V(InnerStatementList) \
	V(InnerStatement) \
	V(Statement) \
	V(CatchList) \
	V(CatchNameList) \
	V(FinallyStatement) \
	V(UnsetVariables) \
	V(UnsetVariable) \
	V(FunctionDeclarationStatement) \
	V(FunctionDeclaration) \
	V(IsReference) \
	V(IsVariadic) \
	V(ClassDeclarationStatement) \
	V(ClassDeclaration) \
	V(ClassBody) \
	V(ClassModifiers) \
	V(ClassModifier) \
	V(TraitDeclarationStatement) \
	V(InterfaceDeclarationStatement) \
	V(ExtendsFrom) \
	V(InterfaceExtendsList) \
	V(ImplementsList) \
	V(ForeachVariable) \
	V(ForStatement) \
	V(ForeachStatement) \
	V(DeclareStatement) \
	V(SwitchCaseList) \
	V(CaseList) \
	V(CaseSeparator) \
	V(WhileStatement) \
	V(IfStmtWithoutElse) \
	V(IfStmt) \
	V(AltIfStmtWithoutElse) \
	V(AltIfStmt) \
	V(ParameterList) \
	V(NonEmptyParameterList) \
	V(Parameter) \
	V(OptionalType) \
	V(TypeExpr) \
	V(Type) \
	V(UnionType2) \
	V(ReturnType) \
	V(ArgumentList) \
	V(NonEmptyArgumentList) \
	V(Argument) \
	V(GlobalVarList) \
	V(GlobalVar) \
	V(StaticVarList) \
	V(StaticVar) \
	V(ClassStatementList) \
	V(ClassStatement) \
	V(ClassMethodDefinition) \
	V(SimpleVarMember) \
	V(ClassNameList) \
	V(TraitAdaptations) \
	V(TraitAdaptationList) \
	V(TraitAdaptation) \
	V(TraitPrecedence) \
	V(TraitAlias) \
	V(TraitMethodReference) \
	V(AbsoluteTraitMethodReference) \
	V(MethodBody) \
	V(VariableModifiers) \
	V(MethodModifiers) \
	V(NonEmptyMemberModifiers) \
	V(MemberModifier) \
	V(PropertyList) \
	V(Property) \
	V(ClassConstList) \
	V(ClassConstDecl) \
	V(ConstDecl) \
	V(EchoExprList) \
	V(EchoExpr) \
	V(ForExprs) \
	V(NonEmptyForExprs) \
	V(AnonymousClass) \
	V(NewExpr) \
	V(ClassConstructor) \
	V(MightDefineExpr) \
	V(Expr) \
	V(InlineFunction) \
	V(Fn) \
	V(Function) \
	V(BackupDocComment) \
	V(BackupFnFlags) \
	V(BackupLexPos) \
	V(ReturnsRef) \
	V(LexicalVars) \
	V(LexicalVarList) \
	V(LexicalVar) \
	V(FunctionCallme) \
	V(ClassName) \
	V(ClassNameReference) \
	V(ExitExpr) \
	V(BackticksExpr) \
	V(CtorArguments) \
	V(DereferencableScalar) \
	V(Scalar) \
	V(Constant) \
	V(OptionalExpr) \
	V(VariableClassName) \
	V(Dereferencable) \
	V(CallableExpr) \
	V(CallableVariable) \
	V(Variable) \
	V(SimpleVariable) \
	V(StaticMember) \
	V(NewVariable) \
	V(MemberName) \
	V(PropertyName) \
	V(ArrayPairList) \
	V(PossibleArrayPair) \
	V(NonEmptyArrayPairList) \
	V(ArrayPair) \
	V(EncapsList) \
	V(StringVarname) \
	V(EncapsVar) \
	V(EncapsVarOffset) \
	V(InternalFunctionsInYacc) \
	V(IssetVariables) \
	V(IssetVariable) \
	V(Identifier) \
	V(DollarVariable) \
	V(NumLiteral) \
	V(FloatLiteral) \
	V(IntLiteral) \
	V(StringLiteral) \


#define ALLDATATYPE(V) \
	V(DataWhatever) \
	V(DataVarType) \
	V(DataClassType) \
	V(DataVarScope) \
	V(DataFunctionType) \
	V(DataFixUnit) \
	V(DataInitiator) \
	V(DataFunctionName) \
	V(DataFunctionBody) \
	V(DataVarDefine) \
	V(DataFunctionArg) \
	V(DataClassName) \
	V(DataVarName) \
	V(DataStructBody) \



#define GLOBAL_SCOPE 0x1000
#define FUNCTION_SCOPE 0x1001
#define STATEMENT_SCOPE 0x1002
#define CLASS_BODY_SCOPE 0x1003

//#define SYNTAX_ONLY

#define INIT_FILE_DIR "/home/yongheng/php_testcase/"

#define SWITCHSTART \
    switch(case_idx_){ 

#define SWITCHEND \
    default: \
        \
        assert(0); \
        \
    }

#define CASESTART(idx) \
    case CASE##idx: {\


#define CASEEND \
            break;\
        }

#define TRANSLATESTART \
    IR *res = NULL; 

#define GENERATESTART(len) \
    case_idx_ = rand() % len ;

#define GENERATEEND \
    return ;

#define TRANSLATEEND \
     v_ir_collector.push_back(res); \
     res->data_type_ = data_type_; \
     res->data_flag_ = data_flag_; \
        \
     return res; 

#define TRANSLATEENDNOPUSH \
     res->data_type_ = data_type_; \
     res->data_flag_ = data_flag_; \
       \
     return res; 

#define SAFETRANSLATE(a) \
    (assert(a != NULL), a->translate(v_ir_collector))

#define SAFEDELETE(a) \
    if(a != NULL) a->deep_delete()

#define SAFEDELETELIST(a) \
    for(auto _i: a) \
        SAFEDELETE(_i)

#define OP1(a) \
    new IROperator(a) 

#define OP2(a, b) \
    new IROperator(a,b)

#define OP3(a,b,c) \
    new IROperator(a,b,c)

#define OPSTART(a) \
    new IROperator(a)

#define OPMID(a) \
new IROperator("", a, "")

#define OPEND(a) \
    new IROperator("", "", a)

#define OP0() \
    new IROperator()


#define TRANSLATELIST(t, a, b) \
    res = SAFETRANSLATE(a[0]); \
    res = new IR(t, OP0(), res) ; \
    v_ir_collector.push_back(res); \
    for(int i = 1; i < a.size(); i++){ \
        IR * tmp = SAFETRANSLATE(a[i]); \
        res = new IR(t, OPMID(b), res, tmp); \
        v_ir_collector.push_back(res); \
    }

#define PUSH(a) \
    v_ir_collector.push_back(a)

#define MUTATESTART \
    IR * res = NULL;       \
    auto randint = get_rand_int(3); \
    switch(randint) { \

#define DOLEFT \
    case 0:{ \

#define DORIGHT \
    break; \
    } \
    \
    case 1: { \
     
#define DOBOTH  \
    break; }  \
    case 2:{ \

#define MUTATEEND \
    } \
    } \
    \
    return res; \
    
#endif
