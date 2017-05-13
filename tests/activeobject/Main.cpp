/*
---------------------------------------------------------------------------
This file is part of patterns.
http://github.com/woodjazz/patterns
Copyright (c) 2016-2017 Néstor Silveira Gorski

---------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
---------------------------------------------------------------------------
*/
#include "ActiveObject.h"
#include <assert.h>
#include <functional>
#include <iostream>
using namespace utils;

static void TestCase01() {
    auto task1 = [](int a, int b) {
        cout << "START task1(" << a << "," << b << ")\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "END task1(" << a << "," << b << ")\n" << std::flush;
        return a + b;
    };

    auto task2 = [](float a, int b, double c) {
        cout << "START task2(" << a << "," << b << "," << c << ")\n"
             << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "END task2(" << a << "," << b << "," << c << ")\n"
             << std::flush;
        return a * b * c;
    };

    std::string helloStr = "Hello";
    auto task3 = [&]() {
        cout << "START task3()\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cout << "END task3()\n" << std::flush;
        return helloStr + " from test3";

    };

    ActiveObject obj;
    auto result1 = obj.Enqueue<int, int, int>(task1, 10, 20);
    auto result2 =
        obj.Enqueue<double, float, int, double>(task2, 10.5f, 20, -2.5);
    auto result3 = obj.Enqueue<std::string>(task3);

    //...

    auto v1 = result1.get();
    auto v2 = result2.get();
    auto v3 = result3.get();

    assert(v1 == 30);
    assert(v2 == 10.5f * 20 * -2.5);
    assert(v3 == "Hello from test3");
}

static void TestCase02() {
    class MyActiveClass : public ActiveObject {
    public:
        MyActiveClass() {}
        ~MyActiveClass() { Join(); }
        std::future<float> AsyncDoSlowProcess(float initialValue) {
            return Enqueue<float>(
                [this](float value) { return DoSlowProcess(value); },
                initialValue);
        }
        struct Result {
            int randoNumber;
            int a;
            float b;
            std::string description;
        };
        std::future<Result> AsyncCalculateBigResult() {
            return Enqueue<Result>([this]() { return CalculateBigResult(); });
        }

    private:
        float DoSlowProcess(float value) {
            cout << "START DoSlowProcess(" << value << ")\n" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            cout << "END DoSlowProcess(" << value << ")\n" << std::flush;
            return value * 2;
        }
        Result CalculateBigResult() {
            cout << "START CalculateBigResult()\n" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cout << "END CalculateBigResult()\n" << std::flush;
            return Result{std::rand(), 123, 200.123f, "I am a big result."};
        }
    };

    MyActiveClass activeObj;
    auto slowProcessResult1 = activeObj.AsyncDoSlowProcess(100);
    auto bigResult1 = activeObj.AsyncCalculateBigResult();
    auto slowProcessResult2 = activeObj.AsyncDoSlowProcess(200);
    auto bigResult2 = activeObj.AsyncCalculateBigResult();

    assert(slowProcessResult1.wait_for(std::chrono::milliseconds(100)) !=
           future_status::ready);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    assert(slowProcessResult1.wait_for(std::chrono::milliseconds(10)) ==
           future_status::ready);
    assert(bigResult1.wait_for(std::chrono::milliseconds(250)) ==
           future_status::ready);
    assert(slowProcessResult2.wait_for(std::chrono::milliseconds(10)) !=
           future_status::ready);
    assert(bigResult2.wait_for(std::chrono::milliseconds(10)) !=
           future_status::ready);

    //...

    assert(slowProcessResult1.get() == 200);
    auto bigResultValue = bigResult1.get();
    assert(bigResultValue.a == 123);
    assert(bigResultValue.b == 200.123f);
    assert(bigResultValue.description == "I am a big result.");
    assert(slowProcessResult2.get() == 400);
}

static void TestCase03() {

    auto task1 = [](int a, int b) {
        cout << "START task1(" << a << "," << b << ")\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "END task1(" << a << "," << b << ")\n" << std::flush;
        return a + b;
    };

    auto task2 = [](float a, int b, double c) {
        cout << "START task2(" << a << "," << b << "," << c << ")\n"
             << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        cout << "END task2(" << a << "," << b << "," << c << ")\n"
             << std::flush;
        return a * b * c;
    };

    std::string helloStr = "Hello";
    auto task3 = [&]() {
        cout << "START task3()\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cout << "END task3()\n" << std::flush;
        return helloStr + " from test3";

    };

    auto now = chrono::steady_clock::now();
    auto start1 = now + Milliseconds(2000);
    auto start2 = now + Milliseconds(5000);
    auto start3 = now + Milliseconds(10000);
    auto start4 = now + Milliseconds(10200);

    ActiveObject obj;
    auto result1 =
        obj.Schedule<int, int, int>(start1, Milliseconds(300), task1, 10, 20);
    auto result2 = obj.Schedule<double, float, int, double>(
        start2, Milliseconds(400), task2, 10.5f, 20, -2.5);
    auto result3 = obj.Schedule<std::string>(start3, Milliseconds(100), task3);
    auto result4 =
        obj.Schedule<int, int, int>(start4, Milliseconds(300), task1, 10, 20);

    //...

    auto v1 = result1.get();
    auto v2 = result2.get();
    auto v3 = result3.get();
    auto v4Exception = false;
    try {
        auto v4 = result4.get();
    } catch (std::future_error& e) {
        printf("%s\n", e.what());
        v4Exception = true;
    }

    assert(v1 == 30);
    assert(v2 == 10.5f * 20 * -2.5);
    assert(v3 == "Hello from test3");
    assert(v4Exception);
}

int main() {
    // TestCase01();
    // TestCase02();
    TestCase03();
    return 0;
}
