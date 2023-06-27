// Fix and improve the code of your task-based coroutine.

#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <optional>
#include <utility>
#include <memory>

template <typename T>
struct coro_deleter {
    // coro_deleter(std::coroutine_handle<T>& handle_) : Handle{&handle_} {}

    // ~coro_deleter() { Handle.destroy(); }

    auto operator()(T* to_delete) -> void
    {
        auto coro = std::coroutine_handle<T>::from_promise(*to_delete);
        coro.destroy();
    }
};

template <typename T>
struct [[nodiscard]] task {
    struct promise_type {
        std::optional<T> result;

        auto get_return_object()
            noexcept(requires { task(this); })
        {
            return task(this);
        }
        auto initial_suspend() const { return std::suspend_never{}; }
        auto final_suspend() const noexcept { return std::suspend_always{}; }
        void return_value(T v) noexcept(std::move_constructible<T>&&
            std::is_move_assignable<T>()) {
            result = std::move(v);
        }
        [[noreturn]] void unhandled_exception() const noexcept(false) { throw; }
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

    }

    [[nodiscard]] const T& get_result() const { return *promise_->result; }

private:
    task(promise_type* p) : promise_(p) {}
    std::unique_ptr<promise_type, coro_deleter<promise_type>> promise_;

};

task<int> foo() { co_return 42; }

int main() { std::cout << foo().get_result() << "\n"; }