/*
Transien signal derivator
slopeAnalyser returns the delta-div over dt set;
Timeout library is high-level code so cannot run so fast;
Result is only positive(just derivate value)
*/


long sample0;
long sample1;
float slope;
bool sample0Busy;
bool sample1Busy;
Timeout T;

float slopeAnalyser(int pin, int dt)
{
	if(!sample0Busy)
	{
		sample0 = analogRead(pin);
		T.startTimer(dt);
		sample0Busy = true;
	}
	if(T.isFinish() && !sample1Busy)
	{
		sample1 =analogRead(pin);
		sample1Busy = true;
	}
	if(sample1Busy && sample0Busy)
	{
		slope = abs(sample1 - sample0) / dt;
		sample0Busy = false;
		sample1Busy = false;
	}


	return slope;
}
