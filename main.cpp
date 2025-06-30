/*
 * Optimized Sorting Algorithm Visualizer (Replit-Compatible)
 * Author: [Your Name]
 * Improvements: reduced space usage, tail-recursive quicksort, iterative heapify, visual limiter
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

// --- Global Color Codes ---
const string RED    = "\033[31m";
const string YELLOW = "\033[33m";
const string GREEN  = "\033[32m";
const string CYAN   = "\033[36m";
const string RESET  = "\033[0m";
const string BOLD   = "\033[1m";

bool enable_visuals = true;

class SortVisualizer {
    vector<int> arr;
    int size;
    int animation_delay;
    string active_algorithm;
    vector<int> buffer; // reusable buffer for merge sort

public:
    explicit SortVisualizer(int n = 25) : size(n), animation_delay(120), buffer(n) {
        generateRandomArray();
    }

    void generateRandomArray() {
        arr.clear();
        for (int i = 1; i <= size; ++i) arr.push_back(i);
        shuffleArray();
    }

    void shuffleArray() {
        random_device rd;
        mt19937 g(rd());
        shuffle(arr.begin(), arr.end(), g);
    }

    void clearConsole() {
        cout << string(50, '\n');
    }

    void renderBarSeparator(int count) {
        cout << string(count, '-') << endl;
    }

    void renderStep(int h1 = -1, int h2 = -1, const string& status = "") {
        if (!enable_visuals) return;
        clearConsole();
        cout << BOLD << CYAN << "\n=== SORTING VISUALIZER ===" << RESET << endl;
        cout << "Algorithm: " << GREEN << active_algorithm << RESET << " | " << YELLOW << status << RESET << endl;
        renderBarSeparator(50);

        int max_value = *max_element(arr.begin(), arr.end());
        int bar_height = 10;

        for (int level = bar_height; level > 0; --level) {
            for (int i = 0; i < size; ++i) {
                int current_height = (arr[i] * bar_height) / max_value;
                if (current_height >= level) {
                    string color = CYAN;
                    if (i == h1 || i == h2) color = RED;
                    cout << color << "█" << RESET;
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
        renderBarSeparator(size);

        for (int i = 0; i < min(25, size); ++i) {
            if (i == h1 || i == h2)
                cout << YELLOW << "[" << arr[i] << "]" << RESET << " ";
            else
                cout << arr[i] << " ";
        }
        cout << endl;
        this_thread::sleep_for(chrono::milliseconds(animation_delay));
    }

    void executeBubbleSort() {
        active_algorithm = "Bubble Sort";
        for (int i = 0; i < size - 1; ++i)
            for (int j = 0; j < size - i - 1; ++j) {
                renderStep(j, j + 1, "Comparing");
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    renderStep(j, j + 1, "Swapped");
                }
            }
        renderStep(-1, -1, "Sorted");
    }

    void executeSelectionSort() {
        active_algorithm = "Selection Sort";
        for (int i = 0; i < size - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < size; ++j) {
                renderStep(min_idx, j, "Finding Min");
                if (arr[j] < arr[min_idx]) min_idx = j;
            }
            swap(arr[i], arr[min_idx]);
            renderStep(i, min_idx, "Swapped");
        }
        renderStep(-1, -1, "Sorted");
    }

    void executeInsertionSort() {
        active_algorithm = "Insertion Sort";
        for (int i = 1; i < size; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                renderStep(j, j + 1, "Shifting");
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
            renderStep(j + 1, -1, "Inserted");
        }
        renderStep(-1, -1, "Sorted");
    }

    void executeQuickSort() {
        active_algorithm = "Quick Sort";
        int low = 0, high = size - 1;
        vector<pair<int, int>> stack;
        stack.emplace_back(low, high);

        while (!stack.empty()) {
            tie(low, high) = stack.back();
            stack.pop_back();

            while (low < high) {
                int pi = partition(low, high);
                if (pi - low < high - pi) {
                    stack.emplace_back(pi + 1, high);
                    high = pi - 1;
                } else {
                    stack.emplace_back(low, pi - 1);
                    low = pi + 1;
                }
            }
        }
        renderStep(-1, -1, "Sorted");
    }

    int partition(int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            renderStep(j, high, "Partitioning");
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
                renderStep(i, j, "Swapped");
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void executeMergeSort() {
        active_algorithm = "Merge Sort";
        mergeSort(0, size - 1);
        renderStep(-1, -1, "Sorted");
    }

    void mergeSort(int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        merge(l, m, r);
    }

    void merge(int l, int m, int r) {
        int i = l, j = m + 1, k = l;
        while (i <= m && j <= r) {
            renderStep(i, j, "Merging");
            if (arr[i] <= arr[j]) buffer[k++] = arr[i++];
            else buffer[k++] = arr[j++];
        }
        while (i <= m) buffer[k++] = arr[i++];
        while (j <= r) buffer[k++] = arr[j++];
        for (int x = l; x <= r; ++x) arr[x] = buffer[x];
    }

    void executeHeapSort() {
        active_algorithm = "Heap Sort";
        for (int i = size / 2 - 1; i >= 0; i--)
            heapify(size, i);
        for (int i = size - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            renderStep(0, i, "Move max");
            heapify(i, 0);
        }
        renderStep(-1, -1, "Sorted");
    }

    void heapify(int n, int i) {
        while (true) {
            int largest = i;
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            if (l < n && arr[l] > arr[largest]) largest = l;
            if (r < n && arr[r] > arr[largest]) largest = r;
            if (largest == i) break;
            swap(arr[i], arr[largest]);
            renderStep(i, largest, "Heapify");
            i = largest;
        }
    }

    void showMenu() {
        int choice;
        do {
            clearConsole();
            cout << BOLD << CYAN << "\nSORTING VISUALIZER" << RESET << endl;
            cout << "1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n4. Quick Sort\n5. Merge Sort\n6. Heap Sort\n0. Exit" << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: executeBubbleSort(); break;
                case 2: executeSelectionSort(); break;
                case 3: executeInsertionSort(); break;
                case 4: executeQuickSort(); break;
                case 5: executeMergeSort(); break;
                case 6: executeHeapSort(); break;
                case 0: cout << GREEN << "Exiting..." << RESET << endl; break;
                default: cout << RED << "Invalid choice!" << RESET << endl;
            }
            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore(); cin.get();
            }
        } while (choice != 0);
    }
};

int main() {
    SortVisualizer app(25);
    app.showMenu();
    return 0;
}
