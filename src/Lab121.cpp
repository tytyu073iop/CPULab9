#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>

void thread(std::atomic<bool>& stop) {
    int i = 0;
    while (!stop.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        i++;
    }
    std::cout << "thread stoped on: " << i << '\n';
}

int main() {
    std::atomic<bool> stop(false);
    std::thread t(thread, std::ref(stop));
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    stop.store(true);
    t.join();

    return 0;
}