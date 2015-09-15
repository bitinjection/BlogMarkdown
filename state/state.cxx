#include <iostream>
#include <unistd.h>
#include <time.h>

class Heroine;

class HeroineState
{
public:
  virtual HeroineState* update_state(Heroine& heroine) = 0;
};



class Jumping : public HeroineState
{
  virtual HeroineState* update_state(Heroine& heroine);
};

class Walking : public HeroineState
{
  virtual HeroineState* update_state(Heroine& heroine)
  {
    std::cout << "walking\n";

    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    if(!(time.tv_sec % 3))
      return new Jumping();

    return this;
  }
};

HeroineState* Jumping::update_state(Heroine& heroine)
{
  std::cout << "Jumping\n";

  timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  if(!(time.tv_sec % 7))
    return new Walking();
  return this;
}

class Heroine
{
public:
  Heroine() { this->state = new Walking(); }
  virtual ~Heroine() {};
  virtual void update() {
    state = state->update_state(*this);
  }

private:
  HeroineState* state;
};

int main()
{
  Heroine heroine;

  while(true)
  {
    heroine.update();

    sleep(1);
  }
}
