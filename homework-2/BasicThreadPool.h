#ifndef BASICTHREADPOOL_H
#define BASICTHREADPOOL_H

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>
#include <functional>

#include "SteeringBehavior.h"

// This class is a pool of worker threads 
class BasicThreadPool
{
private:

    int numThreads;
    // SteeringBehavior * steeringBehavior;
    std::queue<std:: function<void()> > tasks;

    std::mutex queueMutex;
    std::condition_variable condition;

    // pool of worker threads
    std::vector<std::thread> workers;

    void threadFunction(){
        while(true){
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]
                { 
                    return !tasks.empty(); 
                });
                task = tasks.front();
                tasks.pop();
            }
            task();
        }
    }

public:
    
    BasicThreadPool(int numThreads)
    {
        this->numThreads = numThreads;

        for(int i = 0; i < numThreads; i++)
        {
            workers.push_back(std::thread(&BasicThreadPool::threadFunction, this));
        }
    }
    
    void addUpdateJob(SteeringBehavior& SteeringBehavior,Boid& sprite, float elapsedTime)
    {
        // call addTask to add a new task to the queue
        addTask([&SteeringBehavior,&sprite,elapsedTime]
        {
            SteeringBehavior.updateSprite(sprite, elapsedTime);
        });

    }

    void addTask(std::function<void()> task)
    {

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(task);
        }
    
        condition.notify_one();
    }

    ~BasicThreadPool()
    {
        condition.notify_all();
        for(int i = 0; i < numThreads; i++)
        {
            workers[i].join();
        }
    }



};


#endif