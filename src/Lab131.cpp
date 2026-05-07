#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>
#include <iomanip>
#include <iterator>
#include <climits>
#include <string>
#include <memory>
#include <future>
#include <exception>
#include <omp.h>
#include <execution>

std::vector<int> generate_data(std::size_t n) {
    std::vector<int> data(n);
    std::mt19937 rng(42); // фиксированный seed для воспроизводимости
    std::uniform_int_distribution<int> dist(0, 1000000);
    for (auto& x : data) x = dist(rng);
    return data;
}

// ------------------------------------------------------------
// 2. Стиль "простой параллельной версии std::accumulate" (на основе Листинга 2.9)
//    Адаптировано для поиска максимума.
// ------------------------------------------------------------
struct max_block {
    template <typename Iterator>
    void operator()(Iterator first, Iterator last, int& result) const {
        if (first != last) {
            result = std::max(result, *std::max_element(first, last));
        }
    }
};

int max_accumulate_style(const std::vector<int>& data, int num_threads) {
    auto first = data.begin();
    auto last  = data.end();
    auto length = std::distance(first, last);
    if (length == 0)
    throw std::runtime_error("Empty data set");
    
    constexpr unsigned long min_per_thread = 25;
    unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long actual_threads = std::min(static_cast<unsigned long>(std::max(1, num_threads)), max_threads);
    
    if (actual_threads == 1) {
        return *std::max_element(first, last);
    }
    
    unsigned long block_size = length / actual_threads;
    std::vector<int> results(actual_threads, std::numeric_limits<int>::lowest());
    std::vector<std::thread> threads(actual_threads - 1);
    
    auto block_start = first;
    for (unsigned long i = 0; i < actual_threads - 1; ++i) {
        auto block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(max_block{}, block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    // последний блок
    max_block{}(block_start, last, results[actual_threads - 1]);
    
    for (auto& t : threads) t.join();
    
    return std::accumulate(results.begin(), results.end(),
    std::numeric_limits<int>::lowest(),
    [](int a, int b) { return std::max(a, b); });
}

// ------------------------------------------------------------
// 3. Метод «Разделяй и властвуй» с использованием std::async
//    (без жесткого ограничения на число потоков – параметр cores носит информативный характер)
// ------------------------------------------------------------
int dc_max_async_impl(std::vector<int>::const_iterator first, std::vector<int>::const_iterator last, int num_threads, int depth = 0) {
    const int min_len = 1000;
    auto len = std::distance(first, last);
    int max_depth = static_cast<int>(std::log2(std::max(1, num_threads)));
    if (len == 0) return std::numeric_limits<int>::lowest();
    if (len <= min_len || depth >= max_depth) {
        return *std::max_element(first, last);
    }
    auto mid = first + len / 2;
    auto future_left = std::async(std::launch::async, dc_max_async_impl, first, mid, num_threads, depth + 1);
    int right_max = dc_max_async_impl(mid, last, num_threads, depth + 1);
    return std::max(future_left.get(), right_max);
}

int max_divide_and_conquer(const std::vector<int>& data, int num_threads) {
    if (data.empty())
    throw std::runtime_error("Empty data set");
    return dc_max_async_impl(data.begin(), data.end(), num_threads);
}

// ------------------------------------------------------------
// 4. Стандартные алгоритмы с различными политиками выполнения (C++17)
// ------------------------------------------------------------
int max_stl_sequential(const std::vector<int>& data, int /*unused*/) {
    if (data.empty()) throw std::runtime_error("Empty data set");
    return *std::max_element(data.begin(), data.end());
}

int max_stl_parallel(const std::vector<int>& data, int /*unused*/) {
    if (data.empty()) throw std::runtime_error("Empty data set");
    return *std::max_element(std::execution::par, data.begin(), data.end());
}

int max_stl_parallel_unseq(const std::vector<int>& data, int /*unused*/) {
    if (data.empty()) throw std::runtime_error("Empty data set");
    return *std::max_element(std::execution::par_unseq, data.begin(), data.end());
}

// ------------------------------------------------------------
// 5. Технология OpenMP – различные варианты
// ------------------------------------------------------------
int max_omp_parallel_for(const std::vector<int>& data, int num_threads) {
    if (data.empty()) return std::numeric_limits<int>::lowest();
    int max_val = std::numeric_limits<int>::lowest();
    #pragma omp parallel for reduction(max:max_val) num_threads(num_threads)
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] > max_val) max_val = data[i];
    }
    return max_val;
}

// OpenMP tasks (рекурсивное деление)
int omp_task_max_impl(std::vector<int>::const_iterator first, std::vector<int>::const_iterator last, std::size_t threshold) {
    auto len = std::distance(first, last);
    if (len == 0) return std::numeric_limits<int>::lowest();
    if (static_cast<std::size_t>(len) <= threshold) {
        return *std::max_element(first, last);
    }
    auto mid = first + len / 2;
    int left_max, right_max;
    #pragma omp task shared(left_max)
    left_max = omp_task_max_impl(first, mid, threshold);
    #pragma omp task shared(right_max)
    right_max = omp_task_max_impl(mid, last, threshold);
    #pragma omp taskwait
    return std::max(left_max, right_max);
}

int max_omp_tasks(const std::vector<int>& data, int num_threads) {
    if (data.empty()) return std::numeric_limits<int>::lowest();
    omp_set_num_threads(num_threads);
    int result = std::numeric_limits<int>::lowest();
    #pragma omp parallel
    {
        #pragma omp single
        result = omp_task_max_impl(data.begin(), data.end(), 1000);
    }
    return result;
}

// ------------------------------------------------------------
// Вспомогательные функции для замера времени
// ------------------------------------------------------------
using Clock = std::chrono::high_resolution_clock;
using Ms    = std::chrono::milliseconds;

template <typename F>
double measure(F f) {
    auto start = Clock::now();
    f();
    auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

// ------------------------------------------------------------
// Форматированный вывод таблицы
// ------------------------------------------------------------
void print_row(const std::string& method, std::size_t size, double time_ms, int result) {
    std::cout << std::left
    << std::setw(45) << method << "| "
    << std::setw(15) << size << "| "
    << std::setw(20) << std::fixed << std::setprecision(2) << time_ms / 1000.0 << "| "
    << std::setw(10) << result << "\n";
}

int main() {
    for (std::size_t N = 10000000; N <= 1000000000; N *= 10) {
        for (int cores = 8; cores <= 8; cores *= 2) {
            
            std::vector<int> data = generate_data(N);
            int reference = *std::max_element(data.begin(), data.end());
            
            std::cout << "Reference max (sequential): " << reference << "\n\n";
            std::cout << std::left
            << std::setw(45) << "Variant" << "| "
            << std::setw(15) << "Size" << "| "
            << std::setw(20) << "Time (s)" << "| "
            << std::setw(10) << "Result" << "\n";
            std::cout << std::string(45+15+20+10+6, '-') << "\n";
            
            // 1. Accumulate style
            print_row("Accumulate-style (std::thread)", N, measure([&]{ max_accumulate_style(data, cores); }), reference);
            
            // 2. Divide and Conquer (async)
            print_row("Divide & Conquer (std::async)", N, measure([&]{ max_divide_and_conquer(data, cores); }), reference);
            
            // 3. Standard algorithms
            print_row("std::max_element (seq)", N, measure([&]{ max_stl_sequential(data, cores); }), reference);
            
            print_row("std::max_element (par)", N, measure([&]{ max_stl_parallel(data, cores); }), reference);
            print_row("std::max_element (par_unseq)", N, measure([&]{ max_stl_parallel_unseq(data, cores); }), reference);
            
            // 4. OpenMP variants
            print_row("OpenMP parallel for reduction", N,
                measure([&]{ max_omp_parallel_for(data, cores); }),
                reference);
                print_row("OpenMP tasks", N, measure([&]{ max_omp_tasks(data, cores); }), reference);
            }
        }
        
        return 0;
    }