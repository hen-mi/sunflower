//Abstract Syntax Tree
#include <Token.h>
#include <memory>
#include <any>
#include <iostream>

class BinaryExpr;
class UnaryExpr;
class GroupingExpr;
class Literal;

class Visitor
{
public:
	virtual void visit(BinaryExpr& node) = 0;
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

	BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
				mLeft(std::move(left)), mRight(std::move(right)), mOp(op) {}

	void accept(Visitor& v) override { v.visit(*this); }

//private:
	const std::unique_ptr<Expr> mLeft;
	const std::unique_ptr<Expr> mRight;
	const Token mOp;
};

class UnaryExpr : public Expr 
{
public:

	UnaryExpr(Token op, std::unique_ptr<Expr> right) : mRight(std::move(right)), mOp(op) {}

	void accept(Visitor& v) override { v.visit(*this); }

//private:
	const std::unique_ptr<Expr> mRight;
	const Token mOp;
};


class GroupingExpr : public Expr 
{
public:
	GroupingExpr(std::unique_ptr<Expr> expression) : mExpr(std::move(expression)) {}

	void accept(Visitor& v) override { v.visit(*this); }

//private:
	const std::unique_ptr<Expr> mExpr;

};


class Literal : public Expr 
{
public:

	Literal(std::any v) : mValue(v) {}
	void accept(Visitor& v) override { v.visit(*this); }
//private:
	const std::any mValue;

};


class PrintVisitor : public Visitor
{
public:
    void visit(BinaryExpr& node) override
    {
        std::cout << "(" << node.mOp.lexema << " ";
        node.mLeft->accept(*this);
        std::cout << " ";
        node.mRight->accept(*this);
        std::cout << ")";
    }

    void visit(UnaryExpr& node) override
    {
        std::cout << "(" << node.mOp.lexema << " ";
        node.mRight->accept(*this);
        std::cout << ")";
    }

    void visit(GroupingExpr& node) override
    {
        std::cout << "(group ";
        node.mExpr->accept(*this);
        std::cout << ")";
    }

    void visit(Literal& node) override
    {
        std::cout << std::any_cast<std::string>(node.mValue);
    }
};
