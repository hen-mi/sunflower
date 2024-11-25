#pragma once
#include <AST/Expressions.h>
#include <any>
namespace Sunflower 
{
	class ExprStmt;
	class PrintStmt;
	class VarStmt;
	class BlockStmt;
	class WhileStmt;
	class FunctionStmt;
	class ReturnStmt;
	class IfStmt;

	class StmtVisitor
	{
	public:

		virtual std::any visitExprStmt(ExprStmt& stmt) = 0;
		virtual std::any visitPrintStmt(PrintStmt& stmt) = 0;
		virtual std::any visitVarStmt(VarStmt& stmt) = 0;
		virtual std::any visitBlockStmt(BlockStmt& stmt) = 0;
		virtual std::any visitIfStmt(IfStmt& stmt) = 0;
		virtual std::any visitWhileStmt(WhileStmt& stmt) = 0;
		virtual std::any visitFunctionStmt(FunctionStmt& stmt) = 0;
		virtual std::any visitReturnStmt(ReturnStmt& stmt) = 0;
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

	class IfStmt : public Stmt 
	{

	public:
		IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch) 
			: mCondition(std::move(condition)), mThenBranch(std::move(thenBranch)), mElseBranch(std::move(elseBranch)) {};
		std::any accept(StmtVisitor& v) override { return v.visitIfStmt(*this); }

		Expr& getCondition() const { return *mCondition; }
		Stmt& getThenBranch() const { return *mThenBranch; }
		Stmt& getElseBranch() const { return *mElseBranch; }
		bool hasElseBranch() const { return mElseBranch != nullptr; }

	private:
		std::unique_ptr<Expr> mCondition;
		std::unique_ptr<Stmt> mThenBranch;
		std::unique_ptr<Stmt> mElseBranch;
	};


	class WhileStmt : public Stmt
	{
	public:
		WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) 
					: mCondition(std::move(condition)), mBody(std::move(body)) {}

		std::any accept(StmtVisitor& v) override { return v.visitWhileStmt(*this); }
		Expr& getCondition() const { return *mCondition; }
		Stmt& getBody() const { return *mBody; }
	private:

		std::unique_ptr<Expr> mCondition;
		std::unique_ptr<Stmt> mBody;
	};

	class FunctionStmt : public Stmt 
	{
	public:
		FunctionStmt(Token& name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body)
			: mName(std::move(name)), mParams(std::move(params)), mBody(std::move(body)) {};
		std::any accept(StmtVisitor& v) override { return v.visitFunctionStmt(*this); };

		const Token& getName() const { return mName; };
		std::vector<Token>& getParams() { return mParams; }
		std::vector<std::unique_ptr<Stmt>>& getBody() { return mBody; }

	private:
		const Token mName;
		std::vector<Token> mParams;
		std::vector<std::unique_ptr<Stmt>> mBody;
	};

	class ReturnStmt : public Stmt 
	{

	public:
		ReturnStmt(const Token& keyword, std::unique_ptr<Expr> value): mKeyword(keyword), mValue(std::move(value)) {};
		std::any accept(StmtVisitor& v) override { return v.visitReturnStmt(*this); };

		const Token& getKeyword() const { return mKeyword; }
		bool hasValue() const { return mValue.get() != nullptr; }
		Expr& getValue() const { return *mValue; }
	private:
		const Token mKeyword;
		std::unique_ptr<Expr> mValue;
	};
}