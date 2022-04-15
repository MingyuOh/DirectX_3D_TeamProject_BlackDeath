#pragma once

template<class T>
class PSBSingleton
{
public:
	static T& GetInstance()
	{
		static T Singleton;

		return Singleton;
	}
};

