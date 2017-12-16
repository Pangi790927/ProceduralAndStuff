#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Clock.h"

class FPSCounter {
public:
	bool updated = false;

	int count = 0;
	float lastCount = 0;
	long long int lastTime = 0;
	Clock counterClock;

	FPSCounter() : counterClock() {
		lastTime = counterClock.getTime();
	}

	float getCount() {
		count++;
		long long int currentTime = counterClock.getTime();
		
		if (currentTime - lastTime > 1000) {
			lastCount = (currentTime - lastTime) / float(Clock::SEC) * count;
			lastTime = counterClock.getTime();
			updated = true;
			count = 0;
		}

		return lastCount;
	}

	bool hasUpdated() {
		return updated;
	}

	void resetUpdate() {
		updated = false;
	}
};

#endif