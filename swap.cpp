#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

int main() {
    const std::string filename = "/data/data/com.termux/files/home/large_file.bin";
    const std::size_t fileSize = 20 * 1024 * 1024; // 20MB
    const std::size_t bufferSize = 1024 * 1024;    // 1MB

    char buffer[bufferSize];
    std::fill(buffer, buffer + bufferSize, 'A');

    // --- 파일 쓰기 시간 측정 ---
    auto writeStart = std::chrono::high_resolution_clock::now();

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "파일 열기 실패 (쓰기)" << std::endl;
        return 1;
    }

    for (int i = 0; i < 20; ++i) {
        outFile.write(buffer, bufferSize);
    }

    outFile.close();

    auto writeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> writeDuration = writeEnd - writeStart;
    std::cout << "파일 쓰기 시간: " << writeDuration.count() * 1000 << " ms" << std::endl;

    // --- 파일 읽기 시간 측정 ---
    auto readStart = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "파일 열기 실패 (읽기)" << std::endl;
        return 1;
    }

    while (inFile.read(buffer, bufferSize)) {
        // 읽기만 수행 (처리 없음)
    }
    inFile.close();

    auto readEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> readDuration = readEnd - readStart;
    std::cout << "파일 읽기 시간: " << readDuration.count() * 1000 << " ms" << std::endl;

    return 0;
}
