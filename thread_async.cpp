#include <iostream>
#include <thread>
#include <mutex>
#include<future>//异步头文件
#include<Windows.h>
using namespace std;
double t1(const double a, const double b)
{
 double c = a + b;
 Sleep(3000);//假设t1函数是个复杂的计算过程，需要消耗3秒
 return c;
}
//std::async是一个函数模板，用来启动一个异步任务，返回一个std::future类模板对象，future对象起到了占位的作用
int main() 
{
 double a = 2.3;
 double b = 6.7;
 future<double> fu = async(t1, a, b);//创建异步线程线程，并将线程的执行结果用fu占位；
 cout << "computing..." << endl;
 cout << "wait for few second please" << endl;
 cout << "The result is: " << fu.get() << endl;//阻塞主线程，直至异步线程计算完毕return 计算结果
 //cout << "计算结果：" << fu.get() << endl;//取消注释后运行会报错，future对象的get()方法只能调用一次。
 system("pause");
 return 0;
}