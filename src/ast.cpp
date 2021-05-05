#include "../include/ast.h"
#include "../include/define.h"
#include "../include/typesystem.h"
#include "../include/utils.h"
#include "../include/var_definition.h"
#include <cassert>

static bool scope_tranlation = false;

static unsigned long id_counter;
#define GEN_NAME()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    name_ = gen_id_name();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
    id_ = id_counter++;

#define STORE_IR_SCOPE()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    if (scope_tranlation) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
        if (g_scope_current == NULL)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
            return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
        g_scope_current->v_ir_set_.push_back(this);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
        this->scope_id_ = g_scope_current->scope_id_;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  \
    }

IR::IR(IRTYPE type, IROperator *op, IR *left, IR *right, DATATYPE data_type) : type_(type), op_(op), left_(left), right_(right), operand_num_((!!right) + (!!left)), data_type_(data_type) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, string str_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), str_val_(str_val), op_(NULL), left_(NULL), right_(NULL), operand_num_(0), data_type_(data_type), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, const char *str_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), str_val_(str_val), op_(NULL), left_(NULL), right_(NULL), operand_num_(0), data_type_(data_type), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, bool b_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), bool_val_(b_val), left_(NULL), op_(NULL), right_(NULL), operand_num_(0), data_type_(kDataWhatever), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, unsigned long long_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), long_val_(long_val), left_(NULL), op_(NULL), right_(NULL), operand_num_(0), data_type_(kDataWhatever), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, int int_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), int_val_(int_val), left_(NULL), op_(NULL), right_(NULL), operand_num_(0), data_type_(kDataWhatever), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, double f_val, DATATYPE data_type, int scope, DATAFLAG flag) : type_(type), float_val_(f_val), left_(NULL), op_(NULL), right_(NULL), operand_num_(0), data_type_(kDataWhatever), scope_(scope), data_flag_(flag) {
    GEN_NAME();
    STORE_IR_SCOPE();
}

IR::IR(IRTYPE type, IROperator *op, IR *left, IR *right, double f_val, string str_val, string name, unsigned int mutated_times, int scope, DATAFLAG flag) : type_(type), op_(op), left_(left), right_(right), operand_num_((!!right) + (!!left)), name_(name), str_val_(str_val), float_val_(f_val), mutated_times_(mutated_times), data_type_(kDataWhatever), scope_(scope), data_flag_(flag) { STORE_IR_SCOPE(); }

IR::IR(const IR *ir, IR *left, IR *right) {
    // STORE_IR_SCOPE();
    this->type_ = ir->type_;
    if (ir->op_ != NULL)
        this->op_ = OP3(ir->op_->prefix_, ir->op_->middle_, ir->op_->suffix_);
    else {
        this->op_ = OP0();
    }
    this->left_ = left;
    this->right_ = right;
    this->str_val_ = ir->str_val_;
    this->long_val_ = ir->long_val_;
    this->data_type_ = ir->data_type_;
    this->scope_ = ir->scope_;
    this->data_flag_ = ir->data_flag_;
    this->name_ = ir->name_;
    this->operand_num_ = ir->operand_num_;
    this->mutated_times_ = ir->mutated_times_;
}

Node *generate_ast_node_by_type(IRTYPE type) {
#define DECLARE_CASE(classname)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
    if (type == k##classname)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          \
        return new classname();

    ALLCLASS(DECLARE_CASE);
#undef DECLARE_CASE
    return NULL;
}

NODETYPE get_nodetype_by_string(string s) {
#define DECLARE_CASE(datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (s == #datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
        return k##datatypename;

    ALLCLASS(DECLARE_CASE);

#undef DECLARE_CASE
    return kUnknown;
}

string get_string_by_nodetype(NODETYPE tt) {
#define DECLARE_CASE(datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (tt == k##datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
        return string(#datatypename);

    ALLCLASS(DECLARE_CASE);

#undef DECLARE_CASE
    return string("");
}

string get_string_by_datatype(DATATYPE tt) {
#define DECLARE_CASE(datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (tt == k##datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
        return string(#datatypename);

    ALLDATATYPE(DECLARE_CASE);

#undef DECLARE_CASE
    return string("");
}

DATATYPE get_datatype_by_string(string s) {
#define DECLARE_CASE(datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
    if (s == #datatypename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
        return k##datatypename;

    ALLDATATYPE(DECLARE_CASE);

#undef DECLARE_CASE
    return kDataWhatever;
}

void deep_delete(IR *root) {
    if (root == NULL)
        return;
    if (root->left_)
        deep_delete(root->left_);
    if (root->right_)
        deep_delete(root->right_);

    if (root->op_)
        delete root->op_;

    delete root;
}

IR *deep_copy(const IR *root) {
    IR *left = NULL, *right = NULL, *copy_res;

    if (root->left_)
        left = deep_copy(root->left_); // do you have a second version for deep_copy that accept only one argument?
    if (root->right_)
        right = deep_copy(root->right_); // no I forget to update here

    copy_res = new IR(root, left, right);

    return copy_res;
}

string IR::print() {
    string res;
    res = this->name_ + " = ";
    if (!this->str_val_.empty()) {
        res += "str(" + this->str_val_ + ")";
        return res;
    } else if (this->int_val_) {
        res += "int(" + std::to_string(this->int_val_) + ")";
        return res;
    }
    if (this->op_)
        res += this->op_->prefix_;
    if (this->left_)
        res += this->left_->name_;
    if (this->op_)
        res += this->op_->middle_;
    if (this->right_)
        res += this->right_->name_;
    if (this->op_)
        res += this->op_->suffix_;

    return res;
}

string IR::to_string() {
    auto res = to_string_core();
    trim_string(res);
    return res;
}

string IR::to_string_core() {
    // cout << get_string_by_nodetype(this->type_) << endl;
    switch (type_) {
    case kIntLiteral:
        return std::to_string(int_val_);
    case kFloatLiteral:
        return std::to_string(float_val_);
    case kIdentifier:
        return str_val_;
    case kStringLiteral:
        return str_val_;
    case kDollarVariable:
        return str_val_;
    case kStringVarname:
        return str_val_;
    }

    string res;

    if (op_ != NULL) {
        // if(op_->prefix_ == NULL)
        /// cout << "FUCK NULL prefix" << endl;
        // cout << "OP_Prex: " << op_->prefix_ << endl;
        res += op_->prefix_ + " ";
    }
    // cout << "OP_1_" << op_ << endl;
    if (left_ != NULL)
        // res += left_->to_string() + " ";
        res += left_->to_string_core() + " ";
    // cout << "OP_2_" << op_ << endl;
    if (op_ != NULL)
        res += op_->middle_ + " ";
    // cout << "OP_3_" << op_ << endl;
    if (right_ != NULL)
        // res += right_->to_string() + " ";
        res += right_->to_string_core() + " ";
    // cout << "OP_4_" << op_ << endl;
    if (op_ != NULL)
        res += op_->suffix_;

    // cout << "FUCK" << endl;
    // cout << "RETURN" << endl;
    return res;
}

IR *Node::translate(vector<IR *> &v_ir_collector) { return NULL; }

static int cal_list_num_dfs(IR *ir, IRTYPE type) {
    int res = 0;

    if (ir->type_ == type)
        res++;

    if (ir->left_)
        res += cal_list_num_dfs(ir->left_, type);
    if (ir->right_)
        res += cal_list_num_dfs(ir->right_, type);

    return res;
}

void trim_list_to_num(IR *ir, int num) { return; }

int cal_list_num(IR *ir) { return cal_list_num_dfs(ir, ir->type_); }

IR *locate_parent(IR *root, IR *old_ir) {
    if (root->left_ == old_ir || root->right_ == old_ir)
        return root;

    if (root->left_ != NULL)
        if (auto res = locate_parent(root->left_, old_ir))
            return res;
    if (root->right_ != NULL)
        if (auto res = locate_parent(root->right_, old_ir))
            return res;

    return NULL;
}

IR *locate_define_top_ir(IR *root, IR *ir) {
    static set<IRTYPE> define_top_set;
    static bool is_init = false;

    /* FIXME
    if(is_init == false){
                define_top_set.insert(kDeclaration);

    }
    */
    while (auto parent = locate_parent(root, ir)) {
        if (define_top_set.find(parent->type_) != define_top_set.end()) {
            return parent;
        }
        ir = parent;
    }
    cout << "root: " << root->to_string() << endl;
    cout << "find: " << ir->to_string() << endl;
    assert(0);
    return NULL;
}

void set_scope_translation_flag(bool flag) {
    scope_tranlation = flag;
    if (flag == false) {
        id_counter = 0;
    }
}
bool get_scope_translation_flag() { return scope_tranlation; }
IR *Program::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    enter_scope(kScopeGlobal);
    auto tmp1 = SAFETRANSLATE(top_statement_list_);
    res = new IR(kProgram, OP3("<?php", "", ""), tmp1);
    exit_scope();

    TRANSLATEEND
}

void Program::deep_delete() {
    SAFEDELETE(top_statement_list_);
    delete this;
};

void Program::generate() {
    GENERATESTART(1)

    top_statement_list_ = new TopStatementList();
    top_statement_list_->generate();

    GENERATEEND
}

IR *ReservedNonModifiers::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kReservedNonModifiers, OP3("include", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kReservedNonModifiers, OP3("include_once", "", ""));
    CASEEND
    CASESTART(2)
    res = new IR(kReservedNonModifiers, OP3("eval", "", ""));
    CASEEND
    CASESTART(3)
    res = new IR(kReservedNonModifiers, OP3("require", "", ""));
    CASEEND
    CASESTART(4)
    res = new IR(kReservedNonModifiers, OP3("require_once", "", ""));
    CASEEND
    CASESTART(5)
    res = new IR(kReservedNonModifiers, OP3("or", "", ""));
    CASEEND
    CASESTART(6)
    res = new IR(kReservedNonModifiers, OP3("xor", "", ""));
    CASEEND
    CASESTART(7)
    res = new IR(kReservedNonModifiers, OP3("and", "", ""));
    CASEEND
    CASESTART(8)
    res = new IR(kReservedNonModifiers, OP3("instanceof", "", ""));
    CASEEND
    CASESTART(9)
    res = new IR(kReservedNonModifiers, OP3("new", "", ""));
    CASEEND
    CASESTART(10)
    res = new IR(kReservedNonModifiers, OP3("clone", "", ""));
    CASEEND
    CASESTART(11)
    res = new IR(kReservedNonModifiers, OP3("exit", "", ""));
    CASEEND
    CASESTART(12)
    res = new IR(kReservedNonModifiers, OP3("if", "", ""));
    CASEEND
    CASESTART(13)
    res = new IR(kReservedNonModifiers, OP3("elseif", "", ""));
    CASEEND
    CASESTART(14)
    res = new IR(kReservedNonModifiers, OP3("else", "", ""));
    CASEEND
    CASESTART(15)
    res = new IR(kReservedNonModifiers, OP3("endif", "", ""));
    CASEEND
    CASESTART(16)
    res = new IR(kReservedNonModifiers, OP3("echo", "", ""));
    CASEEND
    CASESTART(17)
    res = new IR(kReservedNonModifiers, OP3("do", "", ""));
    CASEEND
    CASESTART(18)
    res = new IR(kReservedNonModifiers, OP3("while", "", ""));
    CASEEND
    CASESTART(19)
    res = new IR(kReservedNonModifiers, OP3("endwhile", "", ""));
    CASEEND
    CASESTART(20)
    res = new IR(kReservedNonModifiers, OP3("for", "", ""));
    CASEEND
    CASESTART(21)
    res = new IR(kReservedNonModifiers, OP3("endfor", "", ""));
    CASEEND
    CASESTART(22)
    res = new IR(kReservedNonModifiers, OP3("foreach", "", ""));
    CASEEND
    CASESTART(23)
    res = new IR(kReservedNonModifiers, OP3("endforeach", "", ""));
    CASEEND
    CASESTART(24)
    res = new IR(kReservedNonModifiers, OP3("declare", "", ""));
    CASEEND
    CASESTART(25)
    res = new IR(kReservedNonModifiers, OP3("enddeclare", "", ""));
    CASEEND
    CASESTART(26)
    res = new IR(kReservedNonModifiers, OP3("as", "", ""));
    CASEEND
    CASESTART(27)
    res = new IR(kReservedNonModifiers, OP3("try", "", ""));
    CASEEND
    CASESTART(28)
    res = new IR(kReservedNonModifiers, OP3("catch", "", ""));
    CASEEND
    CASESTART(29)
    res = new IR(kReservedNonModifiers, OP3("finally", "", ""));
    CASEEND
    CASESTART(30)
    res = new IR(kReservedNonModifiers, OP3("throw", "", ""));
    CASEEND
    CASESTART(31)
    res = new IR(kReservedNonModifiers, OP3("use", "", ""));
    CASEEND
    CASESTART(32)
    res = new IR(kReservedNonModifiers, OP3("insteadof", "", ""));
    CASEEND
    CASESTART(33)
    res = new IR(kReservedNonModifiers, OP3("global", "", ""));
    CASEEND
    CASESTART(34)
    res = new IR(kReservedNonModifiers, OP3("var", "", ""));
    CASEEND
    CASESTART(35)
    res = new IR(kReservedNonModifiers, OP3("unset", "", ""));
    CASEEND
    CASESTART(36)
    res = new IR(kReservedNonModifiers, OP3("isset", "", ""));
    CASEEND
    CASESTART(37)
    res = new IR(kReservedNonModifiers, OP3("empty", "", ""));
    CASEEND
    CASESTART(38)
    res = new IR(kReservedNonModifiers, OP3("continue", "", ""));
    CASEEND
    CASESTART(39)
    res = new IR(kReservedNonModifiers, OP3("goto", "", ""));
    CASEEND
    CASESTART(40)
    res = new IR(kReservedNonModifiers, OP3("function", "", ""));
    CASEEND
    CASESTART(41)
    res = new IR(kReservedNonModifiers, OP3("const", "", ""));
    CASEEND
    CASESTART(42)
    res = new IR(kReservedNonModifiers, OP3("return", "", ""));
    CASEEND
    CASESTART(43)
    res = new IR(kReservedNonModifiers, OP3("print", "", ""));
    CASEEND
    CASESTART(44)
    res = new IR(kReservedNonModifiers, OP3("yield", "", ""));
    CASEEND
    CASESTART(45)
    res = new IR(kReservedNonModifiers, OP3("list", "", ""));
    CASEEND
    CASESTART(46)
    res = new IR(kReservedNonModifiers, OP3("switch", "", ""));
    CASEEND
    CASESTART(47)
    res = new IR(kReservedNonModifiers, OP3("endswitch", "", ""));
    CASEEND
    CASESTART(48)
    res = new IR(kReservedNonModifiers, OP3("case", "", ""));
    CASEEND
    CASESTART(49)
    res = new IR(kReservedNonModifiers, OP3("default", "", ""));
    CASEEND
    CASESTART(50)
    res = new IR(kReservedNonModifiers, OP3("break", "", ""));
    CASEEND
    CASESTART(51)
    res = new IR(kReservedNonModifiers, OP3("array", "", ""));
    CASEEND
    CASESTART(52)
    res = new IR(kReservedNonModifiers, OP3("callable", "", ""));
    CASEEND
    CASESTART(53)
    res = new IR(kReservedNonModifiers, OP3("extends", "", ""));
    CASEEND
    CASESTART(54)
    res = new IR(kReservedNonModifiers, OP3("implements", "", ""));
    CASEEND
    CASESTART(55)
    res = new IR(kReservedNonModifiers, OP3("namespace", "", ""));
    CASEEND
    CASESTART(56)
    res = new IR(kReservedNonModifiers, OP3("trait", "", ""));
    CASEEND
    CASESTART(57)
    res = new IR(kReservedNonModifiers, OP3("interface", "", ""));
    CASEEND
    CASESTART(58)
    res = new IR(kReservedNonModifiers, OP3("class", "", ""));
    CASEEND
    CASESTART(59)
    res = new IR(kReservedNonModifiers, OP3("__CLASS__", "", ""));
    CASEEND
    CASESTART(60)
    res = new IR(kReservedNonModifiers, OP3("__TRAIT__", "", ""));
    CASEEND
    CASESTART(61)
    res = new IR(kReservedNonModifiers, OP3("__FUNCTION__", "", ""));
    CASEEND
    CASESTART(62)
    res = new IR(kReservedNonModifiers, OP3("__METHOD__", "", ""));
    CASEEND
    CASESTART(63)
    res = new IR(kReservedNonModifiers, OP3("__LINE__", "", ""));
    CASEEND
    CASESTART(64)
    res = new IR(kReservedNonModifiers, OP3("__FILE__", "", ""));
    CASEEND
    CASESTART(65)
    res = new IR(kReservedNonModifiers, OP3("__DIR__", "", ""));
    CASEEND
    CASESTART(66)
    res = new IR(kReservedNonModifiers, OP3("__NAMESPACE__", "", ""));
    CASEEND
    CASESTART(67)
    res = new IR(kReservedNonModifiers, OP3("fn", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ReservedNonModifiers::deep_delete() { delete this; };

void ReservedNonModifiers::generate(){GENERATESTART(68)

                                          SWITCHSTART CASESTART(0) CASEEND CASESTART(1) CASEEND CASESTART(2) CASEEND CASESTART(3) CASEEND CASESTART(4) CASEEND CASESTART(5) CASEEND CASESTART(6) CASEEND CASESTART(7) CASEEND CASESTART(8) CASEEND CASESTART(9) CASEEND CASESTART(10) CASEEND CASESTART(11) CASEEND CASESTART(12) CASEEND CASESTART(13) CASEEND CASESTART(14) CASEEND CASESTART(15) CASEEND CASESTART(16) CASEEND CASESTART(17) CASEEND CASESTART(18) CASEEND CASESTART(19) CASEEND CASESTART(20) CASEEND CASESTART(21) CASEEND CASESTART(22) CASEEND CASESTART(23) CASEEND CASESTART(24) CASEEND CASESTART(25) CASEEND CASESTART(26) CASEEND CASESTART(27) CASEEND CASESTART(28) CASEEND CASESTART(29) CASEEND CASESTART(30) CASEEND CASESTART(31) CASEEND CASESTART(32) CASEEND CASESTART(33) CASEEND CASESTART(34) CASEEND CASESTART(35) CASEEND CASESTART(36) CASEEND CASESTART(37) CASEEND CASESTART(38) CASEEND CASESTART(39) CASEEND CASESTART(40) CASEEND CASESTART(41) CASEEND CASESTART(42)
                                              CASEEND CASESTART(43) CASEEND CASESTART(44) CASEEND CASESTART(45) CASEEND CASESTART(46) CASEEND CASESTART(47) CASEEND CASESTART(48) CASEEND CASESTART(49) CASEEND CASESTART(50) CASEEND CASESTART(51) CASEEND CASESTART(52) CASEEND CASESTART(53) CASEEND CASESTART(54) CASEEND CASESTART(55) CASEEND CASESTART(56) CASEEND CASESTART(57) CASEEND CASESTART(58) CASEEND CASESTART(59) CASEEND CASESTART(60) CASEEND CASESTART(61) CASEEND CASESTART(62) CASEEND CASESTART(63) CASEEND CASESTART(64) CASEEND CASESTART(65) CASEEND CASESTART(66) CASEEND CASESTART(67) CASEEND SWITCHEND

                                                  GENERATEEND}

IR *SemiReserved::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(reserved_non_modifiers_);
    res = new IR(kSemiReserved, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    res = new IR(kSemiReserved, OP3("static", "", ""));
    CASEEND
    CASESTART(2)
    res = new IR(kSemiReserved, OP3("abstract", "", ""));
    CASEEND
    CASESTART(3)
    res = new IR(kSemiReserved, OP3("final", "", ""));
    CASEEND
    CASESTART(4)
    res = new IR(kSemiReserved, OP3("private", "", ""));
    CASEEND
    CASESTART(5)
    res = new IR(kSemiReserved, OP3("protected", "", ""));
    CASEEND
    CASESTART(6)
    res = new IR(kSemiReserved, OP3("public", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void SemiReserved::deep_delete() {
    SAFEDELETE(reserved_non_modifiers_);
    delete this;
};

void SemiReserved::generate() {
    GENERATESTART(7)

    SWITCHSTART
    CASESTART(0)
    reserved_non_modifiers_ = new ReservedNonModifiers();
    reserved_non_modifiers_->generate();
    CASEEND
    CASESTART(1)
    CASEEND
    CASESTART(2)
    CASEEND
    CASESTART(3)
    CASEEND
    CASESTART(4)
    CASEEND
    CASESTART(5)
    CASEEND
    CASESTART(6)
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *IdentifierPlus::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kIdentifierPlus, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(semi_reserved_);
    res = new IR(kIdentifierPlus, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IdentifierPlus::deep_delete() {
    SAFEDELETE(identifier_);
    SAFEDELETE(semi_reserved_);
    delete this;
};

void IdentifierPlus::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(1)
    semi_reserved_ = new SemiReserved();
    semi_reserved_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TopStatementList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(top_statement_list_);
    auto tmp2 = SAFETRANSLATE(top_statement_);
    res = new IR(kTopStatementList, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    res = new IR(kTopStatementList, string(""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TopStatementList::deep_delete() {
    SAFEDELETE(top_statement_list_);
    SAFEDELETE(top_statement_);
    delete this;
};

void TopStatementList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    top_statement_list_ = new TopStatementList();
    top_statement_list_->generate();
    top_statement_ = new TopStatement();
    top_statement_->generate();
    CASEEND
    CASESTART(1)

    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)

        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *NamespaceName::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kNamespaceName, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(identifier_);
    res = new IR(kNamespaceName, OP3("", "\\", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NamespaceName::deep_delete() {
    SAFEDELETE(identifier_);
    SAFEDELETE(namespace_name_);
    delete this;
};

void NamespaceName::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(1)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        identifier_ = new Identifier();
        identifier_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *Name::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    res = new IR(kName, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    res = new IR(kName, OP3("namespace \\", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    res = new IR(kName, OP3("\\", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Name::deep_delete() {
    SAFEDELETE(namespace_name_);
    delete this;
};

void Name::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    CASEEND
    CASESTART(1)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    CASEEND
    CASESTART(2)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TopStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kTopStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(function_declaration_statement_);
    res = new IR(kTopStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(class_declaration_statement_);
    res = new IR(kTopStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(trait_declaration_statement_);
    res = new IR(kTopStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(interface_declaration_statement_);
    res = new IR(kTopStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(5)
    res = new IR(kTopStatement, OP3("__halt_compiler ( ) ;", "", ""));
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    res = new IR(kTopStatement, OP3("namespace", ";", ""), tmp1);
    CASEEND
    CASESTART(7)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(top_statement_list_);
    res = new IR(kTopStatement, OP3("namespace", "{", "}"), tmp1, tmp2);
    CASEEND
    CASESTART(8)
    auto tmp1 = SAFETRANSLATE(top_statement_list_);
    res = new IR(kTopStatement, OP3("namespace {", "}", ""), tmp1);
    CASEEND
    CASESTART(9)
    auto tmp1 = SAFETRANSLATE(mixed_group_use_declaration_);
    res = new IR(kTopStatement, OP3("use", ";", ""), tmp1);
    CASEEND
    CASESTART(10)
    auto tmp1 = SAFETRANSLATE(use_type_);
    auto tmp2 = SAFETRANSLATE(group_use_declaration_);
    res = new IR(kTopStatement, OP3("use", "", ";"), tmp1, tmp2);
    CASEEND
    CASESTART(11)
    auto tmp1 = SAFETRANSLATE(use_declarations_);
    res = new IR(kTopStatement, OP3("use", ";", ""), tmp1);
    CASEEND
    CASESTART(12)
    auto tmp1 = SAFETRANSLATE(use_type_);
    auto tmp2 = SAFETRANSLATE(use_declarations_);
    res = new IR(kTopStatement, OP3("use", "", ";"), tmp1, tmp2);
    CASEEND
    CASESTART(13)
    auto tmp1 = SAFETRANSLATE(const_list_);
    res = new IR(kTopStatement, OP3("const", ";", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TopStatement::deep_delete() {
    SAFEDELETE(class_declaration_statement_);
    SAFEDELETE(use_declarations_);
    SAFEDELETE(group_use_declaration_);
    SAFEDELETE(use_type_);
    SAFEDELETE(interface_declaration_statement_);
    SAFEDELETE(const_list_);
    SAFEDELETE(function_declaration_statement_);
    SAFEDELETE(statement_);
    SAFEDELETE(top_statement_list_);
    SAFEDELETE(mixed_group_use_declaration_);
    SAFEDELETE(trait_declaration_statement_);
    SAFEDELETE(namespace_name_);
    delete this;
};

void TopStatement::generate() {
    GENERATESTART(14)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    function_declaration_statement_ = new FunctionDeclarationStatement();
    function_declaration_statement_->generate();
    CASEEND
    CASESTART(2)
    class_declaration_statement_ = new ClassDeclarationStatement();
    class_declaration_statement_->generate();
    CASEEND
    CASESTART(3)
    trait_declaration_statement_ = new TraitDeclarationStatement();
    trait_declaration_statement_->generate();
    CASEEND
    CASESTART(4)
    interface_declaration_statement_ = new InterfaceDeclarationStatement();
    interface_declaration_statement_->generate();
    CASEEND
    CASESTART(5)
    CASEEND
    CASESTART(6)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    CASEEND
    CASESTART(7)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    top_statement_list_ = new TopStatementList();
    top_statement_list_->generate();
    CASEEND
    CASESTART(8)
    top_statement_list_ = new TopStatementList();
    top_statement_list_->generate();
    CASEEND
    CASESTART(9)
    mixed_group_use_declaration_ = new MixedGroupUseDeclaration();
    mixed_group_use_declaration_->generate();
    CASEEND
    CASESTART(10)
    use_type_ = new UseType();
    use_type_->generate();
    group_use_declaration_ = new GroupUseDeclaration();
    group_use_declaration_->generate();
    CASEEND
    CASESTART(11)
    use_declarations_ = new UseDeclarations();
    use_declarations_->generate();
    CASEEND
    CASESTART(12)
    use_type_ = new UseType();
    use_type_->generate();
    use_declarations_ = new UseDeclarations();
    use_declarations_->generate();
    CASEEND
    CASESTART(13)
    const_list_ = new ConstList();
    const_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *UseType::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kUseType, OP3("function", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kUseType, OP3("const", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UseType::deep_delete() { delete this; };

void UseType::generate(){GENERATESTART(2)

                             SWITCHSTART CASESTART(0) CASEEND CASESTART(1) CASEEND SWITCHEND

                                 GENERATEEND}

IR *GroupUseDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(unprefixed_use_declarations_);
    auto tmp3 = SAFETRANSLATE(possible_comma_);
    auto tmp4 = new IR(kUnknown, OP3("", "\\ {", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kGroupUseDeclaration, OP3("", "", "}"), tmp4, tmp3);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(unprefixed_use_declarations_);
    auto tmp3 = SAFETRANSLATE(possible_comma_);
    auto tmp4 = new IR(kUnknown, OP3("\\", "\\ {", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kGroupUseDeclaration, OP3("", "", "}"), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void GroupUseDeclaration::deep_delete() {
    SAFEDELETE(unprefixed_use_declarations_);
    SAFEDELETE(namespace_name_);
    SAFEDELETE(possible_comma_);
    delete this;
};

void GroupUseDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    unprefixed_use_declarations_ = new UnprefixedUseDeclarations();
    unprefixed_use_declarations_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    CASESTART(1)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    unprefixed_use_declarations_ = new UnprefixedUseDeclarations();
    unprefixed_use_declarations_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *MixedGroupUseDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(inline_use_declarations_);
    auto tmp3 = SAFETRANSLATE(possible_comma_);
    auto tmp4 = new IR(kUnknown, OP3("", "\\ {", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kMixedGroupUseDeclaration, OP3("", "", "}"), tmp4, tmp3);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(inline_use_declarations_);
    auto tmp3 = SAFETRANSLATE(possible_comma_);
    auto tmp4 = new IR(kUnknown, OP3("\\", "\\ {", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kMixedGroupUseDeclaration, OP3("", "", "}"), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MixedGroupUseDeclaration::deep_delete() {
    SAFEDELETE(namespace_name_);
    SAFEDELETE(inline_use_declarations_);
    SAFEDELETE(possible_comma_);
    delete this;
};

void MixedGroupUseDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    inline_use_declarations_ = new InlineUseDeclarations();
    inline_use_declarations_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    CASESTART(1)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    inline_use_declarations_ = new InlineUseDeclarations();
    inline_use_declarations_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *PossibleComma::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kPossibleComma, string(""));
    CASEEND
    CASESTART(1)
    res = new IR(kPossibleComma, OP3(",", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void PossibleComma::deep_delete() { delete this; };

void PossibleComma::generate(){GENERATESTART(2)

                                   SWITCHSTART CASESTART(0)

                                       CASEEND CASESTART(1) CASEEND SWITCHEND

                                           GENERATEEND}

IR *InlineUseDeclarations::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(inline_use_declarations_);
    auto tmp2 = SAFETRANSLATE(inline_use_declaration_);
    res = new IR(kInlineUseDeclarations, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inline_use_declaration_);
    res = new IR(kInlineUseDeclarations, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InlineUseDeclarations::deep_delete() {
    SAFEDELETE(inline_use_declaration_);
    SAFEDELETE(inline_use_declarations_);
    delete this;
};

void InlineUseDeclarations::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    inline_use_declarations_ = new InlineUseDeclarations();
    inline_use_declarations_->generate();
    inline_use_declaration_ = new InlineUseDeclaration();
    inline_use_declaration_->generate();
    CASEEND
    CASESTART(1)
    inline_use_declaration_ = new InlineUseDeclaration();
    inline_use_declaration_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        inline_use_declaration_ = new InlineUseDeclaration();
        inline_use_declaration_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *UnprefixedUseDeclarations::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(unprefixed_use_declarations_);
    auto tmp2 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kUnprefixedUseDeclarations, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kUnprefixedUseDeclarations, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UnprefixedUseDeclarations::deep_delete() {
    SAFEDELETE(unprefixed_use_declarations_);
    SAFEDELETE(unprefixed_use_declaration_);
    delete this;
};

void UnprefixedUseDeclarations::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    unprefixed_use_declarations_ = new UnprefixedUseDeclarations();
    unprefixed_use_declarations_->generate();
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND
    CASESTART(1)
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
        unprefixed_use_declaration_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *UseDeclarations::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(use_declarations_);
    auto tmp2 = SAFETRANSLATE(use_declaration_);
    res = new IR(kUseDeclarations, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(use_declaration_);
    res = new IR(kUseDeclarations, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UseDeclarations::deep_delete() {
    SAFEDELETE(use_declarations_);
    SAFEDELETE(use_declaration_);
    delete this;
};

void UseDeclarations::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    use_declarations_ = new UseDeclarations();
    use_declarations_->generate();
    use_declaration_ = new UseDeclaration();
    use_declaration_->generate();
    CASEEND
    CASESTART(1)
    use_declaration_ = new UseDeclaration();
    use_declaration_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        use_declaration_ = new UseDeclaration();
        use_declaration_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *InlineUseDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kInlineUseDeclaration, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(use_type_);
    auto tmp2 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kInlineUseDeclaration, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InlineUseDeclaration::deep_delete() {
    SAFEDELETE(unprefixed_use_declaration_);
    SAFEDELETE(use_type_);
    delete this;
};

void InlineUseDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND
    CASESTART(1)
    use_type_ = new UseType();
    use_type_->generate();
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *UnprefixedUseDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    res = new IR(kUnprefixedUseDeclaration, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(namespace_name_);
    auto tmp2 = SAFETRANSLATE(identifier_);
    res = new IR(kUnprefixedUseDeclaration, OP3("", "as", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UnprefixedUseDeclaration::deep_delete() {
    SAFEDELETE(identifier_);
    SAFEDELETE(namespace_name_);
    delete this;
};

void UnprefixedUseDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    CASEEND
    CASESTART(1)
    namespace_name_ = new NamespaceName();
    namespace_name_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *UseDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kUseDeclaration, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(unprefixed_use_declaration_);
    res = new IR(kUseDeclaration, OP3("\\", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UseDeclaration::deep_delete() {
    SAFEDELETE(unprefixed_use_declaration_);
    delete this;
};

void UseDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND
    CASESTART(1)
    unprefixed_use_declaration_ = new UnprefixedUseDeclaration();
    unprefixed_use_declaration_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ConstList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(const_list_);
    auto tmp2 = SAFETRANSLATE(const_decl_);
    res = new IR(kConstList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(const_decl_);
    res = new IR(kConstList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ConstList::deep_delete() {
    SAFEDELETE(const_decl_);
    SAFEDELETE(const_list_);
    delete this;
};

void ConstList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    const_list_ = new ConstList();
    const_list_->generate();
    const_decl_ = new ConstDecl();
    const_decl_->generate();
    CASEEND
    CASESTART(1)
    const_decl_ = new ConstDecl();
    const_decl_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        const_decl_ = new ConstDecl();
        const_decl_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *InnerStatementList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    auto tmp2 = SAFETRANSLATE(inner_statement_);
    res = new IR(kInnerStatementList, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    res = new IR(kInnerStatementList, string(""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InnerStatementList::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(inner_statement_);
    delete this;
};

void InnerStatementList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    inner_statement_ = new InnerStatement();
    inner_statement_->generate();
    CASEEND
    CASESTART(1)

    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)

        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *InnerStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kInnerStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(function_declaration_statement_);
    res = new IR(kInnerStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(class_declaration_statement_);
    res = new IR(kInnerStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(trait_declaration_statement_);
    res = new IR(kInnerStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(interface_declaration_statement_);
    res = new IR(kInnerStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(5)
    res = new IR(kInnerStatement, OP3("__halt_compiler ( ) ;", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InnerStatement::deep_delete() {
    SAFEDELETE(class_declaration_statement_);
    SAFEDELETE(trait_declaration_statement_);
    SAFEDELETE(function_declaration_statement_);
    SAFEDELETE(statement_);
    SAFEDELETE(interface_declaration_statement_);
    delete this;
};

void InnerStatement::generate() {
    GENERATESTART(6)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    function_declaration_statement_ = new FunctionDeclarationStatement();
    function_declaration_statement_->generate();
    CASEEND
    CASESTART(2)
    class_declaration_statement_ = new ClassDeclarationStatement();
    class_declaration_statement_->generate();
    CASEEND
    CASESTART(3)
    trait_declaration_statement_ = new TraitDeclarationStatement();
    trait_declaration_statement_->generate();
    CASEEND
    CASESTART(4)
    interface_declaration_statement_ = new InterfaceDeclarationStatement();
    interface_declaration_statement_->generate();
    CASEEND
    CASESTART(5)
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Statement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kStatement, OP3("{", "}", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(if_stmt_);
    res = new IR(kStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(alt_if_stmt_);
    res = new IR(kStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(while_statement_);
    res = new IR(kStatement, OP3("while (", ")", ""), tmp1, tmp2);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(statement_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kStatement, OP3("do", "while (", ") ;"), tmp1, tmp2);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(for_exprs_1_);
    auto tmp2 = SAFETRANSLATE(for_exprs_2_);
    auto tmp3 = SAFETRANSLATE(for_exprs_3_);
    auto tmp4 = SAFETRANSLATE(for_statement_);
    auto tmp5 = new IR(kUnknown, OP3("for (", ";", ";"), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ")"), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kStatement, OP3("", "", ""), tmp6, tmp4);
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(switch_case_list_);
    res = new IR(kStatement, OP3("switch (", ")", ""), tmp1, tmp2);
    CASEEND
    CASESTART(7)
    auto tmp1 = SAFETRANSLATE(optional_expr_);
    res = new IR(kStatement, OP3("break", ";", ""), tmp1);
    CASEEND
    CASESTART(8)
    auto tmp1 = SAFETRANSLATE(optional_expr_);
    res = new IR(kStatement, OP3("continue", ";", ""), tmp1);
    CASEEND
    CASESTART(9)
    auto tmp1 = SAFETRANSLATE(optional_expr_);
    res = new IR(kStatement, OP3("return", ";", ""), tmp1);
    CASEEND
    CASESTART(10)
    auto tmp1 = SAFETRANSLATE(global_var_list_);
    res = new IR(kStatement, OP3("global", ";", ""), tmp1);
    CASEEND
    CASESTART(11)
    auto tmp1 = SAFETRANSLATE(static_var_list_);
    res = new IR(kStatement, OP3("static", ";", ""), tmp1);
    CASEEND
    CASESTART(12)
    auto tmp1 = SAFETRANSLATE(echo_expr_list_);
    res = new IR(kStatement, OP3("echo", ";", ""), tmp1);
    CASEEND
    CASESTART(13)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kStatement, OP3("", ";", ""), tmp1);
    CASEEND
    CASESTART(14)
    auto tmp1 = SAFETRANSLATE(unset_variables_);
    auto tmp2 = SAFETRANSLATE(possible_comma_);
    res = new IR(kStatement, OP3("unset (", "", ") ;"), tmp1, tmp2);
    CASEEND
    CASESTART(15)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(foreach_variable_1_);
    auto tmp3 = SAFETRANSLATE(foreach_statement_);
    auto tmp4 = new IR(kUnknown, OP3("foreach (", "as", ")"), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kStatement, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(16)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(foreach_variable_1_);
    auto tmp3 = SAFETRANSLATE(foreach_variable_2_);
    auto tmp4 = SAFETRANSLATE(foreach_statement_);
    auto tmp5 = new IR(kUnknown, OP3("foreach (", "as", "=>"), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ")"), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kStatement, OP3("", "", ""), tmp6, tmp4);
    CASEEND
    CASESTART(17)
    auto tmp1 = SAFETRANSLATE(const_list_);
    auto tmp2 = SAFETRANSLATE(declare_statement_);
    res = new IR(kStatement, OP3("declare (", ")", ""), tmp1, tmp2);
    CASEEND
    CASESTART(18)
    res = new IR(kStatement, OP3(";", "", ""));
    CASEEND
    CASESTART(19)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    auto tmp2 = SAFETRANSLATE(catch_list_);
    auto tmp3 = SAFETRANSLATE(finally_statement_);
    auto tmp4 = new IR(kUnknown, OP3("try {", "}", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kStatement, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(20)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kStatement, OP3("throw", ";", ""), tmp1);
    CASEEND
    CASESTART(21)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kStatement, OP3("goto", ";", ""), tmp1);
    CASEEND
    CASESTART(22)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kStatement, OP3("", ":", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Statement::deep_delete() {
    SAFEDELETE(foreach_variable_1_);
    SAFEDELETE(foreach_variable_2_);
    SAFEDELETE(const_list_);
    SAFEDELETE(possible_comma_);
    SAFEDELETE(optional_expr_);
    SAFEDELETE(declare_statement_);
    SAFEDELETE(foreach_statement_);
    SAFEDELETE(unset_variables_);
    SAFEDELETE(finally_statement_);
    SAFEDELETE(while_statement_);
    SAFEDELETE(statement_);
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(for_statement_);
    SAFEDELETE(static_var_list_);
    SAFEDELETE(alt_if_stmt_);
    SAFEDELETE(for_exprs_1_);
    SAFEDELETE(for_exprs_2_);
    SAFEDELETE(for_exprs_3_);
    SAFEDELETE(if_stmt_);
    SAFEDELETE(echo_expr_list_);
    SAFEDELETE(expr_);
    SAFEDELETE(global_var_list_);
    SAFEDELETE(switch_case_list_);
    SAFEDELETE(catch_list_);
    SAFEDELETE(identifier_);
    delete this;
};

void Statement::generate() {
    GENERATESTART(2300)

    SWITCHSTART
    CASESTART(0)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    CASESTART(1)
    if_stmt_ = new IfStmt();
    if_stmt_->generate();
    CASEEND
    CASESTART(2)
    alt_if_stmt_ = new AltIfStmt();
    alt_if_stmt_->generate();
    CASEEND
    CASESTART(3)
    expr_ = new Expr();
    expr_->generate();
    while_statement_ = new WhileStatement();
    while_statement_->generate();
    CASEEND
    CASESTART(4)
    statement_ = new Statement();
    statement_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(5)
    for_exprs_1_ = new ForExprs();
    for_exprs_1_->generate();
    for_exprs_2_ = new ForExprs();
    for_exprs_2_->generate();
    for_exprs_3_ = new ForExprs();
    for_exprs_3_->generate();
    for_statement_ = new ForStatement();
    for_statement_->generate();
    CASEEND
    CASESTART(6)
    expr_ = new Expr();
    expr_->generate();
    switch_case_list_ = new SwitchCaseList();
    switch_case_list_->generate();
    CASEEND
    CASESTART(7)
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(8)
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(9)
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(10)
    global_var_list_ = new GlobalVarList();
    global_var_list_->generate();
    CASEEND
    CASESTART(11)
    static_var_list_ = new StaticVarList();
    static_var_list_->generate();
    CASEEND
    CASESTART(12)
    echo_expr_list_ = new EchoExprList();
    echo_expr_list_->generate();
    CASEEND
    CASESTART(13)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(14)
    unset_variables_ = new UnsetVariables();
    unset_variables_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    CASESTART(15)
    expr_ = new Expr();
    expr_->generate();
    foreach_variable_1_ = new ForeachVariable();
    foreach_variable_1_->generate();
    foreach_statement_ = new ForeachStatement();
    foreach_statement_->generate();
    CASEEND
    CASESTART(16)
    expr_ = new Expr();
    expr_->generate();
    foreach_variable_1_ = new ForeachVariable();
    foreach_variable_1_->generate();
    foreach_variable_2_ = new ForeachVariable();
    foreach_variable_2_->generate();
    foreach_statement_ = new ForeachStatement();
    foreach_statement_->generate();
    CASEEND
    CASESTART(17)
    const_list_ = new ConstList();
    const_list_->generate();
    declare_statement_ = new DeclareStatement();
    declare_statement_->generate();
    CASEEND
    CASESTART(18)
    CASEEND
    CASESTART(19)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    catch_list_ = new CatchList();
    catch_list_->generate();
    finally_statement_ = new FinallyStatement();
    finally_statement_->generate();
    CASEEND
    CASESTART(20)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(21)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(22)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 22;
    switch (tmp_case_idx) {
        CASESTART(0)
        inner_statement_list_ = new InnerStatementList();
        inner_statement_list_->generate();
        case_idx_ = 0;
        CASEEND
        CASESTART(1)
        if_stmt_ = new IfStmt();
        if_stmt_->generate();
        case_idx_ = 1;
        CASEEND
        CASESTART(2)
        alt_if_stmt_ = new AltIfStmt();
        alt_if_stmt_->generate();
        case_idx_ = 2;
        CASEEND
        CASESTART(3)
        expr_ = new Expr();
        expr_->generate();
        while_statement_ = new WhileStatement();
        while_statement_->generate();
        case_idx_ = 3;
        CASEEND
        CASESTART(4)
        for_exprs_1_ = new ForExprs();
        for_exprs_1_->generate();
        for_exprs_2_ = new ForExprs();
        for_exprs_2_->generate();
        for_exprs_3_ = new ForExprs();
        for_exprs_3_->generate();
        for_statement_ = new ForStatement();
        for_statement_->generate();
        case_idx_ = 5;
        CASEEND
        CASESTART(5)
        expr_ = new Expr();
        expr_->generate();
        switch_case_list_ = new SwitchCaseList();
        switch_case_list_->generate();
        case_idx_ = 6;
        CASEEND
        CASESTART(6)
        optional_expr_ = new OptionalExpr();
        optional_expr_->generate();
        case_idx_ = 7;
        CASEEND
        CASESTART(7)
        optional_expr_ = new OptionalExpr();
        optional_expr_->generate();
        case_idx_ = 8;
        CASEEND
        CASESTART(8)
        optional_expr_ = new OptionalExpr();
        optional_expr_->generate();
        case_idx_ = 9;
        CASEEND
        CASESTART(9)
        global_var_list_ = new GlobalVarList();
        global_var_list_->generate();
        case_idx_ = 10;
        CASEEND
        CASESTART(10)
        static_var_list_ = new StaticVarList();
        static_var_list_->generate();
        case_idx_ = 11;
        CASEEND
        CASESTART(11)
        echo_expr_list_ = new EchoExprList();
        echo_expr_list_->generate();
        case_idx_ = 12;
        CASEEND
        CASESTART(12)
        expr_ = new Expr();
        expr_->generate();
        case_idx_ = 13;
        CASEEND
        CASESTART(13)
        unset_variables_ = new UnsetVariables();
        unset_variables_->generate();
        possible_comma_ = new PossibleComma();
        possible_comma_->generate();
        case_idx_ = 14;
        CASEEND
        CASESTART(14)
        expr_ = new Expr();
        expr_->generate();
        foreach_variable_1_ = new ForeachVariable();
        foreach_variable_1_->generate();
        foreach_statement_ = new ForeachStatement();
        foreach_statement_->generate();
        case_idx_ = 15;
        CASEEND
        CASESTART(15)
        expr_ = new Expr();
        expr_->generate();
        foreach_variable_1_ = new ForeachVariable();
        foreach_variable_1_->generate();
        foreach_variable_2_ = new ForeachVariable();
        foreach_variable_2_->generate();
        foreach_statement_ = new ForeachStatement();
        foreach_statement_->generate();
        case_idx_ = 16;
        CASEEND
        CASESTART(16)
        const_list_ = new ConstList();
        const_list_->generate();
        declare_statement_ = new DeclareStatement();
        declare_statement_->generate();
        case_idx_ = 17;
        CASEEND
        CASESTART(17)
        case_idx_ = 18;
        CASEEND
        CASESTART(18)
        inner_statement_list_ = new InnerStatementList();
        inner_statement_list_->generate();
        catch_list_ = new CatchList();
        catch_list_->generate();
        finally_statement_ = new FinallyStatement();
        finally_statement_->generate();
        case_idx_ = 19;
        CASEEND
        CASESTART(19)
        expr_ = new Expr();
        expr_->generate();
        case_idx_ = 20;
        CASEEND
        CASESTART(20)
        identifier_ = new Identifier();
        identifier_->generate();
        case_idx_ = 21;
        CASEEND
        CASESTART(21)
        identifier_ = new Identifier();
        identifier_->generate();
        case_idx_ = 22;
        CASEEND
    }
}
}

GENERATEEND
}

IR *CatchList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kCatchList, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(catch_list_);
    auto tmp2 = SAFETRANSLATE(catch_name_list_);
    auto tmp3 = SAFETRANSLATE(dollar_variable_);
    auto tmp4 = SAFETRANSLATE(inner_statement_list_);
    auto tmp5 = new IR(kUnknown, OP3("", "catch (", ""), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ")"), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kCatchList, OP3("", "{", "}"), tmp6, tmp4);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CatchList::deep_delete() {
    SAFEDELETE(catch_list_);
    SAFEDELETE(catch_name_list_);
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void CatchList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    catch_list_ = new CatchList();
    catch_list_->generate();
    catch_name_list_ = new CatchNameList();
    catch_name_list_->generate();
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)

        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *CatchNameList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_name_);
    res = new IR(kCatchNameList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(catch_name_list_);
    auto tmp2 = SAFETRANSLATE(class_name_);
    res = new IR(kCatchNameList, OP3("", "|", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CatchNameList::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(catch_name_list_);
    delete this;
};

void CatchNameList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND
    CASESTART(1)
    catch_name_list_ = new CatchNameList();
    catch_name_list_->generate();
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        class_name_ = new ClassName();
        class_name_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *FinallyStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kFinallyStatement, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kFinallyStatement, OP3("finally {", "}", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void FinallyStatement::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void FinallyStatement::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *UnsetVariables::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(unset_variable_);
    res = new IR(kUnsetVariables, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(unset_variables_);
    auto tmp2 = SAFETRANSLATE(unset_variable_);
    res = new IR(kUnsetVariables, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UnsetVariables::deep_delete() {
    SAFEDELETE(unset_variables_);
    SAFEDELETE(unset_variable_);
    delete this;
};

void UnsetVariables::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    unset_variable_ = new UnsetVariable();
    unset_variable_->generate();
    CASEEND
    CASESTART(1)
    unset_variables_ = new UnsetVariables();
    unset_variables_->generate();
    unset_variable_ = new UnsetVariable();
    unset_variable_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        unset_variable_ = new UnsetVariable();
        unset_variable_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *UnsetVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kUnsetVariable, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void UnsetVariable::deep_delete() {
    SAFEDELETE(variable_);
    delete this;
};

void UnsetVariable::generate() {
    GENERATESTART(1)

    variable_ = new Variable();
    variable_->generate();

    GENERATEEND
}

IR *FunctionDeclarationStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(function_declaration_);
    res = new IR(kFunctionDeclarationStatement, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void FunctionDeclarationStatement::deep_delete() {
    SAFEDELETE(function_declaration_);
    delete this;
};

void FunctionDeclarationStatement::generate() {
    GENERATESTART(1)

    function_declaration_ = new FunctionDeclaration();
    function_declaration_->generate();

    GENERATEEND
}

IR *FunctionDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    enter_scope(kScopeFunction);
    auto tmp1 = SAFETRANSLATE(function_);
    auto tmp2 = SAFETRANSLATE(returns_ref_);
    auto tmp3 = SAFETRANSLATE(identifier_);
    auto tmp4 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp5 = SAFETRANSLATE(parameter_list_);
    auto tmp6 = SAFETRANSLATE(return_type_);
    auto tmp7 = SAFETRANSLATE(backup_fn_flags_1_);
    auto tmp8 = SAFETRANSLATE(inner_statement_list_);
    auto tmp9 = SAFETRANSLATE(backup_fn_flags_2_);
    auto tmp10 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp10);
    auto tmp11 = new IR(kUnknown, OP3("", "", ""), tmp10, tmp3);
    PUSH(tmp11);
    auto tmp12 = new IR(kUnknown, OP3("", "", "("), tmp11, tmp4);
    PUSH(tmp12);
    auto tmp13 = new IR(kUnknown, OP3("", "", ")"), tmp12, tmp5);
    PUSH(tmp13);
    auto tmp14 = new IR(kUnknown, OP3("", "", ""), tmp13, tmp6);
    PUSH(tmp14);
    auto tmp15 = new IR(kUnknown, OP3("", "", "{"), tmp14, tmp7);
    PUSH(tmp15);
    auto tmp16 = new IR(kUnknown, OP3("", "", "}"), tmp15, tmp8);
    PUSH(tmp16);
    res = new IR(kFunctionDeclaration, OP3("", "", ""), tmp16, tmp9);
    exit_scope();

    TRANSLATEEND
}

void FunctionDeclaration::deep_delete() {
    SAFEDELETE(function_);
    SAFEDELETE(parameter_list_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(backup_fn_flags_1_);
    SAFEDELETE(backup_fn_flags_2_);
    SAFEDELETE(returns_ref_);
    SAFEDELETE(return_type_);
    SAFEDELETE(identifier_);
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void FunctionDeclaration::generate() {
    GENERATESTART(1)

    function_ = new Function();
    function_->generate();
    returns_ref_ = new ReturnsRef();
    returns_ref_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    parameter_list_ = new ParameterList();
    parameter_list_->generate();
    return_type_ = new ReturnType();
    return_type_->generate();
    backup_fn_flags_1_ = new BackupFnFlags();
    backup_fn_flags_1_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    backup_fn_flags_2_ = new BackupFnFlags();
    backup_fn_flags_2_->generate();

    GENERATEEND
}

IR *IsReference::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kIsReference, string(""));
    CASEEND
    CASESTART(1)
    res = new IR(kIsReference, OP3("&", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IsReference::deep_delete() { delete this; };

void IsReference::generate(){GENERATESTART(2)

                                 SWITCHSTART CASESTART(0)

                                     CASEEND CASESTART(1) CASEEND SWITCHEND

                                         GENERATEEND}

IR *IsVariadic::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kIsVariadic, string(""));
    CASEEND
    CASESTART(1)
    res = new IR(kIsVariadic, OP3("...", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IsVariadic::deep_delete() { delete this; };

void IsVariadic::generate(){GENERATESTART(2)

                                SWITCHSTART CASESTART(0)

                                    CASEEND CASESTART(1) CASEEND SWITCHEND

                                        GENERATEEND}

IR *ClassDeclarationStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(class_declaration_);
    res = new IR(kClassDeclarationStatement, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void ClassDeclarationStatement::deep_delete() {
    SAFEDELETE(class_declaration_);
    delete this;
};

void ClassDeclarationStatement::generate() {
    GENERATESTART(1)

    class_declaration_ = new ClassDeclaration();
    class_declaration_->generate();

    GENERATEEND
}

IR *ClassDeclaration::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_modifiers_);
    auto tmp2 = SAFETRANSLATE(identifier_);
    auto tmp3 = SAFETRANSLATE(extends_from_);
    auto tmp4 = SAFETRANSLATE(implements_list_);
    auto tmp5 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp6 = SAFETRANSLATE(class_body_);
    auto tmp7 = new IR(kUnknown, OP3("", "class", ""), tmp1, tmp2);
    PUSH(tmp7);
    auto tmp8 = new IR(kUnknown, OP3("", "", ""), tmp7, tmp3);
    PUSH(tmp8);
    auto tmp9 = new IR(kUnknown, OP3("", "", ""), tmp8, tmp4);
    PUSH(tmp9);
    auto tmp10 = new IR(kUnknown, OP3("", "", ""), tmp9, tmp5);
    PUSH(tmp10);
    res = new IR(kClassDeclaration, OP3("", "", ""), tmp10, tmp6);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(identifier_);
    auto tmp2 = SAFETRANSLATE(extends_from_);
    auto tmp3 = SAFETRANSLATE(implements_list_);
    auto tmp4 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp5 = SAFETRANSLATE(class_body_);
    auto tmp6 = new IR(kUnknown, OP3("class", "", ""), tmp1, tmp2);
    PUSH(tmp6);
    auto tmp7 = new IR(kUnknown, OP3("", "", ""), tmp6, tmp3);
    PUSH(tmp7);
    auto tmp8 = new IR(kUnknown, OP3("", "", ""), tmp7, tmp4);
    PUSH(tmp8);
    res = new IR(kClassDeclaration, OP3("", "", ""), tmp8, tmp5);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassDeclaration::deep_delete() {
    SAFEDELETE(implements_list_);
    SAFEDELETE(class_body_);
    SAFEDELETE(extends_from_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(identifier_);
    SAFEDELETE(class_modifiers_);
    delete this;
};

void ClassDeclaration::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    class_modifiers_ = new ClassModifiers();
    class_modifiers_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    extends_from_ = new ExtendsFrom();
    extends_from_->generate();
    implements_list_ = new ImplementsList();
    implements_list_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    class_body_ = new ClassBody();
    class_body_->generate();
    CASEEND
    CASESTART(1)
    identifier_ = new Identifier();
    identifier_->generate();
    extends_from_ = new ExtendsFrom();
    extends_from_->generate();
    implements_list_ = new ImplementsList();
    implements_list_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    class_body_ = new ClassBody();
    class_body_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassBody::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    enter_scope(kScopeClass);
    auto tmp1 = SAFETRANSLATE(class_statement_list_);
    res = new IR(kClassBody, OP3("{", "}", ""), tmp1);
    exit_scope();

    TRANSLATEEND
}

void ClassBody::deep_delete() {
    SAFEDELETE(class_statement_list_);
    delete this;
};

void ClassBody::generate() {
    GENERATESTART(1)

    class_statement_list_ = new ClassStatementList();
    class_statement_list_->generate();

    GENERATEEND
}

IR *ClassModifiers::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_modifier_);
    res = new IR(kClassModifiers, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_modifiers_);
    auto tmp2 = SAFETRANSLATE(class_modifier_);
    res = new IR(kClassModifiers, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassModifiers::deep_delete() {
    SAFEDELETE(class_modifiers_);
    SAFEDELETE(class_modifier_);
    delete this;
};

void ClassModifiers::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    class_modifier_ = new ClassModifier();
    class_modifier_->generate();
    CASEEND
    CASESTART(1)
    class_modifiers_ = new ClassModifiers();
    class_modifiers_->generate();
    class_modifier_ = new ClassModifier();
    class_modifier_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        class_modifier_ = new ClassModifier();
        class_modifier_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *ClassModifier::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kClassModifier, OP3("abstract", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kClassModifier, OP3("final", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassModifier::deep_delete() { delete this; };

void ClassModifier::generate(){GENERATESTART(2)

                                   SWITCHSTART CASESTART(0) CASEEND CASESTART(1) CASEEND SWITCHEND

                                       GENERATEEND}

IR *TraitDeclarationStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(identifier_);
    auto tmp2 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp3 = SAFETRANSLATE(class_body_);
    auto tmp4 = new IR(kUnknown, OP3("trait", "", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kTraitDeclarationStatement, OP3("", "", ""), tmp4, tmp3);

    TRANSLATEEND
}

void TraitDeclarationStatement::deep_delete() {
    SAFEDELETE(identifier_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(class_body_);
    delete this;
};

void TraitDeclarationStatement::generate() {
    GENERATESTART(1)

    identifier_ = new Identifier();
    identifier_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    class_body_ = new ClassBody();
    class_body_->generate();

    GENERATEEND
}

IR *InterfaceDeclarationStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(identifier_);
    auto tmp2 = SAFETRANSLATE(interface_extends_list_);
    auto tmp3 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp4 = SAFETRANSLATE(class_body_);
    auto tmp5 = new IR(kUnknown, OP3("interface", "", ""), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ""), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kInterfaceDeclarationStatement, OP3("", "", ""), tmp6, tmp4);

    TRANSLATEEND
}

void InterfaceDeclarationStatement::deep_delete() {
    SAFEDELETE(interface_extends_list_);
    SAFEDELETE(identifier_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(class_body_);
    delete this;
};

void InterfaceDeclarationStatement::generate() {
    GENERATESTART(1)

    identifier_ = new Identifier();
    identifier_->generate();
    interface_extends_list_ = new InterfaceExtendsList();
    interface_extends_list_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    class_body_ = new ClassBody();
    class_body_->generate();

    GENERATEEND
}

IR *ExtendsFrom::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kExtendsFrom, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_);
    res = new IR(kExtendsFrom, OP3("extends", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ExtendsFrom::deep_delete() {
    SAFEDELETE(class_name_);
    delete this;
};

void ExtendsFrom::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *InterfaceExtendsList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kInterfaceExtendsList, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_list_);
    res = new IR(kInterfaceExtendsList, OP3("extends", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InterfaceExtendsList::deep_delete() {
    SAFEDELETE(class_name_list_);
    delete this;
};

void InterfaceExtendsList::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    class_name_list_ = new ClassNameList();
    class_name_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ImplementsList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kImplementsList, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_list_);
    res = new IR(kImplementsList, OP3("implements", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ImplementsList::deep_delete() {
    SAFEDELETE(class_name_list_);
    delete this;
};

void ImplementsList::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    class_name_list_ = new ClassNameList();
    class_name_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ForeachVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kForeachVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kForeachVariable, OP3("&", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kForeachVariable, OP3("list (", ")", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kForeachVariable, OP3("[", "]", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ForeachVariable::deep_delete() {
    SAFEDELETE(variable_);
    SAFEDELETE(array_pair_list_);
    delete this;
};

void ForeachVariable::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(1)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(2)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    CASESTART(3)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ForStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kForStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kForStatement, OP3(":", "endfor ;", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ForStatement::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(statement_);
    delete this;
};

void ForStatement::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ForeachStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kForeachStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kForeachStatement, OP3(":", "endforeach ;", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ForeachStatement::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(statement_);
    delete this;
};

void ForeachStatement::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *DeclareStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kDeclareStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kDeclareStatement, OP3(":", "enddeclare ;", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void DeclareStatement::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(statement_);
    delete this;
};

void DeclareStatement::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *SwitchCaseList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(case_list_);
    res = new IR(kSwitchCaseList, OP3("{", "}", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(case_list_);
    res = new IR(kSwitchCaseList, OP3("{ ;", "}", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(case_list_);
    res = new IR(kSwitchCaseList, OP3(":", "endswitch ;", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(case_list_);
    res = new IR(kSwitchCaseList, OP3(": ;", "endswitch ;", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void SwitchCaseList::deep_delete() {
    SAFEDELETE(case_list_);
    delete this;
};

void SwitchCaseList::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    case_list_ = new CaseList();
    case_list_->generate();
    CASEEND
    CASESTART(1)
    case_list_ = new CaseList();
    case_list_->generate();
    CASEEND
    CASESTART(2)
    case_list_ = new CaseList();
    case_list_->generate();
    CASEEND
    CASESTART(3)
    case_list_ = new CaseList();
    case_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *CaseList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kCaseList, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(case_list_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(case_separator_);
    auto tmp4 = SAFETRANSLATE(inner_statement_list_);
    auto tmp5 = new IR(kUnknown, OP3("", "case", ""), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ""), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kCaseList, OP3("", "", ""), tmp6, tmp4);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(case_list_);
    auto tmp2 = SAFETRANSLATE(case_separator_);
    auto tmp3 = SAFETRANSLATE(inner_statement_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "default", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kCaseList, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CaseList::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(case_list_);
    SAFEDELETE(case_separator_);
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void CaseList::generate() {
    GENERATESTART(300)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    case_list_ = new CaseList();
    case_list_->generate();
    expr_ = new Expr();
    expr_->generate();
    case_separator_ = new CaseSeparator();
    case_separator_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    CASESTART(2)
    case_list_ = new CaseList();
    case_list_->generate();
    case_separator_ = new CaseSeparator();
    case_separator_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)

        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *CaseSeparator::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kCaseSeparator, OP3(":", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kCaseSeparator, OP3(";", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CaseSeparator::deep_delete() { delete this; };

void CaseSeparator::generate(){GENERATESTART(2)

                                   SWITCHSTART CASESTART(0) CASEEND CASESTART(1) CASEEND SWITCHEND

                                       GENERATEEND}

IR *WhileStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(statement_);
    res = new IR(kWhileStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kWhileStatement, OP3(":", "endwhile ;", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void WhileStatement::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(statement_);
    delete this;
};

void WhileStatement::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *IfStmtWithoutElse::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(statement_);
    res = new IR(kIfStmtWithoutElse, OP3("if (", ")", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(if_stmt_without_else_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(statement_);
    auto tmp4 = new IR(kUnknown, OP3("", "elseif (", ")"), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kIfStmtWithoutElse, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IfStmtWithoutElse::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(if_stmt_without_else_);
    SAFEDELETE(statement_);
    delete this;
};

void IfStmtWithoutElse::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    expr_ = new Expr();
    expr_->generate();
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    CASESTART(1)
    if_stmt_without_else_ = new IfStmtWithoutElse();
    if_stmt_without_else_->generate();
    expr_ = new Expr();
    expr_->generate();
    statement_ = new Statement();
    statement_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        expr_ = new Expr();
        expr_->generate();
        statement_ = new Statement();
        statement_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *IfStmt::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(if_stmt_without_else_);
    res = new IR(kIfStmt, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(if_stmt_without_else_);
    auto tmp2 = SAFETRANSLATE(statement_);
    res = new IR(kIfStmt, OP3("", "else", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IfStmt::deep_delete() {
    SAFEDELETE(if_stmt_without_else_);
    SAFEDELETE(statement_);
    delete this;
};

void IfStmt::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    if_stmt_without_else_ = new IfStmtWithoutElse();
    if_stmt_without_else_->generate();
    CASEEND
    CASESTART(1)
    if_stmt_without_else_ = new IfStmtWithoutElse();
    if_stmt_without_else_->generate();
    statement_ = new Statement();
    statement_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *AltIfStmtWithoutElse::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(expr_);
    auto tmp2 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kAltIfStmtWithoutElse, OP3("if (", ") :", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(alt_if_stmt_without_else_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(inner_statement_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "elseif (", ")"), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kAltIfStmtWithoutElse, OP3("", ":", ""), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void AltIfStmtWithoutElse::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(alt_if_stmt_without_else_);
    delete this;
};

void AltIfStmtWithoutElse::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    expr_ = new Expr();
    expr_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    CASESTART(1)
    alt_if_stmt_without_else_ = new AltIfStmtWithoutElse();
    alt_if_stmt_without_else_->generate();
    expr_ = new Expr();
    expr_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        expr_ = new Expr();
        expr_->generate();
        inner_statement_list_ = new InnerStatementList();
        inner_statement_list_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *AltIfStmt::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(alt_if_stmt_without_else_);
    res = new IR(kAltIfStmt, OP3("", "endif ;", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(alt_if_stmt_without_else_);
    auto tmp2 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kAltIfStmt, OP3("", "else :", "endif ;"), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void AltIfStmt::deep_delete() {
    SAFEDELETE(alt_if_stmt_without_else_);
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void AltIfStmt::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    alt_if_stmt_without_else_ = new AltIfStmtWithoutElse();
    alt_if_stmt_without_else_->generate();
    CASEEND
    CASESTART(1)
    alt_if_stmt_without_else_ = new AltIfStmtWithoutElse();
    alt_if_stmt_without_else_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ParameterList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(non_empty_parameter_list_);
    res = new IR(kParameterList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    res = new IR(kParameterList, string(""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ParameterList::deep_delete() {
    SAFEDELETE(non_empty_parameter_list_);
    delete this;
};

void ParameterList::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    non_empty_parameter_list_ = new NonEmptyParameterList();
    non_empty_parameter_list_->generate();
    CASEEND
    CASESTART(1)

    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NonEmptyParameterList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(parameter_);
    res = new IR(kNonEmptyParameterList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_parameter_list_);
    auto tmp2 = SAFETRANSLATE(parameter_);
    res = new IR(kNonEmptyParameterList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NonEmptyParameterList::deep_delete() {
    SAFEDELETE(non_empty_parameter_list_);
    SAFEDELETE(parameter_);
    delete this;
};

void NonEmptyParameterList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    parameter_ = new Parameter();
    parameter_->generate();
    CASEEND
    CASESTART(1)
    non_empty_parameter_list_ = new NonEmptyParameterList();
    non_empty_parameter_list_->generate();
    parameter_ = new Parameter();
    parameter_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        parameter_ = new Parameter();
        parameter_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *Parameter::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(optional_type_);
    auto tmp2 = SAFETRANSLATE(is_reference_);
    auto tmp3 = SAFETRANSLATE(is_variadic_);
    auto tmp4 = SAFETRANSLATE(dollar_variable_);
    auto tmp5 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp5);
    auto tmp6 = new IR(kUnknown, OP3("", "", ""), tmp5, tmp3);
    PUSH(tmp6);
    res = new IR(kParameter, OP3("", "", ""), tmp6, tmp4);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(optional_type_);
    auto tmp2 = SAFETRANSLATE(is_reference_);
    auto tmp3 = SAFETRANSLATE(is_variadic_);
    auto tmp4 = SAFETRANSLATE(dollar_variable_);
    auto tmp5 = SAFETRANSLATE(expr_);
    auto tmp6 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp6);
    auto tmp7 = new IR(kUnknown, OP3("", "", ""), tmp6, tmp3);
    PUSH(tmp7);
    auto tmp8 = new IR(kUnknown, OP3("", "", "="), tmp7, tmp4);
    PUSH(tmp8);
    res = new IR(kParameter, OP3("", "", ""), tmp8, tmp5);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Parameter::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(is_reference_);
    SAFEDELETE(is_variadic_);
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(optional_type_);
    delete this;
};

void Parameter::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    optional_type_ = new OptionalType();
    optional_type_->generate();
    is_reference_ = new IsReference();
    is_reference_->generate();
    is_variadic_ = new IsVariadic();
    is_variadic_->generate();
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    CASESTART(1)
    optional_type_ = new OptionalType();
    optional_type_->generate();
    is_reference_ = new IsReference();
    is_reference_->generate();
    is_variadic_ = new IsVariadic();
    is_variadic_->generate();
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *OptionalType::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kOptionalType, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(type_expr_);
    res = new IR(kOptionalType, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void OptionalType::deep_delete() {
    SAFEDELETE(type_expr_);
    delete this;
};

void OptionalType::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    type_expr_ = new TypeExpr();
    type_expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TypeExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(type_);
    res = new IR(kTypeExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(type_);
    res = new IR(kTypeExpr, OP3("?", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(union_type2_);
    res = new IR(kTypeExpr, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TypeExpr::deep_delete() {
    SAFEDELETE(union_type2_);
    SAFEDELETE(type_);
    delete this;
};

void TypeExpr::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    type_ = new Type();
    type_->generate();
    CASEEND
    CASESTART(1)
    type_ = new Type();
    type_->generate();
    CASEEND
    CASESTART(2)
    union_type2_ = new UnionType2();
    union_type2_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Type::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kType, OP3("array", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kType, OP3("callable", "", ""));
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(name_);
    res = new IR(kType, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Type::deep_delete() {
    SAFEDELETE(name_);
    delete this;
};

void Type::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    CASEEND
    CASESTART(1)
    CASEEND
    CASESTART(2)
    name_ = new Name();
    name_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *UnionType2::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(type_1_);
    auto tmp2 = SAFETRANSLATE(type_2_);
    res = new IR(kUnionType2, OP3("", "|", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(union_type2_);
    auto tmp2 = SAFETRANSLATE(type_1_);
    res = new IR(kUnionType2, OP3("", "|", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void UnionType2::deep_delete() {
    SAFEDELETE(union_type2_);
    SAFEDELETE(type_1_);
    SAFEDELETE(type_2_);
    delete this;
};

void UnionType2::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    type_1_ = new Type();
    type_1_->generate();
    type_2_ = new Type();
    type_2_->generate();
    CASEEND
    CASESTART(1)
    union_type2_ = new UnionType2();
    union_type2_->generate();
    type_1_ = new Type();
    type_1_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        type_1_ = new Type();
        type_1_->generate();
        type_2_ = new Type();
        type_2_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *ReturnType::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kReturnType, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(type_expr_);
    res = new IR(kReturnType, OP3(":", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ReturnType::deep_delete() {
    SAFEDELETE(type_expr_);
    delete this;
};

void ReturnType::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    type_expr_ = new TypeExpr();
    type_expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ArgumentList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kArgumentList, OP3("( )", "", ""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_argument_list_);
    auto tmp2 = SAFETRANSLATE(possible_comma_);
    res = new IR(kArgumentList, OP3("(", "", ")"), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ArgumentList::deep_delete() {
    SAFEDELETE(non_empty_argument_list_);
    SAFEDELETE(possible_comma_);
    delete this;
};

void ArgumentList::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    CASEEND
    CASESTART(1)
    non_empty_argument_list_ = new NonEmptyArgumentList();
    non_empty_argument_list_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NonEmptyArgumentList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(argument_);
    res = new IR(kNonEmptyArgumentList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_argument_list_);
    auto tmp2 = SAFETRANSLATE(argument_);
    res = new IR(kNonEmptyArgumentList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NonEmptyArgumentList::deep_delete() {
    SAFEDELETE(argument_);
    SAFEDELETE(non_empty_argument_list_);
    delete this;
};

void NonEmptyArgumentList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    argument_ = new Argument();
    argument_->generate();
    CASEEND
    CASESTART(1)
    non_empty_argument_list_ = new NonEmptyArgumentList();
    non_empty_argument_list_->generate();
    argument_ = new Argument();
    argument_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        argument_ = new Argument();
        argument_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *Argument::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kArgument, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kArgument, OP3("...", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Argument::deep_delete() {
    SAFEDELETE(expr_);
    delete this;
};

void Argument::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *GlobalVarList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(global_var_list_);
    auto tmp2 = SAFETRANSLATE(global_var_);
    res = new IR(kGlobalVarList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(global_var_);
    res = new IR(kGlobalVarList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void GlobalVarList::deep_delete() {
    SAFEDELETE(global_var_);
    SAFEDELETE(global_var_list_);
    delete this;
};

void GlobalVarList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    global_var_list_ = new GlobalVarList();
    global_var_list_->generate();
    global_var_ = new GlobalVar();
    global_var_->generate();
    CASEEND
    CASESTART(1)
    global_var_ = new GlobalVar();
    global_var_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        global_var_ = new GlobalVar();
        global_var_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *GlobalVar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kGlobalVar, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void GlobalVar::deep_delete() {
    SAFEDELETE(simple_variable_);
    delete this;
};

void GlobalVar::generate() {
    GENERATESTART(1)

    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();

    GENERATEEND
}

IR *StaticVarList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(static_var_list_);
    auto tmp2 = SAFETRANSLATE(static_var_);
    res = new IR(kStaticVarList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(static_var_);
    res = new IR(kStaticVarList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void StaticVarList::deep_delete() {
    SAFEDELETE(static_var_);
    SAFEDELETE(static_var_list_);
    delete this;
};

void StaticVarList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    static_var_list_ = new StaticVarList();
    static_var_list_->generate();
    static_var_ = new StaticVar();
    static_var_->generate();
    CASEEND
    CASESTART(1)
    static_var_ = new StaticVar();
    static_var_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        static_var_ = new StaticVar();
        static_var_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *StaticVar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kStaticVar, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kStaticVar, OP3("", "=", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void StaticVar::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(dollar_variable_);
    delete this;
};

void StaticVar::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    CASESTART(1)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassStatementList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_statement_list_);
    auto tmp2 = SAFETRANSLATE(class_statement_);
    res = new IR(kClassStatementList, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    res = new IR(kClassStatementList, string(""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassStatementList::deep_delete() {
    SAFEDELETE(class_statement_list_);
    SAFEDELETE(class_statement_);
    delete this;
};

void ClassStatementList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    class_statement_list_ = new ClassStatementList();
    class_statement_list_->generate();
    class_statement_ = new ClassStatement();
    class_statement_->generate();
    CASEEND
    CASESTART(1)

    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)

        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *ClassStatement::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(simple_var_member_);
    res = new IR(kClassStatement, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(method_modifiers_);
    auto tmp2 = SAFETRANSLATE(class_const_list_);
    res = new IR(kClassStatement, OP3("", "const", ";"), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(class_name_list_);
    auto tmp2 = SAFETRANSLATE(trait_adaptations_);
    res = new IR(kClassStatement, OP3("use", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(class_method_definition_);
    res = new IR(kClassStatement, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassStatement::deep_delete() {
    SAFEDELETE(trait_adaptations_);
    SAFEDELETE(class_name_list_);
    SAFEDELETE(class_const_list_);
    SAFEDELETE(method_modifiers_);
    SAFEDELETE(simple_var_member_);
    SAFEDELETE(class_method_definition_);
    delete this;
};

void ClassStatement::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    simple_var_member_ = new SimpleVarMember();
    simple_var_member_->generate();
    CASEEND
    CASESTART(1)
    method_modifiers_ = new MethodModifiers();
    method_modifiers_->generate();
    class_const_list_ = new ClassConstList();
    class_const_list_->generate();
    CASEEND
    CASESTART(2)
    class_name_list_ = new ClassNameList();
    class_name_list_->generate();
    trait_adaptations_ = new TraitAdaptations();
    trait_adaptations_->generate();
    CASEEND
    CASESTART(3)
    class_method_definition_ = new ClassMethodDefinition();
    class_method_definition_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassMethodDefinition::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    enter_scope(kScopeFunction);
    auto tmp1 = SAFETRANSLATE(method_modifiers_);
    auto tmp2 = SAFETRANSLATE(function_);
    auto tmp3 = SAFETRANSLATE(returns_ref_);
    auto tmp4 = SAFETRANSLATE(identifier_plus_);
    auto tmp5 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp6 = SAFETRANSLATE(parameter_list_);
    auto tmp7 = SAFETRANSLATE(return_type_);
    auto tmp8 = SAFETRANSLATE(backup_fn_flags_1_);
    auto tmp9 = SAFETRANSLATE(method_body_);
    auto tmp10 = SAFETRANSLATE(backup_fn_flags_2_);
    auto tmp11 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp11);
    auto tmp12 = new IR(kUnknown, OP3("", "", ""), tmp11, tmp3);
    PUSH(tmp12);
    auto tmp13 = new IR(kUnknown, OP3("", "", ""), tmp12, tmp4);
    PUSH(tmp13);
    auto tmp14 = new IR(kUnknown, OP3("", "", "("), tmp13, tmp5);
    PUSH(tmp14);
    auto tmp15 = new IR(kUnknown, OP3("", "", ")"), tmp14, tmp6);
    PUSH(tmp15);
    auto tmp16 = new IR(kUnknown, OP3("", "", ""), tmp15, tmp7);
    PUSH(tmp16);
    auto tmp17 = new IR(kUnknown, OP3("", "", ""), tmp16, tmp8);
    PUSH(tmp17);
    auto tmp18 = new IR(kUnknown, OP3("", "", ""), tmp17, tmp9);
    PUSH(tmp18);
    res = new IR(kClassMethodDefinition, OP3("", "", ""), tmp18, tmp10);
    exit_scope();

    TRANSLATEEND
}

void ClassMethodDefinition::deep_delete() {
    SAFEDELETE(function_);
    SAFEDELETE(parameter_list_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(method_body_);
    SAFEDELETE(backup_fn_flags_1_);
    SAFEDELETE(backup_fn_flags_2_);
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(returns_ref_);
    SAFEDELETE(method_modifiers_);
    SAFEDELETE(return_type_);
    delete this;
};

void ClassMethodDefinition::generate() {
    GENERATESTART(1)

    method_modifiers_ = new MethodModifiers();
    method_modifiers_->generate();
    function_ = new Function();
    function_->generate();
    returns_ref_ = new ReturnsRef();
    returns_ref_->generate();
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    parameter_list_ = new ParameterList();
    parameter_list_->generate();
    return_type_ = new ReturnType();
    return_type_->generate();
    backup_fn_flags_1_ = new BackupFnFlags();
    backup_fn_flags_1_->generate();
    method_body_ = new MethodBody();
    method_body_->generate();
    backup_fn_flags_2_ = new BackupFnFlags();
    backup_fn_flags_2_->generate();

    GENERATEEND
}

IR *SimpleVarMember::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(variable_modifiers_);
    auto tmp2 = SAFETRANSLATE(optional_type_);
    auto tmp3 = SAFETRANSLATE(property_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kSimpleVarMember, OP3("", "", ";"), tmp4, tmp3);

    TRANSLATEEND
}

void SimpleVarMember::deep_delete() {
    SAFEDELETE(variable_modifiers_);
    SAFEDELETE(optional_type_);
    SAFEDELETE(property_list_);
    delete this;
};

void SimpleVarMember::generate() {
    GENERATESTART(1)

    variable_modifiers_ = new VariableModifiers();
    variable_modifiers_->generate();
    optional_type_ = new OptionalType();
    optional_type_->generate();
    property_list_ = new PropertyList();
    property_list_->generate();

    GENERATEEND
}

IR *ClassNameList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_name_);
    res = new IR(kClassNameList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_list_);
    auto tmp2 = SAFETRANSLATE(class_name_);
    res = new IR(kClassNameList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassNameList::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(class_name_list_);
    delete this;
};

void ClassNameList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND
    CASESTART(1)
    class_name_list_ = new ClassNameList();
    class_name_list_->generate();
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        class_name_ = new ClassName();
        class_name_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *TraitAdaptations::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kTraitAdaptations, OP3(";", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kTraitAdaptations, OP3("{ }", "", ""));
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(trait_adaptation_list_);
    res = new IR(kTraitAdaptations, OP3("{", "}", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TraitAdaptations::deep_delete() {
    SAFEDELETE(trait_adaptation_list_);
    delete this;
};

void TraitAdaptations::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    CASEEND
    CASESTART(1)
    CASEEND
    CASESTART(2)
    trait_adaptation_list_ = new TraitAdaptationList();
    trait_adaptation_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TraitAdaptationList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(trait_adaptation_);
    res = new IR(kTraitAdaptationList, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(trait_adaptation_list_);
    auto tmp2 = SAFETRANSLATE(trait_adaptation_);
    res = new IR(kTraitAdaptationList, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TraitAdaptationList::deep_delete() {
    SAFEDELETE(trait_adaptation_);
    SAFEDELETE(trait_adaptation_list_);
    delete this;
};

void TraitAdaptationList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    trait_adaptation_ = new TraitAdaptation();
    trait_adaptation_->generate();
    CASEEND
    CASESTART(1)
    trait_adaptation_list_ = new TraitAdaptationList();
    trait_adaptation_list_->generate();
    trait_adaptation_ = new TraitAdaptation();
    trait_adaptation_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        trait_adaptation_ = new TraitAdaptation();
        trait_adaptation_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *TraitAdaptation::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(trait_precedence_);
    res = new IR(kTraitAdaptation, OP3("", ";", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(trait_alias_);
    res = new IR(kTraitAdaptation, OP3("", ";", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TraitAdaptation::deep_delete() {
    SAFEDELETE(trait_alias_);
    SAFEDELETE(trait_precedence_);
    delete this;
};

void TraitAdaptation::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    trait_precedence_ = new TraitPrecedence();
    trait_precedence_->generate();
    CASEEND
    CASESTART(1)
    trait_alias_ = new TraitAlias();
    trait_alias_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TraitPrecedence::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(absolute_trait_method_reference_);
    auto tmp2 = SAFETRANSLATE(class_name_list_);
    res = new IR(kTraitPrecedence, OP3("", "insteadof", ""), tmp1, tmp2);

    TRANSLATEEND
}

void TraitPrecedence::deep_delete() {
    SAFEDELETE(class_name_list_);
    SAFEDELETE(absolute_trait_method_reference_);
    delete this;
};

void TraitPrecedence::generate() {
    GENERATESTART(1)

    absolute_trait_method_reference_ = new AbsoluteTraitMethodReference();
    absolute_trait_method_reference_->generate();
    class_name_list_ = new ClassNameList();
    class_name_list_->generate();

    GENERATEEND
}

IR *TraitAlias::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(trait_method_reference_);
    auto tmp2 = SAFETRANSLATE(identifier_);
    res = new IR(kTraitAlias, OP3("", "as", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(trait_method_reference_);
    auto tmp2 = SAFETRANSLATE(reserved_non_modifiers_);
    res = new IR(kTraitAlias, OP3("", "as", ""), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(trait_method_reference_);
    auto tmp2 = SAFETRANSLATE(member_modifier_);
    auto tmp3 = SAFETRANSLATE(identifier_plus_);
    auto tmp4 = new IR(kUnknown, OP3("", "as", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kTraitAlias, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(trait_method_reference_);
    auto tmp2 = SAFETRANSLATE(member_modifier_);
    res = new IR(kTraitAlias, OP3("", "as", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TraitAlias::deep_delete() {
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(identifier_);
    SAFEDELETE(trait_method_reference_);
    SAFEDELETE(reserved_non_modifiers_);
    SAFEDELETE(member_modifier_);
    delete this;
};

void TraitAlias::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    trait_method_reference_ = new TraitMethodReference();
    trait_method_reference_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(1)
    trait_method_reference_ = new TraitMethodReference();
    trait_method_reference_->generate();
    reserved_non_modifiers_ = new ReservedNonModifiers();
    reserved_non_modifiers_->generate();
    CASEEND
    CASESTART(2)
    trait_method_reference_ = new TraitMethodReference();
    trait_method_reference_->generate();
    member_modifier_ = new MemberModifier();
    member_modifier_->generate();
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    CASEEND
    CASESTART(3)
    trait_method_reference_ = new TraitMethodReference();
    trait_method_reference_->generate();
    member_modifier_ = new MemberModifier();
    member_modifier_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *TraitMethodReference::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_plus_);
    res = new IR(kTraitMethodReference, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(absolute_trait_method_reference_);
    res = new IR(kTraitMethodReference, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void TraitMethodReference::deep_delete() {
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(absolute_trait_method_reference_);
    delete this;
};

void TraitMethodReference::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    CASEEND
    CASESTART(1)
    absolute_trait_method_reference_ = new AbsoluteTraitMethodReference();
    absolute_trait_method_reference_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *AbsoluteTraitMethodReference::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(class_name_);
    auto tmp2 = SAFETRANSLATE(identifier_plus_);
    res = new IR(kAbsoluteTraitMethodReference, OP3("", "::", ""), tmp1, tmp2);

    TRANSLATEEND
}

void AbsoluteTraitMethodReference::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(identifier_plus_);
    delete this;
};

void AbsoluteTraitMethodReference::generate() {
    GENERATESTART(1)

    class_name_ = new ClassName();
    class_name_->generate();
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();

    GENERATEEND
}

IR *MethodBody::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kMethodBody, OP3(";", "", ""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(inner_statement_list_);
    res = new IR(kMethodBody, OP3("{", "}", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MethodBody::deep_delete() {
    SAFEDELETE(inner_statement_list_);
    delete this;
};

void MethodBody::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    CASEEND
    CASESTART(1)
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *VariableModifiers::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(non_empty_member_modifiers_);
    res = new IR(kVariableModifiers, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    res = new IR(kVariableModifiers, OP3("var", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void VariableModifiers::deep_delete() {
    SAFEDELETE(non_empty_member_modifiers_);
    delete this;
};

void VariableModifiers::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    non_empty_member_modifiers_ = new NonEmptyMemberModifiers();
    non_empty_member_modifiers_->generate();
    CASEEND
    CASESTART(1)
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *MethodModifiers::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kMethodModifiers, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_member_modifiers_);
    res = new IR(kMethodModifiers, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MethodModifiers::deep_delete() {
    SAFEDELETE(non_empty_member_modifiers_);
    delete this;
};

void MethodModifiers::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    non_empty_member_modifiers_ = new NonEmptyMemberModifiers();
    non_empty_member_modifiers_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NonEmptyMemberModifiers::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(member_modifier_);
    res = new IR(kNonEmptyMemberModifiers, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_member_modifiers_);
    auto tmp2 = SAFETRANSLATE(member_modifier_);
    res = new IR(kNonEmptyMemberModifiers, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NonEmptyMemberModifiers::deep_delete() {
    SAFEDELETE(non_empty_member_modifiers_);
    SAFEDELETE(member_modifier_);
    delete this;
};

void NonEmptyMemberModifiers::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    member_modifier_ = new MemberModifier();
    member_modifier_->generate();
    CASEEND
    CASESTART(1)
    non_empty_member_modifiers_ = new NonEmptyMemberModifiers();
    non_empty_member_modifiers_->generate();
    member_modifier_ = new MemberModifier();
    member_modifier_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        member_modifier_ = new MemberModifier();
        member_modifier_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *MemberModifier::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kMemberModifier, OP3("public", "", ""));
    CASEEND
    CASESTART(1)
    res = new IR(kMemberModifier, OP3("protected", "", ""));
    CASEEND
    CASESTART(2)
    res = new IR(kMemberModifier, OP3("private", "", ""));
    CASEEND
    CASESTART(3)
    res = new IR(kMemberModifier, OP3("static", "", ""));
    CASEEND
    CASESTART(4)
    res = new IR(kMemberModifier, OP3("abstract", "", ""));
    CASEEND
    CASESTART(5)
    res = new IR(kMemberModifier, OP3("final", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MemberModifier::deep_delete() { delete this; };

void MemberModifier::generate(){GENERATESTART(6)

                                    SWITCHSTART CASESTART(0) CASEEND CASESTART(1) CASEEND CASESTART(2) CASEEND CASESTART(3) CASEEND CASESTART(4) CASEEND CASESTART(5) CASEEND SWITCHEND

                                        GENERATEEND}

IR *PropertyList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(property_list_);
    auto tmp2 = SAFETRANSLATE(property_);
    res = new IR(kPropertyList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(property_);
    res = new IR(kPropertyList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void PropertyList::deep_delete() {
    SAFEDELETE(property_);
    SAFEDELETE(property_list_);
    delete this;
};

void PropertyList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    property_list_ = new PropertyList();
    property_list_->generate();
    property_ = new Property();
    property_->generate();
    CASEEND
    CASESTART(1)
    property_ = new Property();
    property_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        property_ = new Property();
        property_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *Property::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    auto tmp2 = SAFETRANSLATE(backup_doc_comment_);
    res = new IR(kProperty, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp4 = new IR(kUnknown, OP3("", "=", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kProperty, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Property::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(backup_doc_comment_);
    delete this;
};

void Property::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    CASEEND
    CASESTART(1)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    expr_ = new Expr();
    expr_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassConstList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_const_list_);
    auto tmp2 = SAFETRANSLATE(class_const_decl_);
    res = new IR(kClassConstList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_const_decl_);
    res = new IR(kClassConstList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassConstList::deep_delete() {
    SAFEDELETE(class_const_decl_);
    SAFEDELETE(class_const_list_);
    delete this;
};

void ClassConstList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    class_const_list_ = new ClassConstList();
    class_const_list_->generate();
    class_const_decl_ = new ClassConstDecl();
    class_const_decl_->generate();
    CASEEND
    CASESTART(1)
    class_const_decl_ = new ClassConstDecl();
    class_const_decl_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        class_const_decl_ = new ClassConstDecl();
        class_const_decl_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *ClassConstDecl::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(identifier_plus_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp4 = new IR(kUnknown, OP3("", "=", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kClassConstDecl, OP3("", "", ""), tmp4, tmp3);

    TRANSLATEEND
}

void ClassConstDecl::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(backup_doc_comment_);
    delete this;
};

void ClassConstDecl::generate() {
    GENERATESTART(1)

    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    expr_ = new Expr();
    expr_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();

    GENERATEEND
}

IR *ConstDecl::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(identifier_);
    auto tmp2 = SAFETRANSLATE(expr_);
    auto tmp3 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp4 = new IR(kUnknown, OP3("", "=", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kConstDecl, OP3("", "", ""), tmp4, tmp3);

    TRANSLATEEND
}

void ConstDecl::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(identifier_);
    SAFEDELETE(backup_doc_comment_);
    delete this;
};

void ConstDecl::generate() {
    GENERATESTART(1)

    identifier_ = new Identifier();
    identifier_->generate();
    expr_ = new Expr();
    expr_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();

    GENERATEEND
}

IR *EchoExprList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(echo_expr_list_);
    auto tmp2 = SAFETRANSLATE(echo_expr_);
    res = new IR(kEchoExprList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(echo_expr_);
    res = new IR(kEchoExprList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void EchoExprList::deep_delete() {
    SAFEDELETE(echo_expr_list_);
    SAFEDELETE(echo_expr_);
    delete this;
};

void EchoExprList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    echo_expr_list_ = new EchoExprList();
    echo_expr_list_->generate();
    echo_expr_ = new EchoExpr();
    echo_expr_->generate();
    CASEEND
    CASESTART(1)
    echo_expr_ = new EchoExpr();
    echo_expr_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        echo_expr_ = new EchoExpr();
        echo_expr_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *EchoExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kEchoExpr, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void EchoExpr::deep_delete() {
    SAFEDELETE(expr_);
    delete this;
};

void EchoExpr::generate() {
    GENERATESTART(1)

    expr_ = new Expr();
    expr_->generate();

    GENERATEEND
}

IR *ForExprs::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kForExprs, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(non_empty_for_exprs_);
    res = new IR(kForExprs, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ForExprs::deep_delete() {
    SAFEDELETE(non_empty_for_exprs_);
    delete this;
};

void ForExprs::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    non_empty_for_exprs_ = new NonEmptyForExprs();
    non_empty_for_exprs_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NonEmptyForExprs::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(non_empty_for_exprs_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kNonEmptyForExprs, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kNonEmptyForExprs, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NonEmptyForExprs::deep_delete() {
    SAFEDELETE(non_empty_for_exprs_);
    SAFEDELETE(expr_);
    delete this;
};

void NonEmptyForExprs::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    non_empty_for_exprs_ = new NonEmptyForExprs();
    non_empty_for_exprs_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        expr_ = new Expr();
        expr_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *AnonymousClass::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(ctor_arguments_);
    auto tmp2 = SAFETRANSLATE(extends_from_);
    auto tmp3 = SAFETRANSLATE(implements_list_);
    auto tmp4 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp5 = SAFETRANSLATE(class_body_);
    auto tmp6 = new IR(kUnknown, OP3("class", "", ""), tmp1, tmp2);
    PUSH(tmp6);
    auto tmp7 = new IR(kUnknown, OP3("", "", ""), tmp6, tmp3);
    PUSH(tmp7);
    auto tmp8 = new IR(kUnknown, OP3("", "", ""), tmp7, tmp4);
    PUSH(tmp8);
    res = new IR(kAnonymousClass, OP3("", "", ""), tmp8, tmp5);

    TRANSLATEEND
}

void AnonymousClass::deep_delete() {
    SAFEDELETE(extends_from_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(class_body_);
    SAFEDELETE(implements_list_);
    SAFEDELETE(ctor_arguments_);
    delete this;
};

void AnonymousClass::generate() {
    GENERATESTART(1)

    ctor_arguments_ = new CtorArguments();
    ctor_arguments_->generate();
    extends_from_ = new ExtendsFrom();
    extends_from_->generate();
    implements_list_ = new ImplementsList();
    implements_list_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    class_body_ = new ClassBody();
    class_body_->generate();

    GENERATEEND
}

IR *NewExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_constructor_);
    res = new IR(kNewExpr, OP3("new", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(anonymous_class_);
    res = new IR(kNewExpr, OP3("new", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NewExpr::deep_delete() {
    SAFEDELETE(class_constructor_);
    SAFEDELETE(anonymous_class_);
    delete this;
};

void NewExpr::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    class_constructor_ = new ClassConstructor();
    class_constructor_->generate();
    CASEEND
    CASESTART(1)
    anonymous_class_ = new AnonymousClass();
    anonymous_class_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassConstructor::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_name_reference_);
    res = new IR(kClassConstructor, OP3("", "( )", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_reference_);
    auto tmp2 = SAFETRANSLATE(non_empty_argument_list_);
    auto tmp3 = SAFETRANSLATE(possible_comma_);
    auto tmp4 = new IR(kUnknown, OP3("", "(", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kClassConstructor, OP3("", "", ")"), tmp4, tmp3);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(class_name_reference_);
    res = new IR(kClassConstructor, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassConstructor::deep_delete() {
    SAFEDELETE(class_name_reference_);
    SAFEDELETE(non_empty_argument_list_);
    SAFEDELETE(possible_comma_);
    delete this;
};

void ClassConstructor::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    class_name_reference_ = new ClassNameReference();
    class_name_reference_->generate();
    CASEEND
    CASESTART(1)
    class_name_reference_ = new ClassNameReference();
    class_name_reference_->generate();
    non_empty_argument_list_ = new NonEmptyArgumentList();
    non_empty_argument_list_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    CASESTART(2)
    class_name_reference_ = new ClassNameReference();
    class_name_reference_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *MightDefineExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(variable_1_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kMightDefineExpr, OP3("", "=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(variable_1_);
    auto tmp2 = SAFETRANSLATE(variable_2_);
    res = new IR(kMightDefineExpr, OP3("", "= &", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MightDefineExpr::deep_delete() {
    SAFEDELETE(variable_1_);
    SAFEDELETE(variable_2_);
    SAFEDELETE(expr_);
    delete this;
};

void MightDefineExpr::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    variable_1_ = new Variable();
    variable_1_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(1)
    variable_1_ = new Variable();
    variable_1_->generate();
    variable_2_ = new Variable();
    variable_2_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Expr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("list (", ") =", ""), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("[", "] =", ""), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("clone", "", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(might_define_expr_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "+=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "-=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(7)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "*=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(8)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "**=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(9)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "/=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(10)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", ".=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(11)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "%=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(12)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "&=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(13)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "|=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(14)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "^=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(15)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "<<=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(16)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", ">>=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(17)
    auto tmp1 = SAFETRANSLATE(variable_);
    auto tmp2 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("", "??=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(18)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kExpr, OP3("", "++", ""), tmp1);
    CASEEND
    CASESTART(19)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kExpr, OP3("++", "", ""), tmp1);
    CASEEND
    CASESTART(20)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kExpr, OP3("", "--", ""), tmp1);
    CASEEND
    CASESTART(21)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kExpr, OP3("--", "", ""), tmp1);
    CASEEND
    CASESTART(22)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "||", ""), tmp1, tmp2);
    CASEEND
    CASESTART(23)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "&&", ""), tmp1, tmp2);
    CASEEND
    CASESTART(24)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "or", ""), tmp1, tmp2);
    CASEEND
    CASESTART(25)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "and", ""), tmp1, tmp2);
    CASEEND
    CASESTART(26)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "xor", ""), tmp1, tmp2);
    CASEEND
    CASESTART(27)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "|", ""), tmp1, tmp2);
    CASEEND
    CASESTART(28)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "&", ""), tmp1, tmp2);
    CASEEND
    CASESTART(29)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "^", ""), tmp1, tmp2);
    CASEEND
    CASESTART(30)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", ".", ""), tmp1, tmp2);
    CASEEND
    CASESTART(31)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "+", ""), tmp1, tmp2);
    CASEEND
    CASESTART(32)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "-", ""), tmp1, tmp2);
    CASEEND
    CASESTART(33)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "*", ""), tmp1, tmp2);
    CASEEND
    CASESTART(34)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "**", ""), tmp1, tmp2);
    CASEEND
    CASESTART(35)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "/", ""), tmp1, tmp2);
    CASEEND
    CASESTART(36)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "%", ""), tmp1, tmp2);
    CASEEND
    CASESTART(37)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "<<", ""), tmp1, tmp2);
    CASEEND
    CASESTART(38)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", ">>", ""), tmp1, tmp2);
    CASEEND
    CASESTART(39)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("+", "", ""), tmp1);
    CASEEND
    CASESTART(40)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("-", "", ""), tmp1);
    CASEEND
    CASESTART(41)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("!", "", ""), tmp1);
    CASEEND
    CASESTART(42)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("~", "", ""), tmp1);
    CASEEND
    CASESTART(43)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "===", ""), tmp1, tmp2);
    CASEEND
    CASESTART(44)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "!==", ""), tmp1, tmp2);
    CASEEND
    CASESTART(45)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "==", ""), tmp1, tmp2);
    CASEEND
    CASESTART(46)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "!=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(47)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "<>", ""), tmp1, tmp2);
    CASEEND
    CASESTART(48)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "<", ""), tmp1, tmp2);
    CASEEND
    CASESTART(49)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "<=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(50)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", ">", ""), tmp1, tmp2);
    CASEEND
    CASESTART(51)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", ">=", ""), tmp1, tmp2);
    CASEEND
    CASESTART(52)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "<=>", ""), tmp1, tmp2);
    CASEEND
    CASESTART(53)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(class_name_reference_);
    res = new IR(kExpr, OP3("", "instanceof", ""), tmp1, tmp2);
    CASEEND
    CASESTART(54)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(", ")", ""), tmp1);
    CASEEND
    CASESTART(55)
    auto tmp1 = SAFETRANSLATE(new_expr_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(56)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    auto tmp3 = SAFETRANSLATE(expr_3_);
    auto tmp4 = new IR(kUnknown, OP3("", "?", ":"), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kExpr, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(57)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "? :", ""), tmp1, tmp2);
    CASEEND
    CASESTART(58)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("", "??", ""), tmp1, tmp2);
    CASEEND
    CASESTART(59)
    auto tmp1 = SAFETRANSLATE(internal_functions_in_yacc_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(60)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(int)", "", ""), tmp1);
    CASEEND
    CASESTART(61)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(double)", "", ""), tmp1);
    CASEEND
    CASESTART(62)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(string)", "", ""), tmp1);
    CASEEND
    CASESTART(63)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(array)", "", ""), tmp1);
    CASEEND
    CASESTART(64)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(object)", "", ""), tmp1);
    CASEEND
    CASESTART(65)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(bool)", "", ""), tmp1);
    CASEEND
    CASESTART(66)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("(unset)", "", ""), tmp1);
    CASEEND
    CASESTART(67)
    auto tmp1 = SAFETRANSLATE(exit_expr_);
    res = new IR(kExpr, OP3("exit", "", ""), tmp1);
    CASEEND
    CASESTART(68)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("@", "", ""), tmp1);
    CASEEND
    CASESTART(69)
    auto tmp1 = SAFETRANSLATE(scalar_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(70)
    auto tmp1 = SAFETRANSLATE(backticks_expr_);
    res = new IR(kExpr, OP3("`", "`", ""), tmp1);
    CASEEND
    CASESTART(71)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("print", "", ""), tmp1);
    CASEEND
    CASESTART(72)
    res = new IR(kExpr, OP3("yield", "", ""));
    CASEEND
    CASESTART(73)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("yield", "", ""), tmp1);
    CASEEND
    CASESTART(74)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kExpr, OP3("yield", "=>", ""), tmp1, tmp2);
    CASEEND
    CASESTART(75)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kExpr, OP3("yield from", "", ""), tmp1);
    CASEEND
    CASESTART(76)
    auto tmp1 = SAFETRANSLATE(inline_function_);
    res = new IR(kExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(77)
    auto tmp1 = SAFETRANSLATE(inline_function_);
    res = new IR(kExpr, OP3("static", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Expr::deep_delete() {
    SAFEDELETE(new_expr_);
    SAFEDELETE(inline_function_);
    SAFEDELETE(class_name_reference_);
    SAFEDELETE(expr_1_);
    SAFEDELETE(expr_2_);
    SAFEDELETE(expr_3_);
    SAFEDELETE(exit_expr_);
    SAFEDELETE(backticks_expr_);
    SAFEDELETE(scalar_);
    SAFEDELETE(internal_functions_in_yacc_);
    SAFEDELETE(variable_);
    SAFEDELETE(might_define_expr_);
    SAFEDELETE(array_pair_list_);
    delete this;
};

void Expr::generate() {
    GENERATESTART(7800)

    SWITCHSTART
    CASESTART(0)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(1)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(2)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(3)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(4)
    might_define_expr_ = new MightDefineExpr();
    might_define_expr_->generate();
    CASEEND
    CASESTART(5)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(6)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(7)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(8)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(9)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(10)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(11)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(12)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(13)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(14)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(15)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(16)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(17)
    variable_ = new Variable();
    variable_->generate();
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(18)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(19)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(20)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(21)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(22)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(23)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(24)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(25)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(26)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(27)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(28)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(29)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(30)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(31)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(32)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(33)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(34)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(35)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(36)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(37)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(38)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(39)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(40)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(41)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(42)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(43)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(44)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(45)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(46)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(47)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(48)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(49)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(50)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(51)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(52)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(53)
    expr_1_ = new Expr();
    expr_1_->generate();
    class_name_reference_ = new ClassNameReference();
    class_name_reference_->generate();
    CASEEND
    CASESTART(54)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(55)
    new_expr_ = new NewExpr();
    new_expr_->generate();
    CASEEND
    CASESTART(56)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    expr_3_ = new Expr();
    expr_3_->generate();
    CASEEND
    CASESTART(57)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(58)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(59)
    internal_functions_in_yacc_ = new InternalFunctionsInYacc();
    internal_functions_in_yacc_->generate();
    CASEEND
    CASESTART(60)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(61)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(62)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(63)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(64)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(65)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(66)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(67)
    exit_expr_ = new ExitExpr();
    exit_expr_->generate();
    CASEEND
    CASESTART(68)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(69)
    scalar_ = new Scalar();
    scalar_->generate();
    CASEEND
    CASESTART(70)
    backticks_expr_ = new BackticksExpr();
    backticks_expr_->generate();
    CASEEND
    CASESTART(71)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(72)
    CASEEND
    CASESTART(73)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(74)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(75)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(76)
    inline_function_ = new InlineFunction();
    inline_function_->generate();
    CASEEND
    CASESTART(77)
    inline_function_ = new InlineFunction();
    inline_function_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 14;
    switch (tmp_case_idx) {
        CASESTART(0)
        variable_ = new Variable();
        variable_->generate();
        case_idx_ = 0;
        CASEEND
        CASESTART(1)
        might_define_expr_ = new MightDefineExpr();
        might_define_expr_->generate();
        case_idx_ = 4;
        CASEEND
        CASESTART(2)
        variable_ = new Variable();
        variable_->generate();
        case_idx_ = 18;
        CASEEND
        CASESTART(3)
        variable_ = new Variable();
        variable_->generate();
        case_idx_ = 19;
        CASEEND
        CASESTART(4)
        variable_ = new Variable();
        variable_->generate();
        case_idx_ = 20;
        CASEEND
        CASESTART(5)
        variable_ = new Variable();
        variable_->generate();
        case_idx_ = 21;
        CASEEND
        CASESTART(6)
        new_expr_ = new NewExpr();
        new_expr_->generate();
        case_idx_ = 55;
        CASEEND
        CASESTART(7)
        internal_functions_in_yacc_ = new InternalFunctionsInYacc();
        internal_functions_in_yacc_->generate();
        case_idx_ = 59;
        CASEEND
        CASESTART(8)
        exit_expr_ = new ExitExpr();
        exit_expr_->generate();
        case_idx_ = 67;
        CASEEND
        CASESTART(9)
        scalar_ = new Scalar();
        scalar_->generate();
        case_idx_ = 69;
        CASEEND
        CASESTART(10)
        backticks_expr_ = new BackticksExpr();
        backticks_expr_->generate();
        case_idx_ = 70;
        CASEEND
        CASESTART(11)
        case_idx_ = 72;
        CASEEND
        CASESTART(12)
        inline_function_ = new InlineFunction();
        inline_function_->generate();
        case_idx_ = 76;
        CASEEND
        CASESTART(13)
        inline_function_ = new InlineFunction();
        inline_function_->generate();
        case_idx_ = 77;
        CASEEND
    }
}
}

GENERATEEND
}

IR *InlineFunction::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(function_);
    auto tmp2 = SAFETRANSLATE(returns_ref_);
    auto tmp3 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp4 = SAFETRANSLATE(parameter_list_);
    auto tmp5 = SAFETRANSLATE(lexical_vars_);
    auto tmp6 = SAFETRANSLATE(return_type_);
    auto tmp7 = SAFETRANSLATE(backup_fn_flags_1_);
    auto tmp8 = SAFETRANSLATE(inner_statement_list_);
    auto tmp9 = SAFETRANSLATE(backup_fn_flags_2_);
    auto tmp10 = new IR(kUnknown, OP3("", "", ""), tmp1, tmp2);
    PUSH(tmp10);
    auto tmp11 = new IR(kUnknown, OP3("", "", "("), tmp10, tmp3);
    PUSH(tmp11);
    auto tmp12 = new IR(kUnknown, OP3("", "", ")"), tmp11, tmp4);
    PUSH(tmp12);
    auto tmp13 = new IR(kUnknown, OP3("", "", ""), tmp12, tmp5);
    PUSH(tmp13);
    auto tmp14 = new IR(kUnknown, OP3("", "", ""), tmp13, tmp6);
    PUSH(tmp14);
    auto tmp15 = new IR(kUnknown, OP3("", "", "{"), tmp14, tmp7);
    PUSH(tmp15);
    auto tmp16 = new IR(kUnknown, OP3("", "", "}"), tmp15, tmp8);
    PUSH(tmp16);
    res = new IR(kInlineFunction, OP3("", "", ""), tmp16, tmp9);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(fn_);
    auto tmp2 = SAFETRANSLATE(returns_ref_);
    auto tmp3 = SAFETRANSLATE(parameter_list_);
    auto tmp4 = SAFETRANSLATE(return_type_);
    auto tmp5 = SAFETRANSLATE(backup_doc_comment_);
    auto tmp6 = SAFETRANSLATE(backup_fn_flags_1_);
    auto tmp7 = SAFETRANSLATE(backup_lex_pos_);
    auto tmp8 = SAFETRANSLATE(expr_);
    auto tmp9 = new IR(kUnknown, OP3("", "", "("), tmp1, tmp2);
    PUSH(tmp9);
    auto tmp10 = new IR(kUnknown, OP3("", "", ")"), tmp9, tmp3);
    PUSH(tmp10);
    auto tmp11 = new IR(kUnknown, OP3("", "", ""), tmp10, tmp4);
    PUSH(tmp11);
    auto tmp12 = new IR(kUnknown, OP3("", "", "=>"), tmp11, tmp5);
    PUSH(tmp12);
    auto tmp13 = new IR(kUnknown, OP3("", "", ""), tmp12, tmp6);
    PUSH(tmp13);
    auto tmp14 = new IR(kUnknown, OP3("", "", ""), tmp13, tmp7);
    PUSH(tmp14);
    res = new IR(kInlineFunction, OP3("", "", ""), tmp14, tmp8);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InlineFunction::deep_delete() {
    SAFEDELETE(function_);
    SAFEDELETE(parameter_list_);
    SAFEDELETE(expr_);
    SAFEDELETE(backup_doc_comment_);
    SAFEDELETE(backup_fn_flags_1_);
    SAFEDELETE(backup_fn_flags_2_);
    SAFEDELETE(returns_ref_);
    SAFEDELETE(lexical_vars_);
    SAFEDELETE(return_type_);
    SAFEDELETE(inner_statement_list_);
    SAFEDELETE(fn_);
    SAFEDELETE(backup_lex_pos_);
    delete this;
};

void InlineFunction::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    function_ = new Function();
    function_->generate();
    returns_ref_ = new ReturnsRef();
    returns_ref_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    parameter_list_ = new ParameterList();
    parameter_list_->generate();
    lexical_vars_ = new LexicalVars();
    lexical_vars_->generate();
    return_type_ = new ReturnType();
    return_type_->generate();
    backup_fn_flags_1_ = new BackupFnFlags();
    backup_fn_flags_1_->generate();
    inner_statement_list_ = new InnerStatementList();
    inner_statement_list_->generate();
    backup_fn_flags_2_ = new BackupFnFlags();
    backup_fn_flags_2_->generate();
    CASEEND
    CASESTART(1)
    fn_ = new Fn();
    fn_->generate();
    returns_ref_ = new ReturnsRef();
    returns_ref_->generate();
    parameter_list_ = new ParameterList();
    parameter_list_->generate();
    return_type_ = new ReturnType();
    return_type_->generate();
    backup_doc_comment_ = new BackupDocComment();
    backup_doc_comment_->generate();
    backup_fn_flags_1_ = new BackupFnFlags();
    backup_fn_flags_1_->generate();
    backup_lex_pos_ = new BackupLexPos();
    backup_lex_pos_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Fn::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kFn, OP3("fn", "", ""));

    TRANSLATEEND
}

void Fn::deep_delete() { delete this; };

void Fn::generate(){GENERATESTART(1)

                        GENERATEEND}

IR *Function::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kFunction, OP3("function", "", ""));

    TRANSLATEEND
}

void Function::deep_delete() { delete this; };

void Function::generate(){GENERATESTART(1)

                              GENERATEEND}

IR *BackupDocComment::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kBackupDocComment, string(""));

    TRANSLATEEND
}

void BackupDocComment::deep_delete() { delete this; };

void BackupDocComment::generate(){GENERATESTART(1)

                                      GENERATEEND}

IR *BackupFnFlags::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kBackupFnFlags, string(""));

    TRANSLATEEND
}

void BackupFnFlags::deep_delete() { delete this; };

void BackupFnFlags::generate(){GENERATESTART(1)

                                   GENERATEEND}

IR *BackupLexPos::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kBackupLexPos, string(""));

    TRANSLATEEND
}

void BackupLexPos::deep_delete() { delete this; };

void BackupLexPos::generate(){GENERATESTART(1)

                                  GENERATEEND}

IR *ReturnsRef::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kReturnsRef, string(""));
    CASEEND
    CASESTART(1)
    res = new IR(kReturnsRef, OP3("&", "", ""));
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ReturnsRef::deep_delete() { delete this; };

void ReturnsRef::generate(){GENERATESTART(2)

                                SWITCHSTART CASESTART(0)

                                    CASEEND CASESTART(1) CASEEND SWITCHEND

                                        GENERATEEND}

IR *LexicalVars::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kLexicalVars, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(lexical_var_list_);
    res = new IR(kLexicalVars, OP3("use (", ")", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void LexicalVars::deep_delete() {
    SAFEDELETE(lexical_var_list_);
    delete this;
};

void LexicalVars::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    lexical_var_list_ = new LexicalVarList();
    lexical_var_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *LexicalVarList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(lexical_var_list_);
    auto tmp2 = SAFETRANSLATE(lexical_var_);
    res = new IR(kLexicalVarList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(lexical_var_);
    res = new IR(kLexicalVarList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void LexicalVarList::deep_delete() {
    SAFEDELETE(lexical_var_list_);
    SAFEDELETE(lexical_var_);
    delete this;
};

void LexicalVarList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    lexical_var_list_ = new LexicalVarList();
    lexical_var_list_->generate();
    lexical_var_ = new LexicalVar();
    lexical_var_->generate();
    CASEEND
    CASESTART(1)
    lexical_var_ = new LexicalVar();
    lexical_var_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        lexical_var_ = new LexicalVar();
        lexical_var_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *LexicalVar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kLexicalVar, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kLexicalVar, OP3("&", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void LexicalVar::deep_delete() {
    SAFEDELETE(dollar_variable_);
    delete this;
};

void LexicalVar::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    CASESTART(1)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *FunctionCallme::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(name_);
    auto tmp2 = SAFETRANSLATE(argument_list_);
    res = new IR(kFunctionCallme, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_);
    auto tmp2 = SAFETRANSLATE(member_name_);
    auto tmp3 = SAFETRANSLATE(argument_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "::", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kFunctionCallme, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(variable_class_name_);
    auto tmp2 = SAFETRANSLATE(member_name_);
    auto tmp3 = SAFETRANSLATE(argument_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "::", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kFunctionCallme, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(callable_expr_);
    auto tmp2 = SAFETRANSLATE(argument_list_);
    res = new IR(kFunctionCallme, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void FunctionCallme::deep_delete() {
    SAFEDELETE(name_);
    SAFEDELETE(class_name_);
    SAFEDELETE(variable_class_name_);
    SAFEDELETE(callable_expr_);
    SAFEDELETE(argument_list_);
    SAFEDELETE(member_name_);
    delete this;
};

void FunctionCallme::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    name_ = new Name();
    name_->generate();
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    CASESTART(1)
    class_name_ = new ClassName();
    class_name_->generate();
    member_name_ = new MemberName();
    member_name_->generate();
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    CASESTART(2)
    variable_class_name_ = new VariableClassName();
    variable_class_name_->generate();
    member_name_ = new MemberName();
    member_name_->generate();
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    CASESTART(3)
    callable_expr_ = new CallableExpr();
    callable_expr_->generate();
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassName::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kClassName, OP3("static", "", ""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(name_);
    res = new IR(kClassName, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassName::deep_delete() {
    SAFEDELETE(name_);
    delete this;
};

void ClassName::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    CASEEND
    CASESTART(1)
    name_ = new Name();
    name_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ClassNameReference::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_name_);
    res = new IR(kClassNameReference, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(new_variable_);
    res = new IR(kClassNameReference, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ClassNameReference::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(new_variable_);
    delete this;
};

void ClassNameReference::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    class_name_ = new ClassName();
    class_name_->generate();
    CASEEND
    CASESTART(1)
    new_variable_ = new NewVariable();
    new_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ExitExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kExitExpr, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(optional_expr_);
    res = new IR(kExitExpr, OP3("(", ")", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ExitExpr::deep_delete() {
    SAFEDELETE(optional_expr_);
    delete this;
};

void ExitExpr::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *BackticksExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kBackticksExpr, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(encaps_list_);
    res = new IR(kBackticksExpr, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void BackticksExpr::deep_delete() {
    SAFEDELETE(encaps_list_);
    delete this;
};

void BackticksExpr::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    encaps_list_ = new EncapsList();
    encaps_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *CtorArguments::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kCtorArguments, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(argument_list_);
    res = new IR(kCtorArguments, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CtorArguments::deep_delete() {
    SAFEDELETE(argument_list_);
    delete this;
};

void CtorArguments::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *DereferencableScalar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kDereferencableScalar, OP3("array (", ")", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kDereferencableScalar, OP3("[", "]", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(string_literal_);
    res = new IR(kDereferencableScalar, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void DereferencableScalar::deep_delete() {
    SAFEDELETE(string_literal_);
    SAFEDELETE(array_pair_list_);
    delete this;
};

void DereferencableScalar::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    CASESTART(1)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    CASESTART(2)
    string_literal_ = new StringLiteral();
    string_literal_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Scalar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(num_literal_);
    res = new IR(kScalar, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    res = new IR(kScalar, OP3("__LINE__", "", ""));
    CASEEND
    CASESTART(2)
    res = new IR(kScalar, OP3("__FILE__", "", ""));
    CASEEND
    CASESTART(3)
    res = new IR(kScalar, OP3("__DIR__", "", ""));
    CASEEND
    CASESTART(4)
    res = new IR(kScalar, OP3("__TRAIT__", "", ""));
    CASEEND
    CASESTART(5)
    res = new IR(kScalar, OP3("__METHOD__", "", ""));
    CASEEND
    CASESTART(6)
    res = new IR(kScalar, OP3("__FUNCTION__", "", ""));
    CASEEND
    CASESTART(7)
    res = new IR(kScalar, OP3("__NAMESPACE__", "", ""));
    CASEEND
    CASESTART(8)
    res = new IR(kScalar, OP3("__CLASS__", "", ""));
    CASEEND
    CASESTART(9)
    res = new IR(kScalar, OP3("heredoc start heredoc end", "", ""));
    CASEEND
    CASESTART(10)
    auto tmp1 = SAFETRANSLATE(encaps_list_);
    res = new IR(kScalar, OP3("\"", "\"", ""), tmp1);
    CASEEND
    CASESTART(11)
    auto tmp1 = SAFETRANSLATE(encaps_list_);
    res = new IR(kScalar, OP3("heredoc start", "heredoc end", ""), tmp1);
    CASEEND
    CASESTART(12)
    auto tmp1 = SAFETRANSLATE(dereferencable_scalar_);
    res = new IR(kScalar, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(13)
    auto tmp1 = SAFETRANSLATE(constant_);
    res = new IR(kScalar, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Scalar::deep_delete() {
    SAFEDELETE(encaps_list_);
    SAFEDELETE(dereferencable_scalar_);
    SAFEDELETE(constant_);
    SAFEDELETE(num_literal_);
    delete this;
};

void Scalar::generate() {
    GENERATESTART(14)

    SWITCHSTART
    CASESTART(0)
    num_literal_ = new NumLiteral();
    num_literal_->generate();
    CASEEND
    CASESTART(1)
    CASEEND
    CASESTART(2)
    CASEEND
    CASESTART(3)
    CASEEND
    CASESTART(4)
    CASEEND
    CASESTART(5)
    CASEEND
    CASESTART(6)
    CASEEND
    CASESTART(7)
    CASEEND
    CASESTART(8)
    CASEEND
    CASESTART(9)
    CASEEND
    CASESTART(10)
    encaps_list_ = new EncapsList();
    encaps_list_->generate();
    CASEEND
    CASESTART(11)
    encaps_list_ = new EncapsList();
    encaps_list_->generate();
    CASEEND
    CASESTART(12)
    dereferencable_scalar_ = new DereferencableScalar();
    dereferencable_scalar_->generate();
    CASEEND
    CASESTART(13)
    constant_ = new Constant();
    constant_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Constant::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(name_);
    res = new IR(kConstant, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(class_name_);
    auto tmp2 = SAFETRANSLATE(identifier_plus_);
    res = new IR(kConstant, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(variable_class_name_);
    auto tmp2 = SAFETRANSLATE(identifier_plus_);
    res = new IR(kConstant, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Constant::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(variable_class_name_);
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(name_);
    delete this;
};

void Constant::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    name_ = new Name();
    name_->generate();
    CASEEND
    CASESTART(1)
    class_name_ = new ClassName();
    class_name_->generate();
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    CASEEND
    CASESTART(2)
    variable_class_name_ = new VariableClassName();
    variable_class_name_->generate();
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *OptionalExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kOptionalExpr, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kOptionalExpr, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void OptionalExpr::deep_delete() {
    SAFEDELETE(expr_);
    delete this;
};

void OptionalExpr::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *VariableClassName::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(dereferencable_);
    res = new IR(kVariableClassName, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void VariableClassName::deep_delete() {
    SAFEDELETE(dereferencable_);
    delete this;
};

void VariableClassName::generate() {
    GENERATESTART(1)

    dereferencable_ = new Dereferencable();
    dereferencable_->generate();

    GENERATEEND
}

IR *Dereferencable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kDereferencable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kDereferencable, OP3("(", ")", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(dereferencable_scalar_);
    res = new IR(kDereferencable, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Dereferencable::deep_delete() {
    SAFEDELETE(variable_);
    SAFEDELETE(expr_);
    SAFEDELETE(dereferencable_scalar_);
    delete this;
};

void Dereferencable::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    dereferencable_scalar_ = new DereferencableScalar();
    dereferencable_scalar_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *CallableExpr::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(callable_variable_);
    res = new IR(kCallableExpr, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kCallableExpr, OP3("(", ")", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(dereferencable_scalar_);
    res = new IR(kCallableExpr, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CallableExpr::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(callable_variable_);
    SAFEDELETE(dereferencable_scalar_);
    delete this;
};

void CallableExpr::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    callable_variable_ = new CallableVariable();
    callable_variable_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    dereferencable_scalar_ = new DereferencableScalar();
    dereferencable_scalar_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *CallableVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kCallableVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(dereferencable_);
    auto tmp2 = SAFETRANSLATE(optional_expr_);
    res = new IR(kCallableVariable, OP3("", "[", "]"), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(constant_);
    auto tmp2 = SAFETRANSLATE(optional_expr_);
    res = new IR(kCallableVariable, OP3("", "[", "]"), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(dereferencable_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kCallableVariable, OP3("", "{", "}"), tmp1, tmp2);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(dereferencable_);
    auto tmp2 = SAFETRANSLATE(property_name_);
    auto tmp3 = SAFETRANSLATE(argument_list_);
    auto tmp4 = new IR(kUnknown, OP3("", "->", ""), tmp1, tmp2);
    PUSH(tmp4);
    res = new IR(kCallableVariable, OP3("", "", ""), tmp4, tmp3);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(function_callme_);
    res = new IR(kCallableVariable, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void CallableVariable::deep_delete() {
    SAFEDELETE(constant_);
    SAFEDELETE(function_callme_);
    SAFEDELETE(expr_);
    SAFEDELETE(simple_variable_);
    SAFEDELETE(optional_expr_);
    SAFEDELETE(argument_list_);
    SAFEDELETE(dereferencable_);
    SAFEDELETE(property_name_);
    delete this;
};

void CallableVariable::generate() {
    GENERATESTART(6)

    SWITCHSTART
    CASESTART(0)
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    CASESTART(1)
    dereferencable_ = new Dereferencable();
    dereferencable_->generate();
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(2)
    constant_ = new Constant();
    constant_->generate();
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(3)
    dereferencable_ = new Dereferencable();
    dereferencable_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(4)
    dereferencable_ = new Dereferencable();
    dereferencable_->generate();
    property_name_ = new PropertyName();
    property_name_->generate();
    argument_list_ = new ArgumentList();
    argument_list_->generate();
    CASEEND
    CASESTART(5)
    function_callme_ = new FunctionCallme();
    function_callme_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *Variable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(callable_variable_);
    res = new IR(kVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(static_member_);
    res = new IR(kVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(dereferencable_);
    auto tmp2 = SAFETRANSLATE(property_name_);
    res = new IR(kVariable, OP3("", "->", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void Variable::deep_delete() {
    SAFEDELETE(dereferencable_);
    SAFEDELETE(property_name_);
    SAFEDELETE(static_member_);
    SAFEDELETE(callable_variable_);
    delete this;
};

void Variable::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    callable_variable_ = new CallableVariable();
    callable_variable_->generate();
    CASEEND
    CASESTART(1)
    static_member_ = new StaticMember();
    static_member_->generate();
    CASEEND
    CASESTART(2)
    dereferencable_ = new Dereferencable();
    dereferencable_->generate();
    property_name_ = new PropertyName();
    property_name_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *SimpleVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kSimpleVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kSimpleVariable, OP3("OP_DOLLAR {", "}", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kSimpleVariable, OP3("OP_DOLLAR", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void SimpleVariable::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(simple_variable_);
    delete this;
};

void SimpleVariable::generate() {
    GENERATESTART(300)

    SWITCHSTART
    CASESTART(0)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 2;
    switch (tmp_case_idx) {
        CASESTART(0)
        dollar_variable_ = new DollarVariable();
        dollar_variable_->generate();
        case_idx_ = 0;
        CASEEND
        CASESTART(1)
        expr_ = new Expr();
        expr_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *StaticMember::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(class_name_);
    auto tmp2 = SAFETRANSLATE(simple_variable_);
    res = new IR(kStaticMember, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(variable_class_name_);
    auto tmp2 = SAFETRANSLATE(simple_variable_);
    res = new IR(kStaticMember, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void StaticMember::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(variable_class_name_);
    SAFEDELETE(simple_variable_);
    delete this;
};

void StaticMember::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    class_name_ = new ClassName();
    class_name_->generate();
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    CASESTART(1)
    variable_class_name_ = new VariableClassName();
    variable_class_name_->generate();
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NewVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kNewVariable, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(new_variable_);
    auto tmp2 = SAFETRANSLATE(optional_expr_);
    res = new IR(kNewVariable, OP3("", "[", "]"), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(new_variable_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kNewVariable, OP3("", "{", "}"), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(new_variable_);
    auto tmp2 = SAFETRANSLATE(property_name_);
    res = new IR(kNewVariable, OP3("", "->", ""), tmp1, tmp2);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(class_name_);
    auto tmp2 = SAFETRANSLATE(simple_variable_);
    res = new IR(kNewVariable, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(new_variable_);
    auto tmp2 = SAFETRANSLATE(simple_variable_);
    res = new IR(kNewVariable, OP3("", "::", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NewVariable::deep_delete() {
    SAFEDELETE(class_name_);
    SAFEDELETE(new_variable_);
    SAFEDELETE(simple_variable_);
    SAFEDELETE(optional_expr_);
    SAFEDELETE(expr_);
    SAFEDELETE(property_name_);
    delete this;
};

void NewVariable::generate() {
    GENERATESTART(600)

    SWITCHSTART
    CASESTART(0)
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    CASESTART(1)
    new_variable_ = new NewVariable();
    new_variable_->generate();
    optional_expr_ = new OptionalExpr();
    optional_expr_->generate();
    CASEEND
    CASESTART(2)
    new_variable_ = new NewVariable();
    new_variable_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(3)
    new_variable_ = new NewVariable();
    new_variable_->generate();
    property_name_ = new PropertyName();
    property_name_->generate();
    CASEEND
    CASESTART(4)
    class_name_ = new ClassName();
    class_name_->generate();
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    CASESTART(5)
    new_variable_ = new NewVariable();
    new_variable_->generate();
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 2;
    switch (tmp_case_idx) {
        CASESTART(0)
        simple_variable_ = new SimpleVariable();
        simple_variable_->generate();
        case_idx_ = 0;
        CASEEND
        CASESTART(1)
        class_name_ = new ClassName();
        class_name_->generate();
        simple_variable_ = new SimpleVariable();
        simple_variable_->generate();
        case_idx_ = 4;
        CASEEND
    }
}
}

GENERATEEND
}

IR *MemberName::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_plus_);
    res = new IR(kMemberName, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kMemberName, OP3("{", "}", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kMemberName, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void MemberName::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(identifier_plus_);
    SAFEDELETE(simple_variable_);
    delete this;
};

void MemberName::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    identifier_plus_ = new IdentifierPlus();
    identifier_plus_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *PropertyName::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kPropertyName, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kPropertyName, OP3("{", "}", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(simple_variable_);
    res = new IR(kPropertyName, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void PropertyName::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(identifier_);
    SAFEDELETE(simple_variable_);
    delete this;
};

void PropertyName::generate() {
    GENERATESTART(3)

    SWITCHSTART
    CASESTART(0)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    simple_variable_ = new SimpleVariable();
    simple_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *ArrayPairList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(non_empty_array_pair_list_);
    res = new IR(kArrayPairList, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void ArrayPairList::deep_delete() {
    SAFEDELETE(non_empty_array_pair_list_);
    delete this;
};

void ArrayPairList::generate() {
    GENERATESTART(1)

    non_empty_array_pair_list_ = new NonEmptyArrayPairList();
    non_empty_array_pair_list_->generate();

    GENERATEEND
}

IR *PossibleArrayPair::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    res = new IR(kPossibleArrayPair, string(""));
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(array_pair_);
    res = new IR(kPossibleArrayPair, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void PossibleArrayPair::deep_delete() {
    SAFEDELETE(array_pair_);
    delete this;
};

void PossibleArrayPair::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)

    CASEEND
    CASESTART(1)
    array_pair_ = new ArrayPair();
    array_pair_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *NonEmptyArrayPairList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(non_empty_array_pair_list_);
    auto tmp2 = SAFETRANSLATE(possible_array_pair_);
    res = new IR(kNonEmptyArrayPairList, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(possible_array_pair_);
    res = new IR(kNonEmptyArrayPairList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NonEmptyArrayPairList::deep_delete() {
    SAFEDELETE(possible_array_pair_);
    SAFEDELETE(non_empty_array_pair_list_);
    delete this;
};

void NonEmptyArrayPairList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    non_empty_array_pair_list_ = new NonEmptyArrayPairList();
    non_empty_array_pair_list_->generate();
    possible_array_pair_ = new PossibleArrayPair();
    possible_array_pair_->generate();
    CASEEND
    CASESTART(1)
    possible_array_pair_ = new PossibleArrayPair();
    possible_array_pair_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        possible_array_pair_ = new PossibleArrayPair();
        possible_array_pair_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *ArrayPair::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(expr_2_);
    res = new IR(kArrayPair, OP3("", "=>", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kArrayPair, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(variable_);
    res = new IR(kArrayPair, OP3("", "=> &", ""), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kArrayPair, OP3("&", "", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    res = new IR(kArrayPair, OP3("...", "", ""), tmp1);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(expr_1_);
    auto tmp2 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kArrayPair, OP3("", "=> list (", ")"), tmp1, tmp2);
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(array_pair_list_);
    res = new IR(kArrayPair, OP3("list (", ")", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void ArrayPair::deep_delete() {
    SAFEDELETE(variable_);
    SAFEDELETE(expr_1_);
    SAFEDELETE(expr_2_);
    SAFEDELETE(array_pair_list_);
    delete this;
};

void ArrayPair::generate() {
    GENERATESTART(7)

    SWITCHSTART
    CASESTART(0)
    expr_1_ = new Expr();
    expr_1_->generate();
    expr_2_ = new Expr();
    expr_2_->generate();
    CASEEND
    CASESTART(1)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(2)
    expr_1_ = new Expr();
    expr_1_->generate();
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(3)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    CASESTART(4)
    expr_1_ = new Expr();
    expr_1_->generate();
    CASEEND
    CASESTART(5)
    expr_1_ = new Expr();
    expr_1_->generate();
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    CASESTART(6)
    array_pair_list_ = new ArrayPairList();
    array_pair_list_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *EncapsList::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(encaps_list_);
    auto tmp2 = SAFETRANSLATE(encaps_var_);
    res = new IR(kEncapsList, OP3("", "", ""), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(encaps_var_);
    res = new IR(kEncapsList, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void EncapsList::deep_delete() {
    SAFEDELETE(encaps_list_);
    SAFEDELETE(encaps_var_);
    delete this;
};

void EncapsList::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    encaps_list_ = new EncapsList();
    encaps_list_->generate();
    encaps_var_ = new EncapsVar();
    encaps_var_->generate();
    CASEEND
    CASESTART(1)
    encaps_var_ = new EncapsVar();
    encaps_var_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        encaps_var_ = new EncapsVar();
        encaps_var_->generate();
        case_idx_ = 1;
        CASEEND
    }
}
}

GENERATEEND
}

IR *StringVarname::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kStringVarname, string_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void StringVarname::deep_delete() { delete this; };

void StringVarname::generate() {
    GENERATESTART(1)

    string_val_ = gen_string();

    GENERATEEND
}

IR *EncapsVar::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kEncapsVar, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    auto tmp2 = SAFETRANSLATE(encaps_var_offset_);
    res = new IR(kEncapsVar, OP3("", "[", "]"), tmp1, tmp2);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    auto tmp2 = SAFETRANSLATE(identifier_);
    res = new IR(kEncapsVar, OP3("", "->", ""), tmp1, tmp2);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kEncapsVar, OP3("${", "}", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(string_varname_);
    res = new IR(kEncapsVar, OP3("${", "}", ""), tmp1);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(string_varname_);
    auto tmp2 = SAFETRANSLATE(expr_);
    res = new IR(kEncapsVar, OP3("${", "[", "] }"), tmp1, tmp2);
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(variable_);
    res = new IR(kEncapsVar, OP3("{$", "}", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void EncapsVar::deep_delete() {
    SAFEDELETE(expr_);
    SAFEDELETE(string_varname_);
    SAFEDELETE(variable_);
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(identifier_);
    SAFEDELETE(encaps_var_offset_);
    delete this;
};

void EncapsVar::generate() {
    GENERATESTART(7)

    SWITCHSTART
    CASESTART(0)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    CASESTART(1)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    encaps_var_offset_ = new EncapsVarOffset();
    encaps_var_offset_->generate();
    CASEEND
    CASESTART(2)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(3)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(4)
    string_varname_ = new StringVarname();
    string_varname_->generate();
    CASEEND
    CASESTART(5)
    string_varname_ = new StringVarname();
    string_varname_->generate();
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(6)
    variable_ = new Variable();
    variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *EncapsVarOffset::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(identifier_);
    res = new IR(kEncapsVarOffset, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(string_literal_);
    res = new IR(kEncapsVarOffset, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(string_literal_);
    res = new IR(kEncapsVarOffset, OP3("-", "", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(dollar_variable_);
    res = new IR(kEncapsVarOffset, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void EncapsVarOffset::deep_delete() {
    SAFEDELETE(dollar_variable_);
    SAFEDELETE(identifier_);
    SAFEDELETE(string_literal_);
    delete this;
};

void EncapsVarOffset::generate() {
    GENERATESTART(4)

    SWITCHSTART
    CASESTART(0)
    identifier_ = new Identifier();
    identifier_->generate();
    CASEEND
    CASESTART(1)
    string_literal_ = new StringLiteral();
    string_literal_->generate();
    CASEEND
    CASESTART(2)
    string_literal_ = new StringLiteral();
    string_literal_->generate();
    CASEEND
    CASESTART(3)
    dollar_variable_ = new DollarVariable();
    dollar_variable_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *InternalFunctionsInYacc::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(isset_variables_);
    auto tmp2 = SAFETRANSLATE(possible_comma_);
    res = new IR(kInternalFunctionsInYacc, OP3("isset (", "", ")"), tmp1, tmp2);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("empty (", ")", ""), tmp1);
    CASEEND
    CASESTART(2)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("include", "", ""), tmp1);
    CASEEND
    CASESTART(3)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("include_once", "", ""), tmp1);
    CASEEND
    CASESTART(4)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("eval (", ")", ""), tmp1);
    CASEEND
    CASESTART(5)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("require", "", ""), tmp1);
    CASEEND
    CASESTART(6)
    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kInternalFunctionsInYacc, OP3("require_once", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void InternalFunctionsInYacc::deep_delete() {
    SAFEDELETE(isset_variables_);
    SAFEDELETE(expr_);
    SAFEDELETE(possible_comma_);
    delete this;
};

void InternalFunctionsInYacc::generate() {
    GENERATESTART(7)

    SWITCHSTART
    CASESTART(0)
    isset_variables_ = new IssetVariables();
    isset_variables_->generate();
    possible_comma_ = new PossibleComma();
    possible_comma_->generate();
    CASEEND
    CASESTART(1)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(2)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(3)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(4)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(5)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    CASESTART(6)
    expr_ = new Expr();
    expr_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *IssetVariables::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(isset_variable_);
    res = new IR(kIssetVariables, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(isset_variables_);
    auto tmp2 = SAFETRANSLATE(isset_variable_);
    res = new IR(kIssetVariables, OP3("", ",", ""), tmp1, tmp2);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void IssetVariables::deep_delete() {
    SAFEDELETE(isset_variable_);
    SAFEDELETE(isset_variables_);
    delete this;
};

void IssetVariables::generate() {
    GENERATESTART(200)

    SWITCHSTART
    CASESTART(0)
    isset_variable_ = new IssetVariable();
    isset_variable_->generate();
    CASEEND
    CASESTART(1)
    isset_variables_ = new IssetVariables();
    isset_variables_->generate();
    isset_variable_ = new IssetVariable();
    isset_variable_->generate();
    CASEEND

default: {
    int tmp_case_idx = rand() % 1;
    switch (tmp_case_idx) {
        CASESTART(0)
        isset_variable_ = new IssetVariable();
        isset_variable_->generate();
        case_idx_ = 0;
        CASEEND
    }
}
}

GENERATEEND
}

IR *IssetVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    auto tmp1 = SAFETRANSLATE(expr_);
    res = new IR(kIssetVariable, OP3("", "", ""), tmp1);

    TRANSLATEEND
}

void IssetVariable::deep_delete() {
    SAFEDELETE(expr_);
    delete this;
};

void IssetVariable::generate() {
    GENERATESTART(1)

    expr_ = new Expr();
    expr_->generate();

    GENERATEEND
}

IR *Identifier::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kIdentifier, string_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void Identifier::deep_delete() { delete this; };

void Identifier::generate() {
    GENERATESTART(1)

    string_val_ = gen_string();

    GENERATEEND
}

IR *DollarVariable::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kDollarVariable, string_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void DollarVariable::deep_delete() { delete this; };

void DollarVariable::generate() {
    GENERATESTART(1)

    string_val_ = gen_string();

    GENERATEEND
}

IR *NumLiteral::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    SWITCHSTART
    CASESTART(0)
    auto tmp1 = SAFETRANSLATE(float_literal_);
    res = new IR(kNumLiteral, OP3("", "", ""), tmp1);
    CASEEND
    CASESTART(1)
    auto tmp1 = SAFETRANSLATE(int_literal_);
    res = new IR(kNumLiteral, OP3("", "", ""), tmp1);
    CASEEND
    SWITCHEND

    TRANSLATEEND
}

void NumLiteral::deep_delete() {
    SAFEDELETE(int_literal_);
    SAFEDELETE(float_literal_);
    delete this;
};

void NumLiteral::generate() {
    GENERATESTART(2)

    SWITCHSTART
    CASESTART(0)
    float_literal_ = new FloatLiteral();
    float_literal_->generate();
    CASEEND
    CASESTART(1)
    int_literal_ = new IntLiteral();
    int_literal_->generate();
    CASEEND
    SWITCHEND

    GENERATEEND
}

IR *FloatLiteral::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kFloatLiteral, float_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void FloatLiteral::deep_delete() { delete this; };

void FloatLiteral::generate() {
    GENERATESTART(1)

    float_val_ = gen_float();

    GENERATEEND
}

IR *IntLiteral::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kIntLiteral, int_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void IntLiteral::deep_delete() { delete this; };

void IntLiteral::generate() {
    GENERATESTART(1)

    int_val_ = gen_int();

    GENERATEEND
}

IR *StringLiteral::translate(vector<IR *> &v_ir_collector) {
    TRANSLATESTART

    res = new IR(kStringLiteral, string_val_, data_type_, scope_, data_flag_);

    TRANSLATEEND
}

void StringLiteral::deep_delete() { delete this; };

void StringLiteral::generate() {
    GENERATESTART(1)

    string_val_ = gen_string();

    GENERATEEND
}
