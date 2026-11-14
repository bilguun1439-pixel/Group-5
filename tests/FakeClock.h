#ifndef COINPATH_TESTS_FAKECLOCK_H
#define COINPATH_TESTS_FAKECLOCK_H

#include <chrono>

#include "IClock.h"

// A clock the test moves by hand, so timer behaviour is deterministic.
class FakeClock final : public IClock {
public:
    std::chrono::milliseconds now() const override { return current; }
    void advance(std::chrono::milliseconds amount) { current += amount; }

private:
    std::chrono::milliseconds current{1000};
};

#endif // COINPATH_TESTS_FAKECLOCK_H
