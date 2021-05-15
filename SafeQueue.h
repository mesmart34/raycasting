#pragma once
#include <mutex>
#include <queue>
#include <iostream>
template<class T>
class SafeQueue
{
public:
	SafeQueue() = default;

    void push(T* elem) {

        m_mutex.lock();
        if (elem != nullptr) {
            m_queue.push(elem);
        }
        m_mutex.unlock();

    }

    T* next() {

        T* elem = nullptr;

        m_mutex.lock();
        if (!m_queue.empty()) {
            elem = m_queue.front();
            m_queue.pop();
           // std::cout << m_queue.size() << std::endl;
        }
        m_mutex.unlock();

        return elem;

    }

private:
	std::queue<T*> m_queue;
	std::mutex m_mutex;
};

