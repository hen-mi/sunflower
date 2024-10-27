//Abstract Syntax Tree
#include <Token.h>
#include <memory>
#include <any>
struct Visitor 
{
	virtual void visit(BinaryExpr &node) = 0;
	virtual void visit(UnaryExpr& node) = 0;
	virtual void visit(GroupingExpr& node) = 0;
	virtual void visit(Literal& node) = 0;

	virtual ~Visitor() = default;
};



class Expr 
{
public:
	virtual void accept(Visitor	&v) = 0;
	virtual ~Expr() = default;

};



class BinaryExpr : public Expr 
{
public:

	BinaryExpr(Expr left, Token op, Expr right) : 
				mLeft(&left), mRight(&right), mOp(op) {}

	virtual void accept(Visitor& v) override { v.visit(*this); }

private:
	const std::unique_ptr<Expr> mLeft;
	const std::unique_ptr<Expr> mRight;
	const Token mOp;
};

class UnaryExpr : public Expr 
{
public:

	UnaryExpr(Token op, Expr right) : mRight(&right), mOp(op) {}

	virtual void accept(Visitor& v) override { v.visit(*this); }

private:
	const std::unique_ptr<Expr> mRight;
	const Token mOp;
};


class GroupingExpr : public Expr 
{
public:
	GroupingExpr(Expr expression) : mExpr(&expression) {}

	virtual void accept(Visitor& v) override { v.visit(*this); }

private:
	const std::unique_ptr<Expr> mExpr;

};


class Literal : public Expr 
{
public:

	Literal(std::any v) : mValue(v) {}
	virtual void accept(Visitor& v) override { v.visit(*this); }

private:
	const std::any mValue;

};


