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
} typ;

typedef struct node
{
	char *data;
	typ type;
	struct node *next;
} token;

typedef struct abstract_syntax_tree
{
	token *root;
	typ type;
	struct abstract_syntax_tree *left;
	struct abstract_syntax_tree *right;
} ast;

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
	token *current = head;
	token *next;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}

ast	*parse_expression(token **current);
ast	*parse_term(token **current);
ast	*parse_factor(token **current);
//| + -
// * / && ||
//() commands
ast	*parse_expression(token **current)
{
	ast *left = parse_term(current);
	while (*current != NULL && ((*current)->type == ADD
			|| (*current)->type == SUB))
	{
		token *op = *current;
		*current = (*current)->next;
		ast *right = parse_term(current);
		ast *new_ast = malloc(sizeof(ast));
		new_ast->root = op;
		new_ast->type = op->type;
		new_ast->left = left;
		new_ast->right = right;
		left = new_ast;
	}
	return (left);
}

ast	*parse_term(token **current)
{
	ast *left = parse_factor(current);
	while (*current != NULL && ((*current)->type == MUL
			|| (*current)->type == DIV || (*current)->type == MOD))
	{
		token *op = *current;
		*current = (*current)->next;
		ast *right = parse_factor(current);
		ast *new_ast = malloc(sizeof(ast));
		new_ast->root = op;
		new_ast->type = op->type;
		new_ast->left = left;
		new_ast->right = right;
		left = new_ast;
	}
	return (left);
}

ast	*parse_factor(token **current)
{
	if (*current == NULL)
		return (NULL);
	if ((*current)->type == NUM)
	{
		ast *node = malloc(sizeof(ast));
		node->root = *current;
		node->type = NUM;
		node->left = NULL;
		node->right = NULL;
		*current = (*current)->next;
		return (node);
	}
	if ((*current)->type == LPAR)
	{
		*current = (*current)->next;
		ast *expr = parse_expression(current);
		if (*current == NULL || (*current)->type != RPAR)
		{
			return (expr);
		}
		*current = (*current)->next;
		return (expr);
	}
	return (NULL);
}

ast	*creating_recursing_ast(token *root)
{
	token *current = root;
	return (parse_expression(&current));
}

token	*lexing_the_string(char *str)
{
	token *head = NULL;
	token *tail = NULL;
	char *str_copy = strdup(str);
	char *input = str_copy;

	char *processed = malloc(strlen(str) * 2 + 1);
	char *p = processed;

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

	char *token_str = strtok(processed, " ");
	while (token_str != NULL)
	{
		if (strlen(token_str) == 0)
		{
			token_str = strtok(NULL, " ");
			continue ;
		}

		token *new_token = malloc(sizeof(token));
		new_token->data = strdup(token_str);
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
	token *current = head;
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
	if (root == NULL)
	{
		return (0);
	}

	if (root->type == NUM)
	{
		int value = atoi(root->root->data);
		printf("Calculated number: %d\n", value);
		return (value);
	}

	int left_value = calculate_the_tree(root->left);
	int right_value = calculate_the_tree(root->right);
	int result = 0;

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
	char str[100] = "3 + 4 * 2";
	printf("Evaluating expression: %s\n", str);

	token *tokens = lexing_the_string(str);
	print_tokens(tokens);

	ast *syntax_tree = creating_recursing_ast(tokens);

	printf("\n--- AST STRUCTURE ---\n");
	print_ast_detailed(syntax_tree, 0);
	printf("---------------------\n\n");

	int result = calculate_the_tree(syntax_tree);

	printf("\nFinal Result: %d\n", result);

	free_ast(syntax_tree);
	free_tokens(tokens);

	return (0);
}