#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

static long long number_of_tics = 0;

int getMax(const int array[], int size) {
    number_of_tics += 3;
    int max = array[0];
    for (int i = 1; i < size; i++) {
        number_of_tics += 4;
        if (array[i] > max) {
            max = array[i];
            number_of_tics += 2;
        }
    }
    return max;
}

long long sort(int arr[], int size) {
    auto start = std::chrono::high_resolution_clock::now();
    number_of_tics += 10;
    int output[size];
    int max = getMax(arr, size);
    int count[max + 1];

    for (int j = 0; j <= max; ++j) {
        number_of_tics += 3;
        count[j] = 0;
    }
    for (int i = 0; i < size; ++i) {
        number_of_tics += 4;
        ++count[arr[i]];
    }

    for (int i = 1; i <= max; ++i) {
        count[i] += count[i - 1];
        number_of_tics += 5;
    }

    for (int i = size - 1; i >= 0; --i) {
        number_of_tics += 9;
        output[count[arr[i]] - 1] = arr[i];
        --count[arr[i]];
    }

    for (int i = 0; i < size; ++i) {
        number_of_tics += 2;
        arr[i] = output[i];
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