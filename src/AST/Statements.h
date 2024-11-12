#pragma once
#include <AST/Expressions.h>
#include <any>
namespace Sunflower 
{
	class ExprStmt;
	class PrintStmt;


	class StmtVisitor
	{
	public:

		virtual std::any visitExprStmt(ExprStmt& stmt) = 0;
		virtual std::any visitPrintStmt(PrintStmt& stmt) = 0;
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



}