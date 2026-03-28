#pragma once


namespace Sphynx
{
	class Operation
	{
	public:
		virtual ~Operation() = default;

		virtual void Execute() = 0;
		virtual void Revert() = 0;

	public:
		bool IsTransactional{ false };

	};
}
