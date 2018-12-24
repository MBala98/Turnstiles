#include "turnstile.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "turnstile.h"
#include <iostream>
#include <thread>
#include <condition_variable>

#include <cassert>

std::unique_ptr<Turnstile> Mutex::ts = std::unique_ptr<Turnstile>(new Turnstile());

const static int beginning_size = 10;

//const int first_size = 10;
Turnstile::Turnstile() : /*test(100), */ factor(2), small_factor(0.75) {
    max_size = beginning_size;
    counter = beginning_size;
    for (unsigned int i = 0; i < beginning_size; i++) {
        index_queue.push(i);
      //  test[i] = std::make_unique<std::mutex>();
        mutexes[i] = std::unique_ptr<std::pair<std::mutex, std::condition_variable>>(new std::pair<std::mutex, std::condition_variable>());
      //  mutexes[i].get()->lock();
    }
}



Mutex::Mutex() : index(-1), counter(0) {}

void Turnstile::lock(Mutex *mut) {
    int tmp = 1;
    bool tmp2 = false;
    this->protection.lock();
            mut->counter++;
    //  std::cout << "ELO " << '\n';
        if (mut->index.load() == -1) {
            mut->index.store(give_index());
        }
        //mut->counter++;
           // std::cout << "going to sleep on " << mut->index.load()  << " and " << mut->counter.load() << " with " << mut->id << '\n';

  //  this->protection.unlock();
     //if (!mut->counter.compare_exchange_strong(tmp, 1)) {
        //assert(mut->index.load() > -1);
 //   std::unique_lock<std::mutex> lck(this->mutexes[mut->index.load()].get()->first);

    //    this->mutexes[a].get()->second.wait(lck);
    //this->protection.unlock();
      //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
      if (!mut->counter.compare_exchange_strong(tmp, 1)) {
    //  protection.lock();
        //std::cout << "WTF" << '\n';
      //  protection.unlock();
          std::unique_lock<std::mutex> lck(this->mutexes[mut->index.load()].get()->first);
//protection.lock();
              //    std::cout << "going to sleep on " << mut->index.load()  << " and " << mut->counter.load() << " with " << mut->id << '\n';
    assert(mut->index.load() > -1);
        tmp2 = true;

    this->protection.unlock();
    this->mutexes[mut->index.load()].get()->second.wait(lck);
    }
    if (!tmp2) {
        protection.unlock();
    }
}

void Turnstile::unlock(Mutex *mut) {
int tmp = -1;
    this->protection.lock();
        mut->counter--;
         //   std::cout << "waking up on " << mut->index.load() << " and " << mut->counter.load() << "with " << mut->id <<  '\n';
            //std::cout << mutexes.size() << '\n';

//    this->protection.unlock();
    if (!mut->index.compare_exchange_strong(tmp, -1)) {
    assert((mut->index.load() > -1));
              std::unique_lock<std::mutex> lck(this->mutexes[mut->index.load()].get()->first);

    this->mutexes[mut->index.load()].get()->second.notify_one();
    }
  //  this->protection.lock();
    if (mut->counter.load() == 1 && mut->index.load() != -1) {
            index_queue.push(mut->index.load());
            if (index_queue.size() > small_factor*max_size && max_size > beginning_size) {
                std::cout << "SMALLING" <<  " " << index_queue.size() << " " << max_size << '\n';
                max_size /= factor;
                std::cout << "NOW " << max_size << '\n';
                //int numberOfElementsToDelete = index_queue.size() - max_size;
             //   std::cout << "HELLO " << numberOfElementsToDelete << '\n';
                for (int i = 0; i < max_size; i++) {
                    int index = index_queue.front();
                    index_queue.pop();
                    std::cout << "deleting " << index << '\n';
                    mutexes.erase(index);
                }
            }
            mut->index.store(-1);
        }
    this->protection.unlock();
}

int Turnstile::give_index() {
    //this->protection.lock();
    if (this->index_queue.empty()) {
        std::cout << "RESIZING" << '\n';
        counter += max_size;
        //this->mutexes.resize(max_size);
        for (unsigned long long int i = counter-max_size; i < counter; i++) {
            index_queue.push(i);
            mutexes[i] = std::unique_ptr<std::pair<std::mutex, std::condition_variable>>(new std::pair<std::mutex, std::condition_variable>());
        }
        max_size *= factor;

    }
    int result = this->index_queue.front();
    this->index_queue.pop();
    //this->protection.unlock();
    return result;
}

void Turnstile::push(int a) {
    this->protection.lock();
    this->index_queue.push(a);
    this->protection.unlock();
}

void Mutex::lock() {
    //int tmp = -1;
   // this->counter++;
    //std::cout << "TEST " << this->id << '\n';
       //    this->counter++;

   int tmp2 = 0;

    if (!this->counter.compare_exchange_strong(tmp2, 1)) {
            Mutex::ts.get()->lock(this);
    }
}

void Mutex::unlock() {
    //this->counter--;
    int tmp2 = 1;
    if (!this->counter.compare_exchange_strong(tmp2, 0)/* && !this->index.compare_exchange_strong(tmp3, -1)*/) {
        Mutex::ts.get()->unlock(this);
}

}


