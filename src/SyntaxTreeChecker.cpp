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
    bool is_declared = this->lookup_variable(node.name, this->Expr_int);
    if (!is_declared){
        err.error(node.loc, "The variable has NOT been defined.");
        exit(int(ErrorType::VarUnknown));
    }
}

void SyntaxTreeChecker::visit(Literal& node) {
    this->Expr_int = (node.literal_type == SyntaxTree::Type::INT);
}

void SyntaxTreeChecker::visit(ReturnStmt& node) {
    node.ret->accept(*this);
}

void SyntaxTreeChecker::visit(VarDef& node) {
    bool is_declared = this->lookup_variable(node.name, this->Expr_int);
    if (is_declared){
        err.error(node.loc, "The variable has ALREADY been defined.");
        exit(int(ErrorType::VarDuplicated));
    }
    this->declare_variable(node.name, node.btype);
    if (node.is_inited) {
        node.initializers->accept(*this);
    }
}

void SyntaxTreeChecker::visit(AssignStmt& node) {}
void SyntaxTreeChecker::visit(FuncCallStmt& node) {}
void SyntaxTreeChecker::visit(BlockStmt& node) {
    this->enter_scope();
    for (auto stmt : node.body)
        stmt->accept(*this);
    this->exit_scope();
}
void SyntaxTreeChecker::visit(EmptyStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::ExprStmt& node) {
    node.exp->accept(*this);
}
void SyntaxTreeChecker::visit(SyntaxTree::FuncParam& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::FuncFParamList& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::BinaryCondExpr& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::UnaryCondExpr& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::IfStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::WhileStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::BreakStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::ContinueStmt& node) {}

void SyntaxTreeChecker::visit(SyntaxTree::InitVal& node) {
    if (node.isExp) {
        node.expr->accept(*this);
        printf("We are here in class Initval.");
    } 
    else {
        for (auto element : node.elementList) {
            element->accept(*this);
        }
    }
}