#pragma once
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <utility>

namespace dcore
{
	template<typename T>
	class SparseDataSet
	{
		// Note: (for now) this is a map because we can set any index.
		/** The sparse list contains indices into the packed list. */
		std::unordered_map<size_t, size_t> Sparse_;

		/** The packed list contains indices into the sparse list and data. */
		std::vector<std::pair<size_t, T>> Packed_;

	public:
		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value value to insert
		 */
		void Set(size_t index, const T &value)
		{
			Sparse_[index] = Packed_.size();
			Packed_.push_back({Sparse_.size() - 1, value});
		}
		/**
		 * @brief Checks if the sparse set contains an index.
		 *
		 * @param index The index to check.
		 * @return Whether the sparse set contains an index.
		 */
		bool Contains(size_t index) const
		{
			auto found = Sparse_.find(index);
			// TODO:      do we need this?   vvvvvvvvv
			return found != Sparse_.end() && found->first > 0 && found->first < Packed_.size();
		}

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		const T &operator[](size_t index) const { return Packed_[Sparse_.at(index)].second; }

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		T &operator[](size_t index) { return Packed_[Sparse_[index]].second; }
	};
} // namespace dcore
