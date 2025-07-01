#define _GNU_SOURCE
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <sched.h>
#include <unistd.h>
#include <cstdio>    // for remove()

int main() {
    // --- 원하는 코어 지정 (예: core 1) ---
    int target_core = 5;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(target_core, &cpuset);

    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "코어 고정 실패 (core " << target_core << ")" << std::endl;
    } else {
        std::cout << "코어 고정 성공: core " << target_core << std::endl;
    }

    const std::string filename = "/data/data/com.termux/files/home/large_file.bin";
    const std::size_t fileSize = 100 * 1024 * 1024; // 20MB
    const std::size_t bufferSize = 512 * 1024;    // 512KB
    char buffer[bufferSize];
    std::fill(buffer, buffer + bufferSize, 'A');

    double totalWriteTimeMs = 0.0;
    double totalReadTimeMs = 0.0;

    const int iterations = 100;

    for (int iter = 0; iter < iterations; ++iter) {
        // --- 파일 쓰기 ---
        auto writeStart = std::chrono::high_resolution_clock::now();
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "파일 열기 실패 (쓰기)" << std::endl;
            return 1;
        }

        for (int i = 0; i < fileSize / bufferSize; ++i) {
            outFile.write(buffer, bufferSize);
        }
        outFile.close();
        auto writeEnd = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> writeDuration = writeEnd - writeStart;
        totalWriteTimeMs += writeDuration.count() * 1000;

        // --- 파일 읽기 ---
        auto readStart = std::chrono::high_resolution_clock::now();
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "파일 열기 실패 (읽기)" << std::endl;
            return 1;
        }

        while (inFile.read(buffer, bufferSize)) {
            // 읽기만 수행
        }
        inFile.close();
        auto readEnd = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> readDuration = readEnd - readStart;
        totalReadTimeMs += readDuration.count() * 1000;

        // --- 파일 삭제 ---
        if (std::remove(filename.c_str()) != 0) {
            std::cerr << "파일 삭제 실패" << std::endl;
            return 1;
        }

        std::cout << "[" << iter + 1 << "/" << iterations << "] 완료" << std::endl;
    }

    std::cout << "\n=== 결과 ===" << std::endl;
    std::cout << "평균 파일 쓰기 시간: " << totalWriteTimeMs / iterations << " ms" << std::endl;
    std::cout << "평균 파일 읽기 시간: " << totalReadTimeMs / iterations << " ms" << std::endl;

    return 0;
}
