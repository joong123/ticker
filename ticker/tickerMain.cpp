// tickerMain.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "ticker.h"

int main()
{
	Ticker::Prepare();

	Ticker::ticktime();
	Ticker::tickcompare00();

    return 0;
}

