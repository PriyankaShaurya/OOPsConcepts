#include <iostream>
#include <string>
#include <thread>
#include <mutex>
std::mutex print_mutex;
using namespace std;
thread_local int id;
struct Ex
{
	int num1;
	int num2;
	int sum;
};
struct IF
{
	int num1;
	int num2;
	int sum;
};
namespace Goal
{
template<typename T>
class GlobalEx
{
private:
	T ex[5];
public:
	T* operator->()
	{
		return &ex[id];
	}
};
}
Goal::GlobalEx<Ex> ex;
Goal::GlobalEx<IF> iff;
void FindSum()
{
	ex->sum = ex->num1 + ex->num2;
	iff->sum = iff->num1 + iff->num2;
}
void Run(int threadId)
{
	id = threadId;
	std::this_thread::sleep_for(std::chrono::seconds(rand() % 5));
	ex->num1 = 2 * threadId;
	ex->num2 = 3 * threadId;
	iff->num1 = 6 * threadId;
	iff->num2 = 7 * threadId;
	FindSum();
	std::lock_guard<std::mutex> lock(print_mutex);
	std::cout << " For ThreadId "<<threadId << " => " << "Sum for Ex is: "<<ex->sum<<endl;
	std::cout << " For ThreadId " << threadId << " => " << "Sum for IF is: " << iff->sum << endl;
}
int main()
{
	std::thread a(Run, 0);
	std::thread b(Run, 1);
	std::thread c(Run, 2);
	a.join();
	b.join();
	c.join();
	return 0;
}