#include "Common.h"

#include <set>
#include <utility>
#include <map>
#include <future>
#include <future>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const Settings& settings
        ) : unpacker(move(books_unpacker)), settings_(settings) {
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> g(m);
        auto it = iterators.find(book_name);
        if (it != iterators.end()) {
            auto temp = ratings.extract(it->second);
            maxRat++;
            auto itPair = ratings.insert({maxRat, move(temp.value().second)});
            iterators[book_name] = itPair.first;
            return prev(ratings.end())->second;
        }
        else {
            shared_ptr<IBook> ptr = unpacker->UnpackBook(book_name);
            if (ptr->GetContent().size() > settings_.max_memory) {
                return ptr;
            }
            if (currMem + ptr->GetContent().size() <= settings_.max_memory) {
                maxRat++;
                currMem += ptr->GetContent().size();
                auto itPair = ratings.insert({maxRat, ptr});
                iterators[book_name] = itPair.first;
                return prev(ratings.end())->second;
            }
            else {
                while (currMem + ptr->GetContent().size() > settings_.max_memory) {
                    string tempName = ratings.begin()->second->GetName();
                    size_t tempSize = ratings.begin()->second->GetContent().size();
                    iterators.erase(tempName);
                    ratings.erase(ratings.begin());
                    currMem -= tempSize;
                }
                maxRat++;
                currMem += ptr->GetContent().size();
                auto itPair = ratings.insert({ maxRat, ptr });
                iterators[book_name] = itPair.first;
                return prev(ratings.end())->second;
            }
        }
    }
private:
    shared_ptr<IBooksUnpacker> unpacker;
    Settings settings_;

    set<pair<size_t, shared_ptr<IBook>>> ratings;
    using Iter = set<pair<size_t, shared_ptr<IBook>>>::iterator;
    map<string, Iter> iterators;
    size_t maxRat = 0;
    size_t currMem = 0;

    mutex m;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
    ) {
    unique_ptr<ICache> ptr = make_unique<LruCache>(move(books_unpacker), settings);
    return ptr;
}