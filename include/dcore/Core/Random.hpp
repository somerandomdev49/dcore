#pragma once
#include <string>

namespace dcore::random
{
	/**
	 * @brief Generates a random float in range of [0, 1)
	 *
	 * @return float
	 */
	float RandomFloat();

	/**
	 * @brief Generates a random signed integer (usually 32 bits)
	 */
	int RandomInt();

	/**
	 * @brief Generates a random unsigned long long (usually 64 bits)
	 */
	unsigned long long RandomInt64();

	/**
	 * @brief Generates a random UUID string
	 */
	std::string RandomUUIDString();
} // namespace dcore::random
