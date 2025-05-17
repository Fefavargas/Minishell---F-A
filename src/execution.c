/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/17 22:26:41 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	struct stat	path_stat;

	if (!path || ft_strlen(path) == 0)
		return (error_msg("", "", ": command not found\n", 127));
	if ((path[0] == '.' && path[1] == '/') || path[0] == '/')
	{
		if (stat(path, &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				return (error_msg("", path, ": Is a directory\n", 126));
			else if (access(path, X_OK) == -1)
				return (error_msg("", path, ": Permission denied\n", 126));
		}
		return (error_msg("", path, ": No such file or directory\n", 127));
	}
	return (error_msg("", path, ": command not found\n", 127));
}

int	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;

	path = get_path_bin(shell->env, exec->cmd);
	if (!path)
		path = exec->cmd;
	execve(path, exec->args, shell->arr_env);
	exit(error_message(path));
}

// void	wait_fork(t_mini *shell, t_cmd *cmd)
// {
// 	int		status;
// 	size_t	i;
// 	int		last_cmd_idx;

// 	status = 0;
// 	i = 0;
// 	while (i <= cmd->n_pipes && g_sig.sigchld != 0)
// 	{
// 		if (cmd->arr_pid[i] != 0)
// 		{
// 			last_cmd_idx = i;
// 			waitpid(cmd->arr_pid[i], &status, 0);
// 		}
// 		i++;
// 	}
// 	if (last_cmd_idx != -1)
// 	{
// 		if (WIFEXITED(status))
// 			shell->exit_code = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == SIGPIPE)
// 				ft_putstr_fd("", STDERR_FILENO);
// 			else if (WTERMSIG(status) == SIGQUIT)
// 				ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
// 			shell->exit_code = (128 + WTERMSIG(status));
// 		}
// 	}
// 	g_sig.sigchld = 0;
// 	signal(SIGINT, signal_int);
// 	signal(SIGQUIT, SIG_IGN);
// }

void	wait_fork(t_mini *shell, t_cmd *cmd)
{
	int		status = 0;
	size_t	i = 0;
	int		last_cmd_idx = -1;

	if (g_sig.sigchld != 0)
	{
		while (i <= cmd->n_pipes)
		{
			if (cmd->arr_pid[i] != 0)
			{
				last_cmd_idx = i;
				waitpid(cmd->arr_pid[i], &status, 0);
			}
			i++;
		}
		if (last_cmd_idx != -1)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGPIPE)
					ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
				else if (WTERMSIG(status) == SIGQUIT)
					ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
				shell->exit_code = (128 + WTERMSIG(status));
			}
		}
	}
	g_sig.sigchld = 0;
	signal(SIGINT, signal_int);
	signal(SIGQUIT, SIG_IGN);
}


// void	execute(t_mini *shell, t_cmd *cmd)
// {
// 	t_exec_cmd	*current;
// 	int			i;
// 	bool		ret;

// 	current = cmd->execcmd;
// 	i = 0;
// 	while (current)
// 	{
// 		if (!ft_strcmp(current->cmd, "exit"))
// 			exec_builtin(shell, current);
// 		// else if (!ft_strcmp(current->cmd, "echo"))
// 		// 	exec_builtin(shell, current);
// 		if (current->execution)
// 		{
// 			g_sig.sigchld = fork();
// 			if (g_sig.sigchld == -1)
// 				return ;
// 			if (g_sig.sigchld == 0)
// 			{
// 				prepare_chld(shell, current, cmd);
// 				if (current->args && current->args[0] && is_builtin(current->args[0]))
// 				{
// 					ret = exec_builtin(shell, current);
// 					//free_shell(shell, current);
// 					exit(ret);
// 				}
// 				else
// 					exec_binary(shell, current);
// 			}
// 			else
// 				prepare_parent(&(cmd->arr_pid[i++]), current);
// 		}
// 		else if (shell->exit_code != 130)
// 			shell->exit_code = 1;
// 		current = current->next;
// 	}
// 	free_exec_cmd(cmd->execcmd);
// 	cmd->execcmd = NULL; 
// 	wait_fork(shell, cmd);
// }

void	execute(t_mini *shell, t_cmd *cmd)
{
	t_exec_cmd *exec = cmd->execcmd;
	t_exec_cmd *last_pipeline_cmd = NULL;
	int			pid_arr_idx = 0;
	int			last_cmd_final_status = 0;
	bool		is_last_cmd_status_determined = false;

	// Track last command in pipeline for exit code logic
	if (exec)
	{
		last_pipeline_cmd = exec;
		while (last_pipeline_cmd->next)
			last_pipeline_cmd = last_pipeline_cmd->next;
	}

	while (exec)
	{
		bool is_last_cmd_in_pipeline = (exec == last_pipeline_cmd);

		// Special handling for `exit` with no pipes
		if (!ft_strcmp(exec->cmd, "exit") && cmd->n_pipes == 0 && is_builtin("exit"))
		{
			if (exec->execution)
				exec_builtin(shell, exec); // This will terminate the shell
			else { // Redirection error for exit command (should have set shell->exit_code)
				if (is_last_cmd_in_pipeline) {
					last_cmd_final_status = shell->exit_code; // Capture error code
					is_last_cmd_status_determined = true;
				}
			}
		}
		else if (exec->execution)
		{
			if (exec->args && exec->args[0] && is_builtin(exec->args[0]))
			{
				// Execute builtins directly (in parent)
				int saved_stdin = -1;
				int saved_stdout = -1;

				if (exec->fdin != STDIN_FILENO) {
					saved_stdin = dup(STDIN_FILENO);
					if (saved_stdin == -1)
						perror("minishell: dup failed for saved_stdin");
				}
				if (exec->fdout != STDOUT_FILENO) {
					saved_stdout = dup(STDOUT_FILENO);
					if (saved_stdout == -1)
						perror("minishell: dup failed for saved_stdout");
				}
				prepare_fd(exec);
				shell->exit_code = exec_builtin(shell, exec);

				if (saved_stdin != -1) {
					ft_close(STDIN_FILENO); 
					if (dup2(saved_stdin, STDIN_FILENO) == -1)
						perror("minishell: dup2 failed for restoring stdin");
					ft_close(saved_stdin);
				}
				if (saved_stdout != -1) {
					ft_close(STDOUT_FILENO); 
					if (dup2(saved_stdout, STDOUT_FILENO) == -1)
						perror("minishell: dup2 failed for restoring stdout");
					ft_close(saved_stdout);
				}
				if (is_last_cmd_in_pipeline) {
					last_cmd_final_status = shell->exit_code;
					is_last_cmd_status_determined = true;
				}
			}
			else
			{
				// Fork and exec external command
				g_sig.sigchld = fork();
				if (g_sig.sigchld == -1)
					error_msg("minishell: fork: ", strerror(errno), "", 1);
				else if (g_sig.sigchld == 0)
				{
					prepare_fd(exec);
					signal_chld();
					close_cmd(cmd);
					exec_binary(shell, exec);
				}
				else
					prepare_parent(&(cmd->arr_pid[pid_arr_idx++]), exec);
			}
		}
		else if (shell->exit_code != 130) // exec->execution is false, and not due to SIGINT in heredoc
		{
			// This command (exec) is not executed due to a previous error (e.g. redirection).
			// The redirection handler should have set shell->exit_code (e.g., to 1).
			// If not, this line ensures it's at least 1.
			shell->exit_code = 1; 
			if (is_last_cmd_in_pipeline) {
				last_cmd_final_status = shell->exit_code; // Capture the error code (should be 1)
				is_last_cmd_status_determined = true;
			}
		}
		exec = exec->next;
	}

	close_cmd(cmd); // Parent closes all its pipe FDs after forking all children.
	wait_fork(shell, cmd); // Waits for all children and sets shell->exit_code based on the last child waited for.

	// If the syntactically last command's status was specifically determined (builtin or pre-execution failure), use it.
	if (is_last_cmd_status_determined) {
		shell->exit_code = last_cmd_final_status;
	}
	// If the last command was external and had a redirection error that set its 'execution' to false,
	// and 'is_last_cmd_status_determined' became true with exit_code 1, this is handled.
	// If the last command was external and ran, wait_fork's result is used (unless overridden above).

	// 🧹 Free the command list and reset
	free_exec_cmd(cmd->execcmd);
	cmd->execcmd = NULL;
}
