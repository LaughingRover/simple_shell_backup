#include "shell.h"

/**
 * prompt - prints prompt to the output
 */
void prompt(void)
{
	char *prompt = "$ ";
	size_t prompt_len = _strlen(prompt);

	write(STDOUT_FILENO, prompt, prompt_len);
	fflush(stdout); /*Ensure the prompt is displayed immediately*/
}

/**
 * getArgv - splits string gotten from stdin
 * @input_line: string to split
 * @argv: is updated with tokenized string
 *
 * Return: number of arguments parsed (argc) or -1 in case of an error
 */
int getArgv(char *input_line, char ***argv)
{
	const char *delim = " "; /*Delimeter for tokenization*/
	char *token = NULL;
	size_t max_argc = 16;
	size_t argc = 0;

	*argv = malloc(sizeof(char *) * max_argc);
	if (*argv == NULL)
	{
		perror("Memory allocation failed");
		return (-1);
	}

	token = _strtok(input_line, delim);
	while (token != NULL)
	{
		trim(&token);

		(*argv)[argc] = _strdup(token);
		if ((*argv)[argc] == NULL)
		{
			perror("Memory allocation failed");
			return (-1);
		}

		token = _strtok(NULL, delim);
		argc++;

		if (argc >= max_argc)
		{
			if (resizeArgv(argv, &max_argc) != 0)
				return (-1);
		}
	}

	return (argc);
}

/**
 * freeArgv - frees an array of strings
 * @argv: pointer to an array of strings
 */
void freeArgv(char ***argv)
{
	size_t i = 0;

	while ((*argv)[i] != NULL)
	{
		free((*argv)[i]);
		i++;
	}
	free(*argv);
}

/**
 * resizeArgv - Resizes the argv array by doubling its size
 * @argv: pointer to an array of strings
 * @max_argc: size of argv
 *
 * Return: 0 success, 1 failure
 */
int resizeArgv(char ***argv, size_t *max_argc)
{
	char **new_argv;
	size_t i;
	*max_argc *= 2;

	new_argv = malloc(*max_argc * sizeof(char *));
	if (new_argv == NULL)
	{
		perror("Memory allocation failed");
		return (-1);
	}

	/*Copy existing pointers to the new array*/
	for (i = 0; i < *max_argc / 2; i++)
	{
		new_argv[i] = (*argv)[i];
	}

	freeArgv(argv); /*Free the old array*/
	*argv = new_argv;

	return (0);
}

/**
 * trim - Trim leading and trailing whitespace from a string
 * @str: string
 */
void trim(char **str)
{
	char *start = *str;
	char *end = *str + _strlen(*str) - 1;

	/*Find the index of the first non-whitespace character*/
	while (*start && (*start == ' ' || *start == '\t'))
	{
		start++;
	}

	/*Find the index of the last non-whitespace character*/
	while (end > start && (*end == ' ' || *end == '\t'))
	{
		end--;
	}

	/*Null-terminate the trimmed string*/
	end[1] = '\0';

	/*Update the pointer to the trimmed string*/
	*str = start;
}
