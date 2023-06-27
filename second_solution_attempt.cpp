// - Coroutine should suspend at its final suspend point
//   - allows to read its result
//   - requires manual coroutine destruction
// - `task<T>` is constructed with the pointer to the promise type
//   and should store it as a member
// - Store the result in `std::optional<T>`
// - On the event of unhandled exception just rethrow it
//   - good as long as coroutines are synchronous
// - Remember that `task<T>` is a resource wrapper
//   - make it safe, easy to use and hard to abuse

#include <coroutine>  // coroutine things
#include <exception>  // std::exception
#include <future>     // std::future
#include <iostream>   // std::cout
#include <optional>   // std::optional

template <typename T>
struct task {
    struct promise_type
    {
        std::optional<T> m_value{};
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_value(T x) { m_value = x; }
        void unhandled_exception() { throw std::current_exception(); }
        task<T> get_return_object() { return Handle::from_promise(*this); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    Handle handle{};

    task() {};
    task(std::optional<T> O) {}
    auto get_result() { return O.value; }
    ~task() { handle.destroy(); }
};

template <typename R, typename... Args>
struct std::coroutine_traits<std::future<R>, Args...> {
    struct promise_type {
        std::promise<R> p;
        auto get_return_object() { return p.get_future(); }
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_never{}; }
        void return_value(R v) { p.set_value(v); }
        void unhandled_exception() {
            p.set_exception(std::current_exception());
        }
    };
};

std::future<int> foo1() { co_return 42; }

task<int> foo2() { co_return 42; }

int main() {
    std::cout << foo1().get() << "\n";
    std::cout << foo2().get_result() << "\n";
}
