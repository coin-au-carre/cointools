#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <exception>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>

#include "logger.hpp"
#include "except.hpp"
#include "debug.hpp"

namespace coin {
    
template<typename T, size_t Size> T* begin(T (& array)[Size]) { return array; } 
template<typename T, size_t Size> T* end  (T (& array)[Size]) { return array + Size; } 

template<typename T, size_t N>
constexpr size_t get_size_of_array(T(&)[N]) {
    return N;
}

template< typename T, size_t N>
std::vector<T> make_vector_from_array(const T (&data)[N]) {
    return std::vector<T>(data, data+N);
}

template<typename Container>
void remove_duplicate(Container& cont) {
    using std::begin; using std::end;
    std::sort(begin(cont), end(cont));
    cont.erase(unique(begin(cont), end(cont)), end(cont));
}

template<typename T, typename Alloc>
void remove_element(std::vector<T,Alloc> vec, const T& element) {
    vec.erase(std::remove(vec.begin(), vec.end(), element), vec.end());
}

template<class K, class T, class Comp, class Alloc> 
void remove_element(std::map<K,T,Comp,Alloc>& m, const std::pair<K,T>& p) {
    typename std::map<K,T,Comp,Alloc>::iterator it = m.begin();
    while (it != m.end()) {
        if (it->first == p.first && it->second == p.second) {
            it = m.erase(it);
        } 
        else {
            ++ it;
        }
    }
}

template<class K, class T, class Comp, class Alloc> 
void remove_element(std::map<K,T,Comp,Alloc>& m, const K& k) {
    typename std::map<K,T,Comp,Alloc>::iterator it = m.begin();
    while (it != m.end()) {
        if (it->first == k) {
            it = m.erase(it);
        } 
        else {
            ++ it;
        }
    }
}

template<class ForwardIt, class T>
T lower_bound_index(ForwardIt first, ForwardIt last, const T& value) {
    return (std::lower_bound (first, last, value) - first);
}

template<class ForwardIt, class T>
T upper_bound_index(ForwardIt first, ForwardIt last, const T& value) {
    return (std::upper_bound (first, last, value) - first);
}


template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
bool multi_dim_counter(std::vector<T>& v, const std::vector<T>& lower, const std::vector<T>& upper) {
    assert(v.size() == lower.size());
    assert(v.size() == upper.size());
    for (auto i = v.size(); i-- != 0; ) {
        ++ v[i];
        if (v[i] != upper[i] + 1) {
            return true;
        }
        v[i] = lower[i];
    }
    return false;
}


//! Create a mapping which gives the index from value
template<class Container>
auto create_reverse_index(const Container& cont) 
-> std::unordered_map<typename Container::value_type, size_t> {
    auto reverse_index = std::unordered_map<typename Container::value_type, size_t>{};
    for (size_t i = 0; i < cont.size(); i ++) {
        reverse_index[cont[i]] = i;
    }
    return reverse_index;
}


template<class InputIterator, class T>
bool exist(InputIterator first, InputIterator last, const T& val) {
    return std::find(first, last, val) != last;
}

//! Tell whether a given key exists in a map
template<typename Key, typename Value>
bool exist(const std::map<Key, Value>& m, const Key& key) {
    return  !(m.find(key) == m.end()); 
}

//! Tell whether a given key exists in a vector of reference maps
template<typename Key, typename Value>
bool exist(const std::vector<std::map<Key, Value>*>& maps, const Key& key) {
    Precondition(maps.size() > 0, "exist(): maps must not have null size!");
    for (auto const& m : maps) {
        Assert(m->size(), "exist(): map in vector maps should not be null!");
        if (!exist(*m, key)) {
            return false;
        }
    }
    return true;
}

//! Insert a block of input in output by rescaling input with output.end()
//! Pass by value input to enable move semantics when copying and modying input
template<typename Key, typename Value>
void insert_with_rescale(std::map<Key, Value>& output, std::map<Key, Value> input) {
    auto tmp_input = input; // should use move semantics &&
    // If map is empty we don't have to do the following
    if (output.size() > 0) {
        Precondition(input.begin()->first > output.begin()->first, "Insertion should be done post date to last output element!");
        // if (input.begin()->first <= output.begin()->first) {
        //     LOGERROR << "insert_with_rescale(): " << input.begin()->first << "<=" << output.begin()->first << '\n';
        // }
        Value factor = output.rbegin()->second / input.begin()->second;
        std::for_each(tmp_input.begin(), tmp_input.end(), [=](std::pair<const Key, Value>& p) { p.second *= factor; });
    }
    output.insert(tmp_input.begin(), tmp_input.end());
}



//! Keep the intersection (with same keys) between several maps on (begin,end) range keys
//! Keys must be ++ pre incrementable and have a < operator defined
//! Warning! Keys which are not in the range won't be suppressed !
template<typename Key, typename Value>
void maps_super_intersection(const std::vector<std::map<Key, Value>*>& maps, const Key& begin, const Key& end) {
    Precondition(maps.size() > 0, "vector of map is empty!");
    for (auto actual_key = begin; actual_key <= end; ++ actual_key) {
        // If these keys does not exist in at least one map then we delete these keys in every maps
        if (!exist(maps, actual_key)) {
            for (auto& m : maps) {
                auto it = m->find(actual_key); // boo! redundance with exists here
                if (it != m->end()) {
                    m->erase(it);
                }
            }
        }
    }

    for (auto const& el : maps) {
        Postcondition(el->size() == maps[0]->size(), "maps should have same size");
    }
}

//! Retrieve all keys from a std::map
template<typename Key, typename Value>
std::vector<Key> map_retrieve_keys(const std::map<Key, Value>& m) {
    std::vector<Key> keys;
    keys.reserve(m.size());
    for(auto const& el : m) { keys.push_back(el.first); }
    return keys;
}

//! Return a container difference between two containers
template<typename T>
std::vector<T> give_difference(const std::vector<T>& u, const std::vector<T>& v) {
    std::vector<T> diff;
    using std::begin; using std::end;
    std::set_difference(begin(u), end(u), begin(v), end(v), begin(diff));
    return diff;
}

//! Verify whether keys are the same
template<typename Key, typename Value>
bool maps_check_has_same_keys(const std::vector<std::map<Key, Value>*>& maps) {
    Precondition(maps.size() > 0, "vector of map is empty!");
    for (size_t i = 1; i < maps.size(); i ++) {
        auto diff = coin::give_difference(
            coin::map_retrieve_keys(*maps[i]), 
            coin::map_retrieve_keys(*maps[i-1])
        );
        if (diff.size() > 0) {
            LOGWARNING << "Maps are different with size " << diff.size() << '\n';
            return false;
        }
    }
    return true;
}

//! Store map values into a vector while keeping order
template<typename Key, typename Value>
std::vector<Value> map_values_to_vector(const std::map<Key,Value>& m) {
    auto v = std::vector<Value>{};
    v.reserve(m.size());
    transform(m.begin(), m.end(), std::back_inserter(v),
         [](std::pair<const Key, Value> const & p) { return p.second; });
    return v;
}



//! Simple wrapper on try/retry function with counter
template <typename F, typename... Args>
void retry(F func, Args&& ... args) {
    for(int retry_count: {1, 2}) {
        try {
            func(std::forward<Args>(args)...);
            break;
        }
        catch(std::exception const& e) {
            if (retry_count >= 2) {
                LOGERROR << e.what() << " last retry failed." << '\n';
            }
            else {
                LOGWARNING << e.what() << " retry: #" << retry_count << '\n';
            }
        }
    }
}

} // ns coin



