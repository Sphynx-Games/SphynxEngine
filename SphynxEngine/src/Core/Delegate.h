#pragma once

#include "Core/Core.h"
#include "Container/Array.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"


namespace Sphynx
{
	template<typename T, size_t Size = sizeof(void*) * 2>
	class Delegate;

	template<typename TReturn, typename ...Args, size_t Size>
	class Delegate<TReturn(Args...), Size>
	{
		static_assert(Size >= sizeof(void*), "Size should be at least the size of a pointer");

	public:
		constexpr Delegate() = default;
		~Delegate() { Unbind(); }

		constexpr TReturn Execute(Args... args) const
		{
			SPX_CORE_ASSERT(IsBound(), "Cannot invoke a delegate with no invokable binded");
			return (*m_Callable)(*this, args...);
		}

		template<typename F>
		constexpr void Bind(const F& f)
		{
			SPX_CORE_ASSERT(!IsBound(), "Make sure to unbind before binding");
			m_Callable = &FunctorDelegateImpl<F>::Invoke;
			m_Storage.template Init<FunctorDelegateImpl<F>>(f);
		}

		template<typename T, typename TMethod>
		constexpr void Bind(T* object, TMethod method)
		{
			SPX_CORE_ASSERT(!IsBound(), "Make sure to unbind before binding");
			m_Callable = &MemberFuncDelegateImpl<T, TMethod>::Invoke;
			m_Storage.template Init<MemberFuncDelegateImpl<T, TMethod>>(
				MemberFuncDelegateImpl<T, TMethod>(object, method)
			);
		}

		constexpr void Unbind()
		{
			m_Storage.Clear();
			m_Callable = nullptr;
		}

		constexpr bool IsBound() const
		{
			return m_Callable != nullptr;
		}

	private:

		template<typename T, typename TMethod>
		struct MemberFuncDelegateImpl
		{
			constexpr MemberFuncDelegateImpl(T* object, TMethod method) : object(object), method(method) {}
			constexpr explicit MemberFuncDelegateImpl(const MemberFuncDelegateImpl& other) = default;
			constexpr MemberFuncDelegateImpl(MemberFuncDelegateImpl&& other) = default;

			constexpr inline static TReturn Invoke(const Delegate& d, Args... args)
			{
				const MemberFuncDelegateImpl& impl = d.m_Storage.template Get<const MemberFuncDelegateImpl>();
				return (impl.object->*impl.method)(args...);
			}

			T* object;
			TMethod method;
		};

		template<typename Functor>
		struct FunctorDelegateImpl
		{
			constexpr FunctorDelegateImpl(const Functor& functor) : functor(functor) {};
			constexpr explicit FunctorDelegateImpl(const FunctorDelegateImpl& other) = default;
			constexpr FunctorDelegateImpl(FunctorDelegateImpl&& other) = default;

			constexpr inline static TReturn Invoke(const Delegate& d, Args... args)
			{
				const FunctorDelegateImpl& impl = d.m_Storage.template Get<const FunctorDelegateImpl>();
				return impl.functor(args...);
			}

			Functor functor;
		};

		struct Buffer
		{
			constexpr Buffer() : m_Data{ std::byte(0) } {}

			template<typename T, typename V> constexpr inline T* Init(const V& v) { return new(m_Data) T(v); }
			template<typename T> constexpr inline T& Get() { return *reinterpret_cast<T*>(m_Data); }
			template<typename T> constexpr inline const T& Get() const { return *reinterpret_cast<const T*>(m_Data); }

		private:
			alignas(void*) std::byte m_Data[Size];
		};

		template<typename T>
		struct StackAlloc // stack allocator
		{
			template<typename V> constexpr inline static T* Init(Buffer& buffer, const V& v) { return buffer.template Init<T>(v); }
			constexpr inline static T& Get(Buffer& buffer) { return buffer.template Get<T>(); }
			constexpr inline static const T& Get(const Buffer& buffer) { return buffer.template Get<T>(); }
			constexpr inline static void Delete(Buffer& /*buffer*/) { }
		};

		template<typename T>
		struct HeapAlloc // heap allocator
		{
			template<typename V> constexpr inline static T* Init(Buffer& buffer, const V& v) { return *buffer.template Init<T*>(new T(v)); }
			constexpr inline static T& Get(Buffer& buffer) { return *buffer.template Get<T*>(); }
			constexpr inline static const T& Get(const Buffer& buffer) { return *buffer.template Get<const T*>(); }
			constexpr inline static void Delete(Buffer& buffer) { T* ptr = buffer.template Get<T*>(); delete ptr; }
		};

		template<typename T>
		struct SelectAlloc // given a T, typedefs "Type" as StackAlloc or HeapAlloc using sizeof(T)
		{
			typedef typename std::conditional_t<
				sizeof(T) <= Size,
				StackAlloc<T>,
				HeapAlloc<T>> Type;
		};

		struct Storage // storage interface
		{
			template<typename T, typename V> constexpr inline T* Init(const V& v) { m_Deleter = SelectAlloc<T>::Type::Delete; return SelectAlloc<T>::Type::Init(m_Buffer, v); }
			template<typename T> constexpr inline T& Get() { return SelectAlloc<T>::Type::Get(m_Buffer); }
			template<typename T> constexpr inline const T& Get() const { return SelectAlloc<T>::Type::Get(m_Buffer); }
			constexpr inline void Clear() { if (m_Deleter != nullptr) m_Deleter(m_Buffer); m_Deleter = nullptr; }

			void(*m_Deleter)(Buffer&);
			Buffer m_Buffer;
		} m_Storage{};

		typedef TReturn(*Callable)(const Delegate&, Args...);
		Callable m_Callable = nullptr;

	};

	using DelegateHandle = UniqueType<UUID, struct __DelegateHandle__>;

	template<typename T, size_t Size = sizeof(void*) * 2>
	class MulticastDelegate;

	template<typename TReturn, typename ...Args, size_t Size>
	class MulticastDelegate<TReturn(Args...), Size>
	{
	public:
		constexpr MulticastDelegate() = default;
		~MulticastDelegate()
		{
			for (auto& delegate : m_Delegates)
			{
				delegate.Unbind();
			}
			m_Handles.RemoveAll();
			m_Delegates.RemoveAll();
		}

		constexpr void Broadcast(Args... args) const
		{
			for (const auto& delegate : m_Delegates)
			{
				delegate.Execute(args...);
			}
		}

		template<typename F>
		constexpr DelegateHandle Bind(const F& f)
		{
			DelegateHandle handle;
			handle.Generate();
			m_Handles.Add(handle);

			Delegate<TReturn(Args...), Size> delegate;
			delegate.Bind(f);
			m_Delegates.Add(delegate);

			return handle;
		}

		template<typename T, typename TMethod>
		constexpr DelegateHandle Bind(T* object, TMethod method)
		{
			DelegateHandle handle;
			handle.Generate();
			m_Handles.Add(handle);

			Delegate<TReturn(Args...), Size> delegate;
			delegate.Bind(object, method);
			m_Delegates.Add(delegate);

			return handle;
		}

		constexpr void Unbind(const DelegateHandle& handle)
		{
			size_t index = m_Handles.IndexOf(handle);
			SPX_CORE_ASSERT(index < m_Handles.Size(), "Index out of bounds!");

			m_Delegates[index].Unbind();
			m_Delegates.RemoveAt(index);
			m_Handles.RemoveAt(index);
		}

		constexpr bool IsBound(const DelegateHandle& handle) const
		{
			size_t index = m_Handles.IndexOf(handle);
			SPX_CORE_ASSERT(index < m_Handles.Size(), "Index out of bounds!");
			return m_Delegates[index].IsBound();
		}

	private:
		Array<DelegateHandle> m_Handles;
		Array<Delegate<TReturn(Args...), Size>> m_Delegates;
	};
}
