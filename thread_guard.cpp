#include<iostream>
#include<thread>
#include<mutex>
#include<Windows.h>
using namespace std;
mutex m;//实例化互斥量对象m，不要理解为定义变量
void proc1(int a)
{
    lock_guard<mutex> g1(m);//lock_guard传入一个参数互斥量m，直接调用lock_guard的构造函数，申请锁定m

    /*m.lock();//手动锁定
    lock_guard<mutex> g1(m,adopt_lock); adopt_lock表示构造函数中不再进行互斥量锁定，需要提前手动锁定。*/
    cout<<"--------proc1------------"<<endl;
    cout << "proc1 is trying to change a..." << endl;
    cout << "original a is " << a << endl;
    cout << "now a is " << a + 2 << endl;
    Sleep(5000);
}//g1出了作用域被释放，自动调用析构函数，于是m被解锁

void proc2(int a)
{
    {
        lock_guard<mutex> g2(m);
        cout<<"--------proc2------------"<<endl;
        cout << "proc2 is trying to change a..." << endl;
        cout << "original a is " << a << endl;
        cout << "now a is " << a + 1 << endl;
    }//通过使用{}来调整作用域范围，可使得m在合适的地方被解锁
    cout << "out of range {} a is: " <<a<<endl;
    cout << "out of range {} a is: " <<a+9<< endl;
}
int main()
{
    int a = 0;
    thread t1(proc1, a);//创建并运行
    thread t2(proc2, a);
    t1.join();
    t2.join();
    cout<<" Thread end!Back to the main fun"<<endl;
    system("pause");
    return 0;
}