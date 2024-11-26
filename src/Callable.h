#pragma once 
#include <any>
#include <functional>
#include <Evaluator.h>
#include <Environment.h>
#include <AST/Statements.h>
#include <ReturnException.h>
#include <cassert>
#include <memory>


namespace Sunflower 
{
        class Evaluator;
        class FunctionStmt;

        using FuncType = std::function<std::any(Evaluator&, const std::vector<std::any>&)>;

        class Callable 
        {
        public:
            ~Callable() = default;
            Callable(FunctionStmt* declaration, std::shared_ptr<Environment> closure);
            std::any call(Evaluator& evaluator, std::vector<std::any>& arguments) const;

            int getArity() const { return mArity; }

        private:
            int mArity;
            FuncType mFn;
            std::weak_ptr<Environment> mClosure;
            FunctionStmt* mDeclaration; 
        };
	
}

