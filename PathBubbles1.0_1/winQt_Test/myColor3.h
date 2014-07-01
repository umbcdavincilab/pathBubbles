#ifndef MYCOLOR3_H
#define MYCOLOR3_H


class myColor3
{
public:
	myColor3();
	//myColor3(int a, int b, int c);
	myColor3 operator +(myColor3 add);
	myColor3 operator -(myColor3 sub);
	myColor3 operator *(int mul);
	myColor3 operator /(int div);
	int operator [](int index);
	int ABS(int &color);
	int a;
	int b;
	int c;
};

#endif