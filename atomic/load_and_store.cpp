
#include <iostream>      
#include <atomic>         
#include <thread>         
using namespace std;

/*
typedef enum memory_order {
    memory_order_relaxed,    // 不对执行顺序做保证
    memory_order_acquire,    // 本线程中,所有后续的读操作必须在本条原子操作完成后执行
    memory_order_release,    // 本线程中,所有之前的写操作完成后才能执行本条原子操作
    memory_order_acq_rel,    // 同时包含 memory_order_acquire 和 memory_order_release
    memory_order_consume,    // 本线程中,所有后续的有关本原子类型的操作,必须在本条原子操作完成之后执行
    memory_order_seq_cst    // 全部存取都按顺序执行
    } memory_order;
    */

atomic<int> foo (0);

void set_foo(int x) {
  foo.store(x,memory_order_seq_cst);     // set value atomically自动设置值，写操作
}

void print_foo() {
  int x;
  do {
    x = foo.load(memory_order_seq_cst);  // get value atomically自动得到值，读操作
  } while (x==0);
  cout << "foo: " << x << '\n';
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