#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2) {
        return;
    }
    vector<typename RandomIt::value_type> temp(make_move_iterator(range_begin), make_move_iterator(range_end));
    size_t pivot1 = (temp.size() / 3);
    size_t pivot2 = (temp.size() / 3) * 2;
    MergeSort(temp.begin(), temp.begin() + pivot1);
    MergeSort(temp.begin() + pivot1, temp.begin() + pivot2);
    MergeSort(temp.begin() + pivot2, temp.end());
    vector<typename RandomIt::value_type> temp2;
    merge(
        make_move_iterator(temp.begin()), make_move_iterator(temp.begin() + pivot1),
        make_move_iterator(temp.begin() + pivot1), make_move_iterator(temp.begin() + pivot2),
        back_inserter(temp2)
        );
    merge(
        make_move_iterator(temp2.begin()), make_move_iterator(temp2.end()),
        make_move_iterator(temp.begin() + pivot2), make_move_iterator(temp.end()),
        range_begin
        );
}

void TestIntVector() {
    vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}