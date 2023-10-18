#include "kernel/types.h"
#include "user/user.h"

static void print_help_message() {
  printf("this function toggles dmesg logging for given eventclass.\n"
         "how to use:\n"
         "Arguments:\n"
         "\t\tPossible values:\n"
         "\t\t- '0' or 'interrupt' - device interrupts\n"
         "\t\t- '1' or 'procswitch' - process switches\n"
         "\t\t- '2' or 'syscall' - system calls\n"
         "\t\t- 'all' - all of that\n"
         "\n"
         "\tAction - what to do with class.\n"
         "\t\tPossible values:\n"
         "\t\t- 'disable' - disable logging\n"
         "\t\t- 'enable' - enable logging permanently\n"
         "\t\t- 'enable x', enable logging for x ticks.\n"
         );
}

static int str_equal(const char *l, const char *r) {
  return strcmp(l, r) == 0;
}

typedef enum {
  EC_INTERRUPT = 0,
  EC_PROCSWITCH = 1,
  EC_SYSCALL = 2,
  EC_SIZE,
  EC_ALL,
  EC_INVALID,
} eventclass_t;

#define EVENTCLASS_ALL 4

static eventclass_t read_eventclass(const char *as_str) {
  if (str_equal(as_str, "0") || str_equal(as_str, "interrupt"))
    return EC_INTERRUPT;
  if (str_equal(as_str, "1") || str_equal(as_str, "procswitch"))
    return EC_PROCSWITCH;
  if (str_equal(as_str, "2") || str_equal(as_str, "syscall"))
    return EC_SYSCALL;
  if (str_equal(as_str, "all"))
    return EC_ALL;
  return EC_INVALID;
}

static int read_duration_ticks(int action_argc, const char **action_argv, int *duration_ticks) {
  if (action_argc <= 0 || action_argc > 2)
    return 1; //error
  const char *action = action_argv[0];
  if (str_equal(action, "disable")) {
    if (action_argc > 1)
      return 1;
    *duration_ticks = -1;
    return 0; //all good
  }
  if (str_equal(action, "enable")) {
    if (action_argc == 1) {
      *duration_ticks = 0;
      return 0;
    }
    *duration_ticks = atoi(action_argv[1]);
    return !(*duration_ticks > 0);
  }
  return 1; //also error
}

int main(int argc, const char **argv) {
  if (argc < 3)
    goto print_help_and_exit;
  eventclass_t eventclass = read_eventclass(argv[1]);
  if (eventclass == EC_INVALID)
    goto print_help_and_exit;
  int duration_ticks;
  if (read_duration_ticks(argc - 2, argv + 2, &duration_ticks)) //return 1 is error
    goto print_help_and_exit;

  if (eventclass == EVENTCLASS_ALL) {
    for (int class = 0; class < EC_SIZE; ++class)
      dmesg_log_toggle(class, duration_ticks);
  } else {
    dmesg_log_toggle(eventclass, duration_ticks);
  }
  exit(0);

print_help_and_exit:
  print_help_message();
  exit(1);
}