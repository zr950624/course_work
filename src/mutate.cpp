//#include <queue>
#include <algorithm>
#include <assert.h>
#include <cfloat>
#include <climits>
#include <cstdio>
#include <deque>
#include <fcntl.h>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
using namespace std;

#include "../include/ast.h"
#include "../include/define.h"
#include "../include/mutate.h"
#include "../include/utils.h"

#define _NON_REPLACE_

//#define MUTATE_UNKNOWN

#define MUTATE_DBG 0

static inline bool not_unknown(IR *r) { return r->type_ != kUnknown; }

static inline bool is_leaf(IR *r) { return r->left_ == NULL && r->right_ == NULL; }

// Need No fix
IR *Mutator::deep_copy_with_record(const IR *root, const IR *record) {
    IR *left = NULL, *right = NULL, *copy_res;

    if (root->left_)
        left = deep_copy_with_record(root->left_, record); // do you have a second version for deep_copy that accept only one argument?
    if (root->right_)
        right = deep_copy_with_record(root->right_, record); // no I forget to update here

    if (root->op_ != NULL)
        copy_res = new IR(root->type_, OP3(root->op_->prefix_, root->op_->middle_, root->op_->suffix_), left, right, root->float_val_, root->str_val_, root->name_, root->mutated_times_, root->scope_, root->data_flag_);
    else
        copy_res = new IR(root->type_, NULL, left, right, root->float_val_, root->str_val_, root->name_, root->mutated_times_, root->scope_, root->data_flag_);

    copy_res->data_type_ = root->data_type_;

    if (root == record && record != NULL) {
        this->record_ = copy_res;
    }

    return copy_res;
}

// Need No fix
/*
bool Mutator::check_node_num(IR * root, unsigned int limit) const{

    auto v_statements = extract_statement(root);
    bool is_good = true;

    for(auto stmt: v_statements){
        if(calc_node(stmt) > limit){
            is_good = false;
            break;
        }
    }

    return is_good;
}
*/

// Need Minor Fix
vector<IR *> Mutator::mutate_all(vector<IR *> &v_ir_collector) {
    vector<IR *> res;
    set<unsigned long> res_hash;
    IR *root = v_ir_collector[v_ir_collector.size() - 1];

    mutated_root_ = root;

    for (auto ir : v_ir_collector) {
        if (ir == root)
            continue;
// cout << "Checking: " << ir->to_string() << endl;
#ifdef MUTATE_UNKNOWN
        if (not_mutatable_types_.find(ir->type_) != not_mutatable_types_.end() || !can_be_mutated(ir))
            continue;
#else
        if (ir->type_ == kUnknown || not_mutatable_types_.find(ir->type_) != not_mutatable_types_.end() || is_leaf(ir) || !can_be_mutated(ir))
            continue;
#endif

        // cout << "Yes" << endl;
        if (MUTATE_DBG)
            cout << "Mutating type: " << get_string_by_nodetype(ir->type_) << endl;
        vector<IR *> v_mutated_ir = mutate(ir);
        // cout << "this size: " << v_mutated_ir.size() << endl;

        for (auto i : v_mutated_ir) {
            IR *new_ir_tree = deep_copy_with_record(root, ir);
            if (MUTATE_DBG)
                cout << "NEW type: " << get_string_by_nodetype(i->type_) << endl;

#ifndef SYNTAX_ONLY
// remove_definition(i);
#endif

            replace(new_ir_tree, this->record_, i);

            // extract_struct_after_mutation(new_ir_tree);
            IR *backup = deep_copy(new_ir_tree);
            extract_struct(new_ir_tree);
            string tmp = new_ir_tree->to_string();
            // cout << "Mutated: " << backup->to_string();
            // cout << "Strip:" << tmp << endl;
            unsigned tmp_hash = hash(tmp);
            if (res_hash.find(tmp_hash) != res_hash.end()) {
                deep_delete(new_ir_tree);
                deep_delete(backup);
                continue;
            }

            res_hash.insert(tmp_hash);
            deep_delete(new_ir_tree);

            // extract_struct_after_mutation(backup);
            res.push_back(backup);
        }
    }

    return res;
}

void Mutator::add_ir_to_library(IR *cur) {
    extract_struct(cur);

#ifdef SYNTAX_ONLY
    add_ir_to_library_limited(cur);
#else
    cur = deep_copy(cur);
    add_ir_to_library_no_deepcopy(cur);
#endif
    return;
}

void Mutator::add_ir_to_library_limited(IR *cur) {
    auto type = cur->type_;
    auto h = hash(cur);

    if (ir_library_hash_[type].find(h) != ir_library_hash_[type].end()) {
        return;
    }

    if (ir_library_[type].size() >= max_ir_library_size_) {
        auto rand_idx = get_rand_int(ir_library_[type].size());
        auto removed_ir = ir_library_[type][rand_idx];
        ir_library_[type][rand_idx] = ir_library_[type].back();
        ir_library_[type].pop_back();
        auto removed_h = hash(removed_ir);
        deep_delete(removed_ir);
        ir_library_hash_[type].erase(removed_h);
    }

    ir_library_[type].push_back(deep_copy(cur));
    ir_library_hash_[type].insert(h);

    if (cur->left_)
        add_ir_to_library_limited(cur->left_);
    if (cur->right_)
        add_ir_to_library_limited(cur->right_);
}

void Mutator::add_ir_to_library_no_deepcopy(IR *cur) {

    auto type = cur->type_;
    auto h = hash(cur);
    if (find(ir_library_hash_[type].begin(), ir_library_hash_[type].end(), h) != ir_library_hash_[type].end())
        return;

    ir_library_hash_[type].insert(h);
    ir_library_[type].push_back(cur);

    if (cur->left_)
        add_ir_to_library_no_deepcopy(cur->left_);
    if (cur->right_)
        add_ir_to_library_no_deepcopy(cur->right_);

    return;
}

void Mutator::init_convertable_ir_type_map() {
    m_convertable_map_[kProgram].insert(kProgram);
    m_convertable_map_[kReservedNonModifiers].insert(kReservedNonModifiers);
    m_convertable_map_[kReservedNonModifiers].insert(kSemiReserved);
    m_convertable_map_[kReservedNonModifiers].insert(kIdentifierPlus);
    m_convertable_map_[kReservedNonModifiers].insert(kTraitMethodReference);
    m_convertable_map_[kReservedNonModifiers].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kReservedNonModifiers].insert(kSimpleVariable);
    m_convertable_map_[kReservedNonModifiers].insert(kMemberName);
    m_convertable_map_[kReservedNonModifiers].insert(kIdentifier);
    m_convertable_map_[kReservedNonModifiers].insert(kDollarVariable);
    m_convertable_map_[kSemiReserved].insert(kReservedNonModifiers);
    m_convertable_map_[kSemiReserved].insert(kSemiReserved);
    m_convertable_map_[kSemiReserved].insert(kIdentifierPlus);
    m_convertable_map_[kSemiReserved].insert(kTraitMethodReference);
    m_convertable_map_[kSemiReserved].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kSemiReserved].insert(kSimpleVariable);
    m_convertable_map_[kSemiReserved].insert(kMemberName);
    m_convertable_map_[kSemiReserved].insert(kIdentifier);
    m_convertable_map_[kSemiReserved].insert(kDollarVariable);
    m_convertable_map_[kIdentifierPlus].insert(kReservedNonModifiers);
    m_convertable_map_[kIdentifierPlus].insert(kSemiReserved);
    m_convertable_map_[kIdentifierPlus].insert(kIdentifierPlus);
    m_convertable_map_[kIdentifierPlus].insert(kTraitMethodReference);
    m_convertable_map_[kIdentifierPlus].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kIdentifierPlus].insert(kSimpleVariable);
    m_convertable_map_[kIdentifierPlus].insert(kMemberName);
    m_convertable_map_[kIdentifierPlus].insert(kIdentifier);
    m_convertable_map_[kIdentifierPlus].insert(kDollarVariable);
    m_convertable_map_[kTopStatementList].insert(kTopStatementList);
    m_convertable_map_[kNamespaceName].insert(kNamespaceName);
    m_convertable_map_[kNamespaceName].insert(kName);
    m_convertable_map_[kNamespaceName].insert(kInlineUseDeclarations);
    m_convertable_map_[kNamespaceName].insert(kUnprefixedUseDeclarations);
    m_convertable_map_[kNamespaceName].insert(kUseDeclarations);
    m_convertable_map_[kNamespaceName].insert(kInlineUseDeclaration);
    m_convertable_map_[kNamespaceName].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kNamespaceName].insert(kUseDeclaration);
    m_convertable_map_[kNamespaceName].insert(kCatchNameList);
    m_convertable_map_[kNamespaceName].insert(kOptionalType);
    m_convertable_map_[kNamespaceName].insert(kTypeExpr);
    m_convertable_map_[kNamespaceName].insert(kType);
    m_convertable_map_[kNamespaceName].insert(kUnionType2);
    m_convertable_map_[kNamespaceName].insert(kNonEmptyArgumentList);
    m_convertable_map_[kNamespaceName].insert(kArgument);
    m_convertable_map_[kNamespaceName].insert(kClassNameList);
    m_convertable_map_[kNamespaceName].insert(kEchoExprList);
    m_convertable_map_[kNamespaceName].insert(kEchoExpr);
    m_convertable_map_[kNamespaceName].insert(kForExprs);
    m_convertable_map_[kNamespaceName].insert(kNonEmptyForExprs);
    m_convertable_map_[kNamespaceName].insert(kNewExpr);
    m_convertable_map_[kNamespaceName].insert(kClassConstructor);
    m_convertable_map_[kNamespaceName].insert(kMightDefineExpr);
    m_convertable_map_[kNamespaceName].insert(kExpr);
    m_convertable_map_[kNamespaceName].insert(kInlineFunction);
    m_convertable_map_[kNamespaceName].insert(kFunctionCallme);
    m_convertable_map_[kNamespaceName].insert(kClassName);
    m_convertable_map_[kNamespaceName].insert(kClassNameReference);
    m_convertable_map_[kNamespaceName].insert(kDereferencableScalar);
    m_convertable_map_[kNamespaceName].insert(kScalar);
    m_convertable_map_[kNamespaceName].insert(kConstant);
    m_convertable_map_[kNamespaceName].insert(kOptionalExpr);
    m_convertable_map_[kNamespaceName].insert(kCallableVariable);
    m_convertable_map_[kNamespaceName].insert(kVariable);
    m_convertable_map_[kNamespaceName].insert(kSimpleVariable);
    m_convertable_map_[kNamespaceName].insert(kStaticMember);
    m_convertable_map_[kNamespaceName].insert(kNewVariable);
    m_convertable_map_[kNamespaceName].insert(kArrayPairList);
    m_convertable_map_[kNamespaceName].insert(kPossibleArrayPair);
    m_convertable_map_[kNamespaceName].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kNamespaceName].insert(kArrayPair);
    m_convertable_map_[kNamespaceName].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNamespaceName].insert(kIssetVariables);
    m_convertable_map_[kNamespaceName].insert(kIssetVariable);
    m_convertable_map_[kNamespaceName].insert(kIdentifier);
    m_convertable_map_[kNamespaceName].insert(kDollarVariable);
    m_convertable_map_[kNamespaceName].insert(kNumLiteral);
    m_convertable_map_[kNamespaceName].insert(kFloatLiteral);
    m_convertable_map_[kNamespaceName].insert(kIntLiteral);
    m_convertable_map_[kNamespaceName].insert(kStringLiteral);
    m_convertable_map_[kName].insert(kNamespaceName);
    m_convertable_map_[kName].insert(kName);
    m_convertable_map_[kName].insert(kCatchNameList);
    m_convertable_map_[kName].insert(kOptionalType);
    m_convertable_map_[kName].insert(kTypeExpr);
    m_convertable_map_[kName].insert(kType);
    m_convertable_map_[kName].insert(kUnionType2);
    m_convertable_map_[kName].insert(kNonEmptyArgumentList);
    m_convertable_map_[kName].insert(kArgument);
    m_convertable_map_[kName].insert(kClassNameList);
    m_convertable_map_[kName].insert(kEchoExprList);
    m_convertable_map_[kName].insert(kEchoExpr);
    m_convertable_map_[kName].insert(kForExprs);
    m_convertable_map_[kName].insert(kNonEmptyForExprs);
    m_convertable_map_[kName].insert(kNewExpr);
    m_convertable_map_[kName].insert(kClassConstructor);
    m_convertable_map_[kName].insert(kMightDefineExpr);
    m_convertable_map_[kName].insert(kExpr);
    m_convertable_map_[kName].insert(kInlineFunction);
    m_convertable_map_[kName].insert(kFunctionCallme);
    m_convertable_map_[kName].insert(kClassName);
    m_convertable_map_[kName].insert(kClassNameReference);
    m_convertable_map_[kName].insert(kDereferencableScalar);
    m_convertable_map_[kName].insert(kScalar);
    m_convertable_map_[kName].insert(kConstant);
    m_convertable_map_[kName].insert(kOptionalExpr);
    m_convertable_map_[kName].insert(kCallableVariable);
    m_convertable_map_[kName].insert(kVariable);
    m_convertable_map_[kName].insert(kSimpleVariable);
    m_convertable_map_[kName].insert(kStaticMember);
    m_convertable_map_[kName].insert(kNewVariable);
    m_convertable_map_[kName].insert(kArrayPairList);
    m_convertable_map_[kName].insert(kPossibleArrayPair);
    m_convertable_map_[kName].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kName].insert(kArrayPair);
    m_convertable_map_[kName].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kName].insert(kIssetVariables);
    m_convertable_map_[kName].insert(kIssetVariable);
    m_convertable_map_[kName].insert(kIdentifier);
    m_convertable_map_[kName].insert(kDollarVariable);
    m_convertable_map_[kName].insert(kNumLiteral);
    m_convertable_map_[kName].insert(kFloatLiteral);
    m_convertable_map_[kName].insert(kIntLiteral);
    m_convertable_map_[kName].insert(kStringLiteral);
    m_convertable_map_[kTopStatement].insert(kTopStatement);
    m_convertable_map_[kTopStatement].insert(kStatement);
    m_convertable_map_[kTopStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kTopStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kTopStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kTopStatement].insert(kClassDeclaration);
    m_convertable_map_[kTopStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kTopStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kTopStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kTopStatement].insert(kIfStmt);
    m_convertable_map_[kTopStatement].insert(kAltIfStmt);
    m_convertable_map_[kUseType].insert(kUseType);
    m_convertable_map_[kGroupUseDeclaration].insert(kGroupUseDeclaration);
    m_convertable_map_[kMixedGroupUseDeclaration].insert(kMixedGroupUseDeclaration);
    m_convertable_map_[kPossibleComma].insert(kPossibleComma);
    m_convertable_map_[kInlineUseDeclarations].insert(kNamespaceName);
    m_convertable_map_[kInlineUseDeclarations].insert(kInlineUseDeclarations);
    m_convertable_map_[kInlineUseDeclarations].insert(kInlineUseDeclaration);
    m_convertable_map_[kInlineUseDeclarations].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kInlineUseDeclarations].insert(kIdentifier);
    m_convertable_map_[kUnprefixedUseDeclarations].insert(kNamespaceName);
    m_convertable_map_[kUnprefixedUseDeclarations].insert(kUnprefixedUseDeclarations);
    m_convertable_map_[kUnprefixedUseDeclarations].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kUnprefixedUseDeclarations].insert(kIdentifier);
    m_convertable_map_[kUseDeclarations].insert(kNamespaceName);
    m_convertable_map_[kUseDeclarations].insert(kUseDeclarations);
    m_convertable_map_[kUseDeclarations].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kUseDeclarations].insert(kUseDeclaration);
    m_convertable_map_[kUseDeclarations].insert(kIdentifier);
    m_convertable_map_[kInlineUseDeclaration].insert(kNamespaceName);
    m_convertable_map_[kInlineUseDeclaration].insert(kInlineUseDeclarations);
    m_convertable_map_[kInlineUseDeclaration].insert(kInlineUseDeclaration);
    m_convertable_map_[kInlineUseDeclaration].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kInlineUseDeclaration].insert(kIdentifier);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kNamespaceName);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kInlineUseDeclarations);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kUnprefixedUseDeclarations);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kUseDeclarations);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kInlineUseDeclaration);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kUseDeclaration);
    m_convertable_map_[kUnprefixedUseDeclaration].insert(kIdentifier);
    m_convertable_map_[kUseDeclaration].insert(kNamespaceName);
    m_convertable_map_[kUseDeclaration].insert(kUseDeclarations);
    m_convertable_map_[kUseDeclaration].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kUseDeclaration].insert(kUseDeclaration);
    m_convertable_map_[kUseDeclaration].insert(kIdentifier);
    m_convertable_map_[kConstList].insert(kConstList);
    m_convertable_map_[kConstList].insert(kConstDecl);
    m_convertable_map_[kInnerStatementList].insert(kInnerStatementList);
    m_convertable_map_[kInnerStatement].insert(kInnerStatement);
    m_convertable_map_[kInnerStatement].insert(kStatement);
    m_convertable_map_[kInnerStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kInnerStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kInnerStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kInnerStatement].insert(kClassDeclaration);
    m_convertable_map_[kInnerStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kInnerStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kInnerStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kInnerStatement].insert(kIfStmt);
    m_convertable_map_[kInnerStatement].insert(kAltIfStmt);
    m_convertable_map_[kStatement].insert(kTopStatement);
    m_convertable_map_[kStatement].insert(kInnerStatement);
    m_convertable_map_[kStatement].insert(kStatement);
    m_convertable_map_[kStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kStatement].insert(kClassDeclaration);
    m_convertable_map_[kStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kStatement].insert(kForStatement);
    m_convertable_map_[kStatement].insert(kForeachStatement);
    m_convertable_map_[kStatement].insert(kDeclareStatement);
    m_convertable_map_[kStatement].insert(kWhileStatement);
    m_convertable_map_[kStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kStatement].insert(kIfStmt);
    m_convertable_map_[kStatement].insert(kAltIfStmt);
    m_convertable_map_[kCatchList].insert(kCatchList);
    m_convertable_map_[kCatchNameList].insert(kNamespaceName);
    m_convertable_map_[kCatchNameList].insert(kName);
    m_convertable_map_[kCatchNameList].insert(kCatchNameList);
    m_convertable_map_[kCatchNameList].insert(kClassName);
    m_convertable_map_[kCatchNameList].insert(kIdentifier);
    m_convertable_map_[kFinallyStatement].insert(kFinallyStatement);
    m_convertable_map_[kUnsetVariables].insert(kUnsetVariables);
    m_convertable_map_[kUnsetVariables].insert(kUnsetVariable);
    m_convertable_map_[kUnsetVariables].insert(kFunctionCallme);
    m_convertable_map_[kUnsetVariables].insert(kCallableVariable);
    m_convertable_map_[kUnsetVariables].insert(kVariable);
    m_convertable_map_[kUnsetVariables].insert(kSimpleVariable);
    m_convertable_map_[kUnsetVariables].insert(kStaticMember);
    m_convertable_map_[kUnsetVariables].insert(kDollarVariable);
    m_convertable_map_[kUnsetVariable].insert(kUnsetVariables);
    m_convertable_map_[kUnsetVariable].insert(kUnsetVariable);
    m_convertable_map_[kUnsetVariable].insert(kFunctionCallme);
    m_convertable_map_[kUnsetVariable].insert(kCallableVariable);
    m_convertable_map_[kUnsetVariable].insert(kVariable);
    m_convertable_map_[kUnsetVariable].insert(kSimpleVariable);
    m_convertable_map_[kUnsetVariable].insert(kStaticMember);
    m_convertable_map_[kUnsetVariable].insert(kDollarVariable);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kTopStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kInnerStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kClassDeclaration);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kIfStmt);
    m_convertable_map_[kFunctionDeclarationStatement].insert(kAltIfStmt);
    m_convertable_map_[kFunctionDeclaration].insert(kTopStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kInnerStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kFunctionDeclaration);
    m_convertable_map_[kFunctionDeclaration].insert(kClassDeclarationStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kClassDeclaration);
    m_convertable_map_[kFunctionDeclaration].insert(kTraitDeclarationStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kFunctionDeclaration].insert(kIfStmtWithoutElse);
    m_convertable_map_[kFunctionDeclaration].insert(kIfStmt);
    m_convertable_map_[kFunctionDeclaration].insert(kAltIfStmt);
    m_convertable_map_[kIsReference].insert(kIsReference);
    m_convertable_map_[kIsVariadic].insert(kIsVariadic);
    m_convertable_map_[kClassDeclarationStatement].insert(kTopStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kInnerStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kClassDeclarationStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kClassDeclaration);
    m_convertable_map_[kClassDeclarationStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kClassDeclarationStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kClassDeclarationStatement].insert(kIfStmt);
    m_convertable_map_[kClassDeclarationStatement].insert(kAltIfStmt);
    m_convertable_map_[kClassDeclaration].insert(kTopStatement);
    m_convertable_map_[kClassDeclaration].insert(kInnerStatement);
    m_convertable_map_[kClassDeclaration].insert(kStatement);
    m_convertable_map_[kClassDeclaration].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kClassDeclaration].insert(kFunctionDeclaration);
    m_convertable_map_[kClassDeclaration].insert(kClassDeclarationStatement);
    m_convertable_map_[kClassDeclaration].insert(kClassDeclaration);
    m_convertable_map_[kClassDeclaration].insert(kTraitDeclarationStatement);
    m_convertable_map_[kClassDeclaration].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kClassDeclaration].insert(kIfStmtWithoutElse);
    m_convertable_map_[kClassDeclaration].insert(kIfStmt);
    m_convertable_map_[kClassDeclaration].insert(kAltIfStmt);
    m_convertable_map_[kClassBody].insert(kClassBody);
    m_convertable_map_[kClassModifiers].insert(kClassModifiers);
    m_convertable_map_[kClassModifiers].insert(kClassModifier);
    m_convertable_map_[kClassModifier].insert(kClassModifiers);
    m_convertable_map_[kClassModifier].insert(kClassModifier);
    m_convertable_map_[kTraitDeclarationStatement].insert(kTopStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kInnerStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kTraitDeclarationStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kClassDeclaration);
    m_convertable_map_[kTraitDeclarationStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kTraitDeclarationStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kTraitDeclarationStatement].insert(kIfStmt);
    m_convertable_map_[kTraitDeclarationStatement].insert(kAltIfStmt);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kTopStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kInnerStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kFunctionDeclaration);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kClassDeclarationStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kClassDeclaration);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kTraitDeclarationStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kIfStmt);
    m_convertable_map_[kInterfaceDeclarationStatement].insert(kAltIfStmt);
    m_convertable_map_[kExtendsFrom].insert(kExtendsFrom);
    m_convertable_map_[kInterfaceExtendsList].insert(kInterfaceExtendsList);
    m_convertable_map_[kImplementsList].insert(kImplementsList);
    m_convertable_map_[kForeachVariable].insert(kForeachVariable);
    m_convertable_map_[kForeachVariable].insert(kFunctionCallme);
    m_convertable_map_[kForeachVariable].insert(kCallableVariable);
    m_convertable_map_[kForeachVariable].insert(kVariable);
    m_convertable_map_[kForeachVariable].insert(kSimpleVariable);
    m_convertable_map_[kForeachVariable].insert(kStaticMember);
    m_convertable_map_[kForeachVariable].insert(kDollarVariable);
    m_convertable_map_[kForStatement].insert(kStatement);
    m_convertable_map_[kForStatement].insert(kForStatement);
    m_convertable_map_[kForStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kForStatement].insert(kIfStmt);
    m_convertable_map_[kForStatement].insert(kAltIfStmt);
    m_convertable_map_[kForeachStatement].insert(kStatement);
    m_convertable_map_[kForeachStatement].insert(kForeachStatement);
    m_convertable_map_[kForeachStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kForeachStatement].insert(kIfStmt);
    m_convertable_map_[kForeachStatement].insert(kAltIfStmt);
    m_convertable_map_[kDeclareStatement].insert(kStatement);
    m_convertable_map_[kDeclareStatement].insert(kDeclareStatement);
    m_convertable_map_[kDeclareStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kDeclareStatement].insert(kIfStmt);
    m_convertable_map_[kDeclareStatement].insert(kAltIfStmt);
    m_convertable_map_[kSwitchCaseList].insert(kSwitchCaseList);
    m_convertable_map_[kCaseList].insert(kCaseList);
    m_convertable_map_[kCaseSeparator].insert(kCaseSeparator);
    m_convertable_map_[kWhileStatement].insert(kStatement);
    m_convertable_map_[kWhileStatement].insert(kWhileStatement);
    m_convertable_map_[kWhileStatement].insert(kIfStmtWithoutElse);
    m_convertable_map_[kWhileStatement].insert(kIfStmt);
    m_convertable_map_[kWhileStatement].insert(kAltIfStmt);
    m_convertable_map_[kIfStmtWithoutElse].insert(kTopStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kInnerStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kFunctionDeclaration);
    m_convertable_map_[kIfStmtWithoutElse].insert(kClassDeclarationStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kClassDeclaration);
    m_convertable_map_[kIfStmtWithoutElse].insert(kTraitDeclarationStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kForStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kForeachStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kDeclareStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kWhileStatement);
    m_convertable_map_[kIfStmtWithoutElse].insert(kIfStmtWithoutElse);
    m_convertable_map_[kIfStmtWithoutElse].insert(kIfStmt);
    m_convertable_map_[kIfStmtWithoutElse].insert(kAltIfStmt);
    m_convertable_map_[kIfStmt].insert(kTopStatement);
    m_convertable_map_[kIfStmt].insert(kInnerStatement);
    m_convertable_map_[kIfStmt].insert(kStatement);
    m_convertable_map_[kIfStmt].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kIfStmt].insert(kFunctionDeclaration);
    m_convertable_map_[kIfStmt].insert(kClassDeclarationStatement);
    m_convertable_map_[kIfStmt].insert(kClassDeclaration);
    m_convertable_map_[kIfStmt].insert(kTraitDeclarationStatement);
    m_convertable_map_[kIfStmt].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kIfStmt].insert(kForStatement);
    m_convertable_map_[kIfStmt].insert(kForeachStatement);
    m_convertable_map_[kIfStmt].insert(kDeclareStatement);
    m_convertable_map_[kIfStmt].insert(kWhileStatement);
    m_convertable_map_[kIfStmt].insert(kIfStmtWithoutElse);
    m_convertable_map_[kIfStmt].insert(kIfStmt);
    m_convertable_map_[kIfStmt].insert(kAltIfStmt);
    m_convertable_map_[kAltIfStmtWithoutElse].insert(kAltIfStmtWithoutElse);
    m_convertable_map_[kAltIfStmt].insert(kTopStatement);
    m_convertable_map_[kAltIfStmt].insert(kInnerStatement);
    m_convertable_map_[kAltIfStmt].insert(kStatement);
    m_convertable_map_[kAltIfStmt].insert(kFunctionDeclarationStatement);
    m_convertable_map_[kAltIfStmt].insert(kFunctionDeclaration);
    m_convertable_map_[kAltIfStmt].insert(kClassDeclarationStatement);
    m_convertable_map_[kAltIfStmt].insert(kClassDeclaration);
    m_convertable_map_[kAltIfStmt].insert(kTraitDeclarationStatement);
    m_convertable_map_[kAltIfStmt].insert(kInterfaceDeclarationStatement);
    m_convertable_map_[kAltIfStmt].insert(kForStatement);
    m_convertable_map_[kAltIfStmt].insert(kForeachStatement);
    m_convertable_map_[kAltIfStmt].insert(kDeclareStatement);
    m_convertable_map_[kAltIfStmt].insert(kWhileStatement);
    m_convertable_map_[kAltIfStmt].insert(kIfStmtWithoutElse);
    m_convertable_map_[kAltIfStmt].insert(kIfStmt);
    m_convertable_map_[kAltIfStmt].insert(kAltIfStmt);
    m_convertable_map_[kParameterList].insert(kParameterList);
    m_convertable_map_[kParameterList].insert(kNonEmptyParameterList);
    m_convertable_map_[kParameterList].insert(kParameter);
    m_convertable_map_[kNonEmptyParameterList].insert(kParameterList);
    m_convertable_map_[kNonEmptyParameterList].insert(kNonEmptyParameterList);
    m_convertable_map_[kNonEmptyParameterList].insert(kParameter);
    m_convertable_map_[kParameter].insert(kParameterList);
    m_convertable_map_[kParameter].insert(kNonEmptyParameterList);
    m_convertable_map_[kParameter].insert(kParameter);
    m_convertable_map_[kOptionalType].insert(kNamespaceName);
    m_convertable_map_[kOptionalType].insert(kName);
    m_convertable_map_[kOptionalType].insert(kOptionalType);
    m_convertable_map_[kOptionalType].insert(kTypeExpr);
    m_convertable_map_[kOptionalType].insert(kType);
    m_convertable_map_[kOptionalType].insert(kUnionType2);
    m_convertable_map_[kOptionalType].insert(kIdentifier);
    m_convertable_map_[kTypeExpr].insert(kNamespaceName);
    m_convertable_map_[kTypeExpr].insert(kName);
    m_convertable_map_[kTypeExpr].insert(kOptionalType);
    m_convertable_map_[kTypeExpr].insert(kTypeExpr);
    m_convertable_map_[kTypeExpr].insert(kType);
    m_convertable_map_[kTypeExpr].insert(kUnionType2);
    m_convertable_map_[kTypeExpr].insert(kIdentifier);
    m_convertable_map_[kType].insert(kNamespaceName);
    m_convertable_map_[kType].insert(kName);
    m_convertable_map_[kType].insert(kOptionalType);
    m_convertable_map_[kType].insert(kTypeExpr);
    m_convertable_map_[kType].insert(kType);
    m_convertable_map_[kType].insert(kUnionType2);
    m_convertable_map_[kType].insert(kIdentifier);
    m_convertable_map_[kUnionType2].insert(kNamespaceName);
    m_convertable_map_[kUnionType2].insert(kName);
    m_convertable_map_[kUnionType2].insert(kOptionalType);
    m_convertable_map_[kUnionType2].insert(kTypeExpr);
    m_convertable_map_[kUnionType2].insert(kType);
    m_convertable_map_[kUnionType2].insert(kUnionType2);
    m_convertable_map_[kUnionType2].insert(kIdentifier);
    m_convertable_map_[kReturnType].insert(kReturnType);
    m_convertable_map_[kArgumentList].insert(kArgumentList);
    m_convertable_map_[kArgumentList].insert(kCtorArguments);
    m_convertable_map_[kNonEmptyArgumentList].insert(kNamespaceName);
    m_convertable_map_[kNonEmptyArgumentList].insert(kName);
    m_convertable_map_[kNonEmptyArgumentList].insert(kNonEmptyArgumentList);
    m_convertable_map_[kNonEmptyArgumentList].insert(kArgument);
    m_convertable_map_[kNonEmptyArgumentList].insert(kNewExpr);
    m_convertable_map_[kNonEmptyArgumentList].insert(kMightDefineExpr);
    m_convertable_map_[kNonEmptyArgumentList].insert(kExpr);
    m_convertable_map_[kNonEmptyArgumentList].insert(kInlineFunction);
    m_convertable_map_[kNonEmptyArgumentList].insert(kFunctionCallme);
    m_convertable_map_[kNonEmptyArgumentList].insert(kDereferencableScalar);
    m_convertable_map_[kNonEmptyArgumentList].insert(kScalar);
    m_convertable_map_[kNonEmptyArgumentList].insert(kConstant);
    m_convertable_map_[kNonEmptyArgumentList].insert(kCallableVariable);
    m_convertable_map_[kNonEmptyArgumentList].insert(kVariable);
    m_convertable_map_[kNonEmptyArgumentList].insert(kSimpleVariable);
    m_convertable_map_[kNonEmptyArgumentList].insert(kStaticMember);
    m_convertable_map_[kNonEmptyArgumentList].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNonEmptyArgumentList].insert(kIdentifier);
    m_convertable_map_[kNonEmptyArgumentList].insert(kDollarVariable);
    m_convertable_map_[kNonEmptyArgumentList].insert(kNumLiteral);
    m_convertable_map_[kNonEmptyArgumentList].insert(kFloatLiteral);
    m_convertable_map_[kNonEmptyArgumentList].insert(kIntLiteral);
    m_convertable_map_[kNonEmptyArgumentList].insert(kStringLiteral);
    m_convertable_map_[kArgument].insert(kNamespaceName);
    m_convertable_map_[kArgument].insert(kName);
    m_convertable_map_[kArgument].insert(kNonEmptyArgumentList);
    m_convertable_map_[kArgument].insert(kArgument);
    m_convertable_map_[kArgument].insert(kNewExpr);
    m_convertable_map_[kArgument].insert(kMightDefineExpr);
    m_convertable_map_[kArgument].insert(kExpr);
    m_convertable_map_[kArgument].insert(kInlineFunction);
    m_convertable_map_[kArgument].insert(kFunctionCallme);
    m_convertable_map_[kArgument].insert(kDereferencableScalar);
    m_convertable_map_[kArgument].insert(kScalar);
    m_convertable_map_[kArgument].insert(kConstant);
    m_convertable_map_[kArgument].insert(kCallableVariable);
    m_convertable_map_[kArgument].insert(kVariable);
    m_convertable_map_[kArgument].insert(kSimpleVariable);
    m_convertable_map_[kArgument].insert(kStaticMember);
    m_convertable_map_[kArgument].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kArgument].insert(kIdentifier);
    m_convertable_map_[kArgument].insert(kDollarVariable);
    m_convertable_map_[kArgument].insert(kNumLiteral);
    m_convertable_map_[kArgument].insert(kFloatLiteral);
    m_convertable_map_[kArgument].insert(kIntLiteral);
    m_convertable_map_[kArgument].insert(kStringLiteral);
    m_convertable_map_[kGlobalVarList].insert(kGlobalVarList);
    m_convertable_map_[kGlobalVarList].insert(kGlobalVar);
    m_convertable_map_[kGlobalVarList].insert(kSimpleVariable);
    m_convertable_map_[kGlobalVarList].insert(kDollarVariable);
    m_convertable_map_[kGlobalVar].insert(kGlobalVarList);
    m_convertable_map_[kGlobalVar].insert(kGlobalVar);
    m_convertable_map_[kGlobalVar].insert(kSimpleVariable);
    m_convertable_map_[kGlobalVar].insert(kDollarVariable);
    m_convertable_map_[kStaticVarList].insert(kStaticVarList);
    m_convertable_map_[kStaticVarList].insert(kStaticVar);
    m_convertable_map_[kStaticVarList].insert(kDollarVariable);
    m_convertable_map_[kStaticVar].insert(kStaticVarList);
    m_convertable_map_[kStaticVar].insert(kStaticVar);
    m_convertable_map_[kStaticVar].insert(kDollarVariable);
    m_convertable_map_[kClassStatementList].insert(kClassStatementList);
    m_convertable_map_[kClassStatement].insert(kClassStatement);
    m_convertable_map_[kClassStatement].insert(kClassMethodDefinition);
    m_convertable_map_[kClassStatement].insert(kSimpleVarMember);
    m_convertable_map_[kClassMethodDefinition].insert(kClassStatement);
    m_convertable_map_[kClassMethodDefinition].insert(kClassMethodDefinition);
    m_convertable_map_[kClassMethodDefinition].insert(kSimpleVarMember);
    m_convertable_map_[kSimpleVarMember].insert(kClassStatement);
    m_convertable_map_[kSimpleVarMember].insert(kClassMethodDefinition);
    m_convertable_map_[kSimpleVarMember].insert(kSimpleVarMember);
    m_convertable_map_[kClassNameList].insert(kNamespaceName);
    m_convertable_map_[kClassNameList].insert(kName);
    m_convertable_map_[kClassNameList].insert(kClassNameList);
    m_convertable_map_[kClassNameList].insert(kClassName);
    m_convertable_map_[kClassNameList].insert(kIdentifier);
    m_convertable_map_[kTraitAdaptations].insert(kTraitAdaptations);
    m_convertable_map_[kTraitAdaptationList].insert(kTraitAdaptationList);
    m_convertable_map_[kTraitAdaptationList].insert(kTraitAdaptation);
    m_convertable_map_[kTraitAdaptation].insert(kTraitAdaptationList);
    m_convertable_map_[kTraitAdaptation].insert(kTraitAdaptation);
    m_convertable_map_[kTraitPrecedence].insert(kTraitPrecedence);
    m_convertable_map_[kTraitAlias].insert(kTraitAlias);
    m_convertable_map_[kTraitMethodReference].insert(kReservedNonModifiers);
    m_convertable_map_[kTraitMethodReference].insert(kSemiReserved);
    m_convertable_map_[kTraitMethodReference].insert(kIdentifierPlus);
    m_convertable_map_[kTraitMethodReference].insert(kTraitMethodReference);
    m_convertable_map_[kTraitMethodReference].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kTraitMethodReference].insert(kIdentifier);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kReservedNonModifiers);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kSemiReserved);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kIdentifierPlus);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kTraitMethodReference);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kAbsoluteTraitMethodReference].insert(kIdentifier);
    m_convertable_map_[kMethodBody].insert(kMethodBody);
    m_convertable_map_[kVariableModifiers].insert(kVariableModifiers);
    m_convertable_map_[kVariableModifiers].insert(kNonEmptyMemberModifiers);
    m_convertable_map_[kVariableModifiers].insert(kMemberModifier);
    m_convertable_map_[kMethodModifiers].insert(kMethodModifiers);
    m_convertable_map_[kMethodModifiers].insert(kNonEmptyMemberModifiers);
    m_convertable_map_[kMethodModifiers].insert(kMemberModifier);
    m_convertable_map_[kNonEmptyMemberModifiers].insert(kVariableModifiers);
    m_convertable_map_[kNonEmptyMemberModifiers].insert(kMethodModifiers);
    m_convertable_map_[kNonEmptyMemberModifiers].insert(kNonEmptyMemberModifiers);
    m_convertable_map_[kNonEmptyMemberModifiers].insert(kMemberModifier);
    m_convertable_map_[kMemberModifier].insert(kVariableModifiers);
    m_convertable_map_[kMemberModifier].insert(kMethodModifiers);
    m_convertable_map_[kMemberModifier].insert(kNonEmptyMemberModifiers);
    m_convertable_map_[kMemberModifier].insert(kMemberModifier);
    m_convertable_map_[kPropertyList].insert(kPropertyList);
    m_convertable_map_[kPropertyList].insert(kProperty);
    m_convertable_map_[kProperty].insert(kPropertyList);
    m_convertable_map_[kProperty].insert(kProperty);
    m_convertable_map_[kClassConstList].insert(kClassConstList);
    m_convertable_map_[kClassConstList].insert(kClassConstDecl);
    m_convertable_map_[kClassConstDecl].insert(kClassConstList);
    m_convertable_map_[kClassConstDecl].insert(kClassConstDecl);
    m_convertable_map_[kConstDecl].insert(kConstList);
    m_convertable_map_[kConstDecl].insert(kConstDecl);
    m_convertable_map_[kEchoExprList].insert(kNamespaceName);
    m_convertable_map_[kEchoExprList].insert(kName);
    m_convertable_map_[kEchoExprList].insert(kEchoExprList);
    m_convertable_map_[kEchoExprList].insert(kEchoExpr);
    m_convertable_map_[kEchoExprList].insert(kNewExpr);
    m_convertable_map_[kEchoExprList].insert(kMightDefineExpr);
    m_convertable_map_[kEchoExprList].insert(kExpr);
    m_convertable_map_[kEchoExprList].insert(kInlineFunction);
    m_convertable_map_[kEchoExprList].insert(kFunctionCallme);
    m_convertable_map_[kEchoExprList].insert(kDereferencableScalar);
    m_convertable_map_[kEchoExprList].insert(kScalar);
    m_convertable_map_[kEchoExprList].insert(kConstant);
    m_convertable_map_[kEchoExprList].insert(kCallableVariable);
    m_convertable_map_[kEchoExprList].insert(kVariable);
    m_convertable_map_[kEchoExprList].insert(kSimpleVariable);
    m_convertable_map_[kEchoExprList].insert(kStaticMember);
    m_convertable_map_[kEchoExprList].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kEchoExprList].insert(kIdentifier);
    m_convertable_map_[kEchoExprList].insert(kDollarVariable);
    m_convertable_map_[kEchoExprList].insert(kNumLiteral);
    m_convertable_map_[kEchoExprList].insert(kFloatLiteral);
    m_convertable_map_[kEchoExprList].insert(kIntLiteral);
    m_convertable_map_[kEchoExprList].insert(kStringLiteral);
    m_convertable_map_[kEchoExpr].insert(kNamespaceName);
    m_convertable_map_[kEchoExpr].insert(kName);
    m_convertable_map_[kEchoExpr].insert(kEchoExprList);
    m_convertable_map_[kEchoExpr].insert(kEchoExpr);
    m_convertable_map_[kEchoExpr].insert(kNewExpr);
    m_convertable_map_[kEchoExpr].insert(kMightDefineExpr);
    m_convertable_map_[kEchoExpr].insert(kExpr);
    m_convertable_map_[kEchoExpr].insert(kInlineFunction);
    m_convertable_map_[kEchoExpr].insert(kFunctionCallme);
    m_convertable_map_[kEchoExpr].insert(kDereferencableScalar);
    m_convertable_map_[kEchoExpr].insert(kScalar);
    m_convertable_map_[kEchoExpr].insert(kConstant);
    m_convertable_map_[kEchoExpr].insert(kCallableVariable);
    m_convertable_map_[kEchoExpr].insert(kVariable);
    m_convertable_map_[kEchoExpr].insert(kSimpleVariable);
    m_convertable_map_[kEchoExpr].insert(kStaticMember);
    m_convertable_map_[kEchoExpr].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kEchoExpr].insert(kIdentifier);
    m_convertable_map_[kEchoExpr].insert(kDollarVariable);
    m_convertable_map_[kEchoExpr].insert(kNumLiteral);
    m_convertable_map_[kEchoExpr].insert(kFloatLiteral);
    m_convertable_map_[kEchoExpr].insert(kIntLiteral);
    m_convertable_map_[kEchoExpr].insert(kStringLiteral);
    m_convertable_map_[kForExprs].insert(kNamespaceName);
    m_convertable_map_[kForExprs].insert(kName);
    m_convertable_map_[kForExprs].insert(kForExprs);
    m_convertable_map_[kForExprs].insert(kNonEmptyForExprs);
    m_convertable_map_[kForExprs].insert(kNewExpr);
    m_convertable_map_[kForExprs].insert(kMightDefineExpr);
    m_convertable_map_[kForExprs].insert(kExpr);
    m_convertable_map_[kForExprs].insert(kInlineFunction);
    m_convertable_map_[kForExprs].insert(kFunctionCallme);
    m_convertable_map_[kForExprs].insert(kDereferencableScalar);
    m_convertable_map_[kForExprs].insert(kScalar);
    m_convertable_map_[kForExprs].insert(kConstant);
    m_convertable_map_[kForExprs].insert(kCallableVariable);
    m_convertable_map_[kForExprs].insert(kVariable);
    m_convertable_map_[kForExprs].insert(kSimpleVariable);
    m_convertable_map_[kForExprs].insert(kStaticMember);
    m_convertable_map_[kForExprs].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kForExprs].insert(kIdentifier);
    m_convertable_map_[kForExprs].insert(kDollarVariable);
    m_convertable_map_[kForExprs].insert(kNumLiteral);
    m_convertable_map_[kForExprs].insert(kFloatLiteral);
    m_convertable_map_[kForExprs].insert(kIntLiteral);
    m_convertable_map_[kForExprs].insert(kStringLiteral);
    m_convertable_map_[kNonEmptyForExprs].insert(kNamespaceName);
    m_convertable_map_[kNonEmptyForExprs].insert(kName);
    m_convertable_map_[kNonEmptyForExprs].insert(kForExprs);
    m_convertable_map_[kNonEmptyForExprs].insert(kNonEmptyForExprs);
    m_convertable_map_[kNonEmptyForExprs].insert(kNewExpr);
    m_convertable_map_[kNonEmptyForExprs].insert(kMightDefineExpr);
    m_convertable_map_[kNonEmptyForExprs].insert(kExpr);
    m_convertable_map_[kNonEmptyForExprs].insert(kInlineFunction);
    m_convertable_map_[kNonEmptyForExprs].insert(kFunctionCallme);
    m_convertable_map_[kNonEmptyForExprs].insert(kDereferencableScalar);
    m_convertable_map_[kNonEmptyForExprs].insert(kScalar);
    m_convertable_map_[kNonEmptyForExprs].insert(kConstant);
    m_convertable_map_[kNonEmptyForExprs].insert(kCallableVariable);
    m_convertable_map_[kNonEmptyForExprs].insert(kVariable);
    m_convertable_map_[kNonEmptyForExprs].insert(kSimpleVariable);
    m_convertable_map_[kNonEmptyForExprs].insert(kStaticMember);
    m_convertable_map_[kNonEmptyForExprs].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNonEmptyForExprs].insert(kIdentifier);
    m_convertable_map_[kNonEmptyForExprs].insert(kDollarVariable);
    m_convertable_map_[kNonEmptyForExprs].insert(kNumLiteral);
    m_convertable_map_[kNonEmptyForExprs].insert(kFloatLiteral);
    m_convertable_map_[kNonEmptyForExprs].insert(kIntLiteral);
    m_convertable_map_[kNonEmptyForExprs].insert(kStringLiteral);
    m_convertable_map_[kAnonymousClass].insert(kAnonymousClass);
    m_convertable_map_[kNewExpr].insert(kNamespaceName);
    m_convertable_map_[kNewExpr].insert(kName);
    m_convertable_map_[kNewExpr].insert(kNonEmptyArgumentList);
    m_convertable_map_[kNewExpr].insert(kArgument);
    m_convertable_map_[kNewExpr].insert(kEchoExprList);
    m_convertable_map_[kNewExpr].insert(kEchoExpr);
    m_convertable_map_[kNewExpr].insert(kForExprs);
    m_convertable_map_[kNewExpr].insert(kNonEmptyForExprs);
    m_convertable_map_[kNewExpr].insert(kNewExpr);
    m_convertable_map_[kNewExpr].insert(kMightDefineExpr);
    m_convertable_map_[kNewExpr].insert(kExpr);
    m_convertable_map_[kNewExpr].insert(kInlineFunction);
    m_convertable_map_[kNewExpr].insert(kFunctionCallme);
    m_convertable_map_[kNewExpr].insert(kDereferencableScalar);
    m_convertable_map_[kNewExpr].insert(kScalar);
    m_convertable_map_[kNewExpr].insert(kConstant);
    m_convertable_map_[kNewExpr].insert(kOptionalExpr);
    m_convertable_map_[kNewExpr].insert(kCallableVariable);
    m_convertable_map_[kNewExpr].insert(kVariable);
    m_convertable_map_[kNewExpr].insert(kSimpleVariable);
    m_convertable_map_[kNewExpr].insert(kStaticMember);
    m_convertable_map_[kNewExpr].insert(kArrayPairList);
    m_convertable_map_[kNewExpr].insert(kPossibleArrayPair);
    m_convertable_map_[kNewExpr].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kNewExpr].insert(kArrayPair);
    m_convertable_map_[kNewExpr].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNewExpr].insert(kIssetVariables);
    m_convertable_map_[kNewExpr].insert(kIssetVariable);
    m_convertable_map_[kNewExpr].insert(kIdentifier);
    m_convertable_map_[kNewExpr].insert(kDollarVariable);
    m_convertable_map_[kNewExpr].insert(kNumLiteral);
    m_convertable_map_[kNewExpr].insert(kFloatLiteral);
    m_convertable_map_[kNewExpr].insert(kIntLiteral);
    m_convertable_map_[kNewExpr].insert(kStringLiteral);
    m_convertable_map_[kClassConstructor].insert(kNamespaceName);
    m_convertable_map_[kClassConstructor].insert(kName);
    m_convertable_map_[kClassConstructor].insert(kClassConstructor);
    m_convertable_map_[kClassConstructor].insert(kClassName);
    m_convertable_map_[kClassConstructor].insert(kClassNameReference);
    m_convertable_map_[kClassConstructor].insert(kSimpleVariable);
    m_convertable_map_[kClassConstructor].insert(kNewVariable);
    m_convertable_map_[kClassConstructor].insert(kIdentifier);
    m_convertable_map_[kClassConstructor].insert(kDollarVariable);
    m_convertable_map_[kMightDefineExpr].insert(kNamespaceName);
    m_convertable_map_[kMightDefineExpr].insert(kName);
    m_convertable_map_[kMightDefineExpr].insert(kNonEmptyArgumentList);
    m_convertable_map_[kMightDefineExpr].insert(kArgument);
    m_convertable_map_[kMightDefineExpr].insert(kEchoExprList);
    m_convertable_map_[kMightDefineExpr].insert(kEchoExpr);
    m_convertable_map_[kMightDefineExpr].insert(kForExprs);
    m_convertable_map_[kMightDefineExpr].insert(kNonEmptyForExprs);
    m_convertable_map_[kMightDefineExpr].insert(kNewExpr);
    m_convertable_map_[kMightDefineExpr].insert(kMightDefineExpr);
    m_convertable_map_[kMightDefineExpr].insert(kExpr);
    m_convertable_map_[kMightDefineExpr].insert(kInlineFunction);
    m_convertable_map_[kMightDefineExpr].insert(kFunctionCallme);
    m_convertable_map_[kMightDefineExpr].insert(kDereferencableScalar);
    m_convertable_map_[kMightDefineExpr].insert(kScalar);
    m_convertable_map_[kMightDefineExpr].insert(kConstant);
    m_convertable_map_[kMightDefineExpr].insert(kOptionalExpr);
    m_convertable_map_[kMightDefineExpr].insert(kCallableVariable);
    m_convertable_map_[kMightDefineExpr].insert(kVariable);
    m_convertable_map_[kMightDefineExpr].insert(kSimpleVariable);
    m_convertable_map_[kMightDefineExpr].insert(kStaticMember);
    m_convertable_map_[kMightDefineExpr].insert(kArrayPairList);
    m_convertable_map_[kMightDefineExpr].insert(kPossibleArrayPair);
    m_convertable_map_[kMightDefineExpr].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kMightDefineExpr].insert(kArrayPair);
    m_convertable_map_[kMightDefineExpr].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kMightDefineExpr].insert(kIssetVariables);
    m_convertable_map_[kMightDefineExpr].insert(kIssetVariable);
    m_convertable_map_[kMightDefineExpr].insert(kIdentifier);
    m_convertable_map_[kMightDefineExpr].insert(kDollarVariable);
    m_convertable_map_[kMightDefineExpr].insert(kNumLiteral);
    m_convertable_map_[kMightDefineExpr].insert(kFloatLiteral);
    m_convertable_map_[kMightDefineExpr].insert(kIntLiteral);
    m_convertable_map_[kMightDefineExpr].insert(kStringLiteral);
    m_convertable_map_[kExpr].insert(kNamespaceName);
    m_convertable_map_[kExpr].insert(kName);
    m_convertable_map_[kExpr].insert(kNonEmptyArgumentList);
    m_convertable_map_[kExpr].insert(kArgument);
    m_convertable_map_[kExpr].insert(kEchoExprList);
    m_convertable_map_[kExpr].insert(kEchoExpr);
    m_convertable_map_[kExpr].insert(kForExprs);
    m_convertable_map_[kExpr].insert(kNonEmptyForExprs);
    m_convertable_map_[kExpr].insert(kNewExpr);
    m_convertable_map_[kExpr].insert(kMightDefineExpr);
    m_convertable_map_[kExpr].insert(kExpr);
    m_convertable_map_[kExpr].insert(kInlineFunction);
    m_convertable_map_[kExpr].insert(kFunctionCallme);
    m_convertable_map_[kExpr].insert(kDereferencableScalar);
    m_convertable_map_[kExpr].insert(kScalar);
    m_convertable_map_[kExpr].insert(kConstant);
    m_convertable_map_[kExpr].insert(kOptionalExpr);
    m_convertable_map_[kExpr].insert(kCallableVariable);
    m_convertable_map_[kExpr].insert(kVariable);
    m_convertable_map_[kExpr].insert(kSimpleVariable);
    m_convertable_map_[kExpr].insert(kStaticMember);
    m_convertable_map_[kExpr].insert(kArrayPairList);
    m_convertable_map_[kExpr].insert(kPossibleArrayPair);
    m_convertable_map_[kExpr].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kExpr].insert(kArrayPair);
    m_convertable_map_[kExpr].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kExpr].insert(kIssetVariables);
    m_convertable_map_[kExpr].insert(kIssetVariable);
    m_convertable_map_[kExpr].insert(kIdentifier);
    m_convertable_map_[kExpr].insert(kDollarVariable);
    m_convertable_map_[kExpr].insert(kNumLiteral);
    m_convertable_map_[kExpr].insert(kFloatLiteral);
    m_convertable_map_[kExpr].insert(kIntLiteral);
    m_convertable_map_[kExpr].insert(kStringLiteral);
    m_convertable_map_[kInlineFunction].insert(kNamespaceName);
    m_convertable_map_[kInlineFunction].insert(kName);
    m_convertable_map_[kInlineFunction].insert(kNonEmptyArgumentList);
    m_convertable_map_[kInlineFunction].insert(kArgument);
    m_convertable_map_[kInlineFunction].insert(kEchoExprList);
    m_convertable_map_[kInlineFunction].insert(kEchoExpr);
    m_convertable_map_[kInlineFunction].insert(kForExprs);
    m_convertable_map_[kInlineFunction].insert(kNonEmptyForExprs);
    m_convertable_map_[kInlineFunction].insert(kNewExpr);
    m_convertable_map_[kInlineFunction].insert(kMightDefineExpr);
    m_convertable_map_[kInlineFunction].insert(kExpr);
    m_convertable_map_[kInlineFunction].insert(kInlineFunction);
    m_convertable_map_[kInlineFunction].insert(kFunctionCallme);
    m_convertable_map_[kInlineFunction].insert(kDereferencableScalar);
    m_convertable_map_[kInlineFunction].insert(kScalar);
    m_convertable_map_[kInlineFunction].insert(kConstant);
    m_convertable_map_[kInlineFunction].insert(kOptionalExpr);
    m_convertable_map_[kInlineFunction].insert(kCallableVariable);
    m_convertable_map_[kInlineFunction].insert(kVariable);
    m_convertable_map_[kInlineFunction].insert(kSimpleVariable);
    m_convertable_map_[kInlineFunction].insert(kStaticMember);
    m_convertable_map_[kInlineFunction].insert(kArrayPairList);
    m_convertable_map_[kInlineFunction].insert(kPossibleArrayPair);
    m_convertable_map_[kInlineFunction].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kInlineFunction].insert(kArrayPair);
    m_convertable_map_[kInlineFunction].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kInlineFunction].insert(kIssetVariables);
    m_convertable_map_[kInlineFunction].insert(kIssetVariable);
    m_convertable_map_[kInlineFunction].insert(kIdentifier);
    m_convertable_map_[kInlineFunction].insert(kDollarVariable);
    m_convertable_map_[kInlineFunction].insert(kNumLiteral);
    m_convertable_map_[kInlineFunction].insert(kFloatLiteral);
    m_convertable_map_[kInlineFunction].insert(kIntLiteral);
    m_convertable_map_[kInlineFunction].insert(kStringLiteral);
    m_convertable_map_[kFn].insert(kFn);
    m_convertable_map_[kFunction].insert(kFunction);
    m_convertable_map_[kBackupDocComment].insert(kBackupDocComment);
    m_convertable_map_[kBackupFnFlags].insert(kBackupFnFlags);
    m_convertable_map_[kBackupLexPos].insert(kBackupLexPos);
    m_convertable_map_[kReturnsRef].insert(kReturnsRef);
    m_convertable_map_[kLexicalVars].insert(kLexicalVars);
    m_convertable_map_[kLexicalVarList].insert(kLexicalVarList);
    m_convertable_map_[kLexicalVarList].insert(kLexicalVar);
    m_convertable_map_[kLexicalVarList].insert(kDollarVariable);
    m_convertable_map_[kLexicalVar].insert(kLexicalVarList);
    m_convertable_map_[kLexicalVar].insert(kLexicalVar);
    m_convertable_map_[kLexicalVar].insert(kDollarVariable);
    m_convertable_map_[kFunctionCallme].insert(kNamespaceName);
    m_convertable_map_[kFunctionCallme].insert(kName);
    m_convertable_map_[kFunctionCallme].insert(kUnsetVariables);
    m_convertable_map_[kFunctionCallme].insert(kUnsetVariable);
    m_convertable_map_[kFunctionCallme].insert(kForeachVariable);
    m_convertable_map_[kFunctionCallme].insert(kNonEmptyArgumentList);
    m_convertable_map_[kFunctionCallme].insert(kArgument);
    m_convertable_map_[kFunctionCallme].insert(kEchoExprList);
    m_convertable_map_[kFunctionCallme].insert(kEchoExpr);
    m_convertable_map_[kFunctionCallme].insert(kForExprs);
    m_convertable_map_[kFunctionCallme].insert(kNonEmptyForExprs);
    m_convertable_map_[kFunctionCallme].insert(kNewExpr);
    m_convertable_map_[kFunctionCallme].insert(kMightDefineExpr);
    m_convertable_map_[kFunctionCallme].insert(kExpr);
    m_convertable_map_[kFunctionCallme].insert(kInlineFunction);
    m_convertable_map_[kFunctionCallme].insert(kFunctionCallme);
    m_convertable_map_[kFunctionCallme].insert(kDereferencableScalar);
    m_convertable_map_[kFunctionCallme].insert(kScalar);
    m_convertable_map_[kFunctionCallme].insert(kConstant);
    m_convertable_map_[kFunctionCallme].insert(kOptionalExpr);
    m_convertable_map_[kFunctionCallme].insert(kVariableClassName);
    m_convertable_map_[kFunctionCallme].insert(kDereferencable);
    m_convertable_map_[kFunctionCallme].insert(kCallableExpr);
    m_convertable_map_[kFunctionCallme].insert(kCallableVariable);
    m_convertable_map_[kFunctionCallme].insert(kVariable);
    m_convertable_map_[kFunctionCallme].insert(kSimpleVariable);
    m_convertable_map_[kFunctionCallme].insert(kStaticMember);
    m_convertable_map_[kFunctionCallme].insert(kArrayPairList);
    m_convertable_map_[kFunctionCallme].insert(kPossibleArrayPair);
    m_convertable_map_[kFunctionCallme].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kFunctionCallme].insert(kArrayPair);
    m_convertable_map_[kFunctionCallme].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kFunctionCallme].insert(kIssetVariables);
    m_convertable_map_[kFunctionCallme].insert(kIssetVariable);
    m_convertable_map_[kFunctionCallme].insert(kIdentifier);
    m_convertable_map_[kFunctionCallme].insert(kDollarVariable);
    m_convertable_map_[kFunctionCallme].insert(kNumLiteral);
    m_convertable_map_[kFunctionCallme].insert(kFloatLiteral);
    m_convertable_map_[kFunctionCallme].insert(kIntLiteral);
    m_convertable_map_[kFunctionCallme].insert(kStringLiteral);
    m_convertable_map_[kClassName].insert(kNamespaceName);
    m_convertable_map_[kClassName].insert(kName);
    m_convertable_map_[kClassName].insert(kCatchNameList);
    m_convertable_map_[kClassName].insert(kClassNameList);
    m_convertable_map_[kClassName].insert(kClassConstructor);
    m_convertable_map_[kClassName].insert(kClassName);
    m_convertable_map_[kClassName].insert(kClassNameReference);
    m_convertable_map_[kClassName].insert(kSimpleVariable);
    m_convertable_map_[kClassName].insert(kNewVariable);
    m_convertable_map_[kClassName].insert(kIdentifier);
    m_convertable_map_[kClassName].insert(kDollarVariable);
    m_convertable_map_[kClassNameReference].insert(kNamespaceName);
    m_convertable_map_[kClassNameReference].insert(kName);
    m_convertable_map_[kClassNameReference].insert(kClassConstructor);
    m_convertable_map_[kClassNameReference].insert(kClassName);
    m_convertable_map_[kClassNameReference].insert(kClassNameReference);
    m_convertable_map_[kClassNameReference].insert(kSimpleVariable);
    m_convertable_map_[kClassNameReference].insert(kNewVariable);
    m_convertable_map_[kClassNameReference].insert(kIdentifier);
    m_convertable_map_[kClassNameReference].insert(kDollarVariable);
    m_convertable_map_[kExitExpr].insert(kExitExpr);
    m_convertable_map_[kBackticksExpr].insert(kBackticksExpr);
    m_convertable_map_[kBackticksExpr].insert(kEncapsList);
    m_convertable_map_[kBackticksExpr].insert(kEncapsVar);
    m_convertable_map_[kBackticksExpr].insert(kDollarVariable);
    m_convertable_map_[kCtorArguments].insert(kArgumentList);
    m_convertable_map_[kCtorArguments].insert(kCtorArguments);
    m_convertable_map_[kDereferencableScalar].insert(kNamespaceName);
    m_convertable_map_[kDereferencableScalar].insert(kName);
    m_convertable_map_[kDereferencableScalar].insert(kNonEmptyArgumentList);
    m_convertable_map_[kDereferencableScalar].insert(kArgument);
    m_convertable_map_[kDereferencableScalar].insert(kEchoExprList);
    m_convertable_map_[kDereferencableScalar].insert(kEchoExpr);
    m_convertable_map_[kDereferencableScalar].insert(kForExprs);
    m_convertable_map_[kDereferencableScalar].insert(kNonEmptyForExprs);
    m_convertable_map_[kDereferencableScalar].insert(kNewExpr);
    m_convertable_map_[kDereferencableScalar].insert(kMightDefineExpr);
    m_convertable_map_[kDereferencableScalar].insert(kExpr);
    m_convertable_map_[kDereferencableScalar].insert(kInlineFunction);
    m_convertable_map_[kDereferencableScalar].insert(kFunctionCallme);
    m_convertable_map_[kDereferencableScalar].insert(kDereferencableScalar);
    m_convertable_map_[kDereferencableScalar].insert(kScalar);
    m_convertable_map_[kDereferencableScalar].insert(kConstant);
    m_convertable_map_[kDereferencableScalar].insert(kOptionalExpr);
    m_convertable_map_[kDereferencableScalar].insert(kVariableClassName);
    m_convertable_map_[kDereferencableScalar].insert(kDereferencable);
    m_convertable_map_[kDereferencableScalar].insert(kCallableExpr);
    m_convertable_map_[kDereferencableScalar].insert(kCallableVariable);
    m_convertable_map_[kDereferencableScalar].insert(kVariable);
    m_convertable_map_[kDereferencableScalar].insert(kSimpleVariable);
    m_convertable_map_[kDereferencableScalar].insert(kStaticMember);
    m_convertable_map_[kDereferencableScalar].insert(kArrayPairList);
    m_convertable_map_[kDereferencableScalar].insert(kPossibleArrayPair);
    m_convertable_map_[kDereferencableScalar].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kDereferencableScalar].insert(kArrayPair);
    m_convertable_map_[kDereferencableScalar].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kDereferencableScalar].insert(kIssetVariables);
    m_convertable_map_[kDereferencableScalar].insert(kIssetVariable);
    m_convertable_map_[kDereferencableScalar].insert(kIdentifier);
    m_convertable_map_[kDereferencableScalar].insert(kDollarVariable);
    m_convertable_map_[kDereferencableScalar].insert(kNumLiteral);
    m_convertable_map_[kDereferencableScalar].insert(kFloatLiteral);
    m_convertable_map_[kDereferencableScalar].insert(kIntLiteral);
    m_convertable_map_[kDereferencableScalar].insert(kStringLiteral);
    m_convertable_map_[kScalar].insert(kNamespaceName);
    m_convertable_map_[kScalar].insert(kName);
    m_convertable_map_[kScalar].insert(kNonEmptyArgumentList);
    m_convertable_map_[kScalar].insert(kArgument);
    m_convertable_map_[kScalar].insert(kEchoExprList);
    m_convertable_map_[kScalar].insert(kEchoExpr);
    m_convertable_map_[kScalar].insert(kForExprs);
    m_convertable_map_[kScalar].insert(kNonEmptyForExprs);
    m_convertable_map_[kScalar].insert(kNewExpr);
    m_convertable_map_[kScalar].insert(kMightDefineExpr);
    m_convertable_map_[kScalar].insert(kExpr);
    m_convertable_map_[kScalar].insert(kInlineFunction);
    m_convertable_map_[kScalar].insert(kFunctionCallme);
    m_convertable_map_[kScalar].insert(kDereferencableScalar);
    m_convertable_map_[kScalar].insert(kScalar);
    m_convertable_map_[kScalar].insert(kConstant);
    m_convertable_map_[kScalar].insert(kOptionalExpr);
    m_convertable_map_[kScalar].insert(kCallableVariable);
    m_convertable_map_[kScalar].insert(kVariable);
    m_convertable_map_[kScalar].insert(kSimpleVariable);
    m_convertable_map_[kScalar].insert(kStaticMember);
    m_convertable_map_[kScalar].insert(kArrayPairList);
    m_convertable_map_[kScalar].insert(kPossibleArrayPair);
    m_convertable_map_[kScalar].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kScalar].insert(kArrayPair);
    m_convertable_map_[kScalar].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kScalar].insert(kIssetVariables);
    m_convertable_map_[kScalar].insert(kIssetVariable);
    m_convertable_map_[kScalar].insert(kIdentifier);
    m_convertable_map_[kScalar].insert(kDollarVariable);
    m_convertable_map_[kScalar].insert(kNumLiteral);
    m_convertable_map_[kScalar].insert(kFloatLiteral);
    m_convertable_map_[kScalar].insert(kIntLiteral);
    m_convertable_map_[kScalar].insert(kStringLiteral);
    m_convertable_map_[kConstant].insert(kNamespaceName);
    m_convertable_map_[kConstant].insert(kName);
    m_convertable_map_[kConstant].insert(kNonEmptyArgumentList);
    m_convertable_map_[kConstant].insert(kArgument);
    m_convertable_map_[kConstant].insert(kEchoExprList);
    m_convertable_map_[kConstant].insert(kEchoExpr);
    m_convertable_map_[kConstant].insert(kForExprs);
    m_convertable_map_[kConstant].insert(kNonEmptyForExprs);
    m_convertable_map_[kConstant].insert(kNewExpr);
    m_convertable_map_[kConstant].insert(kMightDefineExpr);
    m_convertable_map_[kConstant].insert(kExpr);
    m_convertable_map_[kConstant].insert(kInlineFunction);
    m_convertable_map_[kConstant].insert(kFunctionCallme);
    m_convertable_map_[kConstant].insert(kDereferencableScalar);
    m_convertable_map_[kConstant].insert(kScalar);
    m_convertable_map_[kConstant].insert(kConstant);
    m_convertable_map_[kConstant].insert(kOptionalExpr);
    m_convertable_map_[kConstant].insert(kCallableVariable);
    m_convertable_map_[kConstant].insert(kVariable);
    m_convertable_map_[kConstant].insert(kSimpleVariable);
    m_convertable_map_[kConstant].insert(kStaticMember);
    m_convertable_map_[kConstant].insert(kArrayPairList);
    m_convertable_map_[kConstant].insert(kPossibleArrayPair);
    m_convertable_map_[kConstant].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kConstant].insert(kArrayPair);
    m_convertable_map_[kConstant].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kConstant].insert(kIssetVariables);
    m_convertable_map_[kConstant].insert(kIssetVariable);
    m_convertable_map_[kConstant].insert(kIdentifier);
    m_convertable_map_[kConstant].insert(kDollarVariable);
    m_convertable_map_[kConstant].insert(kNumLiteral);
    m_convertable_map_[kConstant].insert(kFloatLiteral);
    m_convertable_map_[kConstant].insert(kIntLiteral);
    m_convertable_map_[kConstant].insert(kStringLiteral);
    m_convertable_map_[kOptionalExpr].insert(kNamespaceName);
    m_convertable_map_[kOptionalExpr].insert(kName);
    m_convertable_map_[kOptionalExpr].insert(kNewExpr);
    m_convertable_map_[kOptionalExpr].insert(kMightDefineExpr);
    m_convertable_map_[kOptionalExpr].insert(kExpr);
    m_convertable_map_[kOptionalExpr].insert(kInlineFunction);
    m_convertable_map_[kOptionalExpr].insert(kFunctionCallme);
    m_convertable_map_[kOptionalExpr].insert(kDereferencableScalar);
    m_convertable_map_[kOptionalExpr].insert(kScalar);
    m_convertable_map_[kOptionalExpr].insert(kConstant);
    m_convertable_map_[kOptionalExpr].insert(kOptionalExpr);
    m_convertable_map_[kOptionalExpr].insert(kCallableVariable);
    m_convertable_map_[kOptionalExpr].insert(kVariable);
    m_convertable_map_[kOptionalExpr].insert(kSimpleVariable);
    m_convertable_map_[kOptionalExpr].insert(kStaticMember);
    m_convertable_map_[kOptionalExpr].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kOptionalExpr].insert(kIdentifier);
    m_convertable_map_[kOptionalExpr].insert(kDollarVariable);
    m_convertable_map_[kOptionalExpr].insert(kNumLiteral);
    m_convertable_map_[kOptionalExpr].insert(kFloatLiteral);
    m_convertable_map_[kOptionalExpr].insert(kIntLiteral);
    m_convertable_map_[kOptionalExpr].insert(kStringLiteral);
    m_convertable_map_[kVariableClassName].insert(kFunctionCallme);
    m_convertable_map_[kVariableClassName].insert(kDereferencableScalar);
    m_convertable_map_[kVariableClassName].insert(kVariableClassName);
    m_convertable_map_[kVariableClassName].insert(kDereferencable);
    m_convertable_map_[kVariableClassName].insert(kCallableVariable);
    m_convertable_map_[kVariableClassName].insert(kVariable);
    m_convertable_map_[kVariableClassName].insert(kSimpleVariable);
    m_convertable_map_[kVariableClassName].insert(kStaticMember);
    m_convertable_map_[kVariableClassName].insert(kDollarVariable);
    m_convertable_map_[kVariableClassName].insert(kStringLiteral);
    m_convertable_map_[kDereferencable].insert(kFunctionCallme);
    m_convertable_map_[kDereferencable].insert(kDereferencableScalar);
    m_convertable_map_[kDereferencable].insert(kVariableClassName);
    m_convertable_map_[kDereferencable].insert(kDereferencable);
    m_convertable_map_[kDereferencable].insert(kCallableVariable);
    m_convertable_map_[kDereferencable].insert(kVariable);
    m_convertable_map_[kDereferencable].insert(kSimpleVariable);
    m_convertable_map_[kDereferencable].insert(kStaticMember);
    m_convertable_map_[kDereferencable].insert(kDollarVariable);
    m_convertable_map_[kDereferencable].insert(kStringLiteral);
    m_convertable_map_[kCallableExpr].insert(kFunctionCallme);
    m_convertable_map_[kCallableExpr].insert(kDereferencableScalar);
    m_convertable_map_[kCallableExpr].insert(kCallableExpr);
    m_convertable_map_[kCallableExpr].insert(kCallableVariable);
    m_convertable_map_[kCallableExpr].insert(kSimpleVariable);
    m_convertable_map_[kCallableExpr].insert(kDollarVariable);
    m_convertable_map_[kCallableExpr].insert(kStringLiteral);
    m_convertable_map_[kCallableVariable].insert(kNamespaceName);
    m_convertable_map_[kCallableVariable].insert(kName);
    m_convertable_map_[kCallableVariable].insert(kUnsetVariables);
    m_convertable_map_[kCallableVariable].insert(kUnsetVariable);
    m_convertable_map_[kCallableVariable].insert(kForeachVariable);
    m_convertable_map_[kCallableVariable].insert(kNonEmptyArgumentList);
    m_convertable_map_[kCallableVariable].insert(kArgument);
    m_convertable_map_[kCallableVariable].insert(kEchoExprList);
    m_convertable_map_[kCallableVariable].insert(kEchoExpr);
    m_convertable_map_[kCallableVariable].insert(kForExprs);
    m_convertable_map_[kCallableVariable].insert(kNonEmptyForExprs);
    m_convertable_map_[kCallableVariable].insert(kNewExpr);
    m_convertable_map_[kCallableVariable].insert(kMightDefineExpr);
    m_convertable_map_[kCallableVariable].insert(kExpr);
    m_convertable_map_[kCallableVariable].insert(kInlineFunction);
    m_convertable_map_[kCallableVariable].insert(kFunctionCallme);
    m_convertable_map_[kCallableVariable].insert(kDereferencableScalar);
    m_convertable_map_[kCallableVariable].insert(kScalar);
    m_convertable_map_[kCallableVariable].insert(kConstant);
    m_convertable_map_[kCallableVariable].insert(kOptionalExpr);
    m_convertable_map_[kCallableVariable].insert(kVariableClassName);
    m_convertable_map_[kCallableVariable].insert(kDereferencable);
    m_convertable_map_[kCallableVariable].insert(kCallableExpr);
    m_convertable_map_[kCallableVariable].insert(kCallableVariable);
    m_convertable_map_[kCallableVariable].insert(kVariable);
    m_convertable_map_[kCallableVariable].insert(kSimpleVariable);
    m_convertable_map_[kCallableVariable].insert(kStaticMember);
    m_convertable_map_[kCallableVariable].insert(kArrayPairList);
    m_convertable_map_[kCallableVariable].insert(kPossibleArrayPair);
    m_convertable_map_[kCallableVariable].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kCallableVariable].insert(kArrayPair);
    m_convertable_map_[kCallableVariable].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kCallableVariable].insert(kIssetVariables);
    m_convertable_map_[kCallableVariable].insert(kIssetVariable);
    m_convertable_map_[kCallableVariable].insert(kIdentifier);
    m_convertable_map_[kCallableVariable].insert(kDollarVariable);
    m_convertable_map_[kCallableVariable].insert(kNumLiteral);
    m_convertable_map_[kCallableVariable].insert(kFloatLiteral);
    m_convertable_map_[kCallableVariable].insert(kIntLiteral);
    m_convertable_map_[kCallableVariable].insert(kStringLiteral);
    m_convertable_map_[kVariable].insert(kNamespaceName);
    m_convertable_map_[kVariable].insert(kName);
    m_convertable_map_[kVariable].insert(kUnsetVariables);
    m_convertable_map_[kVariable].insert(kUnsetVariable);
    m_convertable_map_[kVariable].insert(kForeachVariable);
    m_convertable_map_[kVariable].insert(kNonEmptyArgumentList);
    m_convertable_map_[kVariable].insert(kArgument);
    m_convertable_map_[kVariable].insert(kEchoExprList);
    m_convertable_map_[kVariable].insert(kEchoExpr);
    m_convertable_map_[kVariable].insert(kForExprs);
    m_convertable_map_[kVariable].insert(kNonEmptyForExprs);
    m_convertable_map_[kVariable].insert(kNewExpr);
    m_convertable_map_[kVariable].insert(kMightDefineExpr);
    m_convertable_map_[kVariable].insert(kExpr);
    m_convertable_map_[kVariable].insert(kInlineFunction);
    m_convertable_map_[kVariable].insert(kFunctionCallme);
    m_convertable_map_[kVariable].insert(kDereferencableScalar);
    m_convertable_map_[kVariable].insert(kScalar);
    m_convertable_map_[kVariable].insert(kConstant);
    m_convertable_map_[kVariable].insert(kOptionalExpr);
    m_convertable_map_[kVariable].insert(kVariableClassName);
    m_convertable_map_[kVariable].insert(kDereferencable);
    m_convertable_map_[kVariable].insert(kCallableVariable);
    m_convertable_map_[kVariable].insert(kVariable);
    m_convertable_map_[kVariable].insert(kSimpleVariable);
    m_convertable_map_[kVariable].insert(kStaticMember);
    m_convertable_map_[kVariable].insert(kArrayPairList);
    m_convertable_map_[kVariable].insert(kPossibleArrayPair);
    m_convertable_map_[kVariable].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kVariable].insert(kArrayPair);
    m_convertable_map_[kVariable].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kVariable].insert(kIssetVariables);
    m_convertable_map_[kVariable].insert(kIssetVariable);
    m_convertable_map_[kVariable].insert(kIdentifier);
    m_convertable_map_[kVariable].insert(kDollarVariable);
    m_convertable_map_[kVariable].insert(kNumLiteral);
    m_convertable_map_[kVariable].insert(kFloatLiteral);
    m_convertable_map_[kVariable].insert(kIntLiteral);
    m_convertable_map_[kVariable].insert(kStringLiteral);
    m_convertable_map_[kSimpleVariable].insert(kReservedNonModifiers);
    m_convertable_map_[kSimpleVariable].insert(kSemiReserved);
    m_convertable_map_[kSimpleVariable].insert(kIdentifierPlus);
    m_convertable_map_[kSimpleVariable].insert(kNamespaceName);
    m_convertable_map_[kSimpleVariable].insert(kName);
    m_convertable_map_[kSimpleVariable].insert(kUnsetVariables);
    m_convertable_map_[kSimpleVariable].insert(kUnsetVariable);
    m_convertable_map_[kSimpleVariable].insert(kForeachVariable);
    m_convertable_map_[kSimpleVariable].insert(kNonEmptyArgumentList);
    m_convertable_map_[kSimpleVariable].insert(kArgument);
    m_convertable_map_[kSimpleVariable].insert(kGlobalVarList);
    m_convertable_map_[kSimpleVariable].insert(kGlobalVar);
    m_convertable_map_[kSimpleVariable].insert(kEchoExprList);
    m_convertable_map_[kSimpleVariable].insert(kEchoExpr);
    m_convertable_map_[kSimpleVariable].insert(kForExprs);
    m_convertable_map_[kSimpleVariable].insert(kNonEmptyForExprs);
    m_convertable_map_[kSimpleVariable].insert(kNewExpr);
    m_convertable_map_[kSimpleVariable].insert(kClassConstructor);
    m_convertable_map_[kSimpleVariable].insert(kMightDefineExpr);
    m_convertable_map_[kSimpleVariable].insert(kExpr);
    m_convertable_map_[kSimpleVariable].insert(kInlineFunction);
    m_convertable_map_[kSimpleVariable].insert(kFunctionCallme);
    m_convertable_map_[kSimpleVariable].insert(kClassName);
    m_convertable_map_[kSimpleVariable].insert(kClassNameReference);
    m_convertable_map_[kSimpleVariable].insert(kDereferencableScalar);
    m_convertable_map_[kSimpleVariable].insert(kScalar);
    m_convertable_map_[kSimpleVariable].insert(kConstant);
    m_convertable_map_[kSimpleVariable].insert(kOptionalExpr);
    m_convertable_map_[kSimpleVariable].insert(kVariableClassName);
    m_convertable_map_[kSimpleVariable].insert(kDereferencable);
    m_convertable_map_[kSimpleVariable].insert(kCallableExpr);
    m_convertable_map_[kSimpleVariable].insert(kCallableVariable);
    m_convertable_map_[kSimpleVariable].insert(kVariable);
    m_convertable_map_[kSimpleVariable].insert(kSimpleVariable);
    m_convertable_map_[kSimpleVariable].insert(kStaticMember);
    m_convertable_map_[kSimpleVariable].insert(kNewVariable);
    m_convertable_map_[kSimpleVariable].insert(kMemberName);
    m_convertable_map_[kSimpleVariable].insert(kPropertyName);
    m_convertable_map_[kSimpleVariable].insert(kArrayPairList);
    m_convertable_map_[kSimpleVariable].insert(kPossibleArrayPair);
    m_convertable_map_[kSimpleVariable].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kSimpleVariable].insert(kArrayPair);
    m_convertable_map_[kSimpleVariable].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kSimpleVariable].insert(kIssetVariables);
    m_convertable_map_[kSimpleVariable].insert(kIssetVariable);
    m_convertable_map_[kSimpleVariable].insert(kIdentifier);
    m_convertable_map_[kSimpleVariable].insert(kDollarVariable);
    m_convertable_map_[kSimpleVariable].insert(kNumLiteral);
    m_convertable_map_[kSimpleVariable].insert(kFloatLiteral);
    m_convertable_map_[kSimpleVariable].insert(kIntLiteral);
    m_convertable_map_[kSimpleVariable].insert(kStringLiteral);
    m_convertable_map_[kStaticMember].insert(kNamespaceName);
    m_convertable_map_[kStaticMember].insert(kName);
    m_convertable_map_[kStaticMember].insert(kUnsetVariables);
    m_convertable_map_[kStaticMember].insert(kUnsetVariable);
    m_convertable_map_[kStaticMember].insert(kForeachVariable);
    m_convertable_map_[kStaticMember].insert(kNonEmptyArgumentList);
    m_convertable_map_[kStaticMember].insert(kArgument);
    m_convertable_map_[kStaticMember].insert(kEchoExprList);
    m_convertable_map_[kStaticMember].insert(kEchoExpr);
    m_convertable_map_[kStaticMember].insert(kForExprs);
    m_convertable_map_[kStaticMember].insert(kNonEmptyForExprs);
    m_convertable_map_[kStaticMember].insert(kNewExpr);
    m_convertable_map_[kStaticMember].insert(kMightDefineExpr);
    m_convertable_map_[kStaticMember].insert(kExpr);
    m_convertable_map_[kStaticMember].insert(kInlineFunction);
    m_convertable_map_[kStaticMember].insert(kFunctionCallme);
    m_convertable_map_[kStaticMember].insert(kDereferencableScalar);
    m_convertable_map_[kStaticMember].insert(kScalar);
    m_convertable_map_[kStaticMember].insert(kConstant);
    m_convertable_map_[kStaticMember].insert(kOptionalExpr);
    m_convertable_map_[kStaticMember].insert(kVariableClassName);
    m_convertable_map_[kStaticMember].insert(kDereferencable);
    m_convertable_map_[kStaticMember].insert(kCallableVariable);
    m_convertable_map_[kStaticMember].insert(kVariable);
    m_convertable_map_[kStaticMember].insert(kSimpleVariable);
    m_convertable_map_[kStaticMember].insert(kStaticMember);
    m_convertable_map_[kStaticMember].insert(kArrayPairList);
    m_convertable_map_[kStaticMember].insert(kPossibleArrayPair);
    m_convertable_map_[kStaticMember].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kStaticMember].insert(kArrayPair);
    m_convertable_map_[kStaticMember].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kStaticMember].insert(kIssetVariables);
    m_convertable_map_[kStaticMember].insert(kIssetVariable);
    m_convertable_map_[kStaticMember].insert(kIdentifier);
    m_convertable_map_[kStaticMember].insert(kDollarVariable);
    m_convertable_map_[kStaticMember].insert(kNumLiteral);
    m_convertable_map_[kStaticMember].insert(kFloatLiteral);
    m_convertable_map_[kStaticMember].insert(kIntLiteral);
    m_convertable_map_[kStaticMember].insert(kStringLiteral);
    m_convertable_map_[kNewVariable].insert(kNamespaceName);
    m_convertable_map_[kNewVariable].insert(kName);
    m_convertable_map_[kNewVariable].insert(kClassConstructor);
    m_convertable_map_[kNewVariable].insert(kClassName);
    m_convertable_map_[kNewVariable].insert(kClassNameReference);
    m_convertable_map_[kNewVariable].insert(kSimpleVariable);
    m_convertable_map_[kNewVariable].insert(kNewVariable);
    m_convertable_map_[kNewVariable].insert(kIdentifier);
    m_convertable_map_[kNewVariable].insert(kDollarVariable);
    m_convertable_map_[kMemberName].insert(kReservedNonModifiers);
    m_convertable_map_[kMemberName].insert(kSemiReserved);
    m_convertable_map_[kMemberName].insert(kIdentifierPlus);
    m_convertable_map_[kMemberName].insert(kSimpleVariable);
    m_convertable_map_[kMemberName].insert(kMemberName);
    m_convertable_map_[kMemberName].insert(kIdentifier);
    m_convertable_map_[kMemberName].insert(kDollarVariable);
    m_convertable_map_[kPropertyName].insert(kSimpleVariable);
    m_convertable_map_[kPropertyName].insert(kPropertyName);
    m_convertable_map_[kPropertyName].insert(kIdentifier);
    m_convertable_map_[kPropertyName].insert(kDollarVariable);
    m_convertable_map_[kArrayPairList].insert(kNamespaceName);
    m_convertable_map_[kArrayPairList].insert(kName);
    m_convertable_map_[kArrayPairList].insert(kNewExpr);
    m_convertable_map_[kArrayPairList].insert(kMightDefineExpr);
    m_convertable_map_[kArrayPairList].insert(kExpr);
    m_convertable_map_[kArrayPairList].insert(kInlineFunction);
    m_convertable_map_[kArrayPairList].insert(kFunctionCallme);
    m_convertable_map_[kArrayPairList].insert(kDereferencableScalar);
    m_convertable_map_[kArrayPairList].insert(kScalar);
    m_convertable_map_[kArrayPairList].insert(kConstant);
    m_convertable_map_[kArrayPairList].insert(kCallableVariable);
    m_convertable_map_[kArrayPairList].insert(kVariable);
    m_convertable_map_[kArrayPairList].insert(kSimpleVariable);
    m_convertable_map_[kArrayPairList].insert(kStaticMember);
    m_convertable_map_[kArrayPairList].insert(kArrayPairList);
    m_convertable_map_[kArrayPairList].insert(kPossibleArrayPair);
    m_convertable_map_[kArrayPairList].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kArrayPairList].insert(kArrayPair);
    m_convertable_map_[kArrayPairList].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kArrayPairList].insert(kIdentifier);
    m_convertable_map_[kArrayPairList].insert(kDollarVariable);
    m_convertable_map_[kArrayPairList].insert(kNumLiteral);
    m_convertable_map_[kArrayPairList].insert(kFloatLiteral);
    m_convertable_map_[kArrayPairList].insert(kIntLiteral);
    m_convertable_map_[kArrayPairList].insert(kStringLiteral);
    m_convertable_map_[kPossibleArrayPair].insert(kNamespaceName);
    m_convertable_map_[kPossibleArrayPair].insert(kName);
    m_convertable_map_[kPossibleArrayPair].insert(kNewExpr);
    m_convertable_map_[kPossibleArrayPair].insert(kMightDefineExpr);
    m_convertable_map_[kPossibleArrayPair].insert(kExpr);
    m_convertable_map_[kPossibleArrayPair].insert(kInlineFunction);
    m_convertable_map_[kPossibleArrayPair].insert(kFunctionCallme);
    m_convertable_map_[kPossibleArrayPair].insert(kDereferencableScalar);
    m_convertable_map_[kPossibleArrayPair].insert(kScalar);
    m_convertable_map_[kPossibleArrayPair].insert(kConstant);
    m_convertable_map_[kPossibleArrayPair].insert(kCallableVariable);
    m_convertable_map_[kPossibleArrayPair].insert(kVariable);
    m_convertable_map_[kPossibleArrayPair].insert(kSimpleVariable);
    m_convertable_map_[kPossibleArrayPair].insert(kStaticMember);
    m_convertable_map_[kPossibleArrayPair].insert(kArrayPairList);
    m_convertable_map_[kPossibleArrayPair].insert(kPossibleArrayPair);
    m_convertable_map_[kPossibleArrayPair].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kPossibleArrayPair].insert(kArrayPair);
    m_convertable_map_[kPossibleArrayPair].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kPossibleArrayPair].insert(kIdentifier);
    m_convertable_map_[kPossibleArrayPair].insert(kDollarVariable);
    m_convertable_map_[kPossibleArrayPair].insert(kNumLiteral);
    m_convertable_map_[kPossibleArrayPair].insert(kFloatLiteral);
    m_convertable_map_[kPossibleArrayPair].insert(kIntLiteral);
    m_convertable_map_[kPossibleArrayPair].insert(kStringLiteral);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kNamespaceName);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kName);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kNewExpr);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kMightDefineExpr);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kExpr);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kInlineFunction);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kFunctionCallme);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kDereferencableScalar);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kScalar);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kConstant);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kCallableVariable);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kVariable);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kSimpleVariable);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kStaticMember);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kArrayPairList);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kPossibleArrayPair);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kArrayPair);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kIdentifier);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kDollarVariable);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kNumLiteral);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kFloatLiteral);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kIntLiteral);
    m_convertable_map_[kNonEmptyArrayPairList].insert(kStringLiteral);
    m_convertable_map_[kArrayPair].insert(kNamespaceName);
    m_convertable_map_[kArrayPair].insert(kName);
    m_convertable_map_[kArrayPair].insert(kNewExpr);
    m_convertable_map_[kArrayPair].insert(kMightDefineExpr);
    m_convertable_map_[kArrayPair].insert(kExpr);
    m_convertable_map_[kArrayPair].insert(kInlineFunction);
    m_convertable_map_[kArrayPair].insert(kFunctionCallme);
    m_convertable_map_[kArrayPair].insert(kDereferencableScalar);
    m_convertable_map_[kArrayPair].insert(kScalar);
    m_convertable_map_[kArrayPair].insert(kConstant);
    m_convertable_map_[kArrayPair].insert(kCallableVariable);
    m_convertable_map_[kArrayPair].insert(kVariable);
    m_convertable_map_[kArrayPair].insert(kSimpleVariable);
    m_convertable_map_[kArrayPair].insert(kStaticMember);
    m_convertable_map_[kArrayPair].insert(kArrayPairList);
    m_convertable_map_[kArrayPair].insert(kPossibleArrayPair);
    m_convertable_map_[kArrayPair].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kArrayPair].insert(kArrayPair);
    m_convertable_map_[kArrayPair].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kArrayPair].insert(kIdentifier);
    m_convertable_map_[kArrayPair].insert(kDollarVariable);
    m_convertable_map_[kArrayPair].insert(kNumLiteral);
    m_convertable_map_[kArrayPair].insert(kFloatLiteral);
    m_convertable_map_[kArrayPair].insert(kIntLiteral);
    m_convertable_map_[kArrayPair].insert(kStringLiteral);
    m_convertable_map_[kEncapsList].insert(kBackticksExpr);
    m_convertable_map_[kEncapsList].insert(kEncapsList);
    m_convertable_map_[kEncapsList].insert(kEncapsVar);
    m_convertable_map_[kEncapsList].insert(kDollarVariable);
    m_convertable_map_[kStringVarname].insert(kStringVarname);
    m_convertable_map_[kEncapsVar].insert(kBackticksExpr);
    m_convertable_map_[kEncapsVar].insert(kEncapsList);
    m_convertable_map_[kEncapsVar].insert(kEncapsVar);
    m_convertable_map_[kEncapsVar].insert(kDollarVariable);
    m_convertable_map_[kEncapsVarOffset].insert(kEncapsVarOffset);
    m_convertable_map_[kEncapsVarOffset].insert(kIdentifier);
    m_convertable_map_[kEncapsVarOffset].insert(kDollarVariable);
    m_convertable_map_[kEncapsVarOffset].insert(kStringLiteral);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNamespaceName);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kName);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNonEmptyArgumentList);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kArgument);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kEchoExprList);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kEchoExpr);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kForExprs);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNonEmptyForExprs);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNewExpr);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kMightDefineExpr);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kExpr);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kInlineFunction);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kFunctionCallme);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kDereferencableScalar);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kScalar);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kConstant);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kOptionalExpr);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kCallableVariable);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kVariable);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kSimpleVariable);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kStaticMember);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kArrayPairList);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kPossibleArrayPair);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kArrayPair);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kIssetVariables);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kIssetVariable);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kIdentifier);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kDollarVariable);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kNumLiteral);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kFloatLiteral);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kIntLiteral);
    m_convertable_map_[kInternalFunctionsInYacc].insert(kStringLiteral);
    m_convertable_map_[kIssetVariables].insert(kNamespaceName);
    m_convertable_map_[kIssetVariables].insert(kName);
    m_convertable_map_[kIssetVariables].insert(kNewExpr);
    m_convertable_map_[kIssetVariables].insert(kMightDefineExpr);
    m_convertable_map_[kIssetVariables].insert(kExpr);
    m_convertable_map_[kIssetVariables].insert(kInlineFunction);
    m_convertable_map_[kIssetVariables].insert(kFunctionCallme);
    m_convertable_map_[kIssetVariables].insert(kDereferencableScalar);
    m_convertable_map_[kIssetVariables].insert(kScalar);
    m_convertable_map_[kIssetVariables].insert(kConstant);
    m_convertable_map_[kIssetVariables].insert(kCallableVariable);
    m_convertable_map_[kIssetVariables].insert(kVariable);
    m_convertable_map_[kIssetVariables].insert(kSimpleVariable);
    m_convertable_map_[kIssetVariables].insert(kStaticMember);
    m_convertable_map_[kIssetVariables].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kIssetVariables].insert(kIssetVariables);
    m_convertable_map_[kIssetVariables].insert(kIssetVariable);
    m_convertable_map_[kIssetVariables].insert(kIdentifier);
    m_convertable_map_[kIssetVariables].insert(kDollarVariable);
    m_convertable_map_[kIssetVariables].insert(kNumLiteral);
    m_convertable_map_[kIssetVariables].insert(kFloatLiteral);
    m_convertable_map_[kIssetVariables].insert(kIntLiteral);
    m_convertable_map_[kIssetVariables].insert(kStringLiteral);
    m_convertable_map_[kIssetVariable].insert(kNamespaceName);
    m_convertable_map_[kIssetVariable].insert(kName);
    m_convertable_map_[kIssetVariable].insert(kNewExpr);
    m_convertable_map_[kIssetVariable].insert(kMightDefineExpr);
    m_convertable_map_[kIssetVariable].insert(kExpr);
    m_convertable_map_[kIssetVariable].insert(kInlineFunction);
    m_convertable_map_[kIssetVariable].insert(kFunctionCallme);
    m_convertable_map_[kIssetVariable].insert(kDereferencableScalar);
    m_convertable_map_[kIssetVariable].insert(kScalar);
    m_convertable_map_[kIssetVariable].insert(kConstant);
    m_convertable_map_[kIssetVariable].insert(kCallableVariable);
    m_convertable_map_[kIssetVariable].insert(kVariable);
    m_convertable_map_[kIssetVariable].insert(kSimpleVariable);
    m_convertable_map_[kIssetVariable].insert(kStaticMember);
    m_convertable_map_[kIssetVariable].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kIssetVariable].insert(kIssetVariables);
    m_convertable_map_[kIssetVariable].insert(kIssetVariable);
    m_convertable_map_[kIssetVariable].insert(kIdentifier);
    m_convertable_map_[kIssetVariable].insert(kDollarVariable);
    m_convertable_map_[kIssetVariable].insert(kNumLiteral);
    m_convertable_map_[kIssetVariable].insert(kFloatLiteral);
    m_convertable_map_[kIssetVariable].insert(kIntLiteral);
    m_convertable_map_[kIssetVariable].insert(kStringLiteral);
    m_convertable_map_[kIdentifier].insert(kReservedNonModifiers);
    m_convertable_map_[kIdentifier].insert(kSemiReserved);
    m_convertable_map_[kIdentifier].insert(kIdentifierPlus);
    m_convertable_map_[kIdentifier].insert(kNamespaceName);
    m_convertable_map_[kIdentifier].insert(kName);
    m_convertable_map_[kIdentifier].insert(kInlineUseDeclarations);
    m_convertable_map_[kIdentifier].insert(kUnprefixedUseDeclarations);
    m_convertable_map_[kIdentifier].insert(kUseDeclarations);
    m_convertable_map_[kIdentifier].insert(kInlineUseDeclaration);
    m_convertable_map_[kIdentifier].insert(kUnprefixedUseDeclaration);
    m_convertable_map_[kIdentifier].insert(kUseDeclaration);
    m_convertable_map_[kIdentifier].insert(kCatchNameList);
    m_convertable_map_[kIdentifier].insert(kOptionalType);
    m_convertable_map_[kIdentifier].insert(kTypeExpr);
    m_convertable_map_[kIdentifier].insert(kType);
    m_convertable_map_[kIdentifier].insert(kUnionType2);
    m_convertable_map_[kIdentifier].insert(kNonEmptyArgumentList);
    m_convertable_map_[kIdentifier].insert(kArgument);
    m_convertable_map_[kIdentifier].insert(kClassNameList);
    m_convertable_map_[kIdentifier].insert(kTraitMethodReference);
    m_convertable_map_[kIdentifier].insert(kAbsoluteTraitMethodReference);
    m_convertable_map_[kIdentifier].insert(kEchoExprList);
    m_convertable_map_[kIdentifier].insert(kEchoExpr);
    m_convertable_map_[kIdentifier].insert(kForExprs);
    m_convertable_map_[kIdentifier].insert(kNonEmptyForExprs);
    m_convertable_map_[kIdentifier].insert(kNewExpr);
    m_convertable_map_[kIdentifier].insert(kClassConstructor);
    m_convertable_map_[kIdentifier].insert(kMightDefineExpr);
    m_convertable_map_[kIdentifier].insert(kExpr);
    m_convertable_map_[kIdentifier].insert(kInlineFunction);
    m_convertable_map_[kIdentifier].insert(kFunctionCallme);
    m_convertable_map_[kIdentifier].insert(kClassName);
    m_convertable_map_[kIdentifier].insert(kClassNameReference);
    m_convertable_map_[kIdentifier].insert(kDereferencableScalar);
    m_convertable_map_[kIdentifier].insert(kScalar);
    m_convertable_map_[kIdentifier].insert(kConstant);
    m_convertable_map_[kIdentifier].insert(kOptionalExpr);
    m_convertable_map_[kIdentifier].insert(kCallableVariable);
    m_convertable_map_[kIdentifier].insert(kVariable);
    m_convertable_map_[kIdentifier].insert(kSimpleVariable);
    m_convertable_map_[kIdentifier].insert(kStaticMember);
    m_convertable_map_[kIdentifier].insert(kNewVariable);
    m_convertable_map_[kIdentifier].insert(kMemberName);
    m_convertable_map_[kIdentifier].insert(kPropertyName);
    m_convertable_map_[kIdentifier].insert(kArrayPairList);
    m_convertable_map_[kIdentifier].insert(kPossibleArrayPair);
    m_convertable_map_[kIdentifier].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kIdentifier].insert(kArrayPair);
    m_convertable_map_[kIdentifier].insert(kEncapsVarOffset);
    m_convertable_map_[kIdentifier].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kIdentifier].insert(kIssetVariables);
    m_convertable_map_[kIdentifier].insert(kIssetVariable);
    m_convertable_map_[kIdentifier].insert(kIdentifier);
    m_convertable_map_[kIdentifier].insert(kDollarVariable);
    m_convertable_map_[kIdentifier].insert(kNumLiteral);
    m_convertable_map_[kIdentifier].insert(kFloatLiteral);
    m_convertable_map_[kIdentifier].insert(kIntLiteral);
    m_convertable_map_[kIdentifier].insert(kStringLiteral);
    m_convertable_map_[kDollarVariable].insert(kReservedNonModifiers);
    m_convertable_map_[kDollarVariable].insert(kSemiReserved);
    m_convertable_map_[kDollarVariable].insert(kIdentifierPlus);
    m_convertable_map_[kDollarVariable].insert(kNamespaceName);
    m_convertable_map_[kDollarVariable].insert(kName);
    m_convertable_map_[kDollarVariable].insert(kUnsetVariables);
    m_convertable_map_[kDollarVariable].insert(kUnsetVariable);
    m_convertable_map_[kDollarVariable].insert(kForeachVariable);
    m_convertable_map_[kDollarVariable].insert(kNonEmptyArgumentList);
    m_convertable_map_[kDollarVariable].insert(kArgument);
    m_convertable_map_[kDollarVariable].insert(kGlobalVarList);
    m_convertable_map_[kDollarVariable].insert(kGlobalVar);
    m_convertable_map_[kDollarVariable].insert(kStaticVarList);
    m_convertable_map_[kDollarVariable].insert(kStaticVar);
    m_convertable_map_[kDollarVariable].insert(kEchoExprList);
    m_convertable_map_[kDollarVariable].insert(kEchoExpr);
    m_convertable_map_[kDollarVariable].insert(kForExprs);
    m_convertable_map_[kDollarVariable].insert(kNonEmptyForExprs);
    m_convertable_map_[kDollarVariable].insert(kNewExpr);
    m_convertable_map_[kDollarVariable].insert(kClassConstructor);
    m_convertable_map_[kDollarVariable].insert(kMightDefineExpr);
    m_convertable_map_[kDollarVariable].insert(kExpr);
    m_convertable_map_[kDollarVariable].insert(kInlineFunction);
    m_convertable_map_[kDollarVariable].insert(kLexicalVarList);
    m_convertable_map_[kDollarVariable].insert(kLexicalVar);
    m_convertable_map_[kDollarVariable].insert(kFunctionCallme);
    m_convertable_map_[kDollarVariable].insert(kClassName);
    m_convertable_map_[kDollarVariable].insert(kClassNameReference);
    m_convertable_map_[kDollarVariable].insert(kBackticksExpr);
    m_convertable_map_[kDollarVariable].insert(kDereferencableScalar);
    m_convertable_map_[kDollarVariable].insert(kScalar);
    m_convertable_map_[kDollarVariable].insert(kConstant);
    m_convertable_map_[kDollarVariable].insert(kOptionalExpr);
    m_convertable_map_[kDollarVariable].insert(kVariableClassName);
    m_convertable_map_[kDollarVariable].insert(kDereferencable);
    m_convertable_map_[kDollarVariable].insert(kCallableExpr);
    m_convertable_map_[kDollarVariable].insert(kCallableVariable);
    m_convertable_map_[kDollarVariable].insert(kVariable);
    m_convertable_map_[kDollarVariable].insert(kSimpleVariable);
    m_convertable_map_[kDollarVariable].insert(kStaticMember);
    m_convertable_map_[kDollarVariable].insert(kNewVariable);
    m_convertable_map_[kDollarVariable].insert(kMemberName);
    m_convertable_map_[kDollarVariable].insert(kPropertyName);
    m_convertable_map_[kDollarVariable].insert(kArrayPairList);
    m_convertable_map_[kDollarVariable].insert(kPossibleArrayPair);
    m_convertable_map_[kDollarVariable].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kDollarVariable].insert(kArrayPair);
    m_convertable_map_[kDollarVariable].insert(kEncapsList);
    m_convertable_map_[kDollarVariable].insert(kEncapsVar);
    m_convertable_map_[kDollarVariable].insert(kEncapsVarOffset);
    m_convertable_map_[kDollarVariable].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kDollarVariable].insert(kIssetVariables);
    m_convertable_map_[kDollarVariable].insert(kIssetVariable);
    m_convertable_map_[kDollarVariable].insert(kIdentifier);
    m_convertable_map_[kDollarVariable].insert(kDollarVariable);
    m_convertable_map_[kDollarVariable].insert(kNumLiteral);
    m_convertable_map_[kDollarVariable].insert(kFloatLiteral);
    m_convertable_map_[kDollarVariable].insert(kIntLiteral);
    m_convertable_map_[kDollarVariable].insert(kStringLiteral);
    m_convertable_map_[kNumLiteral].insert(kNamespaceName);
    m_convertable_map_[kNumLiteral].insert(kName);
    m_convertable_map_[kNumLiteral].insert(kNonEmptyArgumentList);
    m_convertable_map_[kNumLiteral].insert(kArgument);
    m_convertable_map_[kNumLiteral].insert(kEchoExprList);
    m_convertable_map_[kNumLiteral].insert(kEchoExpr);
    m_convertable_map_[kNumLiteral].insert(kForExprs);
    m_convertable_map_[kNumLiteral].insert(kNonEmptyForExprs);
    m_convertable_map_[kNumLiteral].insert(kNewExpr);
    m_convertable_map_[kNumLiteral].insert(kMightDefineExpr);
    m_convertable_map_[kNumLiteral].insert(kExpr);
    m_convertable_map_[kNumLiteral].insert(kInlineFunction);
    m_convertable_map_[kNumLiteral].insert(kFunctionCallme);
    m_convertable_map_[kNumLiteral].insert(kDereferencableScalar);
    m_convertable_map_[kNumLiteral].insert(kScalar);
    m_convertable_map_[kNumLiteral].insert(kConstant);
    m_convertable_map_[kNumLiteral].insert(kOptionalExpr);
    m_convertable_map_[kNumLiteral].insert(kCallableVariable);
    m_convertable_map_[kNumLiteral].insert(kVariable);
    m_convertable_map_[kNumLiteral].insert(kSimpleVariable);
    m_convertable_map_[kNumLiteral].insert(kStaticMember);
    m_convertable_map_[kNumLiteral].insert(kArrayPairList);
    m_convertable_map_[kNumLiteral].insert(kPossibleArrayPair);
    m_convertable_map_[kNumLiteral].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kNumLiteral].insert(kArrayPair);
    m_convertable_map_[kNumLiteral].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kNumLiteral].insert(kIssetVariables);
    m_convertable_map_[kNumLiteral].insert(kIssetVariable);
    m_convertable_map_[kNumLiteral].insert(kIdentifier);
    m_convertable_map_[kNumLiteral].insert(kDollarVariable);
    m_convertable_map_[kNumLiteral].insert(kNumLiteral);
    m_convertable_map_[kNumLiteral].insert(kFloatLiteral);
    m_convertable_map_[kNumLiteral].insert(kIntLiteral);
    m_convertable_map_[kNumLiteral].insert(kStringLiteral);
    m_convertable_map_[kFloatLiteral].insert(kNamespaceName);
    m_convertable_map_[kFloatLiteral].insert(kName);
    m_convertable_map_[kFloatLiteral].insert(kNonEmptyArgumentList);
    m_convertable_map_[kFloatLiteral].insert(kArgument);
    m_convertable_map_[kFloatLiteral].insert(kEchoExprList);
    m_convertable_map_[kFloatLiteral].insert(kEchoExpr);
    m_convertable_map_[kFloatLiteral].insert(kForExprs);
    m_convertable_map_[kFloatLiteral].insert(kNonEmptyForExprs);
    m_convertable_map_[kFloatLiteral].insert(kNewExpr);
    m_convertable_map_[kFloatLiteral].insert(kMightDefineExpr);
    m_convertable_map_[kFloatLiteral].insert(kExpr);
    m_convertable_map_[kFloatLiteral].insert(kInlineFunction);
    m_convertable_map_[kFloatLiteral].insert(kFunctionCallme);
    m_convertable_map_[kFloatLiteral].insert(kDereferencableScalar);
    m_convertable_map_[kFloatLiteral].insert(kScalar);
    m_convertable_map_[kFloatLiteral].insert(kConstant);
    m_convertable_map_[kFloatLiteral].insert(kOptionalExpr);
    m_convertable_map_[kFloatLiteral].insert(kCallableVariable);
    m_convertable_map_[kFloatLiteral].insert(kVariable);
    m_convertable_map_[kFloatLiteral].insert(kSimpleVariable);
    m_convertable_map_[kFloatLiteral].insert(kStaticMember);
    m_convertable_map_[kFloatLiteral].insert(kArrayPairList);
    m_convertable_map_[kFloatLiteral].insert(kPossibleArrayPair);
    m_convertable_map_[kFloatLiteral].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kFloatLiteral].insert(kArrayPair);
    m_convertable_map_[kFloatLiteral].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kFloatLiteral].insert(kIssetVariables);
    m_convertable_map_[kFloatLiteral].insert(kIssetVariable);
    m_convertable_map_[kFloatLiteral].insert(kIdentifier);
    m_convertable_map_[kFloatLiteral].insert(kDollarVariable);
    m_convertable_map_[kFloatLiteral].insert(kNumLiteral);
    m_convertable_map_[kFloatLiteral].insert(kFloatLiteral);
    m_convertable_map_[kFloatLiteral].insert(kIntLiteral);
    m_convertable_map_[kFloatLiteral].insert(kStringLiteral);
    m_convertable_map_[kIntLiteral].insert(kNamespaceName);
    m_convertable_map_[kIntLiteral].insert(kName);
    m_convertable_map_[kIntLiteral].insert(kNonEmptyArgumentList);
    m_convertable_map_[kIntLiteral].insert(kArgument);
    m_convertable_map_[kIntLiteral].insert(kEchoExprList);
    m_convertable_map_[kIntLiteral].insert(kEchoExpr);
    m_convertable_map_[kIntLiteral].insert(kForExprs);
    m_convertable_map_[kIntLiteral].insert(kNonEmptyForExprs);
    m_convertable_map_[kIntLiteral].insert(kNewExpr);
    m_convertable_map_[kIntLiteral].insert(kMightDefineExpr);
    m_convertable_map_[kIntLiteral].insert(kExpr);
    m_convertable_map_[kIntLiteral].insert(kInlineFunction);
    m_convertable_map_[kIntLiteral].insert(kFunctionCallme);
    m_convertable_map_[kIntLiteral].insert(kDereferencableScalar);
    m_convertable_map_[kIntLiteral].insert(kScalar);
    m_convertable_map_[kIntLiteral].insert(kConstant);
    m_convertable_map_[kIntLiteral].insert(kOptionalExpr);
    m_convertable_map_[kIntLiteral].insert(kCallableVariable);
    m_convertable_map_[kIntLiteral].insert(kVariable);
    m_convertable_map_[kIntLiteral].insert(kSimpleVariable);
    m_convertable_map_[kIntLiteral].insert(kStaticMember);
    m_convertable_map_[kIntLiteral].insert(kArrayPairList);
    m_convertable_map_[kIntLiteral].insert(kPossibleArrayPair);
    m_convertable_map_[kIntLiteral].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kIntLiteral].insert(kArrayPair);
    m_convertable_map_[kIntLiteral].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kIntLiteral].insert(kIssetVariables);
    m_convertable_map_[kIntLiteral].insert(kIssetVariable);
    m_convertable_map_[kIntLiteral].insert(kIdentifier);
    m_convertable_map_[kIntLiteral].insert(kDollarVariable);
    m_convertable_map_[kIntLiteral].insert(kNumLiteral);
    m_convertable_map_[kIntLiteral].insert(kFloatLiteral);
    m_convertable_map_[kIntLiteral].insert(kIntLiteral);
    m_convertable_map_[kIntLiteral].insert(kStringLiteral);
    m_convertable_map_[kStringLiteral].insert(kNamespaceName);
    m_convertable_map_[kStringLiteral].insert(kName);
    m_convertable_map_[kStringLiteral].insert(kNonEmptyArgumentList);
    m_convertable_map_[kStringLiteral].insert(kArgument);
    m_convertable_map_[kStringLiteral].insert(kEchoExprList);
    m_convertable_map_[kStringLiteral].insert(kEchoExpr);
    m_convertable_map_[kStringLiteral].insert(kForExprs);
    m_convertable_map_[kStringLiteral].insert(kNonEmptyForExprs);
    m_convertable_map_[kStringLiteral].insert(kNewExpr);
    m_convertable_map_[kStringLiteral].insert(kMightDefineExpr);
    m_convertable_map_[kStringLiteral].insert(kExpr);
    m_convertable_map_[kStringLiteral].insert(kInlineFunction);
    m_convertable_map_[kStringLiteral].insert(kFunctionCallme);
    m_convertable_map_[kStringLiteral].insert(kDereferencableScalar);
    m_convertable_map_[kStringLiteral].insert(kScalar);
    m_convertable_map_[kStringLiteral].insert(kConstant);
    m_convertable_map_[kStringLiteral].insert(kOptionalExpr);
    m_convertable_map_[kStringLiteral].insert(kVariableClassName);
    m_convertable_map_[kStringLiteral].insert(kDereferencable);
    m_convertable_map_[kStringLiteral].insert(kCallableExpr);
    m_convertable_map_[kStringLiteral].insert(kCallableVariable);
    m_convertable_map_[kStringLiteral].insert(kVariable);
    m_convertable_map_[kStringLiteral].insert(kSimpleVariable);
    m_convertable_map_[kStringLiteral].insert(kStaticMember);
    m_convertable_map_[kStringLiteral].insert(kArrayPairList);
    m_convertable_map_[kStringLiteral].insert(kPossibleArrayPair);
    m_convertable_map_[kStringLiteral].insert(kNonEmptyArrayPairList);
    m_convertable_map_[kStringLiteral].insert(kArrayPair);
    m_convertable_map_[kStringLiteral].insert(kEncapsVarOffset);
    m_convertable_map_[kStringLiteral].insert(kInternalFunctionsInYacc);
    m_convertable_map_[kStringLiteral].insert(kIssetVariables);
    m_convertable_map_[kStringLiteral].insert(kIssetVariable);
    m_convertable_map_[kStringLiteral].insert(kIdentifier);
    m_convertable_map_[kStringLiteral].insert(kDollarVariable);
    m_convertable_map_[kStringLiteral].insert(kNumLiteral);
    m_convertable_map_[kStringLiteral].insert(kFloatLiteral);
    m_convertable_map_[kStringLiteral].insert(kIntLiteral);
    m_convertable_map_[kStringLiteral].insert(kStringLiteral);
}

void Mutator::init_common_string(string filename) {
    common_string_library_.push_back("DO_NOT_BE_EMPTY");
    if (filename != "") {
        ifstream input_string(filename);
        string s;

        while (getline(input_string, s)) {
            common_string_library_.push_back(s);
        }
    }
}

void Mutator::init_data_library_2d(string filename) {
    ifstream input_file(filename);
    string s;

    cout << "[*] init data_library_2d: " << filename << endl;
    while (getline(input_file, s)) {
        vector<string> v_strbuf;
        auto prev_pos = -1;
        for (int i = 0; i < 3; i++) {
            auto pos = s.find(" ", prev_pos + 1);
            v_strbuf.push_back(s.substr(prev_pos + 1, pos - prev_pos - 1));
            prev_pos = pos;
        }
        v_strbuf.push_back(s.substr(prev_pos + 1, s.size() - prev_pos - 1));

        auto data_type1 = get_datatype_by_string(v_strbuf[0]);
        auto data_type2 = get_datatype_by_string(v_strbuf[2]);
        // cout << "g_data_library_2d_[" << data_type1 << "][" << v_strbuf[1] << "][" << data_type2 <<"].push_back(" << v_strbuf[3] <<")" << endl;
        g_data_library_2d_[data_type1][v_strbuf[1]][data_type2].push_back(v_strbuf[3]);
    }

    return;
}

void Mutator::init_data_library(string filename) {
    ifstream input_file(filename);
    string s;

    cout << "[*] init data_library: " << filename << endl;
    while (getline(input_file, s)) {
        auto pos = s.find(" ");
        if (pos == string::npos)
            continue;
        auto data_type = get_datatype_by_string(s.substr(0, pos));
        auto v = s.substr(pos + 1, s.size() - pos - 1);
        g_data_library_[data_type].push_back(v);
    }

    return;
}

void Mutator::init_value_library() {
    vector<unsigned long> value_lib_init = {0, (unsigned long)LONG_MAX, (unsigned long)ULONG_MAX, (unsigned long)CHAR_BIT, (unsigned long)SCHAR_MIN, (unsigned long)SCHAR_MAX, (unsigned long)UCHAR_MAX, (unsigned long)CHAR_MIN, (unsigned long)CHAR_MAX, (unsigned long)MB_LEN_MAX, (unsigned long)SHRT_MIN, (unsigned long)INT_MIN, (unsigned long)INT_MAX, (unsigned long)SCHAR_MIN, (unsigned long)SCHAR_MIN, (unsigned long)UINT_MAX, (unsigned long)FLT_MAX, (unsigned long)DBL_MAX, (unsigned long)LDBL_MAX, (unsigned long)FLT_MIN, (unsigned long)DBL_MIN, (unsigned long)LDBL_MIN};

    value_library_.insert(value_library_.begin(), value_lib_init.begin(), value_lib_init.end());

    return;
}

void Mutator::init_ir_library(string filename) {
    ifstream input_file(filename);
    string line;

    cout << "[*] init ir_library: " << filename << endl;
    while (getline(input_file, line)) {
        if (line.empty())
            continue;
        // cout << "parsing " << line << endl;
        auto p = parser(line);
        if (p == NULL)
            continue;

        vector<IR *> v_ir;
        auto res = p->translate(v_ir);
        p->deep_delete();
        p = NULL;

        // string strip_sql = extract_struct(res);
        // extract_struct(res);

        add_ir_to_library(res);
        deep_delete(res);
    }
    return;
}

void Mutator::init_ir_library_from_a_file(string filename) {
    char content[0x4000] = {0};
    auto fd = open(filename.c_str(), 0);

    read(fd, content, 0x3fff);
    close(fd);

    auto p = parser(content);
    if (p == NULL) {
        cout << "init " << filename << " failed" << endl;
        return;
    }
    // cout << filename << endl;
    vector<IR *> v_ir;
    auto res = p->translate(v_ir);
    p->deep_delete();
    p = NULL;

    add_ir_to_library(res);
    deep_delete(res);
    cout << "init " << filename << " success" << endl;
    return;
}

void Mutator::init_safe_generate_type(string filename) {
    ifstream input_file(filename);
    string line;

    cout << "[*] init safe generate type: " << filename << endl;
    while (getline(input_file, line)) {
        if (line.empty())
            continue;
        auto node_type = get_nodetype_by_string("k" + line);
        safe_generate_type_.insert(node_type);
    }
}

// Need Minor fix
void Mutator::init(string f_testcase, string f_common_string, string file2d, string file1d, string f_gen_type) {

    // init lib from multiple sql
    if (!f_testcase.empty())
        init_ir_library(f_testcase);

    // Keep
    // init value_library_
    init_value_library();

    // init common_string_library
    if (!f_common_string.empty())
        init_common_string(f_common_string);

    // Fix Here
    // init data_library_2d
    if (!file2d.empty())
        init_data_library_2d(file2d); // NEEDFIX

    if (!file1d.empty())
        init_data_library(file1d);
    if (!f_gen_type.empty())
        init_safe_generate_type(f_gen_type);
    // Fix here
    /*
    relationmap[id_column_name] = id_top_table_name;
    relationmap[id_table_name] = id_top_table_name;
    relationmap[id_create_column_name] = id_create_table_name;
    relationmap[id_pragma_value] = id_pragma_name;
    cross_map[id_top_table_name] = id_create_table_name;
    */
    float_types_.insert({kFloatLiteral});
    int_types_.insert(kIntLiteral);
    string_types_.insert(kStringLiteral);

    /*
    relationmap_[kDataColumnName][kDataTableName] = kRelationSubtype;
    relationmap_[kDataPragmaValue][kDataPragmaKey] = kRelationSubtype;
    relationmap_[kDataTableName][kDataTableName] = kRelationElement;
    relationmap_[kDataColumnName][kDataColumnName] = kRelationElement;
    */

    // split_stmt_types_.insert(kStmt);
    // split_substmt_types_.insert({kStmt, kSelectClause, kSelectStmt});

    // not_mutatable_types_.insert({kProgram, kStmtlist, kStmt, kCreateStmt, kDropStmt, kCreateTableStmt, kCreateIndexStmt, kCreateTriggerStmt, kCreateViewStmt, kCreateFunctionStmt, kDropIndexStmt, kDropTableStmt, kDropTriggerStmt, kDropViewStmt, kSelectStmt, kUpdateStmt, kInsertStmt, kAlterStmt, kReindexStmt});
    // not_mutatable_types_.insert({kProgram});

    init_convertable_ir_type_map();

    return;
}

// Need No Fix
vector<IR *> Mutator::mutate(IR *input) {
    vector<IR *> res;

    if (!lucky_enough_to_be_mutated(input->mutated_times_)) {
        assert(0);
        return res; // return a empty set if the IR is not mutated
    }

    for (int i = 0; i < 0x6; i++) {
        auto tmp = strategy_replace_with_constraint(input);
        if (tmp != NULL) {
            res.push_back(tmp);
        }
    }

    IR *tmp = NULL;
    /*
    tmp = strategy_delete(input);
    if(tmp != NULL){
        res.push_back(tmp);
    }
    */

    tmp = strategy_insert(input);
    if (tmp != NULL) {
        res.push_back(tmp);
    }

    tmp = strategy_replace(input);
    if (tmp != NULL) {
        res.push_back(tmp);
        if (MUTATE_DBG) {
            cout << "Replacing " << input->to_string() << endl;
            cout << "With " << tmp->to_string() << endl;
            getchar();
        }
    }

    input->mutated_times_ += res.size();
    for (auto i : res) {
        if (i == NULL)
            continue;
        i->mutated_times_ = input->mutated_times_;
    }
    return res;
}

bool Mutator::replace(IR *root, IR *old_ir, IR *new_ir) {
    auto parent_ir = locate_parent(root, old_ir);
    // if(parent_ir == NULL) return false;
    assert(parent_ir);
    if (parent_ir->left_ == old_ir) {
        deep_delete(old_ir);
        parent_ir->left_ = new_ir;
        return true;
    } else if (parent_ir->right_ == old_ir) {
        deep_delete(old_ir);
        parent_ir->right_ = new_ir;
        return true;
    }
    return false;
}

// Need No Fix
IR *Mutator::strategy_delete(IR *cur) {
    assert(cur);
    // if(!can_be_mutated(cur)) return NULL;
    // cout << "enter strategy_delete" << endl;
    MUTATESTART

    DOLEFT
    res = deep_copy(cur);
#ifdef MUTATE_UNKNOWN
    if (res->left_ != NULL)
#else
    if (res->left_ != NULL && res->left_->type_ != kUnknown)
#endif
        deep_delete(res->left_);
    res->left_ = NULL; // memory leak

    DORIGHT
    res = deep_copy(cur);
#ifdef MUTATE_UNKNOWN
    if (res->right_ != NULL)
#else
    if (res->right_ != NULL && res->right_->type_ != kUnknown)
#endif
        deep_delete(res->right_);
    res->right_ = NULL;

    DOBOTH
    res = deep_copy(cur);
#ifdef MUTATE_UNKNOWN
    if (res->left_ != NULL)
#else
    if (res->left_ != NULL && res->left_->type_ != kUnknown)
#endif
        deep_delete(res->left_);

#ifdef MUTATE_UNKNOWN
    if (res->right_ != NULL)
#else
    if (res->right_ != NULL && res->right_->type_ != kUnknown)
#endif
        deep_delete(res->right_);
    res->left_ = res->right_ = NULL;

    MUTATEEND
}

IR *Mutator::strategy_insert(IR *cur) {
    // cout << "enter strategy_insert" << endl;
    assert(cur);
    // if(!can_be_mutated(cur)) return NULL;

    IR *res = NULL;
    // auto res = deep_copy(cur);
    auto parent_type = cur->type_;

#ifdef MUTATE_UNKNOWN
    if (cur->right_ == NULL && cur->left_ != NULL) {
#else
    if (cur->right_ == NULL && cur->left_ != NULL && not_unknown(cur->left_)) {
#endif
        auto left_type = cur->left_->type_;
        for (int k = 0; k < 4; k++) {
            auto fetch_ir = get_ir_from_library(parent_type);
            if (fetch_ir->left_ != NULL && fetch_ir->left_->type_ == left_type && fetch_ir->right_ != NULL) {
                res = deep_copy(cur);
                res->right_ = deep_copy(fetch_ir->right_);
                return res;
            }
        }
    }
#ifdef MUTATE_UNKNOWN
    else if (cur->right_ != NULL && cur->left_ == NULL) {
#else
    else if (cur->right_ != NULL && cur->left_ == NULL && not_unknown(cur->right_)) {
#endif
        auto right_type = cur->left_->type_;
        for (int k = 0; k < 4; k++) {
            auto fetch_ir = get_ir_from_library(parent_type);
            if (fetch_ir->right_ != NULL && fetch_ir->right_->type_ == right_type && fetch_ir->left_ != NULL) {
                res = deep_copy(cur);
                res->left_ = deep_copy(fetch_ir->left_);
                return res;
            }
        }
    } else if (cur->left_ == NULL && cur->right_ == NULL) {
        for (int k = 0; k < 4; k++) {
            auto fetch_ir = get_ir_from_library(parent_type);
            if (fetch_ir->right_ != NULL && fetch_ir->left_ != NULL) {
                res = deep_copy(cur);
                res->left_ = deep_copy(fetch_ir->left_);
                res->right_ = deep_copy(fetch_ir->right_);
                return res;
            }
        }
    }

    return res;
}

bool Mutator::is_ir_type_connvertable(IRTYPE a, IRTYPE b) {
    if (a == b)
        return true;
    if (m_convertable_map_.find(a) == m_convertable_map_.end()) {
        return false;
    }
    if (m_convertable_map_[a].find(b) == m_convertable_map_[a].end()) {
        return false;
    }

    return true;
}

IR *Mutator::strategy_replace_with_constraint(IR *cur) {
    assert(cur);
    // if(!can_be_mutated(cur)) return NULL;

    if (cur->op_ == NULL || (cur->op_->prefix_.empty() && cur->op_->middle_.empty() && cur->op_->suffix_.empty())) {
        return NULL;
    }

    IRTYPE replace_type = cur->type_;
    if (m_convertable_map_.find(replace_type) != m_convertable_map_.end()) {
        replace_type = *(random_pick(m_convertable_map_[cur->type_]));
    }
    // if(cur->type_ == kIterationStatement && replace_type == kSelectionStatement) cout << "try to mutate while: ";

    auto res = deep_copy(get_ir_from_library(replace_type));

    if (res->left_ && !cur->left_ || cur->left_ && !res->left_ || res->right_ && !cur->right_ || cur->right_ && !res->right_) {
        deep_delete(res);
        // if(cur->type_ == kIterationStatement) cout << "failed" << endl;
        return NULL;
    }

    if (res->left_ && !is_ir_type_connvertable(res->left_->type_, cur->left_->type_)) {
        deep_delete(res);
        // if(cur->type_ == kIterationStatement) cout << "failed" << endl;
        return NULL;
    }

    if (res->right_ && !is_ir_type_connvertable(res->right_->type_, cur->right_->type_)) {
        deep_delete(res);
        // if(cur->type_ == kIterationStatement) cout << "failed" << endl;
        return NULL;
    }
    if (res->left_)
        deep_delete(res->left_);
    if (res->right_)
        deep_delete(res->right_);

    auto new_cur = deep_copy(cur);
    res->left_ = new_cur->left_;
    res->right_ = new_cur->right_;

    new_cur->left_ = new_cur->right_ = NULL;
    deep_delete(new_cur);

    // if(cur->type_ == kIterationStatement) cout << "success, which becomes " << res->to_string() << endl;
    return res;
}

IR *Mutator::strategy_replace(IR *cur) {
    assert(cur);

    MUTATESTART

    DOLEFT
#ifdef MUTATE_UNKOWN
    if (cur->left_ != NULL) {
#else
    if (cur->left_ != NULL && not_unknown(cur->left_)) {
#endif
        res = deep_copy(cur);

        auto new_node = get_ir_from_library(res->left_->type_);
        // new_node->data_type_ = res->left_->data_type_;
        deep_delete(res->left_);
        if (MUTATE_DBG) {
            cout << "Replacing left: " << cur->left_->to_string() << endl;
            cout << "With: " << new_node->to_string() << endl;
            cout << "Type: " << get_string_by_nodetype(new_node->type_) << endl;
        }
        res->left_ = deep_copy(new_node);
    }

    DORIGHT
#ifdef MUTATE_UNKNOWN
    if (cur->right_ != NULL) {
#else
    if (cur->right_ != NULL && not_unknown(cur->right_)) {
#endif
        res = deep_copy(cur);

        auto new_node = get_ir_from_library(res->right_->type_);
        // new_node->data_type_ = res->right_->data_type_;
        if (MUTATE_DBG) {
            cout << "Replacing right: " << cur->right_->to_string() << endl;
            cout << "With: " << new_node->to_string() << endl;
            cout << "Type: " << get_string_by_nodetype(new_node->type_) << endl;
        }
        deep_delete(res->right_);
        res->right_ = deep_copy(new_node);
    }

    DOBOTH
#ifdef MUTATE_UNKNOWN
    if (cur->left_ != NULL && cur->right_ != NULL) {
#else
    if (cur->left_ != NULL && cur->right_ != NULL && not_unknown(cur->left_) && not_unknown(cur->right_)) {
#endif
        res = deep_copy(cur);

        auto new_left = get_ir_from_library(res->left_->type_);
        auto new_right = get_ir_from_library(res->right_->type_);
        // new_left->data_type_ = res->left_->data_type_;
        // new_right->data_type_ = res->right_->data_type_;
        deep_delete(res->right_);
        res->right_ = deep_copy(new_right);

        deep_delete(res->left_);
        res->left_ = deep_copy(new_left);
        if (MUTATE_DBG) {
            cout << "Replacing both: " << cur->to_string() << endl;
            cout << "Left: " << (new_left->to_string()) << endl;
            cout << "Right: " << (new_right->to_string()) << endl;
            cout << "ORiLeft type: " << get_string_by_nodetype(cur->left_->type_) << endl;
            cout << "ORIRight type: " << get_string_by_nodetype(cur->right_->type_) << endl;
            cout << "Left type: " << get_string_by_nodetype(new_left->type_) << endl;
            cout << "Right type: " << get_string_by_nodetype(new_right->type_) << endl;
        }
    }

    MUTATEEND

    return res;
}

// Need No Fix
bool Mutator::lucky_enough_to_be_mutated(unsigned int mutated_times) {
    if (get_rand_int(mutated_times + 1) < LUCKY_NUMBER) {
        return true;
    }
    return false;
}

pair<string, string> Mutator::get_data_2d_by_type(DATATYPE type1, DATATYPE type2) {
    pair<string, string> res("", "");
    auto size = data_library_2d_[type1].size();

    if (size == 0)
        return res;
    auto rint = get_rand_int(size);

    int counter = 0;
    for (auto &i : data_library_2d_[type1]) {
        if (counter++ == rint) {
            return std::make_pair(i.first, vector_rand_ele(i.second[type2]));
        }
    }
    return res;
}

IR *Mutator::generate_ir_by_type(IRTYPE type) {
    auto ast_node = generate_ast_node_by_type(type);
    ast_node->generate();
    vector<IR *> tmp_vector;
    ast_node->translate(tmp_vector);
    // ast_node->deep_delete();
    assert(tmp_vector.size());
    return tmp_vector[tmp_vector.size() - 1];
}

// Fix, if no item, generate from scratch
IR *Mutator::get_ir_from_library(IRTYPE type) {

    const int generate_prop = 1;
    const int threshold = 0;
    static IR *empty_ir = new IR(kStringLiteral, "");
    // static IR* empty_ir = NULL;//new IR(kStringLiteral, "");
#ifdef USEGENERATE
    if (ir_library_[type].empty() == true || (get_rand_int(400) == 0 && type != kUnknown)) {
        auto ir = generate_ir_by_type(type);
        add_ir_to_library_no_deepcopy(ir);
        return ir;
    }
#endif
    /*
    if(type != kUnknown && (get_rand_int(10) == 0) && safe_generate_type_.find(type) != safe_generate_type_.end()){
        auto ir = generate_ir_by_type(type);
        add_ir_to_library_no_deepcopy(ir);
        return ir;
    }
    */
    // cout << "TRIGGER generate" << endl;
    if (ir_library_[type].empty())
        return empty_ir;
    return vector_rand_ele(ir_library_[type]);
}

// Need No Fix
string Mutator::get_a_string() {
    unsigned com_size = common_string_library_.size();
    unsigned lib_size = string_library_.size();
    unsigned double_lib_size = lib_size * 2;

    unsigned rand_int = get_rand_int(double_lib_size + com_size);
    if (rand_int < double_lib_size) {
        return string_library_[rand_int >> 1];
    } else {
        rand_int -= double_lib_size;
        return common_string_library_[rand_int];
    }
}

// Need No Fix
unsigned long Mutator::get_a_val() {
    assert(value_library_.size());

    return vector_rand_ele(value_library_);
}

// Need No Fix
unsigned long Mutator::hash(string &sql) { return fucking_hash(sql.c_str(), sql.size()); }

// Need No Fix
unsigned long Mutator::hash(IR *root) {
    auto tmp_str = move(root->to_string());
    return this->hash(tmp_str);
}

// Need No Fix
void Mutator::debug(IR *root, set<string> &names) {
    // cout << get_string_by_type(root->type_) << endl;
    if (root->type_ == kIdentifier)
        names.insert(root->str_val_);
    if (root->left_)
        debug(root->left_, names);
    if (root->right_)
        debug(root->right_, names);
}

// Need No Fix
Mutator::~Mutator() {}

void extract_struct_after_mutation(IR *root) {

    if (root->left_) {
        if (root->left_->data_type_ == kDataFixUnit) {
            if (contain_fixme(root->left_)) {
                // auto save_ir_type = root->left_->type_;
                auto save_ir_id = root->left_->id_;
                auto save_scope = root->left_->scope_id_;
                deep_delete(root->left_);
                root->left_ = new IR(kStringLiteral, "FIXME");
                // root->left_->type_ = save_ir_type;
                root->left_->scope_id_ = save_scope;
                root->left_->id_ = save_ir_id;
            }
        } else {
            extract_struct_after_mutation(root->left_);
        }
    }
    if (root->right_) {
        if (root->right_->data_type_ == kDataFixUnit) {
            if (contain_fixme(root->right_)) {
                auto save_ir_id = root->right_->id_;
                auto save_ir_type = root->right_->type_;
                auto save_scope = root->right_->scope_id_;
                deep_delete(root->right_);
                root->right_ = new IR(kStringLiteral, "FIXME");
                // root->right_->type_ = save_ir_type;
                root->right_->scope_id_ = save_scope;
                root->right_->id_ = save_ir_id;
            }
        } else {
            extract_struct_after_mutation(root->right_);
        }
    }
    return;
}
void Mutator::extract_struct(IR *root) {
    auto type = root->type_;

#ifndef SYNTAX_ONLY
    if (root->left_) {
        if (root->left_->data_type_ == kDataFixUnit) {
            deep_delete(root->left_);
            root->left_ = new IR(kStringLiteral, "FIXME");
        } else {
            extract_struct(root->left_);
        }
    }
    if (root->right_) {
        if (root->right_->data_type_ == kDataFixUnit) {
            deep_delete(root->right_);
            root->right_ = new IR(kStringLiteral, "FIXME");
        } else {
            extract_struct(root->right_);
        }
    }
#else
    if (root->left_) {
        extract_struct(root->left_);
    }
    if (root->right_) {
        extract_struct(root->right_);
    }
#endif

    if (root->left_ || root->right_)
        return;

    // if(root->str_val_.empty() == false){
    if (root->data_type_ != kDataWhatever && root->data_type_ != kDataVarName && root->data_type_ != kDataClassName && root->data_type_ != kDataFunctionName) {
        root->str_val_ = "x";
        return;
    }

    if (string_types_.find(type) != string_types_.end()) {
        root->str_val_ = "'x'";
    } else if (int_types_.find(type) != int_types_.end()) {
        root->int_val_ = 1;
    } else if (float_types_.find(type) != float_types_.end()) {
        root->float_val_ = 1.0;
    }
}

void Mutator::reset_data_library() {
    data_library_.clear();
    data_library_2d_.clear();
}

string Mutator::parse_data(string &input) {
    string res;
    if (!input.compare("_int_")) {
        res = to_string(get_a_val());
    } else if (!input.compare("_empty_")) {
        res = "";
    } else if (!input.compare("_boolean_")) {
        if (get_rand_int(2) == 0)
            res = "false";
        else
            res = "true";
    } else if (!input.compare("_string_")) {
        res = get_a_string();
    } else {
        res = input;
    }

    return res;
}

bool Mutator::validate(IR *&root) {
    reset_data_library();
    string sql = root->to_string();
    auto ast = parser(sql);
    if (ast == NULL)
        return false;
    deep_delete(root);
    root = NULL;

    if (ast == NULL) {
        return false;
    }

    vector<IR *> ir_vector;
    ast->translate(ir_vector);
    ast->deep_delete();

    root = ir_vector[ir_vector.size() - 1];
    reset_id_counter();
    // bool fix_res = fix(root);
    if (fix(root) == false) {
        // deep_delete(root);
        // root = NULL;
        return false;
    }

    return true;
}

unsigned int calc_node(IR *root) {
    unsigned int res = 0;
    if (root->left_)
        res += calc_node(root->left_);
    if (root->right_)
        res += calc_node(root->right_);

    return res + 1;
}

bool Mutator::fix(IR *root) {
    map<IR **, IR *> m_save;
    bool res = true;

    auto stmts = split_to_stmt(root, m_save, split_stmt_types_);
    // cout << "Statement size: " << stmts.size() << endl;
    // cout << "split set size: " << split_stmt_types_.size() << endl;

    // check statement number
    if (stmts.size() > 8) {
        connect_back(m_save);
        return false;
    }

    clear_scope_library(true);
    for (auto &stmt : stmts) {
        // cout << "Stmt: " << stmt->to_string() << endl;
        map<IR **, IR *> m_substmt_save;
        auto substmts = split_to_stmt(stmt, m_substmt_save, split_substmt_types_);

        // check here
        //直接检查数量，字句不能太多，下面再检查node
        int stmt_num = substmts.size();
        if (stmt_num > 4) {
            connect_back(m_save);
            connect_back(m_substmt_save);
            return false;
        }
        for (auto &substmt : substmts) {
            clear_scope_library(false);
            // auto tmp_scope = scope_library_;
            // tmp_scope[0] = tmp_scope_define;
            // cout << "[now fix]" << substmt->to_string() << endl;
            int tmp_node_num = calc_node(substmt);
            if ((stmt_num == 1 && tmp_node_num > 150) || tmp_node_num > 120) {
                connect_back(m_save);
                connect_back(m_substmt_save);
                return false;
            }
            res = fix_one(substmt, scope_library_) && res;

            if (res == false) { // early break
                connect_back(m_save);
                connect_back(m_substmt_save);
                return false;
            }
            // cout << "After fix: " << substmt->to_string() << endl;
        }
        res = connect_back(m_substmt_save) && res;
    }
    res = connect_back(m_save) && res;

    return res;
}

vector<IR *> Mutator::split_to_stmt(IR *root, map<IR **, IR *> &m_save, set<IRTYPE> &split_set) {
    vector<IR *> res;
    deque<IR *> bfs = {root};

    while (!bfs.empty()) {
        auto node = bfs.front();
        bfs.pop_front();

        /*    if(node && find(split_set.begin(), split_set.end(), node->type_) != split_set.end()){
                res.push_back(node);
                m_save[&node] = node; select a from | (select x from y).
            }
    */
        if (node && node->left_)
            bfs.push_back(node->left_);
        if (node && node->right_)
            bfs.push_back(node->right_);

        if (node->left_ && find(split_set.begin(), split_set.end(), node->left_->type_) != split_set.end()) {
            res.push_back(node->left_);
            m_save[&node->left_] = node->left_;
            node->left_ = NULL;
        }
        if (node->right_ && find(split_set.begin(), split_set.end(), node->right_->type_) != split_set.end()) {
            res.push_back(node->right_);
            m_save[&node->right_] = node->right_;
            node->right_ = NULL;
        }
    }

    if (find(split_set.begin(), split_set.end(), root->type_) != split_set.end())
        res.push_back(root);
    /*

        cout << "now root: " << root->to_string() << endl;
        for(auto &i: res)
            cout << "[in res] " << i->to_string() << endl;
    */

    // reverse(res.begin(), res.end());
    return res;
}

bool Mutator::connect_back(map<IR **, IR *> &m_save) {
    for (auto &iter : m_save) {
        *(iter.first) = iter.second;
    }
    return true;
}

static set<IR *> visited;

bool Mutator::fix_one(IR *stmt_root, map<int, map<DATATYPE, vector<IR *>>> &scope_library) {
    visited.clear();
    analyze_scope(stmt_root);
    auto graph = build_graph(stmt_root, scope_library);

#ifdef GRAPHLOG
    for (auto &iter : graph) {
        cout << "Node: " << iter.first->to_string() << " connected with:" << endl;
        for (auto &k : iter.second) {
            cout << k->to_string() << endl;
        }
        cout << "--------" << endl;
    }
    cout << "OUTPUT END" << endl;
#endif
    // exit(0);
    return fill_stmt_graph(graph);
}

void Mutator::analyze_scope(IR *stmt_root) {
    if (stmt_root->left_) {
        analyze_scope(stmt_root->left_);
    }
    if (stmt_root->right_) {
        analyze_scope(stmt_root->right_);
    }

    auto data_type = stmt_root->data_type_;
    if (data_type == kDataWhatever)
        return;

    // auto data_flag = stmt_root->data_flag_;
    scope_library_[stmt_root->scope_][data_type].push_back(stmt_root);
}

map<IR *, vector<IR *>> Mutator::build_graph(IR *stmt_root, map<int, map<DATATYPE, vector<IR *>>> &scope_library) {
    map<IR *, vector<IR *>> res;
    deque<IR *> bfs = {stmt_root};

    /*
    cout << "======scope_library=======" << endl;
    for(auto &i: scope_library){
        cout << "scope level:" << i.first << endl;
        for(auto &j: i.second){
            cout << "\tDATATYPE: " << j.first << endl;
            for(auto &x: j.second){
                cout << "\t\tname:" << x->str_val_ << endl;
            }

        }
    }
    cout << "======scope_end=======" << endl;
    */
    while (!bfs.empty()) {
        auto node = bfs.front();
        bfs.pop_front();

        auto cur_scope = node->scope_;
        auto cur_data_flag = node->data_flag_;
        auto cur_data_type = node->data_type_;

        // fill literal value here
        if (find(int_types_.begin(), int_types_.end(), node->type_) != int_types_.end()) {
            // cout  << "Here" << endl;
            if (get_rand_int(100) > 50)
                node->int_val_ = vector_rand_ele(value_library_);
            else
                node->int_val_ = get_rand_int(100);
        } else if (find(float_types_.begin(), float_types_.end(), node->type_) != float_types_.end()) {
            node->float_val_ = (double)(get_rand_int(100000000));
        }

        if (node->left_)
            bfs.push_back(node->left_);
        if (node->right_)
            bfs.push_back(node->right_);
        if (cur_data_type == kDataWhatever)
            continue;

        res[node];
        // if(isDefine(cur_data_flag))
        //    cur_scope = cur_scope;
        // else
        cur_scope--;

        if (relationmap_.find(cur_data_type) != relationmap_.end()) {
            auto &target_data_type_map = relationmap_[cur_data_type];
            for (auto &target : target_data_type_map) {
                IR *pick_node = NULL;
                if (isMapToClosestOne(cur_data_flag)) {
                    pick_node = find_closest_node(stmt_root, node, target.first);
                    if (pick_node && pick_node->scope_ != cur_scope) {
                        pick_node = NULL;
                    }
                } else {

                    if (!node->str_val_.empty()) {
                        // cout << "[debug graph " << node->str_val_ << "]  cur_scope:" << cur_scope << " target_datatype:" << target.first << endl;
                    }

                    if (!isDefine(cur_data_flag) || relationmap_[cur_data_type][target.first] != kRelationElement) {
                        if (!scope_library[cur_scope][target.first].empty())
                            pick_node = vector_rand_ele(scope_library[cur_scope][target.first]);
                    }
                    // else
                    // cout << "this is create tablename" << endl;
                }
                if (pick_node != NULL)
                    res[pick_node].push_back(node);
            }
        }
    }

    return res;
}

bool Mutator::fill_stmt_graph(map<IR *, vector<IR *>> &graph) {
    bool res = true;
    // cout << "Filling?" << endl;
    map<IR *, bool> zero_indegrees;
    for (auto &iter : graph) {
        if (zero_indegrees.find(iter.first) == zero_indegrees.end()) {
            zero_indegrees[iter.first] = true;
        }
        for (auto ir : iter.second) {
            zero_indegrees[ir] = false;
        }
    }
    for (auto &iter : graph) {
        auto type1 = iter.first->data_type_;
        auto beg = iter.first;
        if (zero_indegrees[beg] == false || visited.find(beg) != visited.end()) {
            continue;
        }
        res &= fill_one(iter.first);
        res &= fill_stmt_graph_one(graph, iter.first);
    }

    return res;
}

bool Mutator::fill_stmt_graph_one(map<IR *, vector<IR *>> &graph, IR *ir) {
    if (graph.find(ir) == graph.end())
        return true;

    bool res = true;
    auto type = ir->data_type_;
    auto &vec = graph[ir];

    if (!vec.empty()) {
        for (auto d : vec) {
            res = res & fill_one_pair(ir, d);
            res = res & fill_stmt_graph_one(graph, d);
        }
    }
    return res;
}

static bool replace_in_vector(string &old_str, string &new_str, vector<string> &victim) {
    for (int i = 0; i < victim.size(); i++) {
        if (victim[i] == old_str) {
            victim[i] = new_str;
            return true;
        }
    }
    return false;
}

static bool remove_in_vector(string &str_to_remove, vector<string> &victim) {
    for (auto iter = victim.begin(); iter != victim.end(); iter++) {
        if (*iter == str_to_remove) {
            victim.erase(iter);
            return true;
        }
    }
    return false;
}

bool Mutator::remove_one_from_datalibrary(DATATYPE datatype, string &key) { return remove_in_vector(key, data_library_[datatype]); }

bool Mutator::replace_one_from_datalibrary(DATATYPE datatype, string &old_str, string &new_str) { return replace_in_vector(old_str, new_str, data_library_[datatype]); }

bool Mutator::remove_one_pair_from_datalibrary_2d(DATATYPE p_datatype, DATATYPE c_data_type, string &p_key) {
    for (auto &value : data_library_2d_[p_datatype][p_key][c_data_type]) {
        remove_one_from_datalibrary(c_data_type, value);
    }

    data_library_2d_[p_datatype][p_key].erase(c_data_type);
    if (data_library_2d_[p_datatype][p_key].empty()) {
        remove_one_from_datalibrary(p_datatype, p_key);
        data_library_2d_[p_datatype].erase(p_key);
    }

    return true;
}

#define has_element(a, b) (find(a.begin(), a.end(), b) != (a).end())
#define has_key(a, b) ((a).find(b) != (a).end())

bool Mutator::replace_one_value_from_datalibray_2d(DATATYPE p_datatype, DATATYPE c_data_type, string &p_key, string &old_c_value, string &new_c_value) {
    replace_one_from_datalibrary(c_data_type, old_c_value, new_c_value);
    replace_in_vector(old_c_value, new_c_value, data_library_2d_[p_datatype][p_key][c_data_type]);
    return true;
}

bool Mutator::fill_one(IR *ir) {
    auto type = ir->data_type_;
    visited.insert(ir);
    // cout << "Here" << endl;
    if (isDefine(ir->data_flag_)) {
        string new_name = gen_id_name();
        // cout << "generating new name " << new_name << endl;
        data_library_[type].push_back(new_name);
        ir->str_val_ = new_name;

        for (auto iter : relationmap_) {
            for (auto iter2 : iter.second) {
                if (iter2.first == type && iter2.second == kRelationSubtype) {
                    data_library_2d_[type][new_name];
                }
            }
        }
        return true;
    } else if (isAlias(ir->data_flag_)) {
        string alias_target;
        if (data_library_[type].size() != 0)
            alias_target = vector_rand_ele(data_library_[type]);
        else {
            alias_target = get_rand_int(2) ? "v0" : "v1";
            // cout << "Alias empty?" << endl;
        }

        string new_name = gen_id_name();
        data_library_[type].push_back(new_name);
        ir->str_val_ = new_name;
        // cout << "HERE\n" << endl;

        if (has_key(data_library_2d_, type)) {
            if (has_key(data_library_2d_[type], alias_target)) {
                data_library_2d_[type][new_name] = data_library_2d_[type][alias_target];
                // data_library_[type].push_back(new_name);
            }
        }
        return true;
    }

    else if (data_library_.find(type) != data_library_.end()) {
        if (data_library_[type].empty()) {
            ir->str_val_ = "v0";
            return false;
        }
        ir->str_val_ = vector_rand_ele(data_library_[type]);
        if (isUndefine(ir->data_flag_)) {
            remove_one_from_datalibrary(ir->data_type_, ir->str_val_);
            if (has_key(data_library_2d_, type) && has_key(data_library_2d_[type], ir->str_val_)) {
                // for(auto &c_data_type: data_library_2d_[type][ir->str_val_]){
                for (auto itr = data_library_2d_[type][ir->str_val_].begin(); has_key(data_library_2d_[type], ir->str_val_) && itr != data_library_2d_[type][ir->str_val_].end(); itr++) {
                    auto c_data_type = *itr;
                    remove_one_pair_from_datalibrary_2d(type, c_data_type.first, ir->str_val_);
                    itr--;
                    if (!has_key(data_library_2d_[type], ir->str_val_))
                        break;
                }
            }
        }
        return true;
    } else if (g_data_library_.find(type) != g_data_library_.end()) {
        if (g_data_library_[type].empty()) {
            return false;
        }
        ir->str_val_ = vector_rand_ele(g_data_library_[type]);
        return true;
    } else if (g_data_library_2d_.find(type) != g_data_library_2d_.end()) {
        int choice = get_rand_int(g_data_library_2d_[type].size());
        auto iter = g_data_library_2d_[type].begin();
        while (choice > 0) {
            iter++;
            choice--;
        }
        ir->str_val_ = iter->first;
        return true;
    } else {
        return false;
    }
    return true;
}
/*
st
*/
bool Mutator::fill_one_pair(IR *parent, IR *child) {
    // cout << "THERE: " << parent->str_val_ << endl;
    visited.insert(child);

    bool is_define = isDefine(child->data_flag_);
    bool is_replace = isReplace(child->data_flag_);
    bool is_undefine = isUndefine(child->data_flag_);
    bool is_alias = isAlias(child->data_flag_);

    string new_name = "";
    if (is_define || is_replace || is_alias) {
        new_name = gen_id_name();
    }

    auto p_type = parent->data_type_;
    auto c_type = child->data_type_;
    auto p_str = parent->str_val_;

    auto r_type = relationmap_[c_type][p_type];
    switch (r_type) {
    case kRelationElement:
        // if(is)

        if (is_replace) {
            child->str_val_ = new_name;
            replace_one_from_datalibrary(c_type, p_str, new_name);

            if (has_key(data_library_2d_, p_type)) {
                // cout << "Here" << endl;
                if (has_key(data_library_2d_[p_type], p_str)) {
                    auto tmp = data_library_2d_[p_type].extract(p_str);
                    tmp.key() = new_name;
                    data_library_2d_[p_type].insert(move(tmp));
                    // cout << "There" << endl;
                }
            } else {
                for (auto &i1 : data_library_2d_) {
                    for (auto &i2 : i1.second) {
                        for (auto &i3 : i2.second) {
                            if (i3.first == c_type) {
                                if (has_element(i3.second, p_str)) {
                                    replace_in_vector(p_str, new_name, i3.second);
                                    goto END;
                                }
                            }
                        }
                    }
                }
            }
        } else if (is_alias) {
            child->str_val_ = new_name;

            if (has_key(data_library_2d_, p_type)) {
                // cout << "Here" << endl;
                if (has_key(data_library_2d_[p_type], p_str)) {
                    data_library_2d_[p_type][new_name] = data_library_2d_[p_type][p_str];
                    data_library_[p_type].push_back(new_name);
                    // data_library_2d_[p_type].insert(tmp);
                    // tmp.key() = new_name;
                    // data_library_2d_[p_type].insert(move(tmp));
                    // cout << "Alias: " << p_str << " to " << new_name << endl;
                }
            }
            /*
            else{
                for(auto &i1: data_library_2d_){
                    for(auto &i2: i1.second){
                        for(auto &i3: i2.second){
                            if(i3.first == c_type){
                                if(has_element(i3.second, p_str)){
                                    replace_in_vector(p_str, new_name, i3.second);
                                    goto END;
                                }
                            }
                        }
                    }
                }
            }
            */
        } else {
            child->str_val_ = p_str;
        }
    END:
        break;

    case kRelationSubtype:
        if (data_library_2d_.find(p_type) != data_library_2d_.end()) {
            if (data_library_2d_[p_type].find(p_str) != data_library_2d_[p_type].end()) {
                if (is_define) {
                    data_library_2d_[p_type][p_str][c_type].push_back(new_name);
                    child->str_val_ = new_name;
                    data_library_[c_type].push_back(new_name);
                    break;
                } else if (is_undefine) {
                    if ((data_library_2d_[p_type][p_str][c_type]).empty()) {
                        child->str_val_ = "v1";
                        break;
                    }
                    child->str_val_ = vector_rand_ele(data_library_2d_[p_type][p_str][c_type]);
                    remove_in_vector(child->str_val_, data_library_2d_[p_type][p_str][c_type]);
                    remove_in_vector(child->str_val_, data_library_[c_type]);
                    break;
                } else if (data_library_2d_[p_type][p_str].find(c_type) != data_library_2d_[p_type][p_str].end()) {
                    if (data_library_2d_[p_type][p_str][c_type].empty() == false) {
                        child->str_val_ = vector_rand_ele(data_library_2d_[p_type][p_str][c_type]);
                    }
                } else {
                    // cout << "parent: " << p_str << endl;
                    if (data_library_[c_type].empty()) {
                        if (get_rand_int(2) == 1) {
                            child->str_val_ = "v0";
                        } else {
                            child->str_val_ = "v1";
                        }
                    } else
                        child->str_val_ = vector_rand_ele(data_library_[c_type]);
                }
            } else {
                // cout << "WTF?" << endl;
            }
        } else if (g_data_library_2d_.find(p_type) != g_data_library_2d_.end()) {
            // cout << "HERE go?" << endl;
            // cout << p_str << endl;
            if (g_data_library_2d_[p_type].find(p_str) != g_data_library_2d_[p_type].end()) {
                // cout << "HERE go2?" << endl;
                if (g_data_library_2d_[p_type][p_str].find(c_type) != g_data_library_2d_[p_type][p_str].end()) {
                    // cout << "HERE go3?" << endl;
                    if (g_data_library_2d_[p_type][p_str][c_type].empty() == false) {
                        child->str_val_ = vector_rand_ele(g_data_library_2d_[p_type][p_str][c_type]);
                    }
                }
            }
        } else {
            return false;
        }

        break;

    default:
        assert(0);
        break;
    }

    return true;
}

void Mutator::clear_scope_library(bool clear_define) {
    int level = clear_define ? 0 : 1;
    int sz = scope_library_.size();
    scope_library_.clear();
    /*
    for(int i = level; i < sz; i++){
        scope_library_[i].clear();
    }
    */

    return;
}

static IR *search_mapped_ir(IR *ir, DATATYPE type) {
    vector<IR *> to_search;
    vector<IR *> backup;
    to_search.push_back(ir);
    while (!to_search.empty()) {
        for (auto i : to_search) {
            if (i->data_type_ == type) {
                return i;
            }
            if (i->left_) {
                backup.push_back(i->left_);
            }
            if (i->right_) {
                backup.push_back(i->right_);
            }
        }
        to_search = move(backup);
        backup.clear();
    }
    return NULL;
}

IR *Mutator::find_closest_node(IR *stmt_root, IR *node, DATATYPE type) {
    auto cur = node;
    while (true) {
        auto parent = locate_parent(stmt_root, cur);
        if (!parent)
            break;
        bool flag = false;
        while (parent->left_ == NULL || parent->right_ == NULL) {
            cur = parent;
            parent = locate_parent(stmt_root, cur);
            if (!parent) {
                flag = true;
                break;
            }
        }
        if (flag)
            return NULL;

        auto search_root = parent->left_ == cur ? parent->right_ : parent->left_;
        auto res = search_mapped_ir(search_root, type);
        if (res)
            return res;

        cur = parent;
    }
    return NULL;
}

bool Mutator::can_be_mutated(IR *cur) {
#ifdef SYNTAX_ONLY
    return true;
#else
    bool res = true;
    if (cur->data_type_ == kDataVarDefine || isDefine(cur->data_flag_) || cur->data_type_ == kDataVarType || cur->data_type_ == kDataClassType) {
        return false;
    }
    if (cur->left_)
        res = res && can_be_mutated(cur->left_);
    if (cur->right_)
        res = res && can_be_mutated(cur->right_);
    return res;
#endif
}

bool contain_fixme(IR *ir) {
    bool res = false;
    if (ir->left_ || ir->right_) {
        if (ir->left_) {
            res = res || contain_fixme(ir->left_);
        }
        if (ir->right_) {
            res = res || contain_fixme(ir->right_);
        }
        return res;
    }

    if (!ir->str_val_.empty() && ir->str_val_ == "FIXME") {
        return true;
    }

    return false;
}

void remove_definition(IR *&ir) {

    if (ir->data_type_ == kDataVarDefine) {
        deep_delete(ir);
        ir = NULL;
        return;
    }

    if (ir->left_) {
        if (ir->left_->data_type_ == kDataVarDefine) {
            deep_delete(ir->left_);
            ir->left_ = NULL;
        } else {
            remove_definition(ir->left_);
        }
    }
    if (ir->right_) {
        if (ir->right_->data_type_ == kDataVarDefine) {
            deep_delete(ir->right_);
            ir->right_ = NULL;
        } else {
            remove_definition(ir->right_);
        }
    }
}
