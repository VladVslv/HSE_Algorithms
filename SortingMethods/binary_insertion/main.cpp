#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

static long long number_of_tics = 0;

int binarySearch(int a[], int item, int low, int high) {
    ++number_of_tics;
    if (high <= low) {
        number_of_tics += 3;
        return (item > a[low]) ? (low + 1) : low;
    }
    number_of_tics += 4;
    int mid = (low + high) / 2;
    if (item == a[mid]) {
        ++number_of_tics;
        return mid + 1;
    }
    number_of_tics += 2;
    if (item > a[mid]) {
        ++number_of_tics;
        return binarySearch(a, item, mid + 1, high);
    }
    ++number_of_tics;
    return binarySearch(a, item, low, mid - 1);
}

long long sort(int arr[], int size) {
    auto start = std::chrono::high_resolution_clock::now();
    int i, loc, j, selected;
    ++number_of_tics;
    for (i = 1; i < size; ++i) {
        number_of_tics += 7;
        j = i - 1;
        selected = arr[i];
        loc = binarySearch(arr, selected, 0, j);
        while (j >= loc) {
            number_of_tics += 4;
            arr[j + 1] = arr[j];
            j--;
        }
        number_of_tics += 2;
        arr[j + 1] = selected;
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