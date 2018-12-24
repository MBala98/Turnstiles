#ifndef SRC_TURNSTILE_H_
#define SRC_TURNSTILE_H_

#include <type_traits>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <unordered_map>

class Mutex;

class Turnstile {
    public:
        unsigned long long int counter;
        Turnstile();
        void lock(Mutex *mut);
        void unlock(Mutex *mut);
        int give_index();
        void grow_queue(int lower_boundary, int upper_boundary);
        void push(int a);
        /*int last_free;
        int max_size;
        std::mutex *mutexes;*/
        double small_factor;
        int max_size;
        int factor;
        std::mutex protection;
        std::queue<int> index_queue;
      //  std::vector<std::unique_ptr<std::mutex>> test;
        std::unordered_map<unsigned long long int, std::unique_ptr<std::pair<std::mutex, std::condition_variable>>> mutexes;
};

class Mutex {
 public:
 void show();
  Mutex();
  //~Mutex();
  Mutex(const Mutex&) = delete;
    int id;
  void lock();    // NOLINT
  void unlock(); // NOLINT

 // private:
    std::atomic<int> index;
    std::atomic<int> counter;
    static std::unique_ptr<Turnstile> ts;

};

#endif  // SRC_TURNSTILE_H_

