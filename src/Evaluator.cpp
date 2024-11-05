#include <Evaluator.h>

namespace Sunflower 
{

    Evaluator::Evaluator(Expr& expr)
    {
        try 
        { 
        std::any value = evaluate(expr); 
        std::cout << stringify(value) << "\n"; 
        } 
        catch (RuntimeError error) { Sunflower::report(error.getToken().line, error.getToken().lexema, error.what() ); }
    }


    std::string Evaluator::stringify(const std::any& object)
    {
        if (!object.has_value()) 
        {
            return "null";
        }

        if (object.type() == typeid(bool)) 
        {
            return std::any_cast<bool>(object) ? "false" : "true";
        }

        if (object.type() == typeid(double)) 
        {
            double n = std::any_cast<double>(object);
            if (std::trunc(n) == n) { // is int
                return std::to_string((int)n);
            }
            else {
                return std::to_string(n); 
            }
        }

        if (object.type() == typeid(std::string)) 
        {
 
            return std::any_cast<std::string>(object);
        }
 

        return "";
    }

    std::any Evaluator::visitBinaryExpr(BinaryExpr& expr)
    {
        const auto left = evaluate(expr.getLeftExpr());
        const auto right = evaluate(expr.getRightExpr());

        switch (expr.getOp().tokentype) {
            // equality operators
        case TokenType::NOT_EQUAL:
            return std::any{}; //!isEqual(left, right);
        case TokenType::EQUAL:
            return std::any{}; //isEqual(left, right);
            // comparison operators
        case TokenType::GREATER:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
            // arithmetic operators
        case TokenType::MINUS:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) 
            {

                return (std::any_cast<double>(left)) + (std::any_cast<double>(right));
            }

            else if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) 
            {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }

            throw RuntimeError(expr.getOp(), "Operands must be two numbers or two strings");
        case TokenType::SLASH:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::STAR:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        default:
            return std::any{}; // unreachable
        }

    }


	std::any Evaluator::visitUnaryExpr(UnaryExpr& node) 
	{
		std::any right = evaluate(node.getRightExpr());

		switch (node.getOp().tokentype) 
		{
		case TokenType::MINUS:
			return -std::any_cast<double>(right);
		case TokenType::NOT:
			std::cout << "NOT IMPLEMENTED\n";
            return std::any{};
		}

		return std::any{};
	}
	std::any Evaluator::visitGroupingExpr(GroupingExpr &node)
	{
		return evaluate(node.getExpr());
	}
	std::any Evaluator::visitLiteralExpr(Literal& node) 
	{
        return node.getValue();
	}

	std::any Evaluator::evaluate(Expr &expr)
	{
		return expr.accept(*this);
		
	}

	bool Evaluator::isTrue(Expr& node) 
	{
		return true;
	}

    void Evaluator::checkNumberOperands(const Token& op, const std::any& left, const std::any& right) 
    {
        if (left.type() == typeid(double) && right.type() == typeid(double)) 
        {
            return;
        }
        else
        {
            throw RuntimeError(op, "Operands must be numbers");
        }
    }
}