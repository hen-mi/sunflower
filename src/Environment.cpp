#include <Environment.h>

namespace Sunflower
{
	Environment::Environment() : mEnclosing(nullptr) //global scope
	{}

	Environment::Environment(Environment* enclosing) : mEnclosing(enclosing) //local scope
	{}

	std::any Environment::getValue(const Token& name)
	{
		if (mNamedValues.find(name.lexema) != mNamedValues.end())
		{
			return mNamedValues[name.lexema];
		}
		if (mEnclosing != nullptr) return mEnclosing->getValue(name);

		throw RuntimeError(name, "Undefined variable " + name.lexema + "\n");
	}
	void Environment::define(const std::string& name, const std::any& value)
	{
		mNamedValues[name] = value;
	}

	void Environment::assign(const Token& name, const std::any& value)
	{
		if (mNamedValues.find(name.lexema) != mNamedValues.end())
		{
			mNamedValues[name.lexema] = value;
			return;
		}
		if (mEnclosing != nullptr) mEnclosing->assign(name, value); return;

		throw RuntimeError(name, "Undefined variable " + name.lexema + "\n");
	}
}