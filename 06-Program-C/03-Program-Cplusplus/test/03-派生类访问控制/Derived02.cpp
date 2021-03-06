/*************************************************************************
	> File Name: Derived02.cpp
	> Author: Maxwell
	> Mail: miaopei163@163.com 
	> Created Time: 2019年03月27日 星期三 16时26分56秒
 ************************************************************************/
/*
 * 静态成员函数和非静态成员函数的共同点:
 * 1.他们都可以被继承到派生类中。
 * 2.如果重新定义一个静态成员函数，所有在基类中的其他重载函数会被隐藏。
 * 3.如果我们改变基类中一个函数的特征，所有使用该函数名的基类版本都会被隐藏。
 * 静态成员函数不能是虚函数（virtual function）.
 */
#include<iostream>
using namespace std;

class Base {
public:
	static int getNum(){ return sNum; }
	static int getNum(int param){ return sNum + param }
public:
	static int sNum;
};

int Base::sNum = 10;

class Derived : public Base {
public:
	static int sNum;	// 基类静态成员属性将被隐藏
#if 0
	// 重定义一个函数，基类中重载的函数被隐藏
	static int getNum(int param1, int param2)
	{
		return sNum + param1 + param2;
	}
#else
	// 改变基类函数的某个特征，返回值或者参数个数，将会隐藏基类重载的函数
	static int getNum(int param1, int param2)
	{
		cout << sNum + param1 + param2 << endl;
	}
#endif
};

int Derived::sNum = 20;

int main()
{
	return 0;
}
