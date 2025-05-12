#pragma once

#include "ll/api/mod/NativeMod.h"

namespace life_apple {

class LifeApple {

public:
    static LifeApple& getInstance();

    LifeApple() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace life_apple
