#ifndef __AST_H__
#define __AST_H__
#include "define.h"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

enum NODETYPE {
#define DECLARE_TYPE(v) v,
    ALLTYPE(DECLARE_TYPE)
#undef DECLARE_TYPE
};
typedef NODETYPE IRTYPE;

enum CASEIDX {
    CASE0,
    CASE1,
    CASE2,
    CASE3,
    CASE4,
    CASE5,
    CASE6,
    CASE7,
    CASE8,
    CASE9,
    CASE10,
    CASE11,
    CASE12,
    CASE13,
    CASE14,
    CASE15,
    CASE16,
    CASE17,
    CASE18,
    CASE19,
    CASE20,
    CASE21,
    CASE22,
    CASE23,
    CASE24,
    CASE25,
    CASE26,
    CASE27,
    CASE28,
    CASE29,
    CASE30,
    CASE31,
    CASE32,
    CASE33,
    CASE34,
    CASE35,
    CASE36,
    CASE37,
    CASE38,
    CASE39,
    CASE40,
    CASE41,
    CASE42,
    CASE43,
    CASE44,
    CASE45,
    CASE46,
    CASE47,
    CASE48,
    CASE49,
    CASE50,
    CASE51,
    CASE52,
    CASE53,
    CASE54,
    CASE55,
    CASE56,
    CASE57,
    CASE58,
    CASE59,
    CASE60,
    CASE61,
    CASE62,
    CASE63,
    CASE64,
    CASE65,
    CASE66,
    CASE67,
    CASE68,
    CASE69,
    CASE70,
    CASE71,
    CASE72,
    CASE73,
    CASE74,
    CASE75,
    CASE76,
    CASE77,
    CASE78,
    CASE79,
    CASE80,
    CASE81,
    CASE82,
    CASE83,
    CASE84,
    CASE85,
    CASE86,
    CASE87,
    CASE88,
    CASE89,
    CASE90,
    CASE91,
    CASE92,
    CASE93,
    CASE94,
    CASE95,
    CASE96,
    CASE97,
    CASE98,
    CASE99,
    CASE100,
    CASE101,
    CASE102,
    CASE103,
    CASE104,
    CASE105,
    CASE106,
    CASE107,
    CASE108,
    CASE109,
    CASE110,
    CASE111,
    CASE112,
    CASE113,
    CASE114,
    CASE115,
    CASE116,
    CASE117,
    CASE118,
    CASE119,
    CASE120,
    CASE121,
    CASE122,
    CASE123,
    CASE124,
    CASE125,
    CASE126,
    CASE127,
    CASE128,
    CASE129,
    CASE130,
    CASE131,
    CASE132,
    CASE133,
    CASE134,
    CASE135,
    CASE136,
    CASE137,
    CASE138,
    CASE139,
    CASE140,
    CASE141,
    CASE142,
    CASE143,
    CASE144,
    CASE145,
    CASE146,
    CASE147,
    CASE148,
    CASE149,
    CASE150,
    CASE151,
    CASE152,
    CASE153,
    CASE154,
    CASE155,
    CASE156,
    CASE157,
    CASE158,
    CASE159,
    CASE160,
    CASE161,
    CASE162,
    CASE163,
    CASE164,
    CASE165,
    CASE166,
    CASE167,
    CASE168,
    CASE169,
    CASE170,
    CASE171,
    CASE172,
    CASE173,
    CASE174,
    CASE175,
    CASE176,
    CASE177,
    CASE178,
    CASE179,
    CASE180,
    CASE181,
    CASE182,
    CASE183,
    CASE184,
    CASE185,
    CASE186,
    CASE187,
    CASE188,
    CASE189,
    CASE190,
    CASE191,
    CASE192,
    CASE193,
    CASE194,
    CASE195,
    CASE196,
    CASE197,
    CASE198,
    CASE199,
    CASE200,
    CASE201,
    CASE202,
    CASE203,
    CASE204,
    CASE205,
    CASE206,
    CASE207,
    CASE208,
    CASE209,
    CASE210,
    CASE211,
    CASE212,
    CASE213,
    CASE214,
    CASE215,
    CASE216,
    CASE217,
    CASE218,
    CASE219,
    CASE220,
    CASE221,
    CASE222,
    CASE223,
    CASE224,
    CASE225,
    CASE226,
    CASE227,
    CASE228,
    CASE229,
    CASE230,
    CASE231,
    CASE232,
    CASE233,
    CASE234,
    CASE235,
    CASE236,
    CASE237,
    CASE238,
    CASE239,
    CASE240,
    CASE241,
    CASE242,
    CASE243,
    CASE244,
    CASE245,
    CASE246,
    CASE247,
    CASE248,
    CASE249,
    CASE250,
    CASE251,
    CASE252,
    CASE253,
    CASE254,
    CASE255,
    CASE256,
    CASE257,
    CASE258,
    CASE259,
    CASE260,
    CASE261,
    CASE262,
    CASE263,
    CASE264,
    CASE265,
    CASE266,
    CASE267,
    CASE268,
    CASE269,
    CASE270,
    CASE271,
    CASE272,
    CASE273,
    CASE274,
    CASE275,
    CASE276,
    CASE277,
    CASE278,
    CASE279,
    CASE280,
    CASE281,
    CASE282,
    CASE283,
    CASE284,
    CASE285,
    CASE286,
    CASE287,
    CASE288,
    CASE289,
    CASE290,
    CASE291,
    CASE292,
    CASE293,
    CASE294,
    CASE295,
    CASE296,
    CASE297,
    CASE298,
    CASE299,
    CASE300,
    CASE301,
    CASE302,
    CASE303,
    CASE304,
    CASE305,
    CASE306,
    CASE307,
    CASE308,
    CASE309,
    CASE310,
    CASE311,
    CASE312,
    CASE313,
    CASE314,
    CASE315,
    CASE316,
    CASE317,
    CASE318,
    CASE319,
    CASE320,
    CASE321,
    CASE322,
    CASE323,
    CASE324,
    CASE325,
    CASE326,
    CASE327,
    CASE328,
    CASE329,
    CASE330,
    CASE331,
    CASE332,
    CASE333,
    CASE334,
    CASE335,
    CASE336,
    CASE337,
    CASE338,
    CASE339,
    CASE340,
    CASE341,
    CASE342,
    CASE343,
    CASE344,
    CASE345,
    CASE346,
    CASE347,
    CASE348,
    CASE349,
    CASE350,
    CASE351,
    CASE352,
    CASE353,
    CASE354,
    CASE355,
    CASE356,
    CASE357,
    CASE358,
    CASE359,
    CASE360,
    CASE361,
    CASE362,
    CASE363,
    CASE364,
    CASE365,
    CASE366,
    CASE367,
    CASE368,
    CASE369,
    CASE370,
    CASE371,
    CASE372,
    CASE373,
    CASE374,
    CASE375,
    CASE376,
    CASE377,
    CASE378,
    CASE379,
    CASE380,
    CASE381,
    CASE382,
    CASE383,
    CASE384,
    CASE385,
    CASE386,
    CASE387,
    CASE388,
    CASE389,
    CASE390,
    CASE391,
    CASE392,
    CASE393,
    CASE394,
    CASE395,
    CASE396,
    CASE397,
    CASE398,
    CASE399,
};

enum DATATYPE {
#define DECLARE_TYPE(v) k##v,
    ALLDATATYPE(DECLARE_TYPE)
#undef DECLARE_TYPE
};

class IROperator {
  public:
    IROperator(string prefix = "", string middle = "", string suffix = "") : prefix_(prefix), middle_(middle), suffix_(suffix) {}

    string prefix_;
    string middle_;
    string suffix_;
};

enum UnionType {
    kUnionUnknown = 0,
    kUnionString = 1,
    kUnionFloat,
    kUnionInt,
    kUnionLong,
    kUnionBool,
};

enum DATAFLAG {
    kUse = 0x8,
    kMapToClosestOne = 0x10,
    kNoSplit = 0x100,
    kGlobal = 0x4,
    kInsertable = 0x800,
    kReplace = 0x40,
    kUndefine = 0x2,
    kAlias = 0x80,
    kMapToAll = 0x20,
    kClassDefine = 0x200,
    kFunctionDefine = 0x400,
    kDefine = 0x1,
};
#define isUse(a) ((a)&kUse)
#define isMapToClosestOne(a) ((a)&kMapToClosestOne)
#define isNoSplit(a) ((a)&kNoSplit)
#define isGlobal(a) ((a)&kGlobal)
#define isInsertable(a) ((a)&kInsertable)
#define isReplace(a) ((a)&kReplace)
#define isUndefine(a) ((a)&kUndefine)
#define isAlias(a) ((a)&kAlias)
#define isMapToAll(a) ((a)&kMapToAll)
#define isClassDefine(a) ((a)&kClassDefine)
#define isFunctionDefine(a) ((a)&kFunctionDefine)
#define isDefine(a) ((a)&kDefine)

class IR {
  public:
    IR(IRTYPE type, IROperator *op, IR *left = NULL, IR *right = NULL, DATATYPE data_type = kDataWhatever);

    IR(IRTYPE type, string str_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);
    IR(IRTYPE type, const char *str_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);

    IR(IRTYPE type, bool b_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);

    IR(IRTYPE type, unsigned long long_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);

    IR(IRTYPE type, int int_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);

    IR(IRTYPE type, double f_val, DATATYPE data_type = kDataWhatever, int scope = -1, DATAFLAG flag = kUse);

    IR(IRTYPE type, IROperator *op, IR *left, IR *right, double f_val, string str_val, string name, unsigned int mutated_times, int scope, DATAFLAG flag);

    IR(const IR *ir, IR *left, IR *right);

    union {
        int int_val_;
        unsigned long long_val_ = 0;
        double float_val_;
        bool bool_val_;
    };

    int scope_;
    unsigned long scope_id_;
    DATAFLAG data_flag_ = kUse;
    DATATYPE data_type_ = kDataWhatever;
    int value_type_ = 0;
    IRTYPE type_;
    string name_;

    string str_val_;

    IROperator *op_ = NULL;
    IR *left_ = NULL;
    IR *right_ = NULL;
    int operand_num_;
    unsigned int mutated_times_ = 0;

    unsigned long id_;
    string to_string();
    string to_string_core();
    string print();
};

class Node {
  public:
    void set_sub_type(unsigned int i) { case_idx_ = i; }
    NODETYPE type_;
    DATATYPE data_type_;
    DATAFLAG data_flag_;
    int scope_;
    unsigned int case_idx_;
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate() {}
    virtual void deep_delete() {}
    Node(){};
    ~Node(){};
};

Node *generate_ast_node_by_type(IRTYPE);

DATATYPE get_datatype_by_string(string s);

NODETYPE get_nodetype_by_string(string s);

string get_string_by_nodetype(NODETYPE tt);
string get_string_by_datatype(DATATYPE tt);
IR *deep_copy(const IR *root);

int cal_list_num(IR *);

IR *locate_define_top_ir(IR *, IR *);
IR *locate_parent(IR *root, IR *old_ir);

void set_scope_translation_flag(bool flag);
bool get_scope_translation_flag();
// void insert_IR_before(IR* ir_to_insert, IR* ir_location);

// IR* generate_definition_IR(IRTYPEd);

void deep_delete(IR *root);

#define DECLARE_CLASS(v) class v;
ALLCLASS(DECLARE_CLASS);
#undef DECLARE_CLASS

class Program : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TopStatementList *top_statement_list_;
};

class ReservedNonModifiers : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class SemiReserved : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ReservedNonModifiers *reserved_non_modifiers_;
};

class IdentifierPlus : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Identifier *identifier_;
    SemiReserved *semi_reserved_;
};

class TopStatementList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TopStatementList *top_statement_list_;
    TopStatement *top_statement_;
};

class NamespaceName : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Identifier *identifier_;
    NamespaceName *namespace_name_;
};

class Name : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NamespaceName *namespace_name_;
};

class TopStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassDeclarationStatement *class_declaration_statement_;
    UseDeclarations *use_declarations_;
    GroupUseDeclaration *group_use_declaration_;
    UseType *use_type_;
    InterfaceDeclarationStatement *interface_declaration_statement_;
    ConstList *const_list_;
    FunctionDeclarationStatement *function_declaration_statement_;
    Statement *statement_;
    TopStatementList *top_statement_list_;
    MixedGroupUseDeclaration *mixed_group_use_declaration_;
    TraitDeclarationStatement *trait_declaration_statement_;
    NamespaceName *namespace_name_;
};

class UseType : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class GroupUseDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnprefixedUseDeclarations *unprefixed_use_declarations_;
    NamespaceName *namespace_name_;
    PossibleComma *possible_comma_;
};

class MixedGroupUseDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NamespaceName *namespace_name_;
    InlineUseDeclarations *inline_use_declarations_;
    PossibleComma *possible_comma_;
};

class PossibleComma : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class InlineUseDeclarations : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InlineUseDeclaration *inline_use_declaration_;
    InlineUseDeclarations *inline_use_declarations_;
};

class UnprefixedUseDeclarations : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnprefixedUseDeclarations *unprefixed_use_declarations_;
    UnprefixedUseDeclaration *unprefixed_use_declaration_;
};

class UseDeclarations : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UseDeclarations *use_declarations_;
    UseDeclaration *use_declaration_;
};

class InlineUseDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnprefixedUseDeclaration *unprefixed_use_declaration_;
    UseType *use_type_;
};

class UnprefixedUseDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Identifier *identifier_;
    NamespaceName *namespace_name_;
};

class UseDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnprefixedUseDeclaration *unprefixed_use_declaration_;
};

class ConstList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ConstDecl *const_decl_;
    ConstList *const_list_;
};

class InnerStatementList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
    InnerStatement *inner_statement_;
};

class InnerStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassDeclarationStatement *class_declaration_statement_;
    TraitDeclarationStatement *trait_declaration_statement_;
    FunctionDeclarationStatement *function_declaration_statement_;
    Statement *statement_;
    InterfaceDeclarationStatement *interface_declaration_statement_;
};

class Statement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ForeachVariable *foreach_variable_1_;
    ForeachVariable *foreach_variable_2_;
    ConstList *const_list_;
    PossibleComma *possible_comma_;
    OptionalExpr *optional_expr_;
    DeclareStatement *declare_statement_;
    ForeachStatement *foreach_statement_;
    UnsetVariables *unset_variables_;
    FinallyStatement *finally_statement_;
    WhileStatement *while_statement_;
    Statement *statement_;
    InnerStatementList *inner_statement_list_;
    ForStatement *for_statement_;
    StaticVarList *static_var_list_;
    AltIfStmt *alt_if_stmt_;
    ForExprs *for_exprs_1_;
    ForExprs *for_exprs_2_;
    ForExprs *for_exprs_3_;
    IfStmt *if_stmt_;
    EchoExprList *echo_expr_list_;
    Expr *expr_;
    GlobalVarList *global_var_list_;
    SwitchCaseList *switch_case_list_;
    CatchList *catch_list_;
    Identifier *identifier_;
};

class CatchList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    CatchList *catch_list_;
    CatchNameList *catch_name_list_;
    DollarVariable *dollar_variable_;
    InnerStatementList *inner_statement_list_;
};

class CatchNameList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    CatchNameList *catch_name_list_;
};

class FinallyStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
};

class UnsetVariables : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnsetVariables *unset_variables_;
    UnsetVariable *unset_variable_;
};

class UnsetVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Variable *variable_;
};

class FunctionDeclarationStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    FunctionDeclaration *function_declaration_;
};

class FunctionDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Function *function_;
    ParameterList *parameter_list_;
    BackupDocComment *backup_doc_comment_;
    BackupFnFlags *backup_fn_flags_1_;
    BackupFnFlags *backup_fn_flags_2_;
    ReturnsRef *returns_ref_;
    ReturnType *return_type_;
    Identifier *identifier_;
    InnerStatementList *inner_statement_list_;
};

class IsReference : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class IsVariadic : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class ClassDeclarationStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassDeclaration *class_declaration_;
};

class ClassDeclaration : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ImplementsList *implements_list_;
    ClassBody *class_body_;
    ExtendsFrom *extends_from_;
    BackupDocComment *backup_doc_comment_;
    Identifier *identifier_;
    ClassModifiers *class_modifiers_;
};

class ClassBody : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassStatementList *class_statement_list_;
};

class ClassModifiers : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassModifiers *class_modifiers_;
    ClassModifier *class_modifier_;
};

class ClassModifier : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class TraitDeclarationStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Identifier *identifier_;
    BackupDocComment *backup_doc_comment_;
    ClassBody *class_body_;
};

class InterfaceDeclarationStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InterfaceExtendsList *interface_extends_list_;
    Identifier *identifier_;
    BackupDocComment *backup_doc_comment_;
    ClassBody *class_body_;
};

class ExtendsFrom : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
};

class InterfaceExtendsList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassNameList *class_name_list_;
};

class ImplementsList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassNameList *class_name_list_;
};

class ForeachVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Variable *variable_;
    ArrayPairList *array_pair_list_;
};

class ForStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
    Statement *statement_;
};

class ForeachStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
    Statement *statement_;
};

class DeclareStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
    Statement *statement_;
};

class SwitchCaseList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    CaseList *case_list_;
};

class CaseList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    CaseList *case_list_;
    CaseSeparator *case_separator_;
    InnerStatementList *inner_statement_list_;
};

class CaseSeparator : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class WhileStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
    Statement *statement_;
};

class IfStmtWithoutElse : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    IfStmtWithoutElse *if_stmt_without_else_;
    Statement *statement_;
};

class IfStmt : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IfStmtWithoutElse *if_stmt_without_else_;
    Statement *statement_;
};

class AltIfStmtWithoutElse : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    InnerStatementList *inner_statement_list_;
    AltIfStmtWithoutElse *alt_if_stmt_without_else_;
};

class AltIfStmt : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    AltIfStmtWithoutElse *alt_if_stmt_without_else_;
    InnerStatementList *inner_statement_list_;
};

class ParameterList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyParameterList *non_empty_parameter_list_;
};

class NonEmptyParameterList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyParameterList *non_empty_parameter_list_;
    Parameter *parameter_;
};

class Parameter : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    IsReference *is_reference_;
    IsVariadic *is_variadic_;
    DollarVariable *dollar_variable_;
    OptionalType *optional_type_;
};

class OptionalType : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TypeExpr *type_expr_;
};

class TypeExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnionType2 *union_type2_;
    Type *type_;
};

class Type : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Name *name_;
};

class UnionType2 : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    UnionType2 *union_type2_;
    Type *type_1_;
    Type *type_2_;
};

class ReturnType : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TypeExpr *type_expr_;
};

class ArgumentList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyArgumentList *non_empty_argument_list_;
    PossibleComma *possible_comma_;
};

class NonEmptyArgumentList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Argument *argument_;
    NonEmptyArgumentList *non_empty_argument_list_;
};

class Argument : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
};

class GlobalVarList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    GlobalVar *global_var_;
    GlobalVarList *global_var_list_;
};

class GlobalVar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    SimpleVariable *simple_variable_;
};

class StaticVarList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    StaticVar *static_var_;
    StaticVarList *static_var_list_;
};

class StaticVar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    DollarVariable *dollar_variable_;
};

class ClassStatementList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassStatementList *class_statement_list_;
    ClassStatement *class_statement_;
};

class ClassStatement : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TraitAdaptations *trait_adaptations_;
    ClassNameList *class_name_list_;
    ClassConstList *class_const_list_;
    MethodModifiers *method_modifiers_;
    SimpleVarMember *simple_var_member_;
    ClassMethodDefinition *class_method_definition_;
};

class ClassMethodDefinition : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Function *function_;
    ParameterList *parameter_list_;
    BackupDocComment *backup_doc_comment_;
    MethodBody *method_body_;
    BackupFnFlags *backup_fn_flags_1_;
    BackupFnFlags *backup_fn_flags_2_;
    IdentifierPlus *identifier_plus_;
    ReturnsRef *returns_ref_;
    MethodModifiers *method_modifiers_;
    ReturnType *return_type_;
};

class SimpleVarMember : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    VariableModifiers *variable_modifiers_;
    OptionalType *optional_type_;
    PropertyList *property_list_;
};

class ClassNameList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    ClassNameList *class_name_list_;
};

class TraitAdaptations : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TraitAdaptationList *trait_adaptation_list_;
};

class TraitAdaptationList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TraitAdaptation *trait_adaptation_;
    TraitAdaptationList *trait_adaptation_list_;
};

class TraitAdaptation : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    TraitAlias *trait_alias_;
    TraitPrecedence *trait_precedence_;
};

class TraitPrecedence : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassNameList *class_name_list_;
    AbsoluteTraitMethodReference *absolute_trait_method_reference_;
};

class TraitAlias : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IdentifierPlus *identifier_plus_;
    Identifier *identifier_;
    TraitMethodReference *trait_method_reference_;
    ReservedNonModifiers *reserved_non_modifiers_;
    MemberModifier *member_modifier_;
};

class TraitMethodReference : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IdentifierPlus *identifier_plus_;
    AbsoluteTraitMethodReference *absolute_trait_method_reference_;
};

class AbsoluteTraitMethodReference : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    IdentifierPlus *identifier_plus_;
};

class MethodBody : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    InnerStatementList *inner_statement_list_;
};

class VariableModifiers : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyMemberModifiers *non_empty_member_modifiers_;
};

class MethodModifiers : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyMemberModifiers *non_empty_member_modifiers_;
};

class NonEmptyMemberModifiers : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyMemberModifiers *non_empty_member_modifiers_;
    MemberModifier *member_modifier_;
};

class MemberModifier : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class PropertyList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Property *property_;
    PropertyList *property_list_;
};

class Property : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    DollarVariable *dollar_variable_;
    BackupDocComment *backup_doc_comment_;
};

class ClassConstList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassConstDecl *class_const_decl_;
    ClassConstList *class_const_list_;
};

class ClassConstDecl : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    IdentifierPlus *identifier_plus_;
    BackupDocComment *backup_doc_comment_;
};

class ConstDecl : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    Identifier *identifier_;
    BackupDocComment *backup_doc_comment_;
};

class EchoExprList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    EchoExprList *echo_expr_list_;
    EchoExpr *echo_expr_;
};

class EchoExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
};

class ForExprs : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyForExprs *non_empty_for_exprs_;
};

class NonEmptyForExprs : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyForExprs *non_empty_for_exprs_;
    Expr *expr_;
};

class AnonymousClass : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ExtendsFrom *extends_from_;
    BackupDocComment *backup_doc_comment_;
    ClassBody *class_body_;
    ImplementsList *implements_list_;
    CtorArguments *ctor_arguments_;
};

class NewExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassConstructor *class_constructor_;
    AnonymousClass *anonymous_class_;
};

class ClassConstructor : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassNameReference *class_name_reference_;
    NonEmptyArgumentList *non_empty_argument_list_;
    PossibleComma *possible_comma_;
};

class MightDefineExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Variable *variable_1_;
    Variable *variable_2_;
    Expr *expr_;
};

class Expr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NewExpr *new_expr_;
    InlineFunction *inline_function_;
    ClassNameReference *class_name_reference_;
    Expr *expr_1_;
    Expr *expr_2_;
    Expr *expr_3_;
    ExitExpr *exit_expr_;
    BackticksExpr *backticks_expr_;
    Scalar *scalar_;
    InternalFunctionsInYacc *internal_functions_in_yacc_;
    Variable *variable_;
    MightDefineExpr *might_define_expr_;
    ArrayPairList *array_pair_list_;
};

class InlineFunction : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Function *function_;
    ParameterList *parameter_list_;
    Expr *expr_;
    BackupDocComment *backup_doc_comment_;
    BackupFnFlags *backup_fn_flags_1_;
    BackupFnFlags *backup_fn_flags_2_;
    ReturnsRef *returns_ref_;
    LexicalVars *lexical_vars_;
    ReturnType *return_type_;
    InnerStatementList *inner_statement_list_;
    Fn *fn_;
    BackupLexPos *backup_lex_pos_;
};

class Fn : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class Function : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class BackupDocComment : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class BackupFnFlags : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class BackupLexPos : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class ReturnsRef : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();
};

class LexicalVars : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    LexicalVarList *lexical_var_list_;
};

class LexicalVarList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    LexicalVarList *lexical_var_list_;
    LexicalVar *lexical_var_;
};

class LexicalVar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    DollarVariable *dollar_variable_;
};

class FunctionCallme : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Name *name_;
    ClassName *class_name_;
    VariableClassName *variable_class_name_;
    CallableExpr *callable_expr_;
    ArgumentList *argument_list_;
    MemberName *member_name_;
};

class ClassName : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Name *name_;
};

class ClassNameReference : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    NewVariable *new_variable_;
};

class ExitExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    OptionalExpr *optional_expr_;
};

class BackticksExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    EncapsList *encaps_list_;
};

class CtorArguments : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ArgumentList *argument_list_;
};

class DereferencableScalar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    StringLiteral *string_literal_;
    ArrayPairList *array_pair_list_;
};

class Scalar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    EncapsList *encaps_list_;
    DereferencableScalar *dereferencable_scalar_;
    Constant *constant_;
    NumLiteral *num_literal_;
};

class Constant : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    VariableClassName *variable_class_name_;
    IdentifierPlus *identifier_plus_;
    Name *name_;
};

class OptionalExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
};

class VariableClassName : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Dereferencable *dereferencable_;
};

class Dereferencable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Variable *variable_;
    Expr *expr_;
    DereferencableScalar *dereferencable_scalar_;
};

class CallableExpr : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    CallableVariable *callable_variable_;
    DereferencableScalar *dereferencable_scalar_;
};

class CallableVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Constant *constant_;
    FunctionCallme *function_callme_;
    Expr *expr_;
    SimpleVariable *simple_variable_;
    OptionalExpr *optional_expr_;
    ArgumentList *argument_list_;
    Dereferencable *dereferencable_;
    PropertyName *property_name_;
};

class Variable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Dereferencable *dereferencable_;
    PropertyName *property_name_;
    StaticMember *static_member_;
    CallableVariable *callable_variable_;
};

class SimpleVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    DollarVariable *dollar_variable_;
    SimpleVariable *simple_variable_;
};

class StaticMember : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    VariableClassName *variable_class_name_;
    SimpleVariable *simple_variable_;
};

class NewVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ClassName *class_name_;
    NewVariable *new_variable_;
    SimpleVariable *simple_variable_;
    OptionalExpr *optional_expr_;
    Expr *expr_;
    PropertyName *property_name_;
};

class MemberName : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    IdentifierPlus *identifier_plus_;
    SimpleVariable *simple_variable_;
};

class PropertyName : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    Identifier *identifier_;
    SimpleVariable *simple_variable_;
};

class ArrayPairList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    NonEmptyArrayPairList *non_empty_array_pair_list_;
};

class PossibleArrayPair : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    ArrayPair *array_pair_;
};

class NonEmptyArrayPairList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    PossibleArrayPair *possible_array_pair_;
    NonEmptyArrayPairList *non_empty_array_pair_list_;
};

class ArrayPair : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Variable *variable_;
    Expr *expr_1_;
    Expr *expr_2_;
    ArrayPairList *array_pair_list_;
};

class EncapsList : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    EncapsList *encaps_list_;
    EncapsVar *encaps_var_;
};

class StringVarname : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    string string_val_;
};

class EncapsVar : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
    StringVarname *string_varname_;
    Variable *variable_;
    DollarVariable *dollar_variable_;
    Identifier *identifier_;
    EncapsVarOffset *encaps_var_offset_;
};

class EncapsVarOffset : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    DollarVariable *dollar_variable_;
    Identifier *identifier_;
    StringLiteral *string_literal_;
};

class InternalFunctionsInYacc : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IssetVariables *isset_variables_;
    Expr *expr_;
    PossibleComma *possible_comma_;
};

class IssetVariables : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IssetVariable *isset_variable_;
    IssetVariables *isset_variables_;
};

class IssetVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    Expr *expr_;
};

class Identifier : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    string string_val_;
};

class DollarVariable : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    string string_val_;
};

class NumLiteral : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    IntLiteral *int_literal_;
    FloatLiteral *float_literal_;
};

class FloatLiteral : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    float float_val_;
};

class IntLiteral : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    int int_val_;
};

class StringLiteral : public Node {
  public:
    virtual void deep_delete();
    virtual IR *translate(vector<IR *> &v_ir_collector);
    virtual void generate();

    string string_val_;
};

#endif
