#include "runner.h"
#include <fstream>
#include <ctime>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

using namespace std;

void dump_stats(vector<statItem> stats, string dir, int exN) {
	ofstream outS;

	char buffer[200];
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
	string run_dir;
	int* hits_n;
	std::vector<long long>* exec_durations;
};

void perform_run(RunnerParams* d, Solver* s) {
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
			(*d->hits_n)++;
			d->exec_durations->push_back(s->running_time);
		}
		dump_stats(s->stats, d->run_dir, i);
		s->reset();
		d->mtx.unlock();
	}
}

void Runner::execute( std::vector<std::pair<Solver*,long long>> solvers
					, int threads_n
					, int sample_size
					, string dir_pattern) {
	time_t rawtime;
	struct tm * timeinfo;
	char timebuffer[80];

	time (&rawtime); timeinfo = localtime(&rawtime);
	strftime(timebuffer,sizeof(timebuffer),"%Y-%m-%d_%H-%M-%S",timeinfo);
	std::string timestr(timebuffer);

	string experiment_dir = dir_pattern + "_" + timestr;
	system(("mkdir -p " + experiment_dir).c_str());
	cout << "Experiment dir: " << experiment_dir << "\n";

	for (uint i = 0; i < solvers.size(); i++) {
		Solver* solver = get<0>(solvers[i]);
		long long timeout = get<1>(solvers[i]);

		std::cout << "Processing " + solver->get_name() +
			", n = " + to_string(solver->get_N()) + "\n";

		string run_dir =
			experiment_dir + "/" + solver->get_name() + "/n_" +
			to_string(solver->get_N());
		system(("mkdir -p " + run_dir).c_str());

		int hits_n = 0;
		double hits_ratio = 0;
		int average_t = 0;
		std::vector<long long> exec_durations;

		RunnerParams common;
		common.jobs_done = 0;
		common.sample_size = sample_size;
		common.timeout = timeout;
		common.run_dir = run_dir;
		common.exec_durations = &exec_durations;
		common.hits_n = &hits_n;
		RunnerParams* d = &common;

		std::vector<std::thread> t;

		std::cout << "Running experiment, sample size = " << sample_size << std::endl;

		std::vector<Solver*> solver_clones(threads_n);

		cout << "Forking all the threads\n";
		for (int j = 0; j < threads_n; j++) {
			solver_clones[j] = solver->clone();
			t.push_back(std::thread(perform_run, d, solver_clones[j]));
		}
		for (auto& th : t) th.join();
		for (Solver* s : solver_clones) free(s);

		std::cout << std::endl;

		if (hits_n != 0) {
			hits_ratio = (double) hits_n / ((double) sample_size);
			for (uint i = 0; i < exec_durations.size(); i++) {
				average_t += exec_durations[i];
			}
			average_t /= hits_n;
		}

		ofstream outS;
		char buffer[50]; sprintf(buffer, "%s/summary.csv", run_dir.c_str());
		outS.open(buffer);

		std::string summary =
			to_string(timeout) + "," +
			to_string(hits_n) + "," +
			to_string(hits_ratio) + "," +
			to_string(average_t) + "\n";
		std::cout << "timeout/hitsN/hitsRatio/avgT:\n" << summary;
		outS << summary;
		outS.close();

	}
}
