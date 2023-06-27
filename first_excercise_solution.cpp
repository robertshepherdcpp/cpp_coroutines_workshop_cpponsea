// - Compiler will guide you :-)
// - Return object of `std::suspend_never` type from suspend points

#include <exception> // std::exception
#include <future> // std::future
#include <iostream> // std::cout
#include <coroutine> // std::coroutine_traits

// template<typename T>
// struct promise;

// template<typename T>
// struct promise<std::future<T>>
// {
//     using type = std::future<T>::promise_type;
// };

template<typename T>
struct std::coroutine_traits<std::future<T>>
{
    struct promise_type
    {
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_never{}; }
        void return_value(T expr)
        {
            promise_.set_value(expr);
        }
        auto unhandled_exception()
        {
            promise_.set_exception(std::current_exception());
        }
        std::future<T> get_return_object() { return promise_.get_future(); }
        std::promise<T> promise_{};
        std::exception current_exception;
    };
};

// namespace my
// {
//     template<typename T>
//     struct future : std::future<T>
//     {
//         struct promise_type
//         {
//             auto return_value(auto expr) {current_value = expr;}
//             //auto initial_suspend() {std::suspend_never;}
//             //auto final_suspend() noexcept {std::suspend_never;}
//             T current_value = T{};
//         };
//     };
// } // namespace my

std::future<int> foo() {
    // std::promise<int> p;
    // auto f = p.get_future();
    // try {
    //   int i = 42;
    //   p.set_value(i);
    // }
    // catch(...) {
    //   p.set_exception(std::current_exception());
    // }
    // return f;

    co_return 42;
}

int main()
{
    std::cout << foo().get() << "\n";
    // https://godbolt.org/z/9x4WKv78Y
}