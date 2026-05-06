#include <thread>
#include <iostream>
#include <syncstream>
#include <mutex>
#include <vector>



void thread(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
    std::osyncstream(std::cout) << "thread: " << id << " is done!" << '\n';
}

int main() {
    srand(time(0));

    const int n = 5;
    std::vector<std::thread> v(n);
    
    for (int i = 0; i < 5; i++) {
        v[i] = std::thread(thread, i);
    } 
    for (int i = 0; i < 5; i++) {
        v[i].join();
    }

    return 0;
}