#pragma once

#include "Core/Core.h"
#include "Operation.h"
#include <array>
#include <vector>
#include <memory>


#ifndef SPX_MAX_OPERATIONS_COUNT
#define SPX_MAX_OPERATIONS_COUNT 20
#endif

namespace Sphynx
{
	class OperationManager
	{
	public:
		OperationManager();
		~OperationManager();

		void Update();

		// This will create an non-transactional operation.
		template<typename TOperation, typename... Args>
		TOperation* CreateOperation(Args&&... args);

		// Use this to create in place a transactional operation. Meaning it will be executed until
		// EndOperation(operation) is called. This is useful for operations that are time based.
		template<typename TOperation, typename... Args>
		TOperation* BeginOperation(Args&&... args);

		// Ends the transaction of an operation
		void EndOperation(Operation* operation);

		// Discards an operation that is in the queue
		void DiscardOperation(Operation* operation);

		// Get operations count
		size_t GetOperationsCount() const;

		// Get the operation in the specified index. Null if no operation.
		const Operation* GetOperation(size_t index) const;

		// Whether or not an undo operation is available
		bool CanUndo() const;

		// Whether or not a redo operation is available
		bool CanRedo() const;

		// Trigger an undo
		void Undo();

		// Trigger a redo
		void Redo();

	private:
		OperationManager(const OperationManager&) = delete;
		OperationManager& operator=(const OperationManager&) = delete;

		void AddToHistory(std::unique_ptr<Operation>&& operation);

	private:
		std::array<std::unique_ptr<Operation>, SPX_MAX_OPERATIONS_COUNT> m_OperationsHistory;
		std::vector<std::unique_ptr<Operation>> m_QueuedOperations;
		size_t m_OperationsCount;
		size_t m_StartIndex;
		size_t m_Cursor;
	};

	template<typename TOperation, typename... Args>
	inline TOperation* Sphynx::OperationManager::CreateOperation(Args&&... args)
	{
		static_assert(std::is_base_of_v<Operation, TOperation>, "Make sure TOperation inherits from Operation");
		auto operation = std::make_unique<TOperation>(std::forward<Args>(args)...);
		auto& opPtr = m_QueuedOperations.emplace_back(std::move(operation));

		return static_cast<TOperation*>(opPtr.get());
	}

	template<typename TOperation, typename... Args>
	inline TOperation* Sphynx::OperationManager::BeginOperation(Args&&... args)
	{
		static_assert(std::is_base_of_v<Operation, TOperation>, "Make sure TOperation inherits from Operation");

		TOperation* operation = CreateOperation<TOperation>(std::forward<Args>(args)...);
		operation->IsTransactional = true;
		return operation;
	}

	inline void OperationManager::EndOperation(Operation* operation)
	{
		operation->IsTransactional = false;
	}

}
