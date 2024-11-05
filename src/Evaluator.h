#include <AST.h>
#include <Sunflower.h>
namespace Sunflower 
{

	class RuntimeError : public std::runtime_error 
	{
	public:
		RuntimeError(const Token& token, const std::string& message) :
			std::runtime_error(message), token(token)
		{}

		const Token& getToken() const { return token; }

	private:
		Token token;
	};

	class Evaluator : public Sunflower::ExprVisitor
	{
	public:

		Evaluator(Expr& expression);
		~Evaluator() = default;

		std::any evaluate(Expr& expr);

	private:

		std::any visitBinaryExpr(BinaryExpr& expr) override;
		std::any visitUnaryExpr(UnaryExpr& node) override;
		std::any visitGroupingExpr(GroupingExpr& node) override;
		std::any visitLiteralExpr(Literal& node) override;

		bool isTrue(Expr& node);
		std::string stringify(const std::any& object);
		void checkNumberOperands(const Token& op, const std::any& left, const std::any& right);
	};

}


