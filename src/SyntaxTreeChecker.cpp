#include "SyntaxTreeChecker.h"

using namespace SyntaxTree;

void SyntaxTreeChecker::visit(Assembly& node) {
    for (auto def : node.global_defs) {
        def->accept(*this);
    }
}

void SyntaxTreeChecker::visit(FuncDef& node) {}

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

void SyntaxTreeChecker::visit(LVal& node) {}

void SyntaxTreeChecker::visit(Literal& node) {
    this->Expr_int = (node.literal_type == SyntaxTree::Type::INT);
}

void SyntaxTreeChecker::visit(ReturnStmt& node) {}

void SyntaxTreeChecker::visit(VarDef& node) {
    if (node.is_inited) {
        node.initializers->accept(*this);
    }
}

void SyntaxTreeChecker::visit(AssignStmt& node) {}
void SyntaxTreeChecker::visit(FuncCallStmt& node) {}
void SyntaxTreeChecker::visit(BlockStmt& node) {}
void SyntaxTreeChecker::visit(EmptyStmt& node) {}
void SyntaxTreeChecker::visit(SyntaxTree::ExprStmt& node) {}
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
    } 
    else {
        for (auto element : node.elementList) {
            element->accept(*this);
        }
    }
}