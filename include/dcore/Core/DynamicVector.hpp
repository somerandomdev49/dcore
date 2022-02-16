#pragma once
#include <dcore/Core/Type.hpp>

namespace dstd
{
	class DynamicVector
	{
	public:
		/**
		 * @brief Constructor
		 *
		 * @param itemSize the size of a single element in the vector
		 * @see GetItemSize
		 */
		DynamicVector(dstd::USize itemSize) : Data_(nullptr), ItemSize_(itemSize), Size_(0) {}

		~DynamicVector();

		/**
		 * @brief Retreive an element casted to a type.
		 *
		 * @tparam T the return type.
		 * @param index index of the element.
		 * @return element.
		 */
		template<typename T>
		T *Get(dstd::USize index) const
		{
			return (T *)RawGet(index);
		}

		/**
		 * @brief Retreive a pointer to an element.
		 *
		 * @param index index of the element.
		 * @return element.
		 */
		void *RawGet(dstd::USize index) const;

		/**
		 * @brief Add an element to the vector
		 *
		 * @tparam T
		 * @param index
		 * @param value
		 */
		template<typename T>
		void Add(const T &value)
		{
			RawAdd(&value);
		}

		/**
		 * @brief Allocate space for a new item, returning a pointer to it.
		 * @return A pointer to the newly allocated item.
		 */
		template<typename T>
		T *AllocAdd()
		{
			return (T *)RawAllocAdd();
		}

		/**
		 * @brief Allocate space for a new item, returning a pointer to it.
		 * @return A pointer to the newly allocated item.
		 */
		void *RawAllocAdd();

		/**
		 * @brief Add an element to the vector (void pointer)
		 *
		 * @tparam T
		 * @param index
		 * @param value
		 */
		void RawAdd(const void *value);

		/**
		 * @brief Get the amount of elements contained in the vector.
		 *
		 * @return Amount.
		 */
		dstd::USize GetSize() const { return Size_; }

		/**
		 * @brief Get the size of a single element in bytes.
		 *
		 * @return Size.
		 */
		dstd::USize GetItemSize() const { return ItemSize_; }

	private:
		void *Data_;
		dstd::USize ItemSize_, Size_;
	};
} // namespace dstd
