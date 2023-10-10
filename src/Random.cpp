#include <Random.hpp>

RandomGenerator* RandomGenerator::m_instance = nullptr;

RandomGenerator* RandomGenerator::instance()
{
	if(m_instance == nullptr)
		m_instance = new RandomGenerator();

	return m_instance;
}

RandomGenerator::RandomGenerator()
{
	m_beginTime = std::chrono::high_resolution_clock::now();
}

int RandomGenerator::generateRandomInt()
{
	using std::chrono::high_resolution_clock;
	using std::chrono::milliseconds;
	using std::chrono::duration_cast;

	// NOTE: create seed for mt19937 base on time
	auto dur = duration_cast<milliseconds>(high_resolution_clock::now() - m_beginTime);

	std::mt19937 prng(static_cast<std::mt19937::result_type>(dur.count()));

	// std::random_device dev;
	// std::mt19937 rng(dev());

	std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6);

	int res = dist6(prng);
	std::cout << "Random res:" << res << std::endl;

	return res;
}
