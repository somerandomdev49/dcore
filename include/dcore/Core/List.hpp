#pragma once
#include <cstdlib>

//========================================================//
//                      NOT USED                          //
//========================================================//

namespace dcore
{
	template<typename T>
	class DefaultListAllocator
	{
	public:
		using SizeType = std::size_t;

		T *Alloc(const SizeType size) { return new T[size]; }
		void Delete(T *pointer) { delete[] pointer; }
	};

	template<typename T, typename SizeType = typename DefaultListAllocator<T>::SizeType,
	         typename Allocator = DefaultListAllocator<T>>
	class List
	{
		T *Data_;
		SizeType Size_, Reserved_;

	public:
		/**
		 * Retrieves the internal data.
		 * \returns A pointer to internal data.
		 **/
		const T *GetData() const;

		/**
		 * \brief Adds an element to the list.
		 * \param value The value to add.
		 * \returns The new size of the list.
		 **/
		SizeType Add(const T &value);

		/**
		 * \brief Adds an element to the list.
		 * \param value The value to add
		 * \returns The new size of the list.
		 **/
		void Add(T &&value);

		/**
		 * Retrieves the size of the list.
		 * \returns The size of the list.
		 **/
		SizeType Size() const;
	};

	template<typename T, typename SizeType, typename Allocator>
	const T *List<T, SizeType, Allocator>::GetData() const
	{
		return Data_;
	}

	template<typename T, typename SizeType, typename Allocator>
	SizeType List<T, SizeType, Allocator>::Add(const T &value)
	{
		if(Reserved_ != 0) // if we have some space reserved
		{
			Data_[Size_++] = std::move(value);
			Reserved_ -= 1;
		}
		else
		{
			Allocator()
		}

		return Size_;
	}
} // namespace dcore
