#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/DynamicVector.hpp>
#include <dcore/Core/Memory.hpp>

#include <unordered_map>
#include <type_traits>
#include <utility>
#include <vector>

namespace dstd
{
	class DynamicSparseSet
	{
		// Note: (for now) this is a map because we can set any index.
		/** The sparse list contains indices into the packed list. */
		std::unordered_map<USize, USize> Sparse_;

		/** The packed list contains indices into the sparse list and data. */
		DynamicVector Packed_;

		struct Element
		{
			USize Index;
			// + Data
		};

	public:
		DynamicSparseSet(USize elementSize) : Packed_(sizeof(Element) + elementSize) {}

		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value pointer to value to insert
		 */
		template<typename T>
		T *Set(USize index, const T *value)
		{
			return (T *)RawSet(elem);
		}

		/**
		 * @brief Checks if the sparse set contains an index.
		 *
		 * @param index The index to check.
		 * @return Whether the sparse set contains an index.
		 */
		bool Contains(USize index) const;

		/**
		 * @brief Retreive the value from the packed set by the index from the sparse set.
		 * @return The value found.
		 */
		template<typename T>
		const T *Get(USize index) const
		{
			return (const T *)RawGet(index);
		}

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		template<typename T>
		T *Get(USize index)
		{
			return (T *)RawGet(index);
		}

		void *RawGet(USize index);
		const void *RawGet(USize index) const;

		/**
		 * @brief Sets an item
		 *
		 * @param index
		 * @param value
		 * @returns The pointer to the newly created item.
		 */
		void *RawSet(USize index, const void *value);

		/**
		 * @brief Get the internal packed array.
		 *
		 * @return The packed array.
		 */
		const DynamicVector &GetPacked() const { return Packed_; }

	private:
		template<typename T>
		Element *CreateElement_(USize index, USize valueSize, const T *value)
		{
			Byte *bytes = AllocBuffer(sizeof(Element) + valueSize);
			CopyBuffer(valueSize, bytes + sizeof(Element), (Byte *)value);
			return reinterpret_cast<Element *>(bytes);
		}
	};
} // namespace dstd
