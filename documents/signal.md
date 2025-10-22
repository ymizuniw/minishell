SIGNALS
       When bash is interactive, in the absence of any traps, it ignores SIGTERM (so that kill 0 does not kill an interactive shell), and SIGINT is
       caught and handled (so that the wait builtin is interruptible).  In all cases, bash ignores SIGQUIT.  If job control is in effect, bash
       ignores SIGTTIN, SIGTTOU, and SIGTSTP.

       Non-builtin commands run by bash have signal handlers set to the values inherited by the shell from its parent.  When job control is not in
       effect, asynchronous commands ignore SIGINT and SIGQUIT in addition to these inherited handlers.  Commands run as a result of command
       substitution ignore the keyboard-generated job control signals SIGTTIN, SIGTTOU, and SIGTSTP.

       The shell exits by default upon receipt of a SIGHUP.  Before exiting, an interactive shell resends the SIGHUP to all jobs, running or stopped.
       Stopped jobs are sent SIGCONT to ensure that they receive the SIGHUP.  To prevent the shell from sending the signal to a particular job, it
       should be removed from the jobs table with the disown builtin (see SHELL BUILTIN COMMANDS below) or marked to not receive SIGHUP using disown
       -h.

       If the huponexit shell option has been set with shopt, bash sends a SIGHUP to all jobs when an interactive login shell exits.

       If bash is waiting for a command to complete and receives a signal for which a trap has been set, the trap will not be executed until the
       command completes.  When bash is waiting for an asynchronous command via the wait builtin, the reception of a signal for which a trap has been
       set will cause the wait builtin to return immediately with an exit status greater than 128, immediately after which the trap is executed.


NAME
     signal – simplified software signal facilities

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <signal.h>

     void (*
     signal(int sig, void (*func)(int));
     )(int);

     or in the equivalent but easier to read typedef'd version:
     typedef void (*sig_t) (int);

     sig_t
     signal(int sig, sig_t func);

DESCRIPTION
     This signal() facility is a simplified interface to the more general sigaction(2) facility.

     Signals allow the manipulation of a process from outside its domain, as well as allowing the
     process to manipulate itself or copies of itself (children).  There are two general types of
     signals: those that cause termination of a process and those that do not.  Signals which cause
     termination of a program might result from an irrecoverable error or might be the result of a
     user at a terminal typing the `interrupt' character.  Signals are used when a process is stopped
     because it wishes to access its control terminal while in the background (see tty(4)).  Signals
     are optionally generated when a process resumes after being stopped, when the status of child
     processes changes, or when input is ready at the control terminal.  Most signals result in the
     termination of the process receiving them, if no action is taken; some signals instead cause the
     process receiving them to be stopped, or are simply discarded if the process has not requested
     otherwise.  Except for the SIGKILL and SIGSTOP signals, the signal() function allows for a
     signal to be caught, to be ignored, or to generate an interrupt.  These signals are defined in
     the file <signal.h>:

            Name             Default Action       Description
     1     SIGHUP           terminate process    terminal line hangup
     2     SIGINT           terminate process    interrupt program
     3     SIGQUIT          create core image    quit program
     4     SIGILL           create core image    illegal instruction
     5     SIGTRAP          create core image    trace trap
     6     SIGABRT          create core image    abort program (formerly SIGIOT)
     7     SIGEMT           create core image    emulate instruction executed
     8     SIGFPE           create core image    floating-point exception
     9     SIGKILL          terminate process    kill program
     10    SIGBUS           create core image    bus error
     11    SIGSEGV          create core image    segmentation violation
     12    SIGSYS           create core image    non-existent system call invoked
     13    SIGPIPE          terminate process    write on a pipe with no reader
     14    SIGALRM          terminate process    real-time timer expired
     15    SIGTERM          terminate process    software termination signal
     16    SIGURG           discard signal       urgent condition present on socket
     17    SIGSTOP          stop process         stop (cannot be caught or ignored)
     18    SIGTSTP          stop process         stop signal generated from keyboard
     19    SIGCONT          discard signal       continue after stop
     20    SIGCHLD          discard signal       child status has changed
     21    SIGTTIN          stop process         background read attempted from control terminal
22    SIGTTOU          stop process         background write attempted to control terminal
     23    SIGIO            discard signal       I/O is possible on a descriptor (see fcntl(2))
     24    SIGXCPU          terminate process    cpu time limit exceeded (see setrlimit(2))
     25    SIGXFSZ          terminate process    file size limit exceeded (see setrlimit(2))
     26    SIGVTALRM        terminate process    virtual time alarm (see setitimer(2))
     27    SIGPROF          terminate process    profiling timer alarm (see setitimer(2))
     28    SIGWINCH         discard signal       Window size change
     29    SIGINFO          discard signal       status request from keyboard
     30    SIGUSR1          terminate process    User defined signal 1
     31    SIGUSR2          terminate process    User defined signal 2

The sig argument specifies which signal was received.  The func procedure allows a user to
     choose the action upon receipt of a signal.  To set the default action of the signal to occur as
     listed above, func should be SIG_DFL.  A SIG_DFL resets the default action.  To ignore the
     signal, func should be SIG_IGN.  This will cause subsequent instances of the signal to be
     ignored and pending instances to be discarded.  If SIG_IGN is not used, further occurrences of
     the signal are automatically blocked and func is called.

     The handled signal is unblocked when the function returns and the process continues from where
     it left off when the signal occurred.  Unlike previous signal facilities, the handler func()
     remains installed after a signal has been delivered.

     For some system calls, if a signal is caught while the call is executing and the call is
     prematurely terminated, the call is automatically restarted.  Any handler installed with
     signal(3) will have the SA_RESTART flag set, meaning that any restartable system call will not
     return on receipt of a signal.  The affected system calls include read(2), write(2), sendto(2),
     recvfrom(2), sendmsg(2), and recvmsg(2) on a communications channel or a low speed device and
     during a ioctl(2) or wait(2).  However, calls that have already committed are not restarted, but
     instead return a partial success (for example, a short read count).  These semantics could be
     changed with siginterrupt(3).

When a process which has installed signal handlers forks, the child process inherits the
     signals.  All caught signals may be reset to their default action by a call to the execve(2)
     function; ignored signals remain ignored.

     If a process explicitly specifies SIG_IGN as the action for the signal SIGCHLD, the system will
     not create zombie processes when children of the calling process exit.  As a consequence, the
     system will discard the exit status from the child processes.  If the calling process
     subsequently issues a call to wait(2) or equivalent, it will block until all of the calling
     process's children terminate, and then return a value of -1 with errno set to ECHILD.

     See sigaction(2) for a list of functions that are considered safe for use in signal handlers.

RETURN VALUES
     The previous action is returned on a successful call.  Otherwise, SIG_ERR is returned and the
     global variable errno is set to indicate the error.

ERRORS
     The signal() function will fail and no action will take place if one of the following occur:

     [EINVAL]           The sig argument is not a valid signal number.

     [EINVAL]           An attempt is made to ignore or supply a handler for SIGKILL or SIGSTOP.



sigaction
SIGACTION(2)                             System Calls Manual                             SIGACTION(2)

NAME
     sigaction – software signal facilities

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <signal.h>

     struct  sigaction {
             union __sigaction_u __sigaction_u;  /* signal handler */
             sigset_t sa_mask;               /* signal mask to apply */
             int     sa_flags;               /* see signal options below */
     };

     union __sigaction_u {
             void    (*__sa_handler)(int);
             void    (*__sa_sigaction)(int, siginfo_t *,
                            void *);
     };
     #define sa_handler      __sigaction_u.__sa_handler
     #define sa_sigaction    __sigaction_u.__sa_sigaction

     int
     sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact);

DESCRIPTION
     The system defines a set of signals that may be delivered to a process.  Signal delivery
     resembles the occurrence of a hardware interrupt: the signal is normally blocked from further
     occurrence, the current process context is saved, and a new one is built.  A process may specify
     a handler to which a signal is delivered, or specify that a signal is to be ignored.  A process
     may also specify that a default action is to be taken by the system when a signal occurs.  A
     signal may also be blocked, in which case its delivery is postponed until it is unblocked.  The
     action to be taken on delivery is determined at the time of delivery.  Normally, signal handlers
     execute on the current stack of the process.  This may be changed, on a per-handler basis, so
     that signals are taken on a special signal stack.

     Signal routines normally execute with the signal that caused their invocation blocked, but other
     signals may yet occur.  A global signal mask defines the set of signals currently blocked from
     delivery to a process.  The signal mask for a process is initialized from that of its parent
     (normally empty).  It may be changed with a sigprocmask(2) call, or when a signal is delivered
     to the process.

When a signal condition arises for a process, the signal is added to a set of signals pending
     for the process.  If the signal is not currently blocked by the process then it is delivered to
     the process.  Signals may be delivered any time a process enters the operating system (e.g.,
     during a system call, page fault or trap, or clock interrupt).  If multiple signals are ready to
     be delivered at the same time, any signals that could be caused by traps are delivered first.
     Additional signals may be processed at the same time, with each appearing to interrupt the
     handlers for the previous signals before their first instructions.  The set of pending signals
     is returned by the sigpending(2) system call.  When a caught signal is delivered, the current
     state of the process is saved, a new signal mask is calculated (as described below), and the
     signal handler is invoked.  The call to the handler is arranged so that if the signal handling
     routine returns normally the process will resume execution in the context from before the
     signal's delivery.  If the process wishes to resume in a different context, then it must arrange
     to restore the previous context itself.

     When a signal is delivered to a process a new signal mask is installed for the duration of the
     process' signal handler (or until a sigprocmask(2) system call is made).  This mask is formed by
     taking the union of the current signal mask set, the signal to be delivered, and the signal mask
     associated with the handler to be invoked.

   The sigaction() system call assigns an action for a signal specified by sig.  If act is non-
     zero, it specifies an action (SIG_DFL, SIG_IGN, or a handler routine) and mask to be used when
     delivering the specified signal.  If oact is non-zero, the previous handling information for the
     signal is returned to the user.

     Once a signal handler is installed, it normally remains installed until another sigaction()
     system call is made, or an execve(2) is performed.  A signal-specific default action may be
     reset by setting sa_handler to SIG_DFL.  The defaults are process termination, possibly with
     core dump; no action; stopping the process; or continuing the process.  See the signal list
     below for each signal's default action.  If sa_handler is SIG_DFL, the default action for the
     signal is to discard the signal, and if a signal is pending, the pending signal is discarded
     even if the signal is masked.  If sa_handler is set to SIG_IGN current and pending instances of
     the signal are ignored and discarded.

 Options may be specified by setting sa_flags.  The meaning of the various bits is as follows:

           SA_NOCLDSTOP    If this bit is set when installing a catching function for the SIGCHLD
                           signal, the SIGCHLD signal will be generated only when a child process
                           exits, not when a child process stops.

           SA_NOCLDWAIT    If this bit is set when calling sigaction() for the SIGCHLD signal, the
                           system will not create zombie processes when children of the calling
                           process exit.  If the calling process subsequently issues a wait(2) (or
                           equivalent), it blocks until all of the calling process's child processes
                           terminate, and then returns a value of -1 with errno set to ECHILD.

           SA_ONSTACK      If this bit is set, the system will deliver the signal to the process on a
                           signal stack, specified with sigaltstack(2).

           SA_NODEFER      If this bit is set, further occurrences of the delivered signal are not
                           masked during the execution of the handler.

           SA_RESETHAND    If this bit is set, the handler is reset back to SIG_DFL at the moment the
                           signal is delivered.

           SA_RESTART      See paragraph below.

         SA_SIGINFO      If this bit is set, the handler function is assumed to be pointed to by
                           the sa_sigaction member of struct sigaction and should match the prototype
                           shown above or as below in EXAMPLES.  This bit should not be set when
                           assigning SIG_DFL or SIG_IGN.

     If a signal is caught during the system calls listed below, the call may be forced to terminate
     with the error EINTR, the call may return with a data transfer shorter than requested, or the
     call may be restarted.  Restart of pending calls is requested by setting the SA_RESTART bit in
     sa_flags.  The affected system calls include open(2), read(2), write(2), sendto(2), recvfrom(2),
     sendmsg(2) and recvmsg(2) on a communications channel or a slow device (such as a terminal, but
     not a regular file) and during a wait(2) or ioctl(2).  However, calls that have already
     committed are not restarted, but instead return a partial success (for example, a short read
     count).

  After a fork(2) or vfork(2) all signals, the signal mask, the signal stack, and the
     restart/interrupt flags are inherited by the child.

     The execve(2) system call reinstates the default action for all signals which were caught and
     resets all signals to be caught on the user stack.  Ignored signals remain ignored; the signal
     mask remains the same; signals that restart pending system calls continue to do so.

     The following is a list of all signals with names as in the include file ⟨signal.h⟩:
  NAME            Default Action          Description
     SIGHUP          terminate process       terminal line hangup
     SIGINT          terminate process       interrupt program
     SIGQUIT         create core image       quit program
     SIGILL          create core image       illegal instruction
     SIGTRAP         create core image       trace trap
     SIGABRT         create core image       abort(3) call (formerly SIGIOT)
     SIGEMT          create core image       emulate instruction executed
     SIGFPE          create core image       floating-point exception
     SIGKILL         terminate process       kill program
     SIGBUS          create core image       bus error
     SIGSEGV         create core image       segmentation violation
     SIGSYS          create core image       non-existent system call invoked
     SIGPIPE         terminate process       write on a pipe with no reader
     SIGALRM         terminate process       real-time timer expired
     SIGTERM         terminate process       software termination signal
     SIGURG          discard signal          urgent condition present on socket
     SIGSTOP         stop process            stop (cannot be caught or ignored)
     SIGTSTP         stop process            stop signal generated from keyboard
     SIGCONT         discard signal          continue after stop
     SIGCHLD         discard signal          child status has changed
     SIGTTIN         stop process            background read attempted from control terminal
SIGTTOU         stop process            background write attempted to control terminal
     SIGIO           discard signal          I/O is possible on a descriptor (see fcntl(2))
     SIGXCPU         terminate process       cpu time limit exceeded (see setrlimit(2))
     SIGXFSZ         terminate process       file size limit exceeded (see setrlimit(2))
     SIGVTALRM       terminate process       virtual time alarm (see setitimer(2))
     SIGPROF         terminate process       profiling timer alarm (see setitimer(2))
     SIGWINCH        discard signal          Window size change
     SIGINFO         discard signal          status request from keyboard
     SIGUSR1         terminate process       User defined signal 1
     SIGUSR2         terminate process       User defined signal 2

NOTE
     The sa_mask field specified in act is not allowed to block SIGKILL or SIGSTOP.  Any attempt to
     do so will be silently ignored.

     The following functions are either reentrant or not interruptible by signals and are async-
     signal safe.  Therefore applications may invoke them, without restriction, from signal-catching
     functions:

     Base Interfaces:

     _exit(), access(), alarm(), cfgetispeed(), cfgetospeed(), cfsetispeed(), cfsetospeed(), chdir(),
     chmod(), chown(), close(), creat(), dup(), dup2(), execle(), execve(), fcntl(), fork(),
     fpathconf(), fstat(), fsync(), getegid(), geteuid(), getgid(), getgroups(), getpgrp(), getpid(),
     getppid(), getuid(), kill(), link(), lseek(), mkdir(), mkfifo(), open(), pathconf(), pause(),
     pipe(), raise(), read(), rename(), rmdir(), setgid(), setpgid(), setsid(), setuid(),
     sigaction(), sigaddset(), sigdelset(), sigemptyset(), sigfillset(), sigismember(), signal(),
     sigpending(), sigprocmask(), sigsuspend(), sleep(), stat(), sysconf(), tcdrain(), tcflow(),
     tcflush(), tcgetattr(), tcgetpgrp(), tcsendbreak(), tcsetattr(), tcsetpgrp(), time(), times(),
     umask(), uname(), unlink(), utime(), wait(), waitpid(), write().

    All functions not in the above lists are considered to be unsafe with respect to signals.  That
     is to say, the behaviour of such functions when called from a signal handler is undefined.  In
     general though, signal handlers should do little more than set a flag; most other actions are
     not safe.

...
     Also, it is good practice to make a copy of the global variable errno and restore it before
     returning from the signal handler.  This protects against the side effect of errno being set by
     functions called from inside the signal handler.

RETURN VALUES
     The sigaction() function returns the value 0 if successful; otherwise the value -1 is returned
     and the global variable errno is set to indicate the error.

 The handler function should match the SA_SIGINFO prototype if the SA_SIGINFO bit is set in
     flags.  It then should be pointed to by the sa_sigaction member of struct sigaction.  Note that
     you should not assign SIG_DFL or SIG_IGN this way.

     If the SA_SIGINFO flag is not set, the handler function should match either the ANSI C or
     traditional BSD prototype and be pointed to by the sa_handler member of struct sigaction.  In
     practice, FreeBSD always sends the three arguments of the latter and since the ANSI C prototype
     is a subset, both will work.  The sa_handler member declaration in FreeBSD include files is that
     of ANSI C (as required by POSIX), so a function pointer of a BSD-style function needs to be
     casted to compile without warning.  The traditional BSD style is not portable and since its
     capabilities are a full subset of a SA_SIGINFO handler, its use is deprecated.

     The sig argument is the signal number, one of the SIG... values from <signal.h>.

     The code argument of the BSD-style handler and the si_code member of the info argument to a
     SA_SIGINFO handler contain a numeric code explaining the cause of the signal, usually one of the
     SI_... values from <sys/signal.h> or codes specific to a signal, i.e. one of the FPE_... values
     for SIGFPE.

ERRORS
     The sigaction() system call will fail and no new signal handler will be installed if one of the
     following occurs:

     [EFAULT]           Either act or oact points to memory that is not a valid part of the process
                        address space.

     [EINVAL]           The sig argument is not a valid signal number.

     [EINVAL]           An attempt is made to ignore or supply a handler for SIGKILL or SIGSTOP.

     [EINVAL]           An attempt was made to set the action to SIG_DFL for a signal that cannot be
                        caught or ignored (or both).

STANDARDS
     The sigaction() system call is expected to conform to IEEE Std 1003.1-1990 (“POSIX.1”).  The
     SA_ONSTACK and SA_RESTART flags are Berkeley extensions, as are the signals, SIGTRAP, SIGEMT,
     SIGBUS, SIGSYS, SIGURG, SIGIO, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, and SIGINFO.
     Those signals are available on most BSD-derived systems.  The SA_NODEFER and SA_RESETHAND flags
     are intended for backwards compatibility with other operating systems.  The SA_NOCLDSTOP, and
     SA_NOCLDWAIT flags are featuring options commonly found in other operating systems.



sigemptyset

NAME
     sigaddset, sigdelset, sigemptyset, sigfillset, sigismember – manipulate signal sets

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <signal.h>

     int
     sigaddset(sigset_t *set, int signo);

     int
     sigdelset(sigset_t *set, int signo);

     int
     sigemptyset(sigset_t *set);

     int
     sigfillset(sigset_t *set);
     int
     sigismember(const sigset_t *set, int signo);

DESCRIPTION
     These functions manipulate signal sets, stored in a sigset_t.  Either sigemptyset() or
     sigfillset() must be called for every object of type sigset_t before any other use of the
     object.

     The sigemptyset() function initializes a signal set to be empty.

     The sigfillset() function initializes a signal set to contain all signals.

     The sigaddset() function adds the specified signal signo to the signal set.

     The sigdelset() function deletes the specified signal signo from the signal set.

     The sigismember() function returns whether a specified signal signo is contained in the signal
     set.

     These functions are provided as macros in the include file <signal.h>.  Actual functions are
     available if their names are undefined (with #undef name).

RETURN VALUES
     The sigismember() function returns 1 if the signal is a member of the set, 0 otherwise.  The
     other functions return 0.

ERRORS
     Currently, no errors are detected.

SEE ALSO
     kill(2), sigaction(2), sigsuspend(2)

STANDARDS
     These functions are defined by IEEE Std 1003.1-1988 (“POSIX.1”).

NAME
     sigaddset, sigdelset, sigemptyset, sigfillset, sigismember – manipulate signal sets

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <signal.h>

     int
     sigaddset(sigset_t *set, int signo);

     int
     sigdelset(sigset_t *set, int signo);

     int
     sigemptyset(sigset_t *set);

     int
     sigfillset(sigset_t *set);

     int
     sigismember(const sigset_t *set, int signo);

DESCRIPTION
     These functions manipulate signal sets, stored in a sigset_t.  Either sigemptyset() or
     sigfillset() must be called for every object of type sigset_t before any other use of the
     object.

     The sigemptyset() function initializes a signal set to be empty.

     The sigfillset() function initializes a signal set to contain all signals.

     The sigaddset() function adds the specified signal signo to the signal set.

     The sigdelset() function deletes the specified signal signo from the signal set.

     The sigismember() function returns whether a specified signal signo is contained in the signal set.


     These functions are provided as macros in the include file <signal.h>.  Actual functions are
     available if their names are undefined (with #undef name).

RETURN VALUES
     The sigismember() function returns 1 if the signal is a member of the set, 0 otherwise.  The
     other functions return 0.

ERRORS
     Currently, no errors are detected.

     
kill
KILL(1)                                General Commands Manual                                KILL(1)

NAME
     kill – terminate or signal a process

SYNOPSIS
     kill [-s signal_name] pid ...
     kill -l [exit_status]
     kill -signal_name pid ...
     kill -signal_number pid ...

DESCRIPTION
     The kill utility sends a signal to the processes specified by the pid operands.

     Only the super-user may send signals to other users' processes.

     The options are as follows:

     -s signal_name
             A symbolic signal name specifying the signal to be sent instead of the default TERM.

     -l [exit_status]
   -signal_name
             A symbolic signal name specifying the signal to be sent instead of the default TERM.

     -signal_number
             A non-negative decimal integer, specifying the signal to be sent instead of the default
             TERM.

     The following PIDs have special meanings:

     -1      If superuser, broadcast the signal to all processes; otherwise broadcast to all
             processes belonging to the user.

     Some of the more commonly used signals:

     1       HUP (hang up)
     2       INT (interrupt)
     3       QUIT (quit)
     6       ABRT (abort)
     9       KILL (non-catchable, non-ignorable kill)
     14      ALRM (alarm clock)
     15      TERM (software termination signal)

     Some shells may provide a builtin kill command which is similar or identical to this utility.
     Consult the builtin(1) manual page.

EXIT STATUS
     The kill utility exits 0 on success, and >0 if an error occurs.

EXAMPLES
     Terminate the processes with PIDs 142 and 157:

           kill 142 157

     Send the hangup signal (SIGHUP) to the process with PID 507:

           kill -s HUP 507

     Terminate the process group with PGID 117:

           kill -- -117
