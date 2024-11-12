
#pragma once
#include <Token.h>
#include <memory>
#include <any>

namespace Sunflower 
{
		
	class BinaryExpr;
	class UnaryExpr;
	class GroupingExpr;
	class Literal;
	class VarExpr;


	class ExprVisitor
	{
	public:
		virtual std::any visitBinaryExpr(BinaryExpr& node) = 0;
		virtual std::any visitUnaryExpr(UnaryExpr& node) = 0;
		virtual std::any visitGroupingExpr(GroupingExpr& node) = 0;
		virtual std::any visitLiteralExpr(Literal& node) = 0;
		virtual std::any visitVarExpr(VarExpr& node) = 0;
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
		VarExpr(std::string& n) : mName{ n } {}
		std::any accept(ExprVisitor& v) override { return v.visitVarExpr(*this); }
		std::string getName() const { return mName; }
	private:
		const std::string mName;
	};
}
