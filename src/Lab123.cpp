#include <algorithm>
#include <execution>
#include <iostream>
#include <random>
#include <string>
#include <chrono>

template<typename T>
double diffInSec(T end, T start) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return static_cast<double>(duration) / 1000.0;
}

template<typename T, typename Policy, typename Text, typename Number>
void doJob(T v, Policy pol, Text name, Number job) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-100, 100);
    
    auto start_generate = std::chrono::high_resolution_clock::now();
    std::generate(pol, v.begin(), v.end(), [=]() mutable { return dist(gen); });
    auto end_generate = std::chrono::high_resolution_clock::now();
    
    auto start_any = std::chrono::high_resolution_clock::now();
    std::all_of(pol, v.begin(), v.end(), [=](auto val) { return val > 0; });
    auto end_any = std::chrono::high_resolution_clock::now();
    
    auto start_sort = std::chrono::high_resolution_clock::now();
    std::sort(pol, v.begin(), v.end());
    auto end_sort = std::chrono::high_resolution_clock::now();
    
    std::cout << "With policy: " << name << " | " << "Jobs: " << job << " | " << "generate tiem: " << diffInSec(end_generate, start_generate)
    << " | " << "all of time: " << diffInSec(end_any, start_any) << " | " << "sort time: " << diffInSec(end_sort, start_sort) << '\n';
}

int main() {
    
    for (long long job = 1000000; job <= 100000000; job *= 10) {
        std::vector<int> v(job);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(-100, 100);
        
        doJob(v, std::execution::seq, "sequential", job);
        doJob(v, std::execution::par, "parallel", job);
        doJob(v, std::execution::par_unseq, "parallel unseq", job);
    }
    
}