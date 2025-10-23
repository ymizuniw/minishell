#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t g_recept_signal = 0;

static void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_recept_signal = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();		
	}
}

int	signal_initializer(int *g_set)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_event;
	if (sigaction(SIGINT, &sa, NULL))
	{
		perror("minishell: sigaction");
		return (-1);
	}
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL))
	{
		perror("minishell: sigaction");
		return (-1);
	}
	return (1);
}

int handle_child(int *last_exit_status, pid_t pid)
{
	int local_status = 0;
	if (waitpid(pid, &local_status, 0)==-1)
	{
		perror("waitpid");
		*last_exit_status = 1;
		return -1;
	}
	if (WIFEXITED(local_status))
		*last_exit_status = WEXITSTATUS(local_status);
	else if (WIFSIGNALED(local_status))
	{
		*last_exit_status  = 128 + WTERMSIG(local_status);
		if (WTERMSIG(local_status)==SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
	}
	return 0;
}

//Reference for child signal exit value.
// void		status_child(void)
// {
// 	if (WIFEXITED(g_pid))
// 		g_status = WEXITSTATUS(g_pid);
// 	if (WIFSIGNALED(g_pid))
// 	{
// 		g_status = WTERMSIG(g_pid);
// 		if (g_status != 131)
// 			g_status += 128;
// 	}
// }
