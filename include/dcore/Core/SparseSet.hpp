#pragma once
#include <dcore/Core/Type.hpp>

#include <unordered_map>
#include <type_traits>
#include <utility>
#include <vector>

namespace dstd
{
	template<typename T>
	class SparseIntegerSet
	{
		static_assert(std::is_integral_v<T>, "SparseIntegerSet only allows integer type arguments!");

		// Note: (for now) this is a map because we can set any index.
		/** The sparse list contains indices into the packed list. */
		std::unordered_map<dstd::USize, dstd::USize> Sparse_;

		/** The packed list contains indices into the sparse list. */
		std::vector<T> Packed_;
	public:
		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value value to insert
		 */
		void Set(T value)
		{
			Sparse_[value] = Packed_.size();
			Packed_.push_back(value);
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
			return found != Sparse_.end() && found->first < Packed_.size();
		}

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		const T &operator[](dstd::USize index) const { return Packed_[Sparse_.at(index)]; }

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		T &operator[](dstd::USize index) { return Packed_[Sparse_[index]]; }

		/**
		 * @brief Get the internal packed array.
		 *
		 * @return The packed array.
		 */
		const std::vector<T> &GetPacked() const { return Packed_; }
	};

	template<typename T>
	class SparseSet
	{
		// Note: (for now) this is a map because we can set any index.
		/** The sparse list contains indices into the packed list. */
		std::unordered_map<dstd::USize, dstd::USize> Sparse_;

		/** The packed list contains indices into the sparse list and data. */
		std::vector<std::pair<dstd::USize, T>> Packed_;

	public:
		/**
		 * @brief Set a value at an index inside of the sparse list.
		 *
		 * @param index index in the sparse set.
		 * @param value value to insert
		 */
		void Set(dstd::USize index, const T &value)
		{
			Sparse_[index] = Packed_.size();
			Packed_.push_back({index, value});
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
			return found != Sparse_.end() && found->first < Packed_.size();
		}

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		const T &operator[](dstd::USize index) const { return Packed_[Sparse_.at(index)].second; }

		/**
		 * @brief Retreive the value at an index inside of the sparse list.
		 *
		 * @param index index of the value to get in the sparse list.
		 * @return the value found.
		 */
		T &operator[](dstd::USize index) { return Packed_[Sparse_[index]].second; }

		/**
		 * @brief Get the internal packed array.
		 *
		 * @return The packed array.
		 */
		const std::vector<std::pair<dstd::USize, T>> &GetPacked() const { return Packed_; }
	};
} // namespace dcore
