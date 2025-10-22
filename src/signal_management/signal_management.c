#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	sig_int_event(int signum)
{
    (void)signum;  // Suppress unused parameter warning
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	sig_term_event(int signum)
{
    (void)signum;  // Suppress unused parameter warning
	printf("\nTerminating...\n");
	clear_history();
	exit(0);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
		signal(signum, sig_int_event);
	else if (signum == SIGTERM)
		signal(signum, sig_term_event);
}

void	signal_initializer(int *g_set)
{
	sigemptyset(g_set);
	sigaddset(g_set, SIGINT);
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