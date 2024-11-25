#include <Callable.h>
#include <Evaluator.h>
#include <Environment.h>
#include <AST/Statements.h>
#include <ReturnException.h>
#include <cassert>
namespace Sunflower
{
    Callable::Callable(int arity, FuncType f) : mArity(arity), mFn(f), mDeclaration(nullptr)
    {}

    Callable::Callable(FunctionStmt* declaration) : mDeclaration(declaration)
    {

        mArity = static_cast<int>(mDeclaration->getParams().size());
    }

    std::any Callable::call(Evaluator& evaluator, std::vector<std::any>& arguments) const
    {
        if (!mDeclaration)
        {
            return mFn(evaluator, arguments);
        }


        const auto& params = mDeclaration->getParams();
        assert(params.size() == arguments.size()); // before we call, we check for arity, but let's be
        // extra-safe

// define params as local variables for env
        auto env = evaluator.getGlobalEnvironment();
        for (std::size_t i = 0; i < params.size(); ++i)
        {
            env->define(params.at(i).lexema, arguments.at(i));
        }

        try
        {
            evaluator.executeBlock(mDeclaration->getBody(), std::move(env));
        }
        catch (const ReturnException& v)
        {
            return v.getValue();
        }

        return std::any{};
    }
}