#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
using namespace std;

/*以一个生产者，两个消费者为例，通过Dispatch(2,1)里的两个for循环来创建消费者线程2/3和生产者线程4，
注意:1.先进行消费者线程创建的循环，再进行的生产者创建，因此，终端输出中可以发现输出的相关信息消费者在前;
    2.由于此时队列缓冲区中为空，因此，消费者2/3均进入等待，等待生产者的notify信号
    3.生产者线程开始向队列中添加数据，直到达到最大长度10，则进入睡眠等待，等待消费者notify信号
    4.由于生产者随机唤醒了一个线程例如2，则线程2开始取出队列中的消息，直到队列为空，此时由于线程2占有互斥量m，
    其直到队列为空之后才能解锁互斥量m，因此线程3无法获取队列中的消息，直到队列为空解锁之后，线程3占有，但此时发现，
    队列已为空，因此，又进入等待状态
    5.循环2.3，消息队列满了之后，生产者唤醒线程3，取出队列中的消息，其他同4
	通过notify_one的唤醒，若存在多个线程，则通过轮询的方式唤醒线程
*/
//缓冲区存储的数据类型 
struct CacheData
{
	//商品id 
	int id;
	//商品属性 
	string data;
};

queue<CacheData> Q;//缓冲区最大空间队列 
const int MAX_CACHEDATA_LENGTH = 10;//队列中最大长度
//互斥量m，生产者之间，消费者之间，生产者和消费者之间，同时都只能一个线程访问缓冲区 
mutex m;
condition_variable condConsumer;//变量，用于同步线程，消费者变量
condition_variable condProducer;//生产者变量
//全局商品id 
int ID = 1;

//消费者动作 
void ConsumerActor()
{
	unique_lock<mutex> lockerConsumer(m);//此函数结束则自动解锁
    cout<<"--------------ConsumerActor-----------------"<<endl;
	cout << "[" << this_thread::get_id() << "] get lock" << endl; 
	while (Q.empty())//当缓存区队列为空时，等待生产者生产填充缓存区
	{
		cout <<  " Q is empty!" << endl; 
		cout << "Cosumer ["<< this_thread::get_id() << "]are sleeping " << endl;
		condProducer.notify_one();//唤醒一个生产者线程
		cout << "[" << this_thread::get_id() << "] release lock" << endl;
		//队列空， 消费者停止，等待生产者唤醒 
        cout<<"wait for producer..."<<endl;
		condConsumer.wait(lockerConsumer);//wait函数需要传入一个mutex(unique_lock<mutex>),自动释放锁,等待notify唤醒
		cout << "[" << this_thread::get_id() << "] Wake, get the lock again!" << endl; 
	}
	int cut=0;
	cout << "[" << this_thread::get_id() << "] "; 
	CacheData temp = Q.front();//取出队列中的头指针消息
	cout << "out - ID:" << temp.id << " Data:" << temp.data << endl;
	Q.pop(); //出队
	condProducer.notify_one();//生产者唤醒一个等待的线程
}

//生产者动作 
void ProducerActor()
{
	unique_lock<mutex> lockerProducer(m);
    cout<<"--------------ProducerActor-----------------"<<endl;
	cout << "producer thread [" << this_thread::get_id() << "] get lock" << endl; 
	while (Q.size() >= MAX_CACHEDATA_LENGTH)//队列缓冲区满了
	{
		cout <<  "Q are full! producer "<< "[" << this_thread::get_id() << "]are Sleeping " << endl; 
		cout << "[" << this_thread::get_id() << "] release lock" << endl; 
		condConsumer.notify_one();//唤醒第一个等待的线程
		//生产者停止，等待消费者唤醒 
		condProducer.wait(lockerProducer);//队列满之后欧，生产者等待消费者唤醒
		cout << " [" << this_thread::get_id() << "] Wake, get lock again!" << endl; 
	}
	cout << "[" << this_thread::get_id() << "] "; 
	CacheData temp;
	temp.id = ID++;
	temp.data = "*****";
	cout << "in + ID:" << temp.id << " Data:" << temp.data << endl; 
	Q.push(temp);//将生产的消息入队
	
	
}

//消费者任务 
void ConsumerTask()
{
	while(1)
	{
		ConsumerActor();
	}	
}

//生产者任务 
void ProducerTask()
{
	while(1)
	{
		ProducerActor();
	}	
}

//管理线程的函数 
void Dispatch(int ConsumerNum, int ProducerNum)
{
	vector<thread> thsC;//消费者线程
	for (int i = 0; i < ConsumerNum; ++i)
	{
		thsC.push_back(thread(ConsumerTask));
        cout<<"Consumer threads ID are:["<<thsC[i].get_id()<<"]";
	}
    cout<<endl;
	
	vector<thread> thsP;//生产者线程
	for (int j = 0; j < ProducerNum; ++j)
	{
		thsP.push_back(thread(ProducerTask));
        cout<<"producer threads ID are:["<<thsP[j].get_id()<<"]";
	}
    cout<<endl;
	
	for (int i = 0; i < ConsumerNum; ++i)
	{
		if (thsC[i].joinable())
		{
			thsC[i].join();//等待子线程运行结束，阻塞主任务
		}
	}
	
	for (int j = 0; j < ProducerNum; ++j)
	{
		if (thsP[j].joinable())
		{
			thsP[j].join();
		}
	}
}

int main()
{
	//一个消费者线程，5个生产者线程，则生产者经常要等待消费者 
	Dispatch(2,1);
    system("pause");
	return 0; 
}