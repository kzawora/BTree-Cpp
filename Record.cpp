#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>

#define MAX_RECORD_SIZE 15
#define DIST_LOWER_LIMIT -10
#define DIST_UPPER_LIMIT 10
class Record {
	std::vector<double> values;
public:
	Record() = default;
	Record(std::vector<double> vals) {
		for (size_t i = 0; i < MAX_RECORD_SIZE; i++) {
			if (i < vals.size()) values.push_back(vals[i]);
			else values.push_back(std::nan(0));
		}
	}
	void print() {
		std::cout << "RECORD: ";
		for (auto i = values.begin(); i != values.end(); ++i)
			std::cout << *i << ' ';
		std::cout << std::endl;
	}

	static Record generate() {
		Record r;
		std::default_random_engine gen(static_cast<unsigned int>(clock())); // clock()
		std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
			DIST_UPPER_LIMIT);
		auto generator = [&]() { return dist(gen); };
		for (int i = 0; i < MAX_RECORD_SIZE; i++) {
			r.values.push_back(generator());
		}
		return r;
	}

	char* /*bytearray*/ serialize() {}
	static Record deserialize(/*arg*/) {}
};