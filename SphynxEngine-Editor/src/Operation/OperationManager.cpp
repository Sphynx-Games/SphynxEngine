#include <spxpch.h>
#include "OperationManager.h"


namespace Sphynx
{
	OperationManager::OperationManager() :
		m_OperationsHistory(),
		m_OperationsCount(0),
		m_StartIndex(0),
		m_Cursor(0)
	{

	}

	void OperationManager::AddToHistory(std::unique_ptr<Operation>&& operation)
	{
		m_OperationsCount = m_Cursor;

		size_t insertIndex = (m_StartIndex + m_OperationsCount) % SPX_MAX_OPERATIONS_COUNT;

		m_OperationsHistory[insertIndex] = std::move(operation);

		if (m_OperationsCount < SPX_MAX_OPERATIONS_COUNT)
		{
			m_OperationsCount++;
		}
		else
		{
			m_StartIndex = (m_StartIndex + 1) % SPX_MAX_OPERATIONS_COUNT;
		}

		m_Cursor = m_OperationsCount;
	}

	OperationManager::~OperationManager()
	{
		for (auto& op : m_OperationsHistory)
		{
			op.reset();
		}

		m_StartIndex = 0;
		m_OperationsCount = 0;
	}

	void OperationManager::Update()
	{
		std::vector<std::unique_ptr<Operation>> tmp;
		tmp.reserve(m_QueuedOperations.size());

		for (auto& op : m_QueuedOperations)
		{
			op->Execute();

			if (!op->IsTransactional)
			{
				AddToHistory(std::move(op));
			}
			else
			{
				tmp.emplace_back(std::move(op));
			}
		}

		m_QueuedOperations.clear();
		m_QueuedOperations = std::move(tmp);
	}

	void OperationManager::DiscardOperation(Operation* operation)
	{
		auto it = std::find_if(m_QueuedOperations.begin(), m_QueuedOperations.end(),
			[&operation](auto& ptr)
			{
				return operation == ptr.get();
			});

		SPX_CORE_ASSERT(it != m_QueuedOperations.end(), "Trying to discard a non queued operation");
		m_QueuedOperations.erase(it);
	}

	size_t OperationManager::GetOperationsCount() const
	{
		return m_OperationsCount;
	}

	const Operation* OperationManager::GetOperation(size_t index) const
	{
		if (index >= m_OperationsCount) return nullptr;

		const size_t idx = (m_StartIndex + index) % SPX_MAX_OPERATIONS_COUNT;
		return m_OperationsHistory[idx].get();
	}

	bool OperationManager::CanUndo() const
	{
		return m_Cursor > 0;
	}

	bool OperationManager::CanRedo() const
	{
		return m_Cursor < m_OperationsCount;
	}

	void OperationManager::Undo()
	{
		if (m_Cursor == 0) return;

		m_Cursor--;

		const size_t idx = (m_StartIndex + m_Cursor) % SPX_MAX_OPERATIONS_COUNT;
		m_OperationsHistory[idx]->Revert(); // we might need to queue it instead
	}

	void OperationManager::Redo()
	{
		if (m_Cursor >= m_OperationsCount)
			return;

		size_t idx = (m_StartIndex + m_Cursor) % SPX_MAX_OPERATIONS_COUNT;
		m_OperationsHistory[idx]->Execute(); // we might need to queue it instead

		m_Cursor++;
	}
}
