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

coroutine_return_type -->