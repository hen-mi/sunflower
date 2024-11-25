#pragma once
#include <stdexcept>
#include <any>

namespace Sunflower 
{
    class ReturnException: public std::runtime_error
    {
    public:
        ReturnException(std::any value) : std::runtime_error(""), mValue(value) {}

        const std::any & getValue() const { return mValue; }

    private:
        std::any mValue;
    };
}