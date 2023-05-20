#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

static long long number_of_tics = 0;

void merge(int array[], int const left, int const mid, int const right) {
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
    number_of_tics += 13;
    auto *leftArray = new int[subArrayOne], *rightArray = new int[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++) {
        number_of_tics += 5;
        leftArray[i] = array[left + i];
    }
    for (auto j = 0; j < subArrayTwo; j++) {
        number_of_tics += 5;
        rightArray[j] = array[mid + 1 + j];
    }
    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        number_of_tics += 10;
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        } else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        number_of_tics += 5;
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        number_of_tics += 5;
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void mergeSort(int array[], int const begin, int const end) {
    ++number_of_tics;
    if (begin >= end) {
        return;
    }
    number_of_tics += 3;
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

long long sort(int arr[], int size) {
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(arr, 0, size - 1);
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