Coin Tools
----------

A small bunch of C++14 headers which can come in handy ! 

__Warning__ these are mostly drafts I (have) use(d). 

[![Build Status](https://travis-ci.org/coin-au-carre/cointools.svg?branch=master)](https://travis-ci.org/coin-au-carre/cointools)


### Features

#### Pretty print

You can print some STL containers directly. 

```c++
	std::map<std::string, int> students;
	students["John"]=12;
	students["Paul"]=14;
	students["Elton"]=10;
	std::cout << students << std::endl;
```
> [{Elton:10};{John:12};{Paul:14}]


You can even print nested containers !

```c++
std::vector<std::vector<std::string>> v{{"Jess","Samuel","Simon"},{"Natacha","Claudia"},{"Bradd"}};	std::cout << v << std::endl;
```
> [[Jess;Samuel;Simon];[Natacha;Claudia];[Bradd]]


#### Meta / compile-time stuffs 

```c++
std::array<int,4> u{{1,2,3,4}}; // also work with plain C-array
std::array<int,4> v{{4,2,3,2}};
auto w = coin::meta_dot_product(u,v);
std::cout << "coin::dot_product=" << w << std::endl;
std::cout << "coin::sqrt(16)=";
coin::CheckCompileTime<coin::sqrt(16)> out;
```
> coin::dot_product=25
> coin::sqrt(16)=4

#### Functional

Some generic and fast convenient functions such as `template<class Container> coin::remove_duplicate(Container&)` :
 
```c++
int array[] = {4,3,5,7,4,7,2,3};
for(auto el = coin::begin(array); el != coin::end(array); el ++) { std::cout << *el << ";"; } // begin, end for plain C-style array
std::vector<int> v{array, array + coin::get_size_of_array(array)}; // or auto v = coin::make_vector_from_array(array);
coin::remove_element(v, 7); // remove specific element
std::cout << "\nAfter remove_element(v,7) : " << v << std::endl;
coin::remove_duplicate(v);  // remove all redundant elements
std::cout << "After remove_duplicate : " << v << std::endl;
```

> 4;3;5;7;4;7;2;3;  
> After remove_element(v,7) : [4;3;5;7;4;7;2;3]  
> After remove_duplicate : [2;3;4;5;7]


#### Convenient timers, randomizers

```c++
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
	std::cout << "TimerFunc :" << coin::TimerFunc<>::exec<void(void)>(lambda) << " ms" << std::endl;
}
```

> [0.983796;0.763911;0.574967;0.107476;0.228597;0.289640;0.221243;0.346670]  
> [Timer] 100 #  
> [97;29;-5;-86;-17;-24;85;8]  
> [TimerFunc] 101 ms

#### Debug utilities

When not compiling with `-DNDEBUG` flag the debug macros are working :

```c++
struct Foo {
	Foo() : n{4} {}
	void bar() const {
		coin_debug_print("This is a test and n=%d", n);
		coin_debug_info();
	}
	int n;
};
Foo foo;
foo.bar();
coin_assert(2>4,"this message is for assertion debug");
```

> [debug] This is a test and n=4  
> [debug] bar() in demo.cpp:65  
> demo: demo.cpp:73: void demo_debug(): Assertion `2>4 && "this message is for assertion debug"` failed.

#### And much more

* Compile time constants (pi, infinity, epsilon) and computations (min, max, sum) taking variadic arguments `coin::min(1.0,5,-7.5f)`.  
* Helper functions to wrap vectors elements into vector of smart pointers `coin::make_vector_unique(const std::vector&)`.

```c++
auto x = coin::pi<float>();
coin::set_max_decimal_digits<float>();
std::cout << x << std::endl;
std::cout << "max(3,-2.0,6.4,6)=" << coin::max(3,-2.0,6.4,6) << std::endl;
std::cout << "4+5-3=" << coin::sum(4,5,-3) << std::endl;
```

> 3.141593  
> max(3,-2.0,6.4,6)=6.400000  
> 4+5-3=6
