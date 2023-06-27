// Fix and improve the code of your task-based coroutine.

#include <coroutine>
#include <exception>
#include <iostream>
#include <optional>
#include <utility>

template <typename T>
struct [[nodiscard]] task {
    struct promise_type {
        std::optional<T> result;

        auto get_return_object() { return task(this); }
        auto initial_suspend() const { return std::suspend_never{}; }
        auto final_suspend() const noexcept { return std::suspend_always{}; }
        void return_value(T v) { result = std::move(v); }
        [[noreturn]] void unhandled_exception() const { throw; }
    };

    task(const task&) = delete;
    task& operator=(const task&) = delete;
    task(task&& rhs) noexcept
        : promise_(std::exchange(rhs.promise_, nullptr)) {}
    task& operator=(task&& rhs) noexcept {
        promise_ = std::swap(rhs.promise_);
        return *this;
    }
    ~task() {
        auto coro =
            std::coroutine_handle<promise_type>::from_promise(*promise_);
        if (coro) coro.destroy();
    }

    [[nodiscard]] const T& get_result() const { return *promise_->result; }

private:
    task(promise_type* p) : promise_(p) {}
    promise_type* promise_;
};

task<int> foo() { co_return 42; }

int main() { std::cout << foo().get_result() << "\n"; }
