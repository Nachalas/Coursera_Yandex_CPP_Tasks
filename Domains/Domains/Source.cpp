#include <iostream>
#include <sstream>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>

using namespace std;

set<string> getDomainsSet(istream& stream = cin) {
	set<string> toReturn;
	size_t quantity = 0;
	stream >> quantity;
	while (quantity != 0) {
		string temp;
		stream >> temp;
		reverse(temp.begin(), temp.end());
		toReturn.insert(move(temp));
		quantity--;
	}
	return toReturn;
}

vector<string> getDomains(istream& stream = cin) {
	vector<string> toReturn;
	size_t quantity = 0;
	stream >> quantity;
	toReturn.reserve(quantity);
	while (quantity != 0) {
		string temp;
		stream >> temp;
		reverse(temp.begin(), temp.end());
		toReturn.push_back(move(temp));
		quantity--;
	}
	return toReturn;
}

bool checkIfBad(const string& site, const string& bannedIt) {
	//if (site.size() < bannedIt.size()) {
	//	return false;
	//}
	//if (site.size() == bannedIt.size()) {
	//	for (size_t i = 0; i < bannedIt.size(); i++) {
	//		if (site[site.size() - i - 1] != bannedIt[i]) {
	//			return false;
	//		}
	//	}
	//	return true;
	//}
	//// esli bolsh
	//size_t i = 0;
	//for (i = 0; i < bannedIt.size(); i++) {
	//	if (site[site.size() - i - 1] != bannedIt[i]) {
	//		return false;
	//	}
	//}
	//return site[site.size() - i - 1] == '.';
	if (site.size() < bannedIt.size()) {
		return false;
	}
	size_t i = 0;
	for (i = 0; i < bannedIt.size(); i++) {
		if (site[i] != bannedIt[i]) {
			return false;
		}
	}
	if (site.size() == bannedIt.size()) {
		return true;
	}
	if (site[i] == '.') {
		return true;
	}
	else {
		return false;
	}
}

void checkSites(vector<string>& sites, const set<string>& banned, ostream& stream) {
	//unordered_set<string> badSites;
	for (auto& site : sites) {
		//if (badSites.find(site) != badSites.end()) {
		//	stream << "Bad" << "\n";
		//	continue;
		//}
		bool isBad = false;
		size_t pos = site.find('.');
		string toSearch = site.substr(0, pos);
		auto it = banned.lower_bound(toSearch);
		auto itUpper = banned.upper_bound(site);
		for (it; it != itUpper; it++) {
			if (checkIfBad(site, *it)) {
				isBad = true;
				//badSites.insert(move(site));
				break;
			}
		}
		if (isBad) {
			stream << "Bad" << "\n";
		}
		else {
			stream << "Good" << "\n";
		}
		// vyvesti
	}
}

//void filterDomains(set<string>&& bannedDomains, istream& input, ostream& output) {
//	size_t quantity = 0;
//	input >> quantity;
//	unordered_set<string> badSites;
//	while (quantity != 0) {
//		string temp;
//		input >> temp;
//		bool isBad = false;
//		if (badSites.find(temp) != badSites.end()) {
//			output << "Bad" << "\n";
//			quantity--;
//			continue;
//		}
//		for (const auto& i : bannedDomains) {
//			if (temp.size() > i.size()) {
//				if (temp.find(i, temp.size() - i.size()) != string::npos) {
//					if (temp[temp.size() - i.size() - 1] != '.') {
//						continue;
//					}
//					isBad = true;
//					break;
//				}
//			}
//			if (temp.size() == i.size()) {
//				if (temp == i) {
//					isBad = true;
//					break;
//				}	
//			}
//		}
//		if (isBad) {
//			badSites.insert(temp);
//			output << "Bad" << "\n";
//		}
//		else {
//			output << "Good" << "\n";
//		}
//		quantity--;
//	}
//}

int main() {
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);

	//istringstream bannedStr(
	//	"4\n"
	//	"ya.ru\n"
	//	"maps.me\n"
	//	"m.ya.ru\n"
	//	"com\n"
	//);

	//istringstream sitesStr(
	//	"7\n"
	//	"ya.ru\n"
	//	"ya.com\n"
	//	"m.maps.me\n"
	//	"moscow.m.ya.ru\n"
	//	"maps.com\n"
	//	"maps.ru\n"
	//	"ya.ya\n"
	//);

	set<string> banned = getDomainsSet();
	//for (auto& i : banned) {
	//	reverse(i.begin(), i.end());
	//}
	//sort(banned.begin(), banned.end());

	vector<string> toCheck = getDomains();

	//checkIfBad("ya.ru", "ur.ay");

	checkSites(toCheck, banned, cout);

}