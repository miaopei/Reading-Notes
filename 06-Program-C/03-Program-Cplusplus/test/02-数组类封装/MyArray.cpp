/*************************************************************************
	> File Name: MyArray.cpp
	> Author: Maxwell
	> Mail: miaopei163@163.com 
	> Created Time: 2019年03月26日 星期二 15时53分28秒
 ************************************************************************/

#include<iostream>
#include"MyArray.h"
using namespace std;

MyArray::MyArray()
{
	this->mCapacity = 100;
	this->mSize = 0;
	// 在堆开辟空间
	this->pAdress = new int[this->mCapacity];
}

// 有参构造函数，用户指定容量初始化
MyArray::MyArray(int capacity)
{
	this->mCapacity = capacity;
	this->mSize = 0;
	// 在堆开辟空间
	this->pAdress = new int[capacity];
}

// 根据位置添加元素
void MyArray::SetData(int pos, int val)
{
	if (pos < 0 || pos > mCapacity - 1)
		return;
	pAdress[pos] = val;
}

// 获得指定位置元素
int MyArray::GetData(int pos)
{
	if (pos < 0 || pos > mCapacity - 1)
		return -1;
	return pAdress[pos];
}

// 尾插法
void MyArray::PushBack(int val)
{
	if (mSize >= mCapacity)
		return;
	this->pAdress[mSize] = val;
	this->mSize++;
}

// 获得长度
int MyArray::GetLength()
{
	return this->mSize;
}

// 析构函数，释放数组空间
MyArray::~MyArray()
{
	if (this->pAdress != nullptr)
		delete[] this->pAdress;
}

