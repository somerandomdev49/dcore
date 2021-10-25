#pragma once

namespace dcore::random
{
	void InitializeRandom();

	template<typename T>
	T Random();

	// Returns a random number in range [-1.0f, 1.0f)
	float RandomFloat();

	// Returns a random number in range [MIN(INT); MAX(INT) - 1]
	int RandomInt();

	// Returns a random number in range [MIN(ULL); MAX(ULL) - 1]
	unsigned long long RandomUUID();
} // namespace dcore::random
