#ifndef COINPATH_ICLOCK_H
#define COINPATH_ICLOCK_H

#include <chrono>

// IClock - the source of time (the second external boundary of DP7).
//
// GameTimer reads time only through this interface, so tests can replace it
// with a fake clock and check timeouts instantly and deterministically.
class IClock {
public:
    virtual ~IClock() = default;

    // Time since an arbitrary fixed point; only differences are meaningful.
    virtual std::chrono::milliseconds now() const = 0;
};

// The real clock, backed by std::chrono::steady_clock (never jumps backwards).
class SystemClock final : public IClock {
public:
    std::chrono::milliseconds now() const override {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch());
    }
};

#endif // COINPATH_ICLOCK_H
