//Abstract Syntax Tree
#include <Token.h>
#include <memory>

struct Visitor 
{
	virtual void visit(BinaryExpr &node) = 0;

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
				mLeft(&left), mRight(&right), m_op(op) {}

	virtual void accept(Visitor& v) override { v.visit(*this); }

private:
	const std::unique_ptr<Expr> mLeft;
	const std::unique_ptr<Expr> mRight;
	const Token m_op;
};


class UnaryExpr : public Expr 
{
};


class Grouping : public Expr {};


class NumericLiteral : public Expr {};


