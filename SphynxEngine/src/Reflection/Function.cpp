#include "Reflection/Function.h"

namespace Sphynx
{
	namespace Reflection
	{
		Function::Function(const char* name, const Type& returnType, const Parameter* params, size_t paramCount, void(*func)(void*, void*)) :
			Name(name),
			ReturnType(returnType),
			Parameters(params),
			ParameterCount(paramCount),
			FunctionPtr(func),
			Attributes()
		{

		}

		Function::Function(const Function& other) :
			Name(other.Name),
			ReturnType(other.ReturnType),
			Parameters(other.Parameters),
			ParameterCount(other.ParameterCount),
			FunctionPtr(other.FunctionPtr),
			Attributes(other.Attributes)
		{

		}

		Function::Function(Function&& other) noexcept :
			Name(other.Name),
			ReturnType(other.ReturnType),
			Parameters(std::move(other.Parameters)),
			ParameterCount(std::move(other.ParameterCount)),
			FunctionPtr(std::move(other.FunctionPtr)),
			Attributes(std::move(other.Attributes))
		{

		}

		Function::~Function()
		{

		}
	}
}
