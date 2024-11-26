#include <Callable.h>

namespace Sunflower
{
 
    Callable::Callable(FunctionStmt* declaration, std::shared_ptr<Environment> closure)
        : mDeclaration(std::move(declaration)), mClosure(std::move(closure))
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
        
         //auto env = evaluator.getGlobalEnvironment();

        auto env = std::make_shared<Environment>(mClosure.lock());

        for (std::size_t i = 0; i < params.size(); ++i)
        {
            env->define(params.at(i).lexema, arguments.at(i));
        }

        try
        {
            evaluator.executeBlock(mDeclaration->getBody(), env);
        }
        catch (const ReturnException& v)
        {
            return v.getValue();
        }

        return std::any{};
    }
}