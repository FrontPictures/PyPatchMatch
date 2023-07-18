#include "Mat.h"
#include <cmath>
#include <string.h>
#include <iostream>


pm::Mat::Mat()
{
}

pm::Mat::Mat(Size size, int type) : mSize(size), mType(type)
{
	mElementSize = 1 << (type % PM_DEPTH_MAX / 2);
	mChannels = (type - type % PM_DEPTH_MAX) / PM_DEPTH_MAX + 1;
	mData = std::shared_ptr<char[]>(new char[mSize.height * mSize.width * mChannels * mElementSize]);
}

pm::Mat::Mat(Size size, int type, std::shared_ptr<char[]> data) : mSize(size), mType(type), mData(data)
{
	mElementSize = 1 << (type % PM_DEPTH_MAX / 2);
	mChannels = (type - type % PM_DEPTH_MAX) / PM_DEPTH_MAX + 1;
}

pm::Mat& pm::Mat::operator=(const Scalar& scalar)
{
	for (size_t i = 0; i < mSize.height; i++)
	{
		for (size_t j = 0; j < mSize.width; j++)
		{
			for (size_t c = 0; c < mChannels; c++)
			{
				mData[i * mSize.width * mChannels + j * mChannels + c] = scalar.val[c];
			}
		}
	}
	return *this;
}

pm::Mat pm::Mat::clone() const
{
	if (mData == nullptr) {
		return pm::Mat(mSize, mType, nullptr);
	}

	std::shared_ptr<char[]> copy(new char[mSize.height * mSize.width * mChannels * mElementSize]);
	memcpy(copy.get(), mData.get(), mSize.height * mSize.width * mChannels * mElementSize);

	return pm::Mat(mSize, mType, copy);
}

pm::Size pm::Mat::size() const
{
	return mSize;
}

bool pm::Mat::empty() const
{
	return mSize.width * mSize.height == 0 || mData == nullptr;
}

pm::Mat& pm::Mat::setTo(const Scalar& scalar)
{
	for (size_t i = 0; i < mSize.height; i++)
	{
		for (size_t j = 0; j < mSize.width; j++)
		{
			for (size_t c = 0; c < mChannels; c++)
			{
				mData[i * mSize.width * mChannels + j * mChannels + c] = scalar.val[c];
			}
		}
	}
	return *this;
}