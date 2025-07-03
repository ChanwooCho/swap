#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>

#define STEP_MB 100

void print_swap_info() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    while (std::getline(meminfo, line)) {
        if (line.find("SwapTotal") != std::string::npos ||
            line.find("SwapFree") != std::string::npos ||
            line.find("MemAvailable") != std::string::npos) {
            std::cout << line << std::endl;
        }
    }
    meminfo.close();
}

int main() {
    std::vector<char*> allocations;
    size_t step = STEP_MB * 1024 * 1024;
    size_t total = 0;

    while (true) {
        char* block = (char*)malloc(step);
        if (!block) {
            std::cerr << "[!] malloc failed at total = " << total / (1024 * 1024) << " MB" << std::endl;
            break;
        }

        std::memset(block, 0, step); // 실제로 메모리 페이지를 점유하게 함
        allocations.push_back(block);
        total += step;

        std::cout << "[+] Allocated: " << total / (1024 * 1024) << " MB" << std::endl;
        print_swap_info();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // 메모리 해제
    for (auto ptr : allocations) {
        free(ptr);
    }

    return 0;
}
