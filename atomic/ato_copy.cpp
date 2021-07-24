#include <iostream>       
#include <atomic>         
#include <thread>        
using namespace std;

/*流程理解如下：
    1.bar和foo的初值都为0,因此线程123并发，此时bar=0进入while循环，线程1为其他线程让出时间片
    2.线程3，foo为0时，进入while循环，为其他线程让出时间片；
    3.同样，线程2为设置foo的值，由于前面都让出时间片，因此线程2先运行，将foo的值改为10，
    此时其他线程收原子兑现改变而触发,线程3迅速的将foo的值复制到bar
    为何屏幕输出得到的结果总是不相同，我理解的是屏幕同样是线程要竞争的资源，因此，由于其并发性，
    线程交替运行，因此，每次打印的结果不一
    注：
        线程1中set_foo的++foo操作，并未影响bar的输出值，依然为bar=10，
        即通过x将foo赋值之后，copy_foo_to_bar马上跳出循环并进行copy操作，
        因此，bar得到的为设定值10，而后面set_foo函数的++并不影响，线程3print_bar中输出bar=10
        当然，与此同时，单线程jion等待之后，在输出foo和bar的值发现，foo=11，bar=10
        而在set_foo输出++foo之后，在进行++foo，此时main函数中输出的foo=12
        也说明+-适用于原子操作
    */
atomic<int> foo (0);
atomic<int> bar (0);

void set_foo(int x) {
   
  foo = x;
  cout<<"set the foo value: "<<++foo<<endl;//此处对foo进行++操作并未影响bar的输出值，依然为bar=10
   ++foo;
}//即通过x将foo赋值之后，copy_foo_to_bar马上跳出循环并进行copy操作，因此，bar得到的为设定值10，而后面set_foo函数的++并不影响
void copy_foo_to_bar () {
  while (foo==0) this_thread::yield();
  bar = static_cast<int>(foo);
  cout<<"copy foo to the bar!"<<endl;
}
void print_bar() {
  while (bar==0) {
      this_thread::yield();
  }
  std::cout << "bar: " << bar <<endl;
}

int main ()
{
  thread first (print_bar);
  thread second (set_foo,10);
  thread third (copy_foo_to_bar);
  first.join();
  second.join();
  third.join();
  std::cout << "foo: " << foo <<endl;
  std::cout << "bar: " << bar <<endl;
  system("pause");
  return 0;
}