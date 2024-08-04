#pragma once


namespace Sphynx
{
	namespace Reflection
	{
		struct Class;
		struct Type;

		struct Property
		{
			const Type& Type;
			const char* Name;
			size_t Offset;
		};
	}
}
