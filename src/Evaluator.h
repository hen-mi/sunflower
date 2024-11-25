#pragma once
#include <any>
#include <AST/Expressions.h>
#include <AST/Statements.h>
#include <Sunflower.h>
#include <Environment.h>
#include <Callable.h>
#include <RuntimeError.h>
#include <ReturnException.h>
namespace Sunflower
{

	class Evaluator : public Sunflower::ExprVisitor, public Sunflower::StmtVisitor
	{
	public:

		Evaluator(std::vector<std::unique_ptr<Stmt>>&& program);
		~Evaluator() = default;

		std::any evaluate(Expr& expr);
		void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Sunflower::Environment>& enviroment);
		std::shared_ptr<Environment> getGlobalEnvironment();
	private:
		void execute(Stmt& stmt);
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
		
		bool isTrue(const std::any& object);
		bool isEqual(const std::any& left, const std::any& right) const;
		std::string stringify(const std::any& object);
		void checkNumberOperands(const Token& op, const std::any& left, const std::any& right);

	private:

		std::shared_ptr<Environment> mGlobals;
		std::shared_ptr<Environment> mGlobalEnvironment;
		std::shared_ptr<Environment> mEnvironment;

	};

}


