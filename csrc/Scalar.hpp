#pragma once

namespace pm {

	template <typename T>
	class Scalar_
	{
	public:
		T val[4];
		Scalar_(T v0, T v1 = 0, T v2 = 0, T v3 = 0) {
			val[0] = v0;
			val[1] = v1;
			val[2] = v2;
			val[3] = v3;
		};
		static Scalar_<T> all(T v0) {
			return Scalar_<T>(v0, v0, v0, v0);
		};
	};

	typedef Scalar_<int> Scalar;
}