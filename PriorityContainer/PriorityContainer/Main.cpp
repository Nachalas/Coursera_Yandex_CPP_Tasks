#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    PriorityCollection() {
        objects.reserve(1'000'000);
        ratings.reserve(1'000'000);
    }

    using Id = size_t;

    Id Add(T object) {
        objects.push_back(move(object));
        ratings.push_back(0);
        ratingsSet.insert({0, objects.size() - 1 });
        return ratings.size() - 1;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
        IdOutputIt ids_begin) {
        while (range_begin != range_end) {
            *ids_begin = Add(move(*range_begin));
            range_begin++;
            ids_begin++;
        }
    }

    bool IsValid(Id id) const {
        if (id >= ratings.size() || id < 0u) {
            return false;
        }
        return (ratings[id] != -1) ? true : false;
    }

    const T& Get(Id id) const {
        return objects[id];
    }

    void Promote(Id id) {
        auto it = ratingsSet.find({ ratings[id], id});
        ratings[id]++;
        if (ratings[id] > max_rat) {
            max_rat = ratings[id];
            max_rat_id = id;
        }
        else if (ratings[id] == max_rat) {
            if (id > max_rat_id) {
                max_rat_id = id;
            }
        }
        ratingsSet.erase(it);
        ratingsSet.insert({ratings[id], id});
    }

    pair<const T&, int> GetMax() const {
        //return {objects[max_rat_id], ratings[max_rat_id]};
        auto temp = prev(ratingsSet.end());
        return { objects[temp->second], temp->first};
    }

    void findNewMax() {
        int newMR = 0;
        Id newMRI = 0;
        for (size_t i = 0; i < ratings.size(); i++) {
            if (ratings[i] >= newMR) {
                newMR = ratings[i];
                newMRI = i;
            }
        }
        max_rat = newMR;
        max_rat_id = newMRI;
    }

    pair<T, int> PopMax() {
        auto temp = prev(ratingsSet.end());
        ratings[temp->second] = -1;
        pair<T, int> toReturn;
        toReturn.first = move(objects[temp->second]);
        toReturn.second = temp->first;
        ratingsSet.erase(temp);
        return toReturn;
        //pair<T, int> toReturn{ move(objects[max_rat_id]), max_rat };
        //ratings[max_rat_id] = -1;
        //findNewMax();
        //return toReturn;
    }

private:
    vector<int> ratings;
    vector<T> objects;
    int max_rat = 0;
    Id max_rat_id = 0;
    set<pair<int, Id>> ratingsSet;
};


class StringNonCopyable : public string {
public:
    using string::string; 
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}