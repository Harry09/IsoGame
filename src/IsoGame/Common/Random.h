#pragma once

#include <random>

class Random
{
private:
	std::mt19937 m_gen;

public:
	Random()
	{
		m_gen = std::mt19937({ std::random_device{}() });
	}

	Random(unsigned int seed)
	{
		m_gen = std::mt19937({ seed });
	}

	~Random() = default;

	template<typename T>
	T Get(T min, T max)
	{
		using dist = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;

		return dist{ min, max }(m_gen);
	}
};

