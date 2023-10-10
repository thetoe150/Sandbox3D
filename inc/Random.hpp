#include <iostream>
#include <random>
#include <chrono>

class RandomGenerator{
public:
	static RandomGenerator* instance();

	int generateRandomInt();

protected:
	RandomGenerator();

private:
	static RandomGenerator* m_instance;
	std::chrono::high_resolution_clock::time_point m_beginTime;
};
