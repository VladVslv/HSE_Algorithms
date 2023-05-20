#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

static long long number_of_tics = 0;

long long sort(int arr[], int size) {
    auto start = std::chrono::high_resolution_clock::now();
    number_of_tics += 2;
    for (int gap = size / 2; gap > 0; gap /= 2) {
        number_of_tics += 3;
        for (int i = gap; i < size; ++i) { ;
            for (int j = i; j >= gap && arr[j] < arr[j - gap]; j -= gap) {
                number_of_tics += 11;
                std::swap(arr[j], arr[j - gap]);
            }
        }
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start).count();
}

int main(int argc, char *argv[]) {
    int size = std::stoi(argv[3]), step = std::stoi(argv[4]);
    int arr[size];
    int copy[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = std::stoi(argv[i + 5]);
    }
    std::pair<long long, long long> info;
    std::ofstream time;
    time.open(std::string(argv[1]) + "_time.csv");
    time << "size;time\n";
    std::ofstream tics;
    tics.open(std::string(argv[1]) + "_tics.csv");
    tics << "size;tics\n";
    int number_of_repetitions = std::stoi(argv[2]);
    for (int i = step; i <= size; i += step) {
        info = {0, 0};
        for (int j = 0; j < number_of_repetitions; ++j) {
            for (int k = 0; k < size; ++k) {
                copy[k] = arr[k];
            }
            info.second += sort(copy, i);
            info.first += number_of_tics;
            number_of_tics = 0;
        }
        info.first /= number_of_repetitions;
        info.second /= number_of_repetitions;
        tics << i << ";" << info.first;
        time << i << ";" << info.second;
        if (i != size) {
            tics << "\n";
            time << "\n";
        }

        bool correct = true;
        for (int k = 0; k < i - 1; ++k) {
            correct &= copy[k] <= copy[k + 1];
        }
        info.first += number_of_tics;
        number_of_tics = 0;
        if (!correct) {
            std::ofstream checker;
            int k = 0;
            std::string path;
            while (argv[1][k] != '/') {
                path += argv[1][k];
                ++k;
            }
            path += "/checker.txt";
            checker.open(path);
            checker << argv[1] << "\n";
        }
    }
    return 0;
}