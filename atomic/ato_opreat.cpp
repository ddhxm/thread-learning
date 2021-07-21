// atomic::operator=/operator T example:
#include <iostream>       
#include <atomic>         
#include <thread>         
using namespace std;
atomic<int> foo (0);
/*知识点：
  1.this_thread::yield()让当前线程让渡出自己的CPU时间片(给其他线程使用
  first线程创建并运行，此时原子对象的值为0，因此first空出时间片，此时，second线程将foo值改为10，此时first跳出循环
*/
void set_foo(int x) {
  foo = x;
  cout <<"second thread "<< this_thread::get_id()<<" foo: " << foo << '\n';
}

void print_foo() {
  while (foo==0) {      
    this_thread::yield();
    cout<<"first thread "<<this_thread::get_id()<<" empty the time for other thread!"<<endl;
  }
  cout <<"first thread "<< this_thread::get_id()<<" foo: " << foo << '\n';
}

int main ()
{
  thread first (print_foo);
  thread second (set_foo,10);
  first.join();
  second.join();
  system("pause");
  return 0;
}