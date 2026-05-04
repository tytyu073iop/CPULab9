#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <cmath>

/**
* Параллельное вычисление определённого интеграла методом левых прямоугольников.
*
* @param a         левая граница интегрирования
* @param b         правая граница интегрирования
* @param f         интегрируемая функция (принимает double, возвращает double)
* @param n         количество прямоугольников (разбиений)
* @param num_threads количество потоков (должно быть > 0)
* @return приближённое значение интеграла
*/
double integrate_parallel(double a, double b, const std::function<double(double)>& f, int n, int num_threads) {
    double h = (b - a) / n;               // шаг интегрирования
    std::vector<double> partial_sums(num_threads, 0.0);
    std::vector<std::thread> threads;
    
    // Равномерное распределение индексов между потоками
    int base_count = n / num_threads;     // базовое количество точек на поток
    int remainder = n % num_threads;      // остаток, отдаём первым потокам
    
    int start = 0;                        // начальный индекс для текущего потока
    for (int t = 0; t < num_threads; ++t) {
        int count = base_count + (t < remainder ? 1 : 0);
        int end = start + count;         // конечный индекс (не включая)
        
        // Запуск потока: вычисляет сумму f(a + i*h) на отрезке [start, end)
        threads.emplace_back([&, t, start, end]() {
            double sum = 0.0;
            for (int i = start; i < end; ++i) {
                double x = a + i * h;
                sum += f(x);
            }
            partial_sums[t] = sum * h;    // умножаем на шаг один раз
        });
        
        start = end;                      // смещаем начало для следующего потока
    }
    
    // Ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }
    
    // Сборка результата из частичных сумм
    double total = 0.0;
    for (double ps : partial_sums) {
        total += ps;
    }
    return total;
}

// Пример использования
int main() {
    // Пример: интегрируем f(x) = x^2 на отрезке [0, 1] (точное значение = 1/3)
    auto f = [](double x) { return x * x; };
    double a = 0.0, b = 1.0;
    for (int n = 1000000; n <= 100000000; n *= 10) {             // количество прямоугольников (для точности)
        for (int num_threads = 1; num_threads <= 8; num_threads *= 2) {          // количество потоков
            auto start = std::chrono::high_resolution_clock::now();
            double result = integrate_parallel(a, b, f, n, num_threads);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            
            std::cout << "Threads: " << num_threads << " | " << "Time: " << static_cast<double>(duration) / 1000.0 << " | " << "Steps: " << n << " | " << "Приближённое значение интеграла: " << result << " | ";
            std::cout << "Абсолютная ошибка: " << std::fabs(result - 1.0/3.0) << std::endl;
        }
    }
    
    return 0;
}