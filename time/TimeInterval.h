#ifndef TIME_INTERVAL_H
#define TIME_INTERVAL_H

#include "Clock.h"

class TimeInterval {
public:
	long long int startTime;
	long long int endTime;

	Clock clock;

	void start() {
		startTime = clock.getTime();
	}

	void end() {
		endTime = clock.getTime();
	}

	long long int get() {
		return endTime - startTime;
	}
};

#endif