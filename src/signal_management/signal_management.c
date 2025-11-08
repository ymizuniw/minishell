#include "../../includes/minishell.h"

volatile sig_atomic_t	g_signum = 0;

void	signal_handler(int signum)
{
	if (signum == SIGINT)
		g_signum = SIGINT;
}

int	set_sig_dfl(void)
{
	struct sigaction	sact;

	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = SIG_DFL;
	sigaction(SIGTERM, &sact, NULL);
	sigaction(SIGQUIT, &sact, NULL);
	sigaction(SIGINT, &sact, NULL);
	return (0);
}

int	signal_initializer(bool interactive)
{
	struct sigaction	sact;

	g_signum = 0;
	if (interactive == true)
	{
		sigemptyset(&sact.sa_mask);
		sact.sa_handler = SIG_IGN;
		sact.sa_flags = 0;
		sigaction(SIGTERM, &sact, NULL);
		sigaction(SIGQUIT, &sact, NULL);
		// sact.sa_flags = SA_RESTART;
		sact.sa_flags = 0;
		sact.sa_handler = signal_handler;
		sigaction(SIGINT, &sact, NULL);
		return (0);
	}
	set_sig_dfl();
	return (0);
}

int	handle_child(int *last_exit_status, pid_t pid)
{
	int	local_status;

	local_status = 0;
	if (waitpid(pid, &local_status, 0) == -1)
	{
		*last_exit_status = 1;
		return (-1);
	}
	if (WIFSIGNALED(local_status))
	{
		*last_exit_status = 128 + WTERMSIG(local_status);
		if (WTERMSIG(local_status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	else if (WIFEXITED(local_status))
		*last_exit_status = WEXITSTATUS(local_status);
	else
		*last_exit_status = EXIT_FAILURE;
	return (0);
}
