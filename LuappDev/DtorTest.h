#pragma once
#include <functional>

namespace LuappDev::cls
{
    class DtorTest
    {
        std::function<void()> f;

    public:
        explicit DtorTest(std::function<void()> f) : f(std::move(f)) {}

        ~DtorTest() { f(); }
    };
}