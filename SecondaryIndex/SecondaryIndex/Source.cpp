#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

bool operator==(const Record& lhs, const Record& rhs) {
    return (lhs.timestamp == rhs.timestamp) &&
        (lhs.karma == rhs.karma) &&
        (lhs.id == rhs.id) &&
        (lhs.title == rhs.title) &&
        (lhs.user == rhs.user);
}

struct RecordHasher {
    // Axxxx + Bxxx + Cxx + Dx + E
    size_t operator() (const Record& obj) const {
        size_t x = 199;
        return (shash(obj.id) * x * x * x * x)
            + (shash(obj.title) * x * x * x)
            + (ihash(obj.karma))
            + (shash(obj.user) * x * x)
            + (ihash(obj.timestamp) * x);
    }

    hash<int> ihash;
    hash<string> shash;
};


//bool operator<(const Record& lhs, const Record& rhs) {
//    return tie(lhs.id, lhs.title, lhs.user, lhs.timestamp, lhs.karma)
//        < tie(rhs.id, rhs.title, rhs.user, rhs.timestamp, rhs.karma);
//}

class Database {
public:
    bool Put(const Record& record) {
        if (idToRecord.find(record.id) != idToRecord.end()) {
            return false;
        }
        auto temp = allRecords.insert(record);
        if (!temp.second) {
            return false;
        }
        else {
            //idToRecord[record.id] = temp.first;
            idToRecord.insert({record.id, *temp.first});

            auto tempT = timestampsSI.insert({record.timestamp, *temp.first });
            auto tempK = karmasSI.insert({ record.karma, *temp.first });
            auto tempU = usersSI.insert({ record.user , *temp.first });
            
            auto& iteratorTemp = iterators[record.id];
            iteratorTemp.KSI_Iterator = tempK;
            iteratorTemp.TSI_Iterator = tempT;
            iteratorTemp.USI_Iterator = tempU;

            return true;
        }
    }

    const Record* GetById(const string& id) const {
        auto it = idToRecord.find(id);
        if (it != idToRecord.end()) {
            const Record* pointer = &it->second;
            return pointer;
        }
        else {
            return nullptr;
        }
    }

    bool Erase(const string& id) {
        auto it = idToRecord.find(id);
        if (it != idToRecord.end()) {
            allRecords.erase(it->second);

            auto& iteratorTemp = iterators[id];
            timestampsSI.erase(iteratorTemp.TSI_Iterator);
            karmasSI.erase(iteratorTemp.KSI_Iterator);
            usersSI.erase(iteratorTemp.USI_Iterator);
            iterators.erase(id);

            idToRecord.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto itLower = timestampsSI.lower_bound(low);
        auto itHigher = timestampsSI.upper_bound(high);
        while (itLower != itHigher && callback(itLower->second)) {
            itLower++;
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto itLower = karmasSI.lower_bound(low);
        auto itHigher = karmasSI.upper_bound(high);
        while (itLower != itHigher && callback(itLower->second)) {
            itLower++;
        }
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto range = usersSI.equal_range(user);
        auto itLower = range.first;
        auto itHigher = range.second;
        while (itLower != itHigher && callback(itLower->second)) {
            itLower++;
        }
    }

private:
    struct ThreeIt {
        multimap<int, const Record&>::iterator TSI_Iterator;
        multimap<int, const Record&>::iterator KSI_Iterator;
        multimap<string, const Record&>::iterator USI_Iterator;
    };

    unordered_set<Record, RecordHasher> allRecords;
    unordered_map<string, const Record&> idToRecord;
    multimap<int, const Record&> timestampsSI;
    multimap<int, const Record&> karmasSI;
    multimap<string, const Record&> usersSI;
    unordered_map<string, ThreeIt> iterators;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
    db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
    db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
    db.Erase("id");
    db.Put({ "id", final_body, "not-master", 1536107260, -10 });

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);

    return 0;
}