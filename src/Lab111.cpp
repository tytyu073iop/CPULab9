#include <thread>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <chrono>

template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init, int cores)
{
    unsigned long const length=std::distance(first,last);
    
    if(!length) 
    {
        return init;
    }
    
    unsigned long const num_threads = cores;
    unsigned long const block_size=length / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator,T>(), block_start, block_end, std::ref(results[i]));
        block_start=block_end;
    }
    accumulate_block<Iterator,T>()(block_start,last, results[num_threads-1]);
    for(auto& entry: threads) {
        entry.join();
    }
    return std::accumulate(results.begin(), results.end(), init);
}

int main() {
    srand(time(0));
    for (long long jobs = 1000000; jobs <= 100000000; jobs *= 10) {
        std::vector<long long> arr(jobs);
        std::for_each(arr.begin(), arr.end(), [](auto& val) { val = rand(); });

        for (int cores = 1; cores <= 8; cores *= 2) {
            auto start = std::chrono::high_resolution_clock::now();
            long long result = 0;

            result = parallel_accumulate(arr.begin(), arr.end(), 0LL, cores);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Threads: " << cores << " | " << "Time: " << static_cast<double>(duration) / 1000.0 << " | " << "result: " << result << " | " << "steps: " << jobs << '\n';
        }
    }
}