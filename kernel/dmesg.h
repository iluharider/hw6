void dmesg_init();

enum dmesg_log_eventclass {
  DMESG_LOG_INTERRUPT = 0,
  DMESG_LOG_PROCSWITCH = 1,
  DMESG_LOG_SYSCALL = 2,
  DMESG_LOG_EVENTCLASS_COUNT, //the number of eventclasses
};

//Log message if logging is enabled for eventclass.
void dmesg_log(enum dmesg_log_eventclass eventclass, const char *format, ...);

//Tumbler for logging for eventclass.
//If duration_ticks > 0, logging is turned on only for duration_ticks
//If duartion_ticks = 0, logging is enabled permanently.
//If duration_ticks < 0, logging is turned off.
void dmesg_log_eventclass_toggle(enum dmesg_log_eventclass eventclass, int duration_ticks);

//Add a message to diagnostic messages buffer.
//called only by kernel.
void pr_msg(const char *format, ...);