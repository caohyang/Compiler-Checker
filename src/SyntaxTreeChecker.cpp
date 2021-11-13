#include "SyntaxTreeChecker.h"

using namespace SyntaxTree;

void SyntaxTreeChecker::visit(Assembly& node) {
    this->enter_scope();
    for (auto def : node.global_defs) {
        def->accept(*this);
    }
    this->exit_scope();
}

void SyntaxTreeChecker::visit(FuncDef& node) {
    this->enter_scope();
    PtrFunction ptr = this->lookup_functions(node.name);
    if (ptr != nullptr){
        err.error(node.loc, "The function has ALREADY been defined.");
        exit(int(ErrorType::FuncDuplicated));
    }
    this->declare_functions(node.name, node.ret_type, node.param_list);
    if (node.param_list != nullptr)
        node.param_list->accept(*this);
    node.body->accept(*this);
    this->exit_scope();
}

void SyntaxTreeChecker::visit(BinaryExpr& node) {
    node.lhs->accept(*this);
    bool lhs_int = this->Expr_int;
    node.rhs->accept(*this);
    bool rhs_int = this->Expr_int;
    if (node.op == SyntaxTree::BinOp::MODULO) {
        if (!lhs_int || !rhs_int) {
            err.error(node.loc, "Operands of modulo should be integers.");
            exit(int(ErrorType::Modulo));
        }
    }
    this->Expr_int = lhs_int & rhs_int;
}

void SyntaxTreeChecker::visit(UnaryExpr& node) {
    node.rhs->accept(*this);
}

void SyntaxTreeChecker::visit(LVal& node) {
    bool is_declared = this->lookup_variable(node.name, this->Expr_int);;
    if (!is_declared){
        err.error(node.loc, "The variable has NOT been defined.");
        exit(int(ErrorType::VarUnknown));
    }
    for (auto element : node.array_index)
        if (element != nullptr)
            element->accept(*this);
}

void SyntaxTreeChecker::visit(Literal& node) {
    this->Expr_int = (node.literal_type == SyntaxTree::Type::INT);
}

void SyntaxTreeChecker::visit(ReturnStmt& node) {
    node.ret->accept(*this);
}

void SyntaxTreeChecker::visit(VarDef& node) {
    /*
    std::cout << node.name << ' ' << this->variables.size() << std::endl;
    for (long unsigned int i=0;i<this->variables.size();i++){
        std::unordered_map<std::string, bool> tmp = this->variables[i];
        for (auto x : tmp)
            std::cout << x.first << ' ' << x.second << std::endl;
    }
    */

    bool flag = this->declare_variable(node.name, node.btype);
    if (!flag){
        err.error(node.loc, "The variable has ALREADY been defined.");
        exit(int(ErrorType::VarDuplicated));
    }
    this->lookup_variable(node.name, this->Expr_int);
    if (node.is_inited) {
        node.initializers->accept(*this);
    }
}

void SyntaxTreeChecker::visit(AssignStmt& node) {
    node.target->accept(*this);
    node.value->accept(*this);
}

void SyntaxTreeChecker::visit(FuncCallStmt& node) {
    PtrFunction ptr = this->lookup_functions(node.name);
    if (ptr == nullptr){
        err.error(node.loc, "The function has NOT been defined.");
        exit(int(ErrorType::FuncUnknown));
    }
    if (node.params.size() != ptr->args_int.size()){
        err.error(node.loc, "The NUMBERS of parameters are DIFFERENT.");
        exit(int(ErrorType::FuncParams));
    }
    int cnt = 0;
    for (auto funcparam : node.params){
        funcparam->accept(*this);
        //std::cout << node.name << ' ' << this->Expr_int << ' ' << ptr->args_int[cnt] << std::endl;
        if (this->Expr_int != ptr->args_int[cnt]){
            err.error(node.loc, "The TYPES of parameters are DIFFERENT.");
            exit(int(ErrorType::FuncParams));
        }
        cnt ++;
    }
    this->Expr_int = ptr->ret_int;
}

void SyntaxTreeChecker::visit(BlockStmt& node) {
    this->enter_scope();
    for (auto stmt : node.body){   
        if (stmt != nullptr) 
            stmt->accept(*this);
    }
    this->exit_scope();
}

void SyntaxTreeChecker::visit(EmptyStmt& node) {}

void SyntaxTreeChecker::visit(SyntaxTree::ExprStmt& node) {
    node.exp->accept(*this);
}

void SyntaxTreeChecker::visit(SyntaxTree::FuncParam& node) {
    bool flag = this->declare_variable(node.name, node.param_type);
    if (!flag){
        err.error(node.loc, "The variable has ALREADY been defined.");
        exit(int(ErrorType::VarDuplicated));
    }
    this->lookup_variable(node.name, this->Expr_int);
}

void SyntaxTreeChecker::visit(SyntaxTree::FuncFParamList& node) {
    for (auto funcparam : node.params)
        funcparam->accept(*this);
}

void SyntaxTreeChecker::visit(SyntaxTree::BinaryCondExpr& node) {
    node.lhs->accept(*this);
    node.rhs->accept(*this);
}
void SyntaxTreeChecker::visit(SyntaxTree::UnaryCondExpr& node) {
    node.rhs->accept(*this);
}
void SyntaxTreeChecker::visit(SyntaxTree::IfStmt& node) {
    node.cond_exp->accept(*this);
    node.if_statement->accept(*this);
    if (node.else_statement != nullptr)
        node.else_statement->accept(*this);
}
void SyntaxTreeChecker::visit(SyntaxTree::WhileStmt& node) {
    node.cond_exp->accept(*this);
    this->enter_scope();
    node.statement->accept(*this);
    this->exit_scope();
}
void SyntaxTreeChecker::visit(SyntaxTree::BreakStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::ContinueStmt& node) {}

void SyntaxTreeChecker::visit(SyntaxTree::InitVal& node) {
    if (node.isExp) {
        node.expr->accept(*this);
    } 
    else {
        for (auto element : node.elementList) {
            element->accept(*this);
        }
    }
}