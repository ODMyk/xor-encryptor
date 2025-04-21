#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <mpi.h>

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

void process(const std::vector<char>& key, const std::vector<char>& local_data, std::vector<char>& local_result) {
    local_result = xor_encrypt_decrypt(local_data, key);
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string key_path = DEFAULT_KEY_PATH;
    std::string data_path = DEFAULT_DATA_PATH;
    std::string output_path = DEFAULT_OUTPUT_PATH;

    key_path = (argc >= 2) ? argv[1] : DEFAULT_KEY_PATH;
    data_path = (argc >= 3) ? argv[2] : DEFAULT_DATA_PATH;
    output_path = (argc >= 4) ? argv[3] : DEFAULT_OUTPUT_PATH;

    try {
        std::vector<char> key;
        std::vector<char> data;

        if (rank == 0) {
            key = read_file(key_path);
            data = read_file(data_path);
        }

        auto start = std::chrono::high_resolution_clock::now();

        int key_size = 0;
        if (rank == 0) {
            key_size = key.size();
        }
        MPI_Bcast(&key_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0) {
            key.resize(key_size);
        }
        MPI_Bcast(key.data(), key_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        size_t chunk_size = 0;
        if (rank == 0) {
            chunk_size = data.size() / size;
        }
        MPI_Bcast(&chunk_size, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

        std::vector<char> local_data(chunk_size);
        MPI_Scatter(data.data(), chunk_size, MPI_CHAR, local_data.data(), chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        std::vector<char> local_result;
        process(key, local_data, local_result);

        std::vector<char> result_data;
        if (rank == 0) {
            result_data.resize(chunk_size * size);
        }

        MPI_Gather(local_result.data(), chunk_size, MPI_CHAR, result_data.data(), chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << BENCHMARK_BEFORE << duration.count() << BENCHMARK_AFTER << std::endl;
            write_file(output_path, result_data);
        }

    } catch (const std::exception& e) {
        std::cerr << ERROR_PREFIX << e.what() << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    MPI_Finalize();
    return 0;
}

