#pragma once
#include <type_traits>
#include <functional>
#include <chrono>


template<typename Rep,
	typename Period, typename Callable, typename... Args>
using BenchmarkReturnType = std::conditional_t<std::is_same_v<std::invoke_result_t<Callable, Args...>, void>,
	std::chrono::duration<Rep, Period>,
	std::pair<std::invoke_result_t<Callable, Args...>, std::chrono::duration<Rep, Period>>
>;

template<typename Rep= double,
	typename Period = std::ratio<1>,
	typename Callable,typename... Args>
BenchmarkReturnType<Rep, Period, Callable,Args...> benchmark(Callable&& callable, Args... args) {
	const auto start = std::chrono::high_resolution_clock::now();
	if constexpr (std::is_same_v<std::invoke_result_t<Callable,Args...>, void>) {
		std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
		return std::chrono::high_resolution_clock::now() - start;
	}
	else {
		decltype(auto) result{ std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...) };
		const auto function_runtime{ std::chrono::high_resolution_clock::now() - start };
		return std::pair<std::invoke_result_t<Callable, Args...>, std::chrono::duration<Rep, Period>>(result,function_runtime);
	}
}


