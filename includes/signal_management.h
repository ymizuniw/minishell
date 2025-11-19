/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:47 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/19 21:36:20 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_MANAGEMENT_H
# define SIGNAL_MANAGEMENT_H

# include "minishell_types.h"

extern volatile sig_atomic_t	g_signum;

int								signal_initializer(bool interactive);
int								set_sig_dfl(void);
int								handle_child(int *last_exit_status, pid_t pid);
void							signal_handler(int signum);

#endif
