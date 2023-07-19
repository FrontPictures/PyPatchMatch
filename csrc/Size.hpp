#pragma once

namespace pm {
	template <typename T>
	class Size_
	{
	public:
        T height{};
        T width{};

		Size_() {};
		Size_(T width, T height) : width(width), height(height) {};
	};
	typedef Size_<int> Size;
}