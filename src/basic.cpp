#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <functional>

#define DEFAULT_KEY_PATH "io/key.txt"
#define DEFAULT_DATA_PATH "io/data.txt"
#define DEFAULT_OUTPUT_PATH "io/output.txt"
#define BENCHMARK_BEFORE "Done in "
#define BENCHMARK_AFTER " microseconds"
#define ERROR_PREFIX "Error: "
#define NO_FILE_PREFIX "Cannot open file: "

std::vector<char> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error(NO_FILE_PREFIX + path);
    return std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void write_file(const std::string& path, const std::vector<char>& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error(NO_FILE_PREFIX + path);
    file.write(content.data(), content.size());
}

std::vector<char> xor_encrypt_decrypt(const std::vector<char>& data, const std::vector<char>& key) {
    std::vector<char> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];
    }
    return result;
}

template <typename Func, typename... Args>
auto with_benchmark(Func&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << BENCHMARK_BEFORE << duration.count() << BENCHMARK_AFTER << std::endl;
    } else {
        auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << BENCHMARK_BEFORE << duration.count() << BENCHMARK_AFTER << std::endl;
        return result;
    }
}

int main(int argc, char* argv[]) {
    std::string key_path = DEFAULT_KEY_PATH;
    std::string data_path = DEFAULT_DATA_PATH;
    std::string output_path = DEFAULT_OUTPUT_PATH;

    if (argc >= 2) key_path = argv[1];
    if (argc >= 3) data_path = argv[2];
    if (argc >= 4) output_path = argv[3];

    try {
        std::vector<char> key = read_file(key_path);
        std::vector<char> data = read_file(data_path);

        std::vector<char> processed = with_benchmark(xor_encrypt_decrypt, data, key);

        write_file(output_path, processed);

    } catch (const std::exception& e) {
        std::cerr << ERROR_PREFIX << e.what() << std::endl;
        return 1;
    }

    return 0;
}
