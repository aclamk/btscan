#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#include <stdio.h>
#include <thread>
#include <signal.h>
#include <unistd.h>

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BT_BUF_SIZE 100

void
myfunc3(void)
{
  int j, nptrs;
  void *buffer[BT_BUF_SIZE];
  char **strings;

  nptrs = backtrace(buffer, BT_BUF_SIZE);
  printf("backtrace() returned %d addresses\n", nptrs);

  /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
          would produce similar output to the following: */

  strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }

  for (j = 0; j < nptrs; j++)
    printf("%s\n", strings[j]);

  free(strings);
}

int getpid(const char* pipe_name)
{
  int pid = -1;
  ifstream myfile( pipe_name );
  if (myfile)  // same as: if (myfile.good())
  {
    string line;
    if( getline( myfile, line ))  // same as: while (getline( myfile, line ).good())
    {
      pid = std::stoi(line);
    }
    myfile.close();
  }
  return pid;
}

static void process_SIG_handler(int signum, siginfo_t *info, void *context)
{
  myfunc3();
}

int install_SIG_handlers(const int signum)
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = process_SIG_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(signum, &act, NULL))
        return errno;
    return 0;
}

#if 0
void func(std::string str, int& x);
void do_something();
int main() {
  std::string str = "Test";
  int x = 5;
  std::thread t{func, str, std::ref(x)};
  do_something();
  t.join();
}
#endif

void btscan()
{
  sleep(1);
  install_SIG_handlers(SIGRTMAX-3);
  while (true) {
    int pid = getpid ("name" );
    std::cout << "PID=" << pid << std::endl;
    while(true)
    {
      kill(pid, SIGRTMAX-3);
      sleep(1);
    }
  }
}

__attribute__((constructor))
void init()
{
  static std::thread worker {btscan};
  //std::cout << "INIT" << std::endl;
}



int main(int argc, char** argv)
{

}
