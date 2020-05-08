#pragma once
// 다른 클래스를 싱글톤 만들기 편하게 하려고
template <typename T>
class SingletonBase 
{
public:
	static void Create()
	{
		if (instance == nullptr)
			instance = new T();
	}
	static void Delete()
	{
		delete(instance);
		instance = nullptr;
	}
	static T* Get()
	{
		return instance;
	}
protected:
	static T* instance;

};

template<typename T>
T* SingletonBase<T>::instance = nullptr;