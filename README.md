Introduction to C++ coroutines. Phil Nash

co_ make a function a couroutine.

 The caller can finish before the coroutine does, both can do on seperate threads.

 Only 1 heap allocation

 co_await
 co_yield
 co_return

cant tell if it is a coroutine just form the function signature

constexpr functions, constructors, coroutines and the main() function cannot be coroutines.

Coroutines cannot use variadic arguements
cant use plain return type
cant use auto for the placeholder return type

this is allowed

template<typename... Ts>
auto function(Ts...)          // unpacked at compile time so it is fine

template<typename T>
auto function(T x, ...)       // c-style is not allowed

coroutine_return_type

std::coroutine_traits : ReturnType, Args...  --> provides the promise_type
std::coroutine_handle : Promise              --> does not do lifetime management, you will need to destroy the coroutine using
                                                 .destroy();
task<void> coro()
{
    int i = co_await f();
    std::cout << "f() => " << i << "\n";
}

template<typename T>
struct task
{
    // this needs to be here.
    stuct promise_type
    {
    };
}

template<class R, class... ARgs>
  requires requires {typename R::promise_type;}
sturct promise_type<R, ARgs>
{
    using type = R::promise_type;
};

struct promise_type
{
    promise_type() = defualt;
    promise_type(coro_args...);

    get_return_object;
    return_value(auto expr)
    yield_value(auto expr);
};

initial_suspend        --> returns awaitable, that specifies the coroutines initilization
final_suspend noexcept --> return awaitable that specifies that the coroutines behaviour on its finalization.

std::suspend_always
std::suspend_never

only return_value or return_void NOT BOTH

struct coro_deleter
{
};

```cpp
struct awaiter
{
    bool await_ready();
    auto await_suspend(std::coroutine_handle<P> H)
    T await_resume();
};
```