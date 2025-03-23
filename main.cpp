#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

constexpr int THINK_TIME               = 100;
constexpr int WAIT_TIME                = 50;
constexpr int EAT_TIME                 = 200;
constexpr int SECOND_CHOPSTICK_TIMEOUT = 10;

constexpr auto THINK_MS     = std::chrono::milliseconds(THINK_TIME);
constexpr auto WAIT_MS      = std::chrono::milliseconds(WAIT_TIME);
constexpr auto EAT_MS       = std::chrono::milliseconds(EAT_TIME);
constexpr auto SECOND_CS_MS = std::chrono::milliseconds(SECOND_CHOPSTICK_TIMEOUT);

#define TIME_NOW std::chrono::high_resolution_clock::now()

#define DUR_TO_MS(X) std::chrono::duration_cast<std::chrono::milliseconds>((X))

#define DEBUG_LOGS 0

#ifdef DEBUG_LOGS
  #define DEBUG(...) fprintf(__VA_ARGS__)
#elif
  #define DEBUG(...) (void)0
#endif

class chopstick
{
public:

  chopstick()
  {
  }

  chopstick(chopstick&& other)
  {
    std::unique_lock lck(this->mtx);
    std::unique_lock lck_other(other.mtx);

    this->avail = other.avail;
  }

  bool try_get()
  {
    std::unique_lock lck (this->mtx, std::try_to_lock);

    const bool acquired = lck.owns_lock();

    if (acquired == false)
      return false;

    if (this->avail == false)
      return false;

    this->avail = false;
    return true;
  }

  void put()
  {
    std::unique_lock lck(this->mtx);
    this->avail = true;
  }

private:
  bool avail = true;
  std::mutex mtx;
};

chopstick* chopsticks;

struct phil
{
private:
  void think()
  {
    fprintf(stderr, "Philosopher %d is thinking!\n", this->id);
    std::this_thread::sleep_for(THINK_MS);
  }

  void eat()
  {
    fprintf(stderr, "Philosopher %d is eatting!\n", this->id);
    std::this_thread::sleep_for(EAT_MS);
  }

  void put_chopsticks()
  {
    fprintf(stderr, "Philosopher %d is putting chopsticks!\n", this->id);

    chopsticks[chop1_id].put();
    chopsticks[chop2_id].put();
  }

  bool get_first_chopstick()
  {
    fprintf(stderr, "Philosopher %d is trying to get 1st chopstick!\n", this->id);

    if(chopsticks[chop1_id].try_get() == false)
      return false;

    return true;
  }

  bool get_second_chopstick()
  {
    std::chrono::duration<double, std::micro> time_passed(0.0);

    do
    {
      fprintf(stderr, "Philosopher %d is trying to get 2nd chopstick!\n", this->id);

      auto start = TIME_NOW;

      if(chopsticks[chop2_id].try_get() == false)
      {
        auto end = TIME_NOW;
        std::this_thread::sleep_for(SECOND_CS_MS);
        time_passed += (end - start);
      }
      else
      {
        return true;
      }

    } while(WAIT_MS > DUR_TO_MS(time_passed));

    // timeout

    fprintf(stderr, "Philosopher %d could not get 2nd chopstick!\n", this->id);

    chopsticks[chop1_id].put();

    return false;
  }

  void work()
  {
    while (true)
    {
      this->think();

      if (this->get_first_chopstick() == false)
        continue;

      if (get_second_chopstick() == false)
        continue;

      this->eat();

      this->put_chopsticks();
    }
  }

public:

  void run()
  {
    this->th = std::thread([this]() { this->work(); });
  }

  void join()
  {
    if (this->th.joinable())
      this->th.join();
  }

  void set_chopsticks_num(int i)
  {
    this->chopsticks_num = i;
  }

  void set_id(int i)
  {
    this->id = i;
    this->chop1_id = i;
    this->chop2_id = (i + 1) % this->chopsticks_num;

    DEBUG(stderr, "id = %d, chop1 = %d, chop2 = %d\n", this->id, this->chop1_id, this->chop2_id);
  }

private:
  int id = 0;

  std::thread th;

  int chop1_id = 0;
  int chop2_id = 0;
  int chopsticks_num = 0;
};

std::vector<phil> phils;

void usage(char* p)
{
  fprintf(stderr, "Usage: %s <number of philosophers>\n", p);
  fprintf(stderr, "Example: %s 5\n", p);
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    usage(argv[0]);
    return -1;
  }

  const int num = std::stoi(argv[1]);

  if (num <= 0)
  {
    usage(argv[0]);
    fprintf(stderr, "Number cannot be <= 0!\n");
    return -1;
  }

  phils.resize(num);
  chopsticks = new chopstick[num];

  // initialize and run threads
  for (int i = 0; auto& p : phils)
  {
    p.set_chopsticks_num(num);
    p.set_id(i++);
    p.run();
  }

  for (auto& p : phils)
    p.join();

  delete[] chopsticks;

  return 0;
}
