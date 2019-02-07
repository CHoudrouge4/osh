#include "runner.h"
#include <fstream>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

using namespace std;

void dump_stats(vector<statItem> stats, string dir, int exN) {
	ofstream outS;

	char buffer[50];
	sprintf(buffer, "%s/run%d.csv", dir.c_str(), exN);
	outS.open(buffer);

	for (uint j = 0; j < stats.size(); j++) {
		auto val = stats[j];
		outS << get<0>(val) << "  " << get<1>(val) << "  " << get<2>(val) << "\n";
	}
	outS.close();
}

Runner::Runner() { }

struct RunnerParams {
	std::mutex mtx;
	int jobs_done;
	int sample_size;
	int timeout;
	string report_dir;
};

void perform_run(RunnerParams* d, Runner* r, Solver* s) {
	int timeout = d->timeout;
	while (true) {
		d->mtx.lock();
		if (d->jobs_done >= d->sample_size) { d->mtx.unlock(); return; }
		int i = d->jobs_done;
		std::cout << i << " " << std::flush;
		d->jobs_done++;
		d->mtx.unlock();

		bool hit = s->run(timeout);

		d->mtx.lock();
		if (hit) {
			r->hits_n++;
			r->exec_durations.push_back(s->running_time);
		}
		dump_stats(s->stats, d->report_dir, i);
		s->reset();
		d->mtx.unlock();
	}
}

void Runner::execute(std::vector<Solver*> solvers, int sample_size, int timeout, string dirPattern) {
	exec_durations.clear();
	hits_n = 0;
	hits_ratio = 0;
	average_t = 0;

	long long time = get_time_mcs() / 1000;
	string report_dir = "plotData/" + dirPattern + "_" + to_string(time);

	system(("mkdir -p " + report_dir).c_str());

	RunnerParams common;
	common.jobs_done = 0;
	common.sample_size = sample_size;
	common.timeout = timeout;
	common.report_dir = report_dir;
	RunnerParams* d = &common;
	Runner* r = this;

	std::vector<std::thread> t;

	std::cout << "Running experiment, sample size = " << sample_size << std::endl;

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

	// todo dump this info to the file
	cout << "Hits: " << hits_n <<
		", hits ratio: " << hits_ratio <<
		", avg t: " << average_t <<
		"\n";
}
