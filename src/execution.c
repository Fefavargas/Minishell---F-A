/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 09:34:14 by albermud         ###   ########.fr       */
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
	int		final_status_to_consider = 0;
	bool		a_process_was_waited_for = false;

	// Only proceed if arr_pid is valid and there are actual binary commands counted.
	if (cmd->arr_pid && cmd->n_binary > 0)
	{
		// Iterate through all PID slots that were allocated based on n_binary.
		while (i < cmd->n_binary)
		{
			// Only wait if a PID was actually stored in this slot (i.e., fork happened for this cmd).
			if (cmd->arr_pid[i] != 0)
			{
				waitpid(cmd->arr_pid[i], &status, 0);
				// Update with the status of the current process being waited for.
				// This ensures final_status_to_consider holds the status of the
				// highest-indexed (rightmost in arr_pid) process that was waited for.
				final_status_to_consider = status;
				a_process_was_waited_for = true;
			}
			i++;
		}

		// If we actually waited for at least one process, set the shell exit code.
		if (a_process_was_waited_for)
		{
			if (WIFEXITED(final_status_to_consider))
			{
				shell->exit_code = WEXITSTATUS(final_status_to_consider);
			}
			else if (WIFSIGNALED(final_status_to_consider))
			{
				if (WTERMSIG(final_status_to_consider) == SIGPIPE)
				{
					// Bash often prints nothing for SIGPIPE in pipelines, exit code is 141.
					ft_putstr_fd("", STDERR_FILENO);
				}
				else if (WTERMSIG(final_status_to_consider) == SIGQUIT)
				{
					ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
				}
				shell->exit_code = (128 + WTERMSIG(final_status_to_consider));
			}
		}
		// If !a_process_was_waited_for (e.g. all commands had redir errors before forking, or n_binary was 0),
		// shell->exit_code remains as set by prior logic in execute() (e.g. from a redir error or builtin).
	}
	g_sig.sigchld = 0; // Reset global flag indicating child processes have been handled.
	signal(SIGINT, signal_int); // Re-establish main shell's signal handlers.
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
				shell->arr_env = free_array(shell->arr_env); // Free old one
				shell->arr_env = env_list_to_array(shell->env); // Rebuild from current t_env list
				if (!shell->arr_env) // Handle potential malloc failure in env_list_to_array
				{
					error_msg("minishell: failed to update environment for execution", "", "\n", 1);
					// Decide on error strategy: continue, or treat as fork failure? 
					// For now, let it proceed, execve might fail if env is NULL, or use a minimal default.
					// Or, we could set shell->exit_code = 1; and skip forking for this command.
					// Let's make it skip this command's execution part if env prep fails.
					shell->exit_code = 1;
					exec = exec->next; // Advance to next command in pipeline
					continue; // Continue the while loop for the next exec_cmd
				}

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
