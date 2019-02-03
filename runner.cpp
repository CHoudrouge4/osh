#include "runner.h"
#include <vector>

Runner::Runner() { }

void Runner::execute(Solver& s, int sample_size, int timeout) {
	std::cout << "Running experiment... " << sample_size << "\n";
	std::vector<long long> exec_durations;
	for (int i = 0; i < sample_size; i++) {
		std::cout << i << " ";
		bool hit = s.run(timeout);
		if (hit) {
			hits_n++;
			exec_durations.push_back(s.running_time);
		}
		hits_ratio = (double) hits_n / ((double) i+1);
		stats.push_back(s.stats);
		s.reset();
	}
	std::cout << "\n";

	for (uint i = 0; i < exec_durations.size(); i++) {
		average_t += exec_durations[i];
	}
	average_t /= hits_n;
}
