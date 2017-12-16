#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock {
public:
	long long int startMeasure;

	constexpr const static long long int MSEC = 1;
	constexpr const static long long int SEC = 1e3;

	Clock () {
		startMeasure = getTime();
	}

private:
	long long int _getTime() {
		using namespace std;
		using namespace std::chrono;
		
		return duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
		).count();
	}
public:

	long long int getTime() {
		return _getTime();
	}

	long long int getTimeFromStart() {
		return _getTime() - startMeasure;	
	}
};

#endif