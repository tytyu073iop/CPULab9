#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>

void thread(std::stop_token st) {
    int i = 0;
    while (!st.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        i++;
    }
    std::cout << "thread stoped on: " << i << '\n';
}

int main() {
    std::jthread t(thread);
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    t.request_stop();
    t.join();

    return 0;
}