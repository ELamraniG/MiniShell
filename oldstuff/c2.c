#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum types
{
	ADD = 1,
	SUB,
	MUL,
	DIV,
	MOD,
	LPAR,
	RPAR,
	NUM
}								typ;

typedef struct node
{
	char						*data;
	typ							type;
	struct node					*next;
}								token;

typedef struct abstract_syntax_tree
{
	token						*root;
	typ							type;
	struct abstract_syntax_tree	*left;
	struct abstract_syntax_tree	*right;
}								ast;

void	free_ast(ast *root)
{
	if (root == NULL)
	{
		return ;
	}
	free_ast(root->left);
	free_ast(root->right);
	free(root);
}

void	free_tokens(token *head)
{
	token	*current;
	token	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}


ast	*creating_recursing_ast(token *root)
{
	token	*current;

	current = root;
	return (build_ast(&current, 0));
}

token	*lexing_the_string(char *str)
{
	token	*head;
	token	*tail;
	char	*str_copy;
	char	*processed;
	char	*p;
	char	*token_str;
	token	*new_token;

	head = NULL;
	tail = NULL;
	str_copy = strdup(str);
	if (!str_copy)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return (NULL);
	}
	processed = malloc(strlen(str) * 2 + 1);
	if (!processed)
	{
		fprintf(stderr, "Memory allocation failed\n");
		free(str_copy);
		return (NULL);
	}
	p = processed;
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] == '(' || str[i] == ')' || str[i] == '+' || str[i] == '-'
			|| str[i] == '*' || str[i] == '/' || str[i] == '%')
		{
			if (i > 0 && str[i - 1] != ' ' && p != processed)
			{
				*p++ = ' ';
			}
			*p++ = str[i];
			if (i < strlen(str) - 1 && str[i + 1] != ' ')
			{
				*p++ = ' ';
			}
		}
		else
		{
			*p++ = str[i];
		}
	}
	*p = '\0';
	printf("Processed input: [%s]\n", processed);
	token_str = strtok(processed, " ");
	while (token_str != NULL)
	{
		if (strlen(token_str) == 0)
		{
			token_str = strtok(NULL, " ");
			continue ;
		}
		new_token = malloc(sizeof(token));
		if (!new_token)
		{
			fprintf(stderr, "Memory allocation failed\n");
			free(processed);
			free(str_copy);
			free_tokens(head);
			return (NULL);
		}
		new_token->data = strdup(token_str);
		if (!new_token->data)
		{
			fprintf(stderr, "Memory allocation failed\n");
			free(new_token);
			free(processed);
			free(str_copy);
			free_tokens(head);
			return NULL;
		}
		new_token->next = NULL;
		if (isdigit(token_str[0]))
		{
			new_token->type = NUM;
		}
		else if (strcmp(token_str, "+") == 0)
		{
			new_token->type = ADD;
		}
		else if (strcmp(token_str, "-") == 0)
		{
			new_token->type = SUB;
		}
		else if (strcmp(token_str, "*") == 0)
		{
			new_token->type = MUL;
		}
		else if (strcmp(token_str, "/") == 0)
		{
			new_token->type = DIV;
		}
		else if (strcmp(token_str, "%") == 0)
		{
			new_token->type = MOD;
		}
		else if (strcmp(token_str, "(") == 0)
		{
			new_token->type = LPAR;
		}
		else if (strcmp(token_str, ")") == 0)
		{
			new_token->type = RPAR;
		}
		else
		{
			free(new_token->data);
			free(new_token);
			token_str = strtok(NULL, " ");
			continue ;
		}
		if (head == NULL)
		{
			head = new_token;
			tail = new_token;
		}
		else
		{
			tail->next = new_token;
			tail = new_token;
		}
		token_str = strtok(NULL, " ");
	}
	free(processed);
	free(str_copy);
	return (head);
}

void	print_ast(ast *root)
{
	if (root == NULL)
	{
		return ;
	}
	printf("AST Node: %s, Type: %d\n", root->root->data, root->type);
	if (root->left)
	{
		printf("Left child of %s: ", root->root->data);
		if (root->left->root)
		{
			printf("%s\n", root->left->root->data);
		}
		else
		{
			printf("NULL\n");
		}
	}
	if (root->right)
	{
		printf("Right child of %s: ", root->root->data);
		if (root->right->root)
		{
			printf("%s\n", root->right->root->data);
		}
		else
		{
			printf("NULL\n");
		}
	}
	print_ast(root->left);
	print_ast(root->right);
}

void	print_tokens(token *head)
{
	token	*current;

	current = head;
	while (current != NULL)
	{
		printf("Token: %s, Type: %d\n", current->data, current->type);
		current = current->next;
	}
}

void	print_ast_detailed(ast *root, int level)
{
	if (root == NULL)
	{
		return ;
	}
	for (int i = 0; i < level; i++)
	{
		printf("  ");
	}
	printf("Node: %s, Type: %d\n", root->root->data, root->type);
	print_ast_detailed(root->left, level + 1);
	print_ast_detailed(root->right, level + 1);
}

int	calculate_the_tree(ast *root)
{
	int	value;
	int	left_value;
	int	right_value;
	int	result;

	if (root == NULL)
	{
		return (0);
	}
	if (root->type == NUM)
	{
		value = atoi(root->root->data);
		printf("Calculated number: %d\n", value);
		return (value);
	}
	left_value = calculate_the_tree(root->left);
	right_value = calculate_the_tree(root->right);
	result = 0;
	switch (root->type)
	{
	case ADD:
		result = left_value + right_value;
		printf("Calculated %d + %d = %d\n", left_value, right_value, result);
		return (result);
	case SUB:
		result = left_value - right_value;
		printf("Calculated %d - %d = %d\n", left_value, right_value, result);
		return (result);
	case MUL:
		result = left_value * right_value;
		printf("Calculated %d * %d = %d\n", left_value, right_value, result);
		return (result);
	case DIV:
		if (right_value == 0)
		{
			fprintf(stderr, "Error: Division by zero\n");
			exit(1);
		}
		result = left_value / right_value;
		printf("Calculated %d / %d = %d\n", left_value, right_value, result);
		return (result);
	case MOD:
		if (right_value == 0)
		{
			fprintf(stderr, "Error: Modulo by zero\n");
			exit(1);
		}
		result = left_value % right_value;
		printf("Calculated %d %% %d = %d\n", left_value, right_value, result);
		return (result);
	default:
		fprintf(stderr, "Error: Unknown operator type: %d\n", root->type);
		return (0);
	}
}

int	main(void)
{
	char str[100] = "(99 * (9 / 3) + (6 * 54 -3 + 4)) * 2";
	printf("Evaluating expression: %s\n", str);

	token *tokens = lexing_the_string(str);
	if (!tokens)
	{
		fprintf(stderr, "Failed to tokenize input\n");
		return 1;
	}

	print_tokens(tokens);

	ast *syntax_tree = creating_recursing_ast(tokens);
	if (!syntax_tree)
	{
		fprintf(stderr, "Failed to parse expression\n");
		free_tokens(tokens);
		return 1;
	}

	printf("\n--- AST STRUCTURE ---\n");
	print_ast_detailed(syntax_tree, 0);
	printf("---------------------\n\n");

	int result = calculate_the_tree(syntax_tree);

	printf("\nFinal Result: %d\n", result);

	free_ast(syntax_tree);
	free_tokens(tokens);

	return (0);
}