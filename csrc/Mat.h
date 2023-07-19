#pragma once

#include "interface.h"
#include "Size.hpp"
#include "Scalar.hpp"
#include <limits>
#include <memory>

namespace pm {
	class Mat
	{
	private:
		std::shared_ptr<char[]> mData;
		Size mSize;
		int mChannels = 0;
		int mElementSize = 0;
		int mType = 0;
	public:
		Mat();
		Mat(Size size, int type);
		Mat(Size size, int type, std::shared_ptr<char[]> data);
		Mat& operator=(const Scalar& scalar);
		Mat clone() const;
		Size size() const;
		bool empty() const;
		Mat& setTo(const Scalar& scalar);
		template<typename T> T& at(int row, int col) {
			return reinterpret_cast<T*>(mData.get())[row * mSize.width * mChannels + col * mChannels];
		};
		template<typename T> const T& at(int row, int col) const {
			return reinterpret_cast<T*>(mData.get())[row * mSize.width * mChannels + col * mChannels];
		};
		template<typename T> T* ptr(int row, int col) {
			return reinterpret_cast<T*>(mData.get()) + row * mSize.width * mChannels + col * mChannels;
		};
		template<typename T> const T* ptr(int row, int col) const {
			return reinterpret_cast<T*>(mData.get()) + row * mSize.width * mChannels + col * mChannels;
		};
		~Mat() {};
	};

	template <typename ToType, typename FromType>
	ToType saturate_cast(const FromType& value) {
		return static_cast<ToType>(value < std::numeric_limits<ToType>::min() ? std::numeric_limits<ToType>::min() :
			value > std::numeric_limits<ToType>::max() ? std::numeric_limits<ToType>::max() :
			value);
	}
}