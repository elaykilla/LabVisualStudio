#ifndef MYTIMER_HPP
#define MYTIMER_HPP

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

//! a task has name and time.
struct task
{
	task(const string& inname, const LONGLONG intime): name(inname), time(intime) {}

	string name;
	LONGLONG time;
};

//! timer 
class MyTimer
{
public:
	MyTimer(): m_file(false) { Intialize(); }
	MyTimer(const char* filename):m_fileout(filename), m_file(true) { Intialize(); }
	~MyTimer() { if(m_file) m_fileout.close(); }

	inline void Intialize();
	inline void PushTask(const char *name);
	inline void PopTask();

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_time;
	vector<task> m_tasks;

	ofstream m_fileout;
	bool m_file;
};

void MyTimer::Intialize() 
{
	m_tasks.reserve(10);
	memset(&m_frequency, 0x00, sizeof m_frequency);
	memset(&m_time, 0x00, sizeof m_time);
	QueryPerformanceFrequency(&m_frequency);
}	

void MyTimer::PushTask(const char *name)
{
	QueryPerformanceCounter(&m_time);	
	task in(name, m_time.QuadPart);
	m_tasks.push_back(in);
}

void MyTimer::PopTask()
{
	QueryPerformanceCounter(&m_time);
	DWORD dwTime = (DWORD)((m_time.QuadPart - m_tasks.back().time) * 1000 / m_frequency.QuadPart);
	cout << m_tasks.back().name.c_str() << ": " << static_cast<int>(dwTime) << "msec" << endl;

	printf("%d\n", static_cast<int>(dwTime));

	if(m_file){
		m_fileout << m_tasks.back().name.c_str() << ": " << static_cast<int>(dwTime) << "msec" << endl;
	}
	m_tasks.pop_back();
}

#endif