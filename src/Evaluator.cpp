#include <Evaluator.h>

namespace Sunflower 
{

    Evaluator::Evaluator() : mGlobals(std::make_shared<Environment>()), mGlobalEnvironment(mGlobals), mEnvironment(mGlobals)
    { }
    void Evaluator::run(std::vector<std::unique_ptr<Stmt>>&& program) 
    {
        try
        {
            for (const auto& stmt : program)
            {
                execute(*stmt);
            }
        }
        catch (RuntimeError error) { Sunflower::report(error.getToken().line, error.getToken().lexema, error.what()); }
    }
    void Evaluator::execute(Stmt& stmt) 
    {
        stmt.accept(*this);
    }
    /*
    void Evaluator::resolve(Expr& expr, int depth) 
    {
        mLocals[expr] = depth;

    }

    std::any Evaluator::lookUpVariable(const Token& name, Expr& expr) 
    {
        auto elem = mLocals.find(expr);
        if (elem != mLocals.end()) 
        {
            int distance = elem->second;
            return mEnvironment->getAt(distance, name.lexema);
        }
        else {
            return mGlobals->getValue(name);
        }
    }
    */
    std::shared_ptr<Environment> Evaluator::getGlobalEnvironment()
    {
        return mGlobalEnvironment;
    }

    std::string Evaluator::stringify(const std::any& object)
    {
        if (!object.has_value()) 
        {
            return "null";
        }

        if (object.type() == typeid(bool)) 
        {
            return std::any_cast<bool>(object) ? "true" : "false";
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
    bool Evaluator::isEqual(const std::any& left, const std::any& right) const 
    {
        // nil is only equal to nil
        if (!left.has_value() && !right.has_value()) {
            return true;
        }
        if (!left.has_value()) {
            return false;
        }

        if (left.type() != right.type()) {
            return false;
        }

        if (left.type() == typeid(bool)) {
            return std::any_cast<bool>(left) == std::any_cast<bool>(right);
        }

        if (left.type() == typeid(double)) {
            return std::any_cast<double>(left) == std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string)) {
            return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
        }

        return false;
    }
    std::any Evaluator::visitBinaryExpr(BinaryExpr& expr)
    {
        const auto left = evaluate(expr.getLeftExpr());
        const auto right = evaluate(expr.getRightExpr());

        switch (expr.getOp().tokentype) {
            // equality operators
        case TokenType::NOT_EQUAL:
            return !isEqual(left, right);
        case TokenType::EQUAL_EQUAL:
            return isEqual(left, right);
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

            else if (left.type() == typeid(std::string) && right.type() == typeid(double)) 
            {
                return std::any_cast<std::string>(left) + std::to_string(std::any_cast<double>(right));
            }
            else if (left.type() == typeid(double) && right.type() == typeid(std::string))
            {
                return std::to_string(std::any_cast<double>(left)) + std::any_cast<std::string>(right);
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
			return std::any_cast<double>(right) * -1;
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

	bool Evaluator::isTrue(const std::any& object)
	{
        if (!object.has_value()) 
        {
            
            return false;
        }

        if (object.type() == typeid(bool)) 
        {
            
            return std::any_cast<bool>(object);
        }

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

    std::any Evaluator::visitLogicalExpr(LogicalExpr& node) 
    {
        auto left = evaluate(node.getLeftExpr());

        if (node.getOp().tokentype == TokenType::OR && isTrue(left))
        {
            return left;
        }
        else if (!isTrue(left)) return left;

        return evaluate(node.getRightExpr());
    }
    std::any Evaluator::visitExprStmt(ExprStmt& stmt) 
    {
        auto value = evaluate(stmt.getExpr());
        return {};
    }

    std::any Evaluator::visitPrintStmt(PrintStmt& stmt) 
    {
        std::any value = evaluate(stmt.getExpr());
        std::cout << stringify(value) << "\n";
        return std::any{};
    }

    std::any Evaluator::visitWhileStmt(WhileStmt& stmt) 
    {
        while (isTrue(evaluate(stmt.getCondition())))
        {
            execute(stmt.getBody());
        }

        return { };
    }
    std::any Evaluator::visitIfStmt(IfStmt& stmt) 
    {
      
    
        if (isTrue(evaluate(stmt.getCondition()))) 
        {
            
            execute(stmt.getThenBranch());
        } 
        else if(stmt.hasElseBranch()) 
        {
            execute(stmt.getElseBranch());
        }

        return {};
    }
    std::any Evaluator::visitVarExpr(VarExpr& node) 
    {
        return mEnvironment->getValue(node.getName());
    }
    std::any Evaluator::visitVarStmt(VarStmt& stmt) 
    {
        std::any value;

        if(stmt.hasInitializer()) 
        {
            value = evaluate(stmt.getInitializer());
        }

        mEnvironment->define(stmt.getName().lexema, value);

        return {};
    }


    std::any Evaluator::visitAssignExpr(AssignExpr& node) 
    {
        
        auto value = evaluate(node.getValueExpr());
        /*
        if(mLocals.find(node) != mLocals.end()) 
        {
            int distance = mLocals[node];

            mEnvironment->assignAt(distance, node.getName(), value);
        }
        */
        mEnvironment->assign(node.getName(), value);
        return value;
    }

    std::any Evaluator::visitCallExpr(CallExpr& node) 
    {
        auto callee = evaluate(node.getCallee());
        
        std::vector<std::any> arguments;

        for (const auto& argument : node.getArguments())
        {
            arguments.push_back(evaluate(*argument));
        }

        if (callee.type() != typeid(std::shared_ptr<Callable>)) 
        {
            throw RuntimeError(node.getParen(), "Can only call functions and classes.");
        }

        auto function = std::any_cast<std::shared_ptr<Callable>>(callee);
        if (arguments.size() != function->getArity()) 
        {
            throw RuntimeError(node.getParen(), 
                                "Invalid number of arguments, expected: " + std::to_string(function->getArity()) + ", but got " + std::to_string(arguments.size()));
        }

        return function->call(*this, std::move(arguments));
    }
    std::any Evaluator::visitFunctionStmt(FunctionStmt& stmt) 
    {
        auto callable = std::make_shared<Callable>(&stmt, mEnvironment);
        mEnvironment->define(stmt.getName().lexema, callable);
        return {};
    }

    std::any Evaluator::visitReturnStmt(ReturnStmt& stmt) 
    {
        std::any value;
        if (stmt.hasValue()) 
        {
            value = evaluate(stmt.getValue());
        }

        throw ReturnException(value);
    }

    std::any Evaluator::visitBlockStmt(BlockStmt& stmt) 
    {
        executeBlock(stmt.getStatements(), std::make_shared<Environment>(mEnvironment));
        return {};
    }

    void Evaluator::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment>& environment) 
    {
        std::shared_ptr<Environment> previous = std::move(mEnvironment);
        try {
            mEnvironment = std::move(environment);
            for (const auto& statementPtr : statements)
            {

                execute(*statementPtr);
            }
        }
        catch (RuntimeError error) 
        {
            environment = std::move(previous);
            throw error;
        }

        mEnvironment = std::move(previous);
        
               
    }

}