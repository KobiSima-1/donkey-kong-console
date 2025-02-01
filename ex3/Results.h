#pragma once

#include "utils.h"
#include <list>
#include <limits>

using namespace std;

class Results {
public:
	enum ResultValue { marioHitEnemy, finished, noResult };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	Results() = default;
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		std::pair<size_t, ResultValue> result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t getNextIteration() const;

	void reportResultError(const std::string& message, const std::string& filename, size_t iteration) const;

	size_t getResultsSize() const {
		return results.size();
	}
};