
#pragma once
#include <Token.h>
#include <memory>
#include <any>

namespace Sunflower 
{
		
	class BinaryExpr;
	class LogicalExpr;
	class UnaryExpr;
	class GroupingExpr;
	class Literal;
	class VarExpr;
	class AssignExpr;
	class CallExpr;

	class ExprVisitor
	{
	public:
		virtual std::any visitBinaryExpr(BinaryExpr& node) = 0;
		virtual std::any visitUnaryExpr(UnaryExpr& node) = 0;
		virtual std::any visitGroupingExpr(GroupingExpr& node) = 0;
		virtual std::any visitLiteralExpr(Literal& node) = 0;
		virtual std::any visitVarExpr(VarExpr& node) = 0;
		virtual std::any visitAssignExpr(AssignExpr& node) = 0;
		virtual std::any visitLogicalExpr(LogicalExpr& node) = 0;
		virtual std::any visitCallExpr(CallExpr& node) = 0;
		virtual ~ExprVisitor() = default;
	};



	class Expr
	{
	public:
		virtual std::any accept(ExprVisitor& v) = 0;
		virtual ~Expr() = default;

	};



	class BinaryExpr : public Expr
	{
	public:

		BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
			mLeft(std::move(left)), mRight(std::move(right)), mOp(op) {}

		std::any accept(ExprVisitor& v) override { return v.visitBinaryExpr(*this); }

		Expr& getLeftExpr() const { return *mLeft; }
		Expr& getRightExpr() const { return *mRight; }
		const Token& getOp() const { return mOp; }

	private:
		const std::unique_ptr<Expr> mLeft;
		const std::unique_ptr<Expr> mRight;
		const Token mOp;
	};

	class LogicalExpr : public Expr
	{
	public:

		LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
			mLeft(std::move(left)), mRight(std::move(right)), mOp(op) {}

		std::any accept(ExprVisitor& v) override { return v.visitLogicalExpr(*this); }

		Expr& getLeftExpr() const { return *mLeft; }
		Expr& getRightExpr() const { return *mRight; }
		const Token& getOp() const { return mOp; }

	private:
		const std::unique_ptr<Expr> mLeft;
		const std::unique_ptr<Expr> mRight;
		const Token mOp;
	};

	class UnaryExpr : public Expr
	{
	public:

		UnaryExpr(Token op, std::unique_ptr<Expr> right) : mRight(std::move(right)), mOp(op) {}

		std::any accept(ExprVisitor& v) override { return v.visitUnaryExpr(*this); }
		Expr& getRightExpr() const { return *mRight; }
		const Token& getOp() const { return mOp; }

	private:
		const std::unique_ptr<Expr> mRight;
		const Token mOp;
	};


	class GroupingExpr : public Expr
	{
	public:
		GroupingExpr(std::unique_ptr<Expr> expression) : mExpr(std::move(expression)) {}

		std::any accept(ExprVisitor& v) override { return v.visitGroupingExpr(*this); }
		Expr& getExpr() const { return *mExpr.get(); }

	private:
		const std::unique_ptr<Expr> mExpr;

	};


	class Literal : public Expr
	{
	public:

		Literal(std::any v) : mValue(v) {}
		std::any accept(ExprVisitor& v) override { return v.visitLiteralExpr(*this); }
		const std::any& getValue() const { return mValue; }


	private:
		const std::any mValue;

	};


	class VarExpr: public Expr
	{
	public:
		VarExpr(Token& n) : mName{ n } {}
		std::any accept(ExprVisitor& v) override { return v.visitVarExpr(*this); }
		const Token& getName() const { return mName; }
	private:
		const Token mName;
	};

	class AssignExpr : public Expr 
	{
	public:
		AssignExpr(const Token& name, std::unique_ptr<Expr> value) : mName(name), mValue(std::move(value)) {}

		std::any accept(ExprVisitor& v) override { return v.visitAssignExpr(*this); }
		Expr& getValueExpr() const { return *mValue; }
		const Token& getName() const { return mName; }

	private:
		const std::unique_ptr<Expr> mValue;
		const Token mName;
	};

	class CallExpr : public Expr
	{
	public:
		CallExpr(std::unique_ptr<Expr> callee, const Token& paren, std::vector<std::unique_ptr<Expr>> arguments)
					: mArguments(std::move(arguments)), mParen(paren), mCallee(std::move(callee)) {}

		std::any accept(ExprVisitor& v) override { return v.visitCallExpr(*this); }
		const std::vector<std::unique_ptr<Expr>>& getArguments() { return mArguments; }
		const Token& getParen() const { return mParen; }
		Expr& getCallee() const { return *mCallee; }
	private:
		const Token mParen;
		std::unique_ptr<Expr> mCallee;
		std::vector<std::unique_ptr<Expr>> mArguments;
	};
}
