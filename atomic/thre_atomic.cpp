// constructing atomics建立原子
#include <iostream>       
#include <atomic>         
#include <thread>         
#include <vector>     
using namespace std;    
/*
知识点：
      1；this_thread::yield()：避免一个线程频繁与其他线程争抢CPU时间片,从而导致多线程处理性能下降.
      this_thread::yield() 是让当前线程让渡出自己的CPU时间片(给其他线程使用)
      流程：
      1.线程创建
      通过创建10个线程，同时开始执行，此时由于原子对象初始化为false，因此，线程均进入让渡模式；
      2.同时main函数中，也将ready置TRUE，几个线程开始跳过while循环，进入计数
      3.几个线程谁先完成计数，即for循环，线程x通过调用winner.test_and_set()将初始化atomic_flag=false返回，
      输出线程x胜利的ID，且线程x重新set为TRUE，使得其他的线程无法进入if语句
      atomic_flag主要用于标志10个线程中的胜者，即第一个调用test_and_set()函数的线程
      


*/
atomic<bool> ready (false);//创建bool型原子对象，同时初始化值为false
atomic_flag winner = ATOMIC_FLAG_INIT;//将atomic_flag初始化为false

void count1m (int id) {
  //当main函数将ready置TRUE时，tread x开始计数直到100000
  while (!ready) { this_thread::yield(); }      // 等待ready信号，ready=false时，让渡时间片给其他线程
  for (volatile int i=0; i<1000000; ++i) {}          // 开始计数
  if (!winner.test_and_set()) { cout << "thread #" << id << " won!\n"; }
  //返回当前atomic_flag的值，若为false，则进入输出行，并将atomic_flag=TRUE，此时其他其他线程无法进入，得到胜出的线程id
};
 
int main ()
{
  vector<thread> threads;//创建线程数组
  cout << "spawning 10 threads that count to 1 million...\n";//创建10个线程，用于计数到100万
  for (int i=1; i<=10; ++i) threads.push_back(thread(count1m,i));
  //_sleep(1000);
  ready = true;
  for (auto& th : threads) th.join();
  system("pause");
  return 0;
}