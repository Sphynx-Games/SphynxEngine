#include "PropertyDrawer.h"
#include <xutility>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include "EditorApplication.h"
#include "Operation/OperationManager.h"
#include "Operation/Operation.h"
#include "Reflection/TypedValue.h"


template<typename T>
struct scalar_to_imgui_scalar_type;

#define DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(Type, ScalarType) template<> struct scalar_to_imgui_scalar_type<Type> : std::integral_constant<ImGuiDataType, ScalarType> {}

DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(char, ImGuiDataType_S8);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(short, ImGuiDataType_S16);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(int, ImGuiDataType_S32);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(long long, ImGuiDataType_S64);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(unsigned char, ImGuiDataType_U8);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(unsigned short, ImGuiDataType_U16);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(unsigned int, ImGuiDataType_U32);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(unsigned long long, ImGuiDataType_U64);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(float, ImGuiDataType_Float);
DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE(double, ImGuiDataType_Double);

#undef DECLARE_SCALAR_TO_IMGUI_SCALAR_TYPE


namespace Sphynx
{
	class ChangePropertyValueOperation : public Operation
	{
	public:
		ChangePropertyValueOperation(Reflection::TypedValue&& value, void* data) :
			m_PrevValue(Reflection::TypedValue::Create(*value.GetType(), data)),
			m_NewValue(std::move(value)),
			m_Data(data)
		{

		}

	public:
		virtual void Execute() override
		{
			void* newValue = m_NewValue.GetValue();
			memcpy(m_Data, newValue, m_NewValue.GetType()->Size);
		}

		virtual void Revert() override
		{
			void* prevValue = m_PrevValue.GetValue();
			memcpy(m_Data, prevValue, m_PrevValue.GetType()->Size);
		}

		template<typename T>
		const T& GetNewValue() const
		{
			return *reinterpret_cast<const T*>(m_NewValue.GetValue());
		}

		template<typename T>
		T& GetNewValue()
		{
			return *reinterpret_cast<T*>(m_NewValue.GetValue());
		}

		bool HasChange() const 
		{
			return memcmp(m_NewValue.GetValue(), m_PrevValue.GetValue(), m_NewValue.GetType()->Size);
		}

	private:
		Reflection::TypedValue m_PrevValue;
		Reflection::TypedValue m_NewValue;
		void* m_Data;
	};

	static std::unordered_map<uintptr_t, Operation*> s_Operations;

	std::vector<IPropertyDrawer*>* IPropertyDrawer::s_Registry = nullptr;

	void IPropertyDrawer::DrawLabel(const Reflection::Property& property, void* data)
	{
		SPX_UNUSED(data);
		IPropertyDrawer::DrawDefaultLabel(property);
	}

	void IPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		SPX_UNUSED(property);
		SPX_UNUSED(data);
		ImGui::Text("Not implemented");
	}

	void IPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		ImGui::TableSetColumnIndex(0);
		DrawLabel(property, data);
		ImGui::TableSetColumnIndex(1);
		DrawWidget(property, data);
	}

	void IPropertyDrawer::DrawDefaultLabel(const Reflection::Property& property)
	{
		const char* label = std::strrchr(property.Name, ':');
		if (label == nullptr) label = property.Name;
		else label += 1;
		DrawDefaultLabel(label);
	}

	void IPropertyDrawer::DrawDefaultLabel(const char* label)
	{
		float width = ImGui::CalcItemWidth();

		float x = ImGui::GetCursorPosX();
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetNextItemWidth(-1);
	}

	void IPropertyDrawer::DrawCheckbox(const Reflection::Property& property, bool& value)
	{
		std::string label{ "##" };
		label += property.Name;

		bool buffer = value;
		if (ImGui::Checkbox(label.c_str(), &buffer))
		{
			auto* operationManager = EditorApplication::GetInstance()->GetOperationManager();
			operationManager->CreateOperation<ChangePropertyValueOperation>(
				Reflection::TypedValue::Create(buffer),
				&value
			);
		}
	}

	template<typename TScalar>
	void IPropertyDrawer::DrawDragScalar(const Reflection::Property& property, TScalar& value)
	{
		std::string label{ "##" };
		label += property.Name;

		using TValue = std::remove_reference_t<decltype(value)>;
		ChangePropertyValueOperation* operation = s_Operations.find((uintptr_t)&value) != s_Operations.end()
			? static_cast<ChangePropertyValueOperation*>(s_Operations[(uintptr_t)&value])
			: nullptr;
		TValue tmp = value;
		TValue& buffer = operation == nullptr ? tmp : operation->GetNewValue<TValue>();

		auto* operationManager = EditorApplication::GetInstance()->GetOperationManager();
		if (ImGui::DragScalar(label.c_str(), ::scalar_to_imgui_scalar_type<TScalar>::value, &buffer))
		{
			if (operation == nullptr)
			{
				s_Operations[(uintptr_t)&value] = operationManager->BeginOperation<ChangePropertyValueOperation>(
					Reflection::TypedValue::Create(buffer),
					&value
				);
			}
		}

		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			if (operation->HasChange())
			{
				operationManager->EndOperation(operation);
			}
			else
			{
				operationManager->DiscardOperation(operation);
			}
			//operation = nullptr;
			s_Operations.erase((uintptr_t)&value);
		}
	}

	template void IPropertyDrawer::DrawDragScalar<char>(const Reflection::Property& property, char& value);
	template void IPropertyDrawer::DrawDragScalar<short>(const Reflection::Property& property, short& value);
	template void IPropertyDrawer::DrawDragScalar<int>(const Reflection::Property& property, int& value);
	template void IPropertyDrawer::DrawDragScalar<long long>(const Reflection::Property& property, long long& value);
	template void IPropertyDrawer::DrawDragScalar<unsigned char>(const Reflection::Property& property, unsigned char& value);
	template void IPropertyDrawer::DrawDragScalar<unsigned short>(const Reflection::Property& property, unsigned short& value);
	template void IPropertyDrawer::DrawDragScalar<unsigned int>(const Reflection::Property& property, unsigned int& value);
	template void IPropertyDrawer::DrawDragScalar<unsigned long long>(const Reflection::Property& property, unsigned long long& value);
	template void IPropertyDrawer::DrawDragScalar<float>(const Reflection::Property& property, float& value);
	template void IPropertyDrawer::DrawDragScalar<double>(const Reflection::Property& property, double& value);

	void IPropertyDrawer::DrawEnum(const Reflection::Property& property, void* data)
	{
		SPX_CORE_ASSERT(property.GetType().Kind == Reflection::TypeKind::ENUM, "Property type is not enum!");

		std::string label{ "##" };
		label += property.Name;

		const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property.GetType());
		const char* currentValue = rEnum.GetName((const void*)data);
		if (ImGui::BeginCombo(label.c_str(), currentValue))
		{
			for (const Reflection::Enum::Entry& entry : rEnum)
			{
				const char* optionName = entry.Name;
				if (ImGui::Selectable(optionName, false))
				{
					auto* operationManager = EditorApplication::GetInstance()->GetOperationManager();
					operationManager->CreateOperation<ChangePropertyValueOperation>(
						Reflection::TypedValue::Create((int32_t)entry.Value),
						data
					);
				}
			}
			ImGui::EndCombo();
		}
	}

	void IPropertyDrawer::EnsureInitialized()
	{
		if (s_Registry == nullptr)
		{
			static std::vector<IPropertyDrawer*> sRegistry{};
			IPropertyDrawer::s_Registry = &sRegistry;
		}
	}

	void IPropertyDrawer::RegisterImpl::Register(IPropertyDrawer* drawer)
	{
		EnsureInitialized();
		s_Registry->push_back(drawer);
	}

	void IPropertyDrawer::RegisterImpl::Unregister(IPropertyDrawer* drawer)
	{
		EnsureInitialized();
		s_Registry->erase(std::find(s_Registry->begin(), s_Registry->end(), drawer));
	}
}
