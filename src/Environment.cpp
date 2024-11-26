#include <Environment.h>

namespace Sunflower
{
	Environment::Environment() : mEnclosing(nullptr) //global scope
	{}

	Environment::Environment(std::shared_ptr<Environment> enclosing) : mEnclosing(std::move(enclosing)) //local scope
	{}

	std::any Environment::getValue(const Token& name)
	{
		if (mNamedValues.find(name.lexema) != mNamedValues.end())
		{
			return mNamedValues[name.lexema];
		}
		if (mEnclosing) return mEnclosing->getValue(name);

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
		if (mEnclosing) { mEnclosing->assign(name, value); return; }

		throw RuntimeError(name, "Undefined variable " + name.lexema + "\n");
	}
	/*
	std::any Environment::getAt(int distance, std::string name) 
	{
		return ancestor(distance)->mNamedValues[name];
	}

	std::shared_ptr<Environment> Environment::ancestor(int distance) 
	{
		std::shared_ptr<Environment> currentEnv = shared_from_this();

		for(int i = 0; i < distance; i++) 
		{
			currentEnv = currentEnv->mEnclosing;
		}

		return currentEnv;
	}

	void Environment::assignAt(int distance, const Token& name, const std::any& value) 
	{
		ancestor(distance)->mNamedValues[name.lexema] = value;
	}

	*/
}