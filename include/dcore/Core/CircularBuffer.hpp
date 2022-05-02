#pragma once
#include <dcore/Core/Type.hpp>
#include <vector>

namespace dstd
{
	template<typename T>
	class CircularBuffer
	{
	public:
		CircularBuffer(dstd::USize count)
		{
			Data_.reserve(count);
			Index_ = 0;
		}
		
		void Add(T &&value)
		{
			if(Data_.size() == Data_.capacity())
			{
				Index_ = 0;
				Data_[Index_++] = std::move(value);
			}
			else Data_.push_back(std::move(value));
		}

		T &operator[](dstd::USize index) { return Data_[index]; }
		const T &operator[](dstd::USize index) const { return Data_[index]; }

		dstd::USize GetSize() const { return Data_.size(); }
	private:
		std::vector<T> Data_;
		dstd::USize Index_;
	};
}
