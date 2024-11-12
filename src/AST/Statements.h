#pragma once
#include <AST/Expressions.h>
#include <any>
namespace Sunflower 
{
	class ExprStmt;
	class PrintStmt;
	class VarStmt;
	class BlockStmt;

	class StmtVisitor
	{
	public:

		virtual std::any visitExprStmt(ExprStmt& stmt) = 0;
		virtual std::any visitPrintStmt(PrintStmt& stmt) = 0;
		virtual std::any visitVarStmt(VarStmt& stmt) = 0;
		virtual std::any visitBlockStmt(BlockStmt& stmt) = 0;
		virtual ~StmtVisitor() = default;
	};


	class Stmt 
	{
	public:
		virtual std::any accept(StmtVisitor& v) = 0;
		virtual ~Stmt() = default;

	};

	class ExprStmt : public Stmt 
	{
	public:

		ExprStmt(std::unique_ptr<Expr> Expr) : mExpr{std::move(Expr)} {};
		std::any accept(StmtVisitor& v) override { return v.visitExprStmt(*this); }
		Expr& getExpr() const { return *mExpr; }
	private:
		const std::unique_ptr<Expr> mExpr;
	};

	class PrintStmt : public Stmt
	{
	public:
		PrintStmt(std::unique_ptr<Expr> Expr) : mExpr{ std::move(Expr) } {};
		std::any accept(StmtVisitor& v) override { return v.visitPrintStmt(*this); }
		Expr& getExpr() const { return *mExpr; }
	private:

		const std::unique_ptr<Expr> mExpr;
	};

	class VarStmt : public Stmt
	{
	public:
		VarStmt(Token& n, std::unique_ptr<Expr> init) : mName{ n }, mExprInit{ std::move(init) } {}
		std::any accept(StmtVisitor& v) override { return v.visitVarStmt(*this); }
		const Token& getName() const { return mName; }
		bool hasInitializer() const { return mExprInit != nullptr; }
		Expr& getInitializer() const { return *mExprInit; }
	private:
		const Token mName;
		const std::unique_ptr<Expr> mExprInit;
	};

	class BlockStmt : public Stmt 
	{
	public:
		BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : mStatements(std::move(statements)) {}

		std::any accept(StmtVisitor& v) override { return v.visitBlockStmt(*this); }
		const std::vector<std::unique_ptr<Stmt>>& getStatements() { return mStatements; }
	private:

		std::vector<std::unique_ptr<Stmt>> mStatements;
	};
}