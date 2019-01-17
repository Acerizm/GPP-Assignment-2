#pragma once
#ifndef _TIMER_H             // prevent multiple definitions if this 
#define _TIMER_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <chrono>

class Timer {

private:
	//declare start and end variables with the same type
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

public:

	Timer() {
		start = std::chrono::steady_clock::now();
	}

	//this will be called when the function ends :D
	~Timer() {
		end = std::chrono::steady_clock::now();
		duration = end - start;
	}

	//this is to get how many seconds have passed since the start of the game
	float getCurrentElapsedTime(bool isPaused) {
		
		std::chrono::time_point<std::chrono::steady_clock>  currentTimePoint;
		
		currentTimePoint = std::chrono::steady_clock::now();
		std::chrono::duration<float> currentTimeDuration;
		currentTimeDuration = (currentTimePoint - start);
		return currentTimeDuration.count();
	}

};


#endif