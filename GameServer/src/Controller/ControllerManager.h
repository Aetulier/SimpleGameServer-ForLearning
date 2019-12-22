#pragma once
#include <iostream>
#include <map>
#include <string>

typedef void *(*PCreateObject)(void);

//反射工厂类
class ClassFactory
{
private:
	std::map<std::string, PCreateObject> m_classMap;
	ClassFactory(){};

public:
	void *CreateObjectByName(std::string className)
	{
		std::map<std::string, PCreateObject>::const_iterator iter;
		iter = m_classMap.find(className);
		if (iter == m_classMap.end())
			return NULL;
		else
			return iter->second();
	}
	void registClass(std::string name, PCreateObject method)
	{
		m_classMap.insert(std::pair<std::string, PCreateObject>(name, method));
	}
	static ClassFactory &getInstance()
	{
		{
			static ClassFactory cf;
			return cf;
		}
	}
};
//注册反射类
class RegisterAction
{
public:
	RegisterAction(std::string className, PCreateObject ptrCreateFn)
	{
		ClassFactory::getInstance().registClass(className, ptrCreateFn);
	}
};
//注册反射类宏定义
#define REGISTER(className)                     \
	className *objectCreator##className()       \
	{                                           \
		return new className;                   \
	}                                           \
	RegisterAction g_RegisterAction##className( \
		#className, (PCreateObject)objectCreator##className);
