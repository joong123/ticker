#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <functional>

#include "ticker.h"

using std::cout;
using std::endl;
using std::setfill;
using std::ios;
using std::setw;
using std::setprecision;
using std::function;
using std::thread;
using std::ref;

LONGLONG Ticker::freqCounter = 0;
const double Ticker::freqSystem = 10000000.0;


void Ticker::ticktime()
{
	Ticker::Prepare();
	tickc	tp1 = 0;
	tickc	tp2 = 0;
	utick	ts1 = 0;
	utick	ts2 = 0;
	utick	tc1 = 0;
	utick	tc2 = 0;

	cout << "===begin timing===" << endl;

	Ticker::GetTickSystem(ts1);
	Ticker::GetTickCounter(tp1);
	tc1 = Ticker::GetTickCycle();

	for (__int64 i = 0; i >= 0; i++)
	{
		if (i % 1000000 == 0)
		{
			double t1 = 0, t2 = 0, t3 = 0;
			Ticker::GetTickSystemSecond(ts1, t1);
			Ticker::GetTickCounterSecond(tp1, t2);
			Ticker::GetTickCycleSecond(tc1, t3);

			cout << "\rC:" << setprecision(8) << t1
				<< "\tP:" << t2
				<< "\tS:" << t3
				<< "\t";
		}
	}
}

void fun1(double &t1, double &t2)
{
	const int loops = 40000000;
	tickc	ts1 = 0;
	tickc	ts2 = 0;

	// cache init
	double time1 = 0, time2 = 0;
	for (int i = 0; i < loops; i++)
	{
		utick ut = 0;
		ut = Ticker::GetTickCycle();
	}

	// calc
	Ticker::GetTickCounter(ts1);
	for (int i = 0; i < loops; i++)
	{
		utick ut = 0;
		ut = Ticker::GetTickCycle();
	}
	time1 = 0;
	Ticker::GetTickCounterSecond(ts1, time1);
	t1 += time1;

	// cache init
	for (int i = 0; i < loops; i++)
	{
		tickc ut = 0;
		ut = Ticker::GetTickCounter();
	}

	// calc
	Ticker::GetTickCounter(ts1);
	for (int i = 0; i < loops; i++)
	{
		tickc ut = 0;
		ut = Ticker::GetTickCounter();
	}
	time2 = 0;
	Ticker::GetTickCounterSecond(ts1, time2);
	t2 += time2;

	cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
		<< "\tt2:" << t2 << " [" << time2 / loops << ']'
		<< "\tdiff:" << t1 - t2
		<< "\t" << (t1 - t2) / t1 * 100 << '%';
}

void fun2(double &t1, double &t2)
{
	const int loops = 40000000;
	tickc	ts1 = 0;
	tickc	ts2 = 0;

	// cache init
	double time1 = 0, time2 = 0;
	for (int i = 0; i < loops; i++)
	{
		tickc ut = 0;
		ut = Ticker::GetTickCounter();
	}

	// calc
	Ticker::GetTickCounter(ts1);
	for (int i = 0; i < loops; i++)
	{
		tickc ut = 0;
		ut = Ticker::GetTickCounter();
	}
	time2 = 0;
	Ticker::GetTickCounterSecond(ts1, time2);
	t2 += time2;

	// cache init
	for (int i = 0; i < loops; i++)
	{
		utick ut = 0;
		ut = Ticker::GetTickCycle();
	}

	// calc
	Ticker::GetTickCounter(ts1);
	for (int i = 0; i < loops; i++)
	{
		utick ut = 0;
		ut = Ticker::GetTickCycle();
	}
	time1 = 0;
	Ticker::GetTickCounterSecond(ts1, time1);
	t1 += time1;

	cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
		<< "\tt2:" << t2 << " [" << time2 / loops << ']'
		<< "\tdiff:" << t1 - t2
		<< "\t" << (t1 - t2) / t1 * 100 << '%';
}

void Ticker::tickcompare00()
{
	double t1 = 0, t2 = 0;
	bool bbb = true;

	auto fun = std::bind(fun1, ref(t1), ref(t2));
	for (int i = 0;; i++)
	{
		if (bbb)
		{
			fun = std::bind(fun1, ref(t1), ref(t2));
		}
		else
		{
			fun = std::bind(fun2, ref(t1), ref(t2));
		}
		fun();
		bbb = !bbb;
	}
}

void Ticker::tickcompare0()
{
	double t1 = 0, t2 = 0;
	volatile bool bbb = true;// make this function not precise
	for (int i = 0;; i++)
	{
		// cache init
		if (bbb)
		{
			auto fun1 = [&t1, &t2]() {
				const int loops = 40000000;
				tickc	ts1 = 0;
				tickc	ts2 = 0;

				// cache init
				double time1 = 0, time2 = 0;
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem();
				}

				// calc
				Ticker::GetTickCounter(ts1);
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem();
				}
				time1 = 0;
				Ticker::GetTickCounterSecond(ts1, time1);
				t1 += time1;

				// cache init
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem2();
				}

				// calc
				Ticker::GetTickCounter(ts1);
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem2();
				}
				time2 = 0;
				Ticker::GetTickCounterSecond(ts1, time2);
				t2 += time2;

				cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
					<< "\tt2:" << t2 << " [" << time2 / loops << ']'
					<< "\tdiff:" << t1 - t2
					<< "\t" << (t1 - t2) / t1 * 100 << '%';
			};
			fun1();
		}
		else
		{
			auto fun2 = [&t1, &t2]() {
				const int loops = 40000000;
				tickc	ts1 = 0;
				tickc	ts2 = 0;

				// cache init
				double time1 = 0, time2 = 0;
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem2();
				}

				// calc
				Ticker::GetTickCounter(ts1);
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem2();
				}
				time2 = 0;
				Ticker::GetTickCounterSecond(ts1, time2);
				t2 += time2;

				// cache init
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem();
				}

				// calc
				Ticker::GetTickCounter(ts1);
				for (int i = 0; i < loops; i++)
				{
					utick ut = 0;
					ut = Ticker::GetTickSystem();
				}
				time1 = 0;
				Ticker::GetTickCounterSecond(ts1, time1);
				t1 += time1;

				cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
					<< "\tt2:" << t2 << " [" << time2 / loops << ']'
					<< "\tdiff:" << t1 - t2
					<< "\t" << (t1 - t2) / t1 * 100 << '%';
			};
			fun2();
		}
		bbb = !bbb;
	}
}

void Ticker::tickcompare()
{
	utick	ts1 = 0;
	utick	ts2 = 0;

	const int loops = 40000000;
	double t1 = 0, t2 = 0;
	for (int i = 0;; i++)
	{
		double time1 = 0, time2 = 0;
		if (i & 0x01 == 0)
		{
			// cache init
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}

			// calc
			Ticker::GetTickSystem(ts1);
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}
			time1 = 0;
			Ticker::GetTickSystemSecond(ts1, time1);
			t1 += time1;

			// cache init
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}

			// calc
			Ticker::GetTickSystem(ts1);
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}
			time2 = 0;
			Ticker::GetTickSystemSecond(ts1, time2);
			t2 += time2;

			cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
				<< "\tt2:" << t2 << " [" << time2 / loops << ']'
				<< "\tdiff:" << t1 - t2
				<< "\t" << (t1 - t2) / t1 * 100 << '%';
		}
		else
		{
			// cache init
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}

			// calc
			Ticker::GetTickSystem(ts1);
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}
			time1 = 0;
			Ticker::GetTickSystemSecond(ts1, time1);
			t2 += time1;

			// cache init
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}

			// calc
			Ticker::GetTickSystem(ts1);
			for (int i = 0; i < loops; i++)
			{
				utick ut = 0;
				ut = time(0);
			}
			time2 = 0;
			Ticker::GetTickSystemSecond(ts1, time2);
			t1 += time2;

			cout << "\rt1:" << t1 << " [" << time1 / loops << ']'
				<< "\tt2:" << t2 << " [" << time2 / loops << ']'
				<< "\tdiff:" << t1 - t2
				<< "\t" << (t1 - t2) / t1 * 100 << '%';
		}
	}
}