#pragma once

template<class T>
class LAPSBSingleton
{
public:
	static T& GetInstance()
	{
		static T Singleton;

		return Singleton;
	}
};

