#pragma once

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

struct PriorityTuple2
{
	float priority;
	int value1;
	int value2;
};

class ComparePriorityTuple2
{
public:
	bool operator() (const PriorityTuple2 t1, const PriorityTuple2 t2)
	{
		return t1.priority > t2.priority;
	}
};

