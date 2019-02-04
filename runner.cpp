#include "runner.h"
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

Runner::Runner() { }

struct RunnerParams {
	std::mutex mtx;
	int jobs_left;
	int timeout;
};

void perform_run(RunnerParams* d, Runner* r, Solver* s) {
	int timeout = d->timeout;
	while (true) {
		d->mtx.lock();
		if (d->jobs_left == 0) { d->mtx.unlock(); return; }
		int i = d->jobs_left;
		std::cout << i << " " << std::flush;
		d->jobs_left--;
		d->mtx.unlock();

		bool hit = s->run(timeout);

		d->mtx.lock();
		if (hit) {
			r->hits_n++;
			r->exec_durations.push_back(s->running_time);
		}
		r->stats.push_back(s->stats);
		s->reset();

		d->mtx.unlock();
	}
}

void Runner::execute(std::vector<Solver*> solvers, int sample_size, int timeout) {
	exec_durations.clear();
	stats.clear();
	hits_n = 0;
	hits_ratio = 0;
	average_t = 0;

	RunnerParams common;
	common.jobs_left = sample_size;
	common.timeout = timeout;
	RunnerParams* d = &common;
	Runner* r = this;

	std::vector<std::thread> t;

	std::cout << "Running experiment, size " << sample_size << std::endl;

	for (uint i = 0; i < solvers.size(); i++) {
		t.push_back(std::thread(perform_run, d, r, solvers[i]));
	}
	for (auto& th : t) th.join();

	std::cout << std::endl;

	if (hits_n != 0) {
		hits_ratio = (double) hits_n / ((double) sample_size);
		for (uint i = 0; i < exec_durations.size(); i++) {
			average_t += exec_durations[i];
		}
		average_t /= hits_n;
	}
}
