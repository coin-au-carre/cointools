#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>

#include "coin/coin"

void demo_pretty_print() {
	using coin::operator<<; // using the pretty print feature
	std::vector<int> v1(10);
	std::generate(v1.begin(), v1.end(), [](){static int n{0}; return n ++;});
	std::cout << v1 << std::endl;


	std::vector<std::vector<double>> v2{{1.4,4.3,2.4},{2.0,42.1},{1.4}};
	std::cout << v2 << std::endl;

	std::vector<std::vector<std::string>> v3{{"Jess","Samuel","Simon"},{"Natacha","Claudia"},{"Bradd"}};
	std::cout << v3 << std::endl;

	std::map<std::string, int> students;

	students["John"]=12;
	students["Paul"]=14;
	students["Elton"]=10;
	std::cout << students << std::endl;
	}

void demo_timer_and_random() {
	using coin::operator<<; // using the pretty print feature
	{
		coin::Timer<> timer; // begin timer which will automatically end at end of scope
		std::vector<float> v(8);
		coin::uniform_randomizer(v); // fill v with random float values
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
		std::cout << v << std::endl;
	}
	{
		auto lambda = [] { 
			std::vector<int> v(8);
			coin::uniform_randomizer(v,-100,100); // fill v with random int values
			std::cout << v << std::endl; 
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
			};
		std::cout << "[TimerFunc] " << coin::TimerFunc<>::exec<void(void)>(lambda) << " ms" << std::endl;
	}
}

void demo_vector_smart_ptr() {
	std::vector<int> v(6);
	std::generate(v.begin(), v.end(), [](){static int n{1}; return n ++;});
	auto smart_v = coin::make_unique_vector(v);
	smart_v.push_back(std::move(smart_v[0]));
	smart_v[0] = std::make_unique<int>(42);
	for (auto const& el : smart_v) { std::cout << *el << ";"; }
	std::cout << std::endl;
}


void demo_functional() {
	using coin::operator<<;
	std::vector<int> v{4,3,5,7,4,7,2,3};
	std::cout << "Before : " << v << std::endl;
	coin::remove_duplicate(v);
	std::cout << "After  : " << v << std::endl;
}	

void demo_compile_time() {
	auto x = coin::pi<float>();
	coin::set_max_decimal_digits<float>();
	std::cout << x << std::endl;
	std::cout << "max(3,-2.0,6.4,6)=" << coin::max(3,-2.0,6.4,6) << std::endl;
	std::cout << "4+5-3=" << coin::sum(4,5,-3) << std::endl;
}

void demo_debug() {
	struct Foo {
		Foo() : n{4} {}
		void bar() const {
			coin_debug_print("This is a test and n=%d", n);
			coin_debug_info();
			coin_debug_to_string();
		}
		std::string to_string() const { return "Calling Foo::to_string() !"; }
		int n;
	};
	Foo foo;
	foo.bar();
	coin_assert(2>4,"this message is for assertion debug");
}

int main() {
	demo_pretty_print();
	demo_timer_and_random();
	demo_vector_smart_ptr();
	demo_compile_time();
	demo_functional();
	demo_debug();
}
