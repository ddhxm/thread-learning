#include<iostream>
#include<thread>
#include<Windows.h>
using namespace std;

void proc(int a)
{   cout<<"\nProc!"<<endl;
    cout << "I am tread " << a << "in thread proc"<<endl;
    cout << "thread id in proc: " << this_thread::get_id()<< endl;
    _sleep(5000);
}

int main()
{
    cout << "I am the main tread!" << endl; //main函数运行
    int a = 9;
    thread th2(proc,a);//线程创建，同时线程进入运行
    //_sleep(5000);
    cout << "thread id in main is: " << th2.get_id() << endl;//此时main和线程是交替运行的，
    //即并发的，所以有时屏幕上有时打印的会交错,可以将屏幕看成需要竞争的资源
    th2.join();//此时主线程被阻塞直至子线程执行结束,即子线程等待主线阻塞。
    //调用join()会清理线程相关的存储部分，这代表了join()只能调用一次,因此系列语句输出id为无此线程
    cout << "thread id in main is: " << th2.get_id() << endl;//此时main和线程是交替运行的，
    cout<<" Thread 2 end!Back to the main fun"<<endl;
    system("pause");
    return 0;
}
