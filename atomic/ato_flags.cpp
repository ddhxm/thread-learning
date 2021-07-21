// using atomic_flag as a lock
#include <iostream>       
#include <atomic>         
#include <thread>         
#include <vector>         
#include <sstream>        
using namespace std;
/*知识点：
ATOMIC_FLAG_INIT:用来将atomic_flag类型的对象初始化为清除状态,即clear
atomic_flag::clear():将atomic_flag对象设置为false，
使对此对象的成员atomic_flag::test_and_set的下一次调用返回false。
atomic_flag::test_and_set():设置atomic_flag，并且返回 调用该函数之前atomic_flag是否被set。
    1.test：读取当前atomic_flag的值,并返回个这结果值
    2.set：将atomic_flag状态设置为ture
    调用此函数之后atomic_flag一定等于true

*/
atomic_flag lock_stream = ATOMIC_FLAG_INIT;//设置为false
stringstream stream;

void append_number(int x) {
  while (lock_stream.test_and_set()) {cout<<lock_stream.test_and_set()<<endl;}//读取当前atomic_flag的值，由于未被set，返回false
  stream << "thread #" << x << '\n';//调用之后，atomic_flag=true
  lock_stream.clear();//因此，又清除atomic_flag=false,若注释掉这行，则atomic_flag=true，进入while循环
}

int main ()
{
  vector<thread> threads;
  for (int i=1; i<=10; ++i) threads.push_back(thread(append_number,i));
  for (auto& th : threads) th.join();//main函数阻塞直到所有线程运行结束

  cout << stream.str();//输出所有线程中的输出行
  system("pause");
  return 0;
}