#include <iostream>

#include "benchmark.h"


void test_function_1() {
	std::cout << "Time me : " << std::endl;
}

int& test_function_2(int x) {
	static int counter = 0;
	counter += x;
	return counter;
}

class RefTest {
public:
	int a;
	int& GetA() { return a; }
};



int main() {

	std::cout << "Test function 1 Runtime: " << benchmark(test_function_1).count() << std::endl;

	// This works for references even though you would think this would create a copy
	// Reason is std::get is internally used at structural binding resolution for pairs and tuple
	// and it has return type&&  .
	// 
	// Normal return by values is also valid and works as expected
	auto [function2_result, function2_time] { benchmark(test_function_2, 5)};
	std::cout << "Test function 2 Runtime: " << function2_time.count() << std::endl;
	std::cout << "Return value of funtion 2 " << function2_result << std::endl;
	function2_result = 7;
	auto [function2_result2, function2_time2] {benchmark(test_function_2, 5)};
	std::cout << "Static variable result from reference " << function2_result << " Result of new call " << function2_result2 << " should be equal and be 12" << std::endl;

	RefTest ref_test(10);
	auto [a_ref, member_var_access_time] {benchmark(&RefTest::GetA,&ref_test)};
	std::cout << "Member variable access time: " << member_var_access_time.count() << std::endl;
	a_ref = 12;
	std::cout << "a_ref value: " << a_ref << " a inside ref_test: " << ref_test.a << std::endl;
	ref_test.a = 9;
	std::cout << "a_ref value: " << a_ref << " a inside ref_test: " << ref_test.a << std::endl;


	auto [a_ref2, member_var_access_time_ns] {benchmark<int64_t,std::nano>(&RefTest::GetA, &ref_test)};
	auto [a_ref3, member_var_access_time_micros] {benchmark<double,std::micro>(&RefTest::GetA, &ref_test)};
	auto [a_ref4, member_var_access_time_ms] {benchmark<double,std::milli>(&RefTest::GetA, &ref_test)};
	auto [a_ref5, member_var_access_time_s] {benchmark<double>(&RefTest::GetA, &ref_test)};
	auto [a_ref6, member_var_access_time_decas] {benchmark<double,std::deca>(&RefTest::GetA, &ref_test)};
	std::cout << "Member variable access time in ns: " << member_var_access_time_ns.count() << std::endl;
	std::cout << "Member variable access time in microsecond: " << member_var_access_time_micros.count() << std::endl;
	std::cout << "Member variable access time in millisecond: " << member_var_access_time_ms.count() << std::endl;
	std::cout << "Member variable access time in second: " << member_var_access_time_s.count() << std::endl;
	std::cout << "Member variable access time in decasecond: " << member_var_access_time_decas.count() << std::endl;

}