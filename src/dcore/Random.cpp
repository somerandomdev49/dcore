#include <dcore/Core/Random.hpp>
#include <random>
#include <limits>
#include <ctime>
using namespace dcore::random;

static std::random_device rdev__;
static std::mt19937       rgen__ {rdev__()};

static std::uniform_int_distribution<int> rdst32__ {std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};

static std::uniform_int_distribution<unsigned long long> rdst64__ {std::numeric_limits<unsigned long long>::min(),
                                                                   std::numeric_limits<unsigned long long>::max()};

static std::uniform_real_distribution<> rdstfl__ {-1.0f, 1.0f};

int                dcore::random::RandomInt() { return rdst32__(rgen__); }
float              dcore::random::RandomFloat() { return rdstfl__(rgen__); }
unsigned long long dcore::random::RandomUUID() { return rdst64__(rgen__); }