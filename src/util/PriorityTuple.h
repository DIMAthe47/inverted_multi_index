#ifndef PRIORITYTUPLE_H
#define PRIORITYTUPLE_H

template <typename T>
struct PriorityTuple
{
	float priority;
	T value;
};

template <typename T>
class ComparePriorityTuple
{
public:
	bool operator() (PriorityTuple<T> t1, PriorityTuple<T> t2)
	{
		return t1.priority > t2.priority;
	}
};

#endif