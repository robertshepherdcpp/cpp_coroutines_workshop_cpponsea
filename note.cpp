template<typename T>
struct generator
{
};

template<typename T>
struct lazy
{
};

template<typename T>
struct task
{
};

auto iota(int n) -> generator<int>
{
	// co_yield, suspends then resumes, this is infinite.
	while (true)
	{
		co_yield n++;
	}
}

lazy<int> f()
{
	// co_return, basically just returns 7
	co_return 7;
}

task<int> tcp_echo_server()
{
	// co_await, suspends the coroutine
	socket_ socket;
	std::size_t n = co_await socket.async_read(asio::buffer(data));
	co_await socket.async_write(data, n);
}