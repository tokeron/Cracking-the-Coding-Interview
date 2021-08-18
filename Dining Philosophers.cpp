#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

mutex write_lock;

class chopstick{
public:
    mutex m;
    bool pickup(){
        if (m.try_lock()){
            return true;
        }
        return false;
    }
    void putDown(){
        m.unlock();
    }
};

void eat(chopstick& left, chopstick& right, int i)
{
    bool ate = false;
    while (!ate)
    {
        if (left.pickup()){
            if (right.pickup()){
                right.putDown();
                ate = true;
                write_lock.lock();
                cout << "philosopher No." << i << " ate" << endl;
                write_lock.unlock();
            }
            left.putDown();
        }
    }
}

void simulate(int number_of_philosophers)
{
    vector<thread> philosophers(number_of_philosophers);
    vector<chopstick> chopsticks(number_of_philosophers);

    for (int i = 0; i < number_of_philosophers; i++)
    {
        philosophers[i] = thread(eat, std::ref(chopsticks[((i - 1) == -1)? (number_of_philosophers - 2) : (i - 1) ]), std::ref(chopsticks[i]), i);
    }
    for (int i = 0; i < number_of_philosophers; i++)
    {
        philosophers[i].join();
    }
}

int main()
{
    int number_of_philosophers = 10;
    simulate(number_of_philosophers);
    return 0;
}
