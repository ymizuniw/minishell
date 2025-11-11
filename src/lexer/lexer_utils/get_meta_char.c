#include "../../../includes/minishell.h"

// Check if character is a metacharacter and return its type
t_metachar	is_meta_char(int c)
{
	if (c == '&')
		return (MT_AND);
	if (c == '|')
		return (MT_OR);
	if (c == '(')
		return (MT_LPAREN);
	if (c == ')')
		return (MT_RPAREN);
	if (c == '<')
		return (MT_LESS);
	if (c == '>')
		return (MT_GREAT);
	if (c == ' ')
		return (MT_SPACE);
	if (c == '\t')
		return (MT_TAB);
	if (c == '\n')
		return (MT_NEWLINE);
	return (MT_OTHER);
}
