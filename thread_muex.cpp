#include<iostream>
#include<thread>
#include<mutex>//互斥量头文件
using namespace std;
mutex m;//互斥量，实例化m对象，不能理解为定义变量

/*需要在进入临界区之前对互斥量加锁lock，退出临界区时对互斥量解锁unlock；
当一个线程使用特定互斥量锁住共享数据时，其他的线程想要访问锁住的数据，都必须等到之前那个线程对数据进行解锁后，
才能进行访问。需注意若直接调用lock，容易忘记解锁，因此可调用lock_guard*/

void proc1(int a)
{
    m.lock();//临界区对互斥量加锁
    cout << "proc1 is trying to change a..." << endl;
    cout << "original a is " << a << endl;
    cout << "now a is " << a + 2 << endl;
    m.unlock();//临界区对互斥量解锁
}

void proc2(int a)
{
    m.lock();
    cout << "proc2 is trying to change a..." << endl;
    cout << "original a is " << a << endl;
    cout << "now a is " << a + 1 << endl;
    m.unlock();
}
int main()
{
    int a = 0;
    cout<<a<<" is in main function"<<endl;
    
    thread t1(proc1, a);//创建线程,同时线程开始执行
    thread t2(proc2, a);
    
    t2.join();//等待
    t1.join();
    system("pause");
    return 0;
}