#pragma once
#include <dcore/Core/Type.hpp>
#include <dcore/Core/DynamicVector.hpp>

#include <unordered_map>
#include <type_traits>
#include <utility>
#include <vector>

namespace dcore
{
	class DynamicSparseDataSet
	{
		// Note: (for now) this is a map because we can set any index.
		/** The sparse list contains indices into the packed list. */
		std::unordered_map<dstd::USize, dstd::USize> Sparse_;

		/** The packed list contains indices into the sparse list and data. */
		DynamicVector Packed_;

		struct Element
		{
			dstd::USize index;
			// DO NOT UNCOMMENT! dstd::Byte data[];
		};

	public:
		DynamicSparseDataSet(dstd::USize elementSize);

		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value value to insert
		 */
		template<typename T>
		void Set(dstd::USize index, const T &value)
		{
			Sparse_[index] = Packed_.size();
			Packed_.Add(CreateElement_(index, sizeof(T), &value));
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
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		template<typename T>
		const T &Get(dstd::USize index) const
		{
			return *(T *)(Packed_.Get<Element>(Sparse_.at(index)) + sizeof(Element));
		}

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		template<typename T>
		T &Get(dstd::USize index)
		{
			return *(T *)(Packed_.Get<Element>(Sparse_.at(index)) + sizeof(Element));
		}

		/**
		 * @brief Get the internal packed array.
		 *
		 * @return The packed array.
		 */
		const DynamicVector &GetPacked() const { return Packed_; }
	};
} // namespace dcore
