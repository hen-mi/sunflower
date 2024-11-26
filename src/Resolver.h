#pragma once

#include <vector>
#include <map>
#include <Evaluator.h>
#include <AST/Expressions.h>
#include <AST/Statements.h>
#include <Token.h>
namespace Sunflower
{

    class Evaluator;

    class Resolver : public Sunflower::ExprVisitor, public Sunflower::StmtVisitor 
    {

    public:
        Resolver(Evaluator& evaluator);
        void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);
        void resolve(Stmt& statement);
        void resolve(Expr& expression);
        std::any visitBlockStmt(BlockStmt& stmt) override;
        std::any visitExprStmt(ExprStmt& stmt) override;
        std::any visitPrintStmt(PrintStmt& stmt) override;
        std::any visitIfStmt(IfStmt& stmt) override;
        std::any visitWhileStmt(WhileStmt& stmt) override;
        std::any visitFunctionStmt(FunctionStmt& stmt) override;
        std::any visitReturnStmt(ReturnStmt& stmt) override;
        std::any visitVarStmt(VarStmt& stmt) override;
        std::any visitBinaryExpr(BinaryExpr& expr) override;
        std::any visitUnaryExpr(UnaryExpr& node) override;
        std::any visitGroupingExpr(GroupingExpr& node) override;
        std::any visitLiteralExpr(Literal& node) override;
        std::any visitVarExpr(VarExpr& node) override;
        std::any visitAssignExpr(AssignExpr& node) override;
        std::any visitLogicalExpr(LogicalExpr& node) override;
        std::any visitCallExpr(CallExpr& node) override;

    private:

        Evaluator& mEvaluator;
        std::vector<std::map<std::string, bool>> mScopes;
        std::vector<std::map<Token, int>> mIdentifiers;
        enum class FType 
        {
            NONE,
            FUNCTION,
            METHOD
        };

        FType currentFunction = FType::NONE;

        void beginScope();
        void endScope();
        void declare(const Token& name);
        void define(const Token& name);
        void resolveLocal(Expr& expr, const Token& name);
        void resolveFunction(std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body, FType type);
    };
}