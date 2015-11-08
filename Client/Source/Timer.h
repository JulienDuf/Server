#pragma once
#include <chrono>

class Timer {
	
private:

	double duration;
	std::chrono::high_resolution_clock::time_point lastTime;

public:

	Timer(double duration) {
		restart();
		this->duration = duration;
	}

	bool isFinished() {
		std::chrono::duration<double> time = std::chrono::duration_cast< std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - lastTime);
		return time.count() >= duration;
	}

	void restart() {
		lastTime = std::chrono::high_resolution_clock::now();
	}

	void setTime(double duration) {

		this->duration = duration;
	}
};