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
		std::unordered_map<dstd::USize, dstd::USize> Sparse_;

		/** The packed list contains indices into the sparse list and data. */
		DynamicVector Packed_;

		struct Element
		{
			dstd::USize Index;
			// + Data
		};

	public:
		DynamicSparseSet(dstd::USize elementSize) : Packed_(sizeof(Element) + elementSize) {}

		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value pointer to value to insert
		 */
		template<typename T>
		void Set(dstd::USize index, const T *value)
		{
			Sparse_[index] = Packed_.GetSize();
			auto elem      = CreateElement_(index, sizeof(T), value);
			Packed_.RawAdd(elem);
			delete elem; // RawAdd copies.
		}

		/**
		 * @brief Checks if the sparse set contains an index.
		 *
		 * @param index The index to check.
		 * @return Whether the sparse set contains an index.
		 */
		bool Contains(dstd::USize index) const
		{
			auto found = Sparse_.find(index);
			return found != Sparse_.end() && found->first < Packed_.GetSize();
		}

		/**
		 * @brief Retreive the value from the packed set by the index from the sparse set.
		 * @return The value found.
		 */
		template<typename T>
		const T *Get(dstd::USize index) const
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
		T *Get(dstd::USize index)
		{
			return (T *)RawGet(index);
		}

		void *RawGet(dstd::USize index);
		const void *RawGet(dstd::USize index) const;

		/**
		 * @brief Get the internal packed array.
		 *
		 * @return The packed array.
		 */
		const DynamicVector &GetPacked() const { return Packed_; }

	private:
		template<typename T>
		Element *CreateElement_(dstd::USize index, dstd::USize valueSize, const T *value)
		{
			dstd::Byte *bytes = dstd::AllocBuffer(sizeof(Element) + valueSize);
			dstd::CopyBuffer(valueSize, bytes + sizeof(Element), (dstd::Byte *)value);
			return reinterpret_cast<Element *>(bytes);
		}
	};
} // namespace dstd
