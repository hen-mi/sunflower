#pragma once 
#include <any>
#include <functional>

namespace Sunflower 
{
        class Evaluator;
        class FunctionStmt;

        using FuncType = std::function<std::any(Evaluator&, const std::vector<std::any>&)>;

        class Callable 
        {
        public:
            Callable(int arity, FuncType f);
            Callable(FunctionStmt* declaration); 

            std::any call(Evaluator& evaluator, std::vector<std::any>& arguments) const;

            int getArity() const { return mArity; }

        private:
            int mArity;
            FuncType mFn;

            FunctionStmt* mDeclaration; 
        };
	
}

