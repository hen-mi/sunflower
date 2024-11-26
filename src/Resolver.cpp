#include <Resolver.h>

namespace Sunflower 
{
	Resolver::Resolver(Evaluator& evaluator) : mEvaluator(std::move(evaluator)) {}

	void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements) 
	{
		for(auto const& stmt : statements) 
		{
			resolve(*stmt);
		}
	}

	void Resolver::resolve(Stmt& statement) 
	{
		statement.accept(*this);
	}

	void Resolver::resolve(Expr& expression)
	{
		expression.accept(*this);
	}

	void Resolver::beginScope() 
	{
		mIdentifiers.push_back(std::map<Token, int>{});
		mScopes.push_back(std::map<std::string, bool>{});
	}

	void Resolver::endScope() 
	{
		mScopes.pop_back();
		mIdentifiers.pop_back();
	}
	std::any Resolver::visitBlockStmt(BlockStmt& stmt) 
	{
		beginScope();
		resolve(stmt.getStatements());
		endScope();

		return {};
	}
	std::any Resolver::visitVarStmt(VarStmt& stmt) 
	{
		declare(stmt.getName());
		if (stmt.hasInitializer()) resolve(stmt.getInitializer());
		define(stmt.getName());
		return {};
	}

	std::any Resolver::visitVarExpr(VarExpr& node) 
	{
		if (!mScopes.empty()) 
		{
			auto& currentScope = mScopes.back();
			auto elem = currentScope.find(node.getName().lexema);

			if (elem != currentScope.end() && elem->second == false) 
			{
				Sunflower::error(node.getName(), "Can't read local variable in this own initializer.");
			}
		}
		resolveLocal(node, node.getName());
		return {};
	}

	std::any Resolver::visitAssignExpr(AssignExpr& node) 
	{
		resolve(node.getValueExpr());
		resolveLocal(node, node.getName());

		return {};
	}
	std::any Resolver::visitFunctionStmt(FunctionStmt& stmt) 
	{
		declare(stmt.getName());
		define(stmt.getName());

		resolveFunction(stmt.getParams(), stmt.getBody(), FType::FUNCTION);
		return {};
	}

	std::any Resolver::visitExprStmt(ExprStmt& stmt) 
	{
		resolve(stmt.getExpr());

		return {};
	}

	std::any Resolver::visitPrintStmt(PrintStmt& stmt) 
	{
		resolve(stmt.getExpr());

		return {};
	}
	std::any Resolver::visitReturnStmt(ReturnStmt& stmt) 
	{
		if (stmt.hasValue()) resolve(stmt.getValue());

		return {};
	}

	std::any Resolver::visitWhileStmt(WhileStmt& stmt)
	{
		resolve(stmt.getCondition());
		resolve(stmt.getBody());

		return {};
	}
	std::any Resolver::visitIfStmt(IfStmt& stmt) 
	{
		resolve(stmt.getCondition());
		resolve(stmt.getThenBranch());
		
		if (stmt.hasElseBranch()) resolve(stmt.getElseBranch());

		return {};
	}
	std::any Resolver::visitBinaryExpr(BinaryExpr& expr)
	{
		resolve(expr.getLeftExpr());
		resolve(expr.getRightExpr());

		return {};
	}
	std::any Resolver::visitLogicalExpr(LogicalExpr& node) 
	{
		resolve(node.getLeftExpr());
		resolve(node.getRightExpr());

		return {};
	}
	std::any Resolver::visitCallExpr(CallExpr& node)
	{
		resolve(node.getCallee());

		for(const auto& argument : node.getArguments()) 
		{
			resolve(*argument);
		}

		return {};
	}
	std::any Resolver::visitUnaryExpr(UnaryExpr& node) 
	{
		resolve(node.getRightExpr());
		
		return {};
	}
	std::any Resolver::visitGroupingExpr(GroupingExpr& node) 
	{
		resolve(node.getExpr());
		return {};
	}
	std::any Resolver::visitLiteralExpr(Literal& node) 
	{
		return {};
	}
	void Resolver::declare(const Token& name) 
	{
		if (mScopes.empty() && mIdentifiers.empty()) return;

		std::map<Token, int> currentBlock = mIdentifiers.back();
		std::map<std::string, bool> currentScope = mScopes.back();

		if (currentScope.find(name.lexema) != currentScope.end()) 
		{
			Sunflower::error(name, "Multiples variables with the same name is not allowed");
		}
		currentBlock[name] = 0;
		currentScope[name.lexema] = false;

	}

	void Resolver::define(const Token& name) 
	{
		if (mScopes.empty() && mIdentifiers.empty()) return;

		mScopes.back()[name.lexema] = true;
	}

	void Resolver::resolveLocal(Expr& expr, const Token& name) 
	{
		size_t scopeSize = mScopes.size() - 1;
		for (size_t i = scopeSize; i < mScopes.size(); i--) 
		{
			if (mScopes[i].find(name.lexema) != mScopes[i].end()) 
			{
				mIdentifiers[i].erase(name);
				mEvaluator.resolve(expr, scopeSize - i);
				return;
			}
		}
	}

	void Resolver::resolveFunction(std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body, FType type) 
	{
		FType enclosingFunction = currentFunction;
		currentFunction = type;
		beginScope();
		for (Token& param : params) 
		{
			declare(param);
			define(param);
		}
		resolve(body);
		endScope();
		currentFunction = enclosingFunction;
	}
}