t_lex_list *find_matching_closed_parenthesis(t_lex_list *token)
{
    int i;

    i = 1;
    token = token->next;
    while (token)
    {
        if (token->a_type == OP_PAREN)
            i++;
        else if (token->a_type == CL_PAREN)
        {
            i--;
            if (i == 0)
                return (token);
        }
        if (i < 0)
            printf("\nerrror\n");
        token = token->next;
    }
    return (token);
}

int get_precedence(t_type_arg type)
{
    if (type == WORD)
        return (1);
    else if (type == PIPE)
        return (2);
    else if (type == AND || type == OR)
        return (3);
    return (0);
}

t_lex_list *highest_prec(t_lex_list *first, t_lex_list *last)
{
    t_lex_list *h_prec;

    if (!first)
        return (NULL);
    h_prec = first;
    while (first != last && first)
    {
        if (first->a_type == OP_PAREN)
        {
            t_lex_list *tmp = find_matching_closed_parenthesis(first);
            if (tmp)
                first = tmp; // Move directly to the closing parenthesis
        }
        else if (get_precedence(first->a_type) >= get_precedence(h_prec->a_type))
            h_prec = first;
        
        if (first != last)
            first = first->next;
    }
    return (h_prec);
}

t_ast_tree *create_ast_node(t_lex_list *token)
{
    t_ast_tree *ast_node;

    if (!token)
        return (NULL);
    ast_node = malloc(sizeof(t_ast_tree));
    ast_node->content = token->s;
    ast_node->type = token->a_type;
    ast_node->right = NULL;
    ast_node->left = NULL;
    return (ast_node);
}

t_ast_tree *create_ast_tree(t_lex_list *current, t_lex_list *last)
{
    t_ast_tree *root;
    t_ast_tree *right;
    t_ast_tree *left;
    t_lex_list *h_precedence;

    if (!current || current == last)
        return (NULL);

    // Handle parentheses first
    if (current->a_type == OP_PAREN)
    {
        t_lex_list *closing = find_matching_closed_parenthesis(current);
        if (!closing)
            return (NULL); // Error: no matching closing parenthesis
        
        // Process expression inside parentheses
        root = create_ast_tree(current->next, closing);
        
        // If there are more tokens after the closing parenthesis and before last
        if (closing->next != last && closing->next)
        {
            // Find highest precedence operator after parentheses
            h_precedence = highest_prec(closing->next, last);
            
            // Create node for this operator
            t_ast_tree *op_node = create_ast_node(h_precedence);
            op_node->left = root; // Left is the parenthetical expression
            op_node->right = create_ast_tree(h_precedence->next, last); // Right is the rest
            return op_node;
        }
        return root;
    }

    // Single token case
    if (current->next == last || !current->next)
        return (create_ast_node(current));

    // Normal case: find highest precedence operator and create tree
    h_precedence = highest_prec(current, last);
    root = create_ast_node(h_precedence);
    
    // Create left subtree (everything before h_precedence)
    left = create_ast_tree(current, h_precedence);
    
    // Create right subtree (everything after h_precedence)
    right = create_ast_tree(h_precedence->next, last);
    
    root->left = left;
    root->right = right;
    return (root);
}

t_ast_tree *we_tree(t_lex_list *current, t_lex_list *last)
{
    t_ast_tree *root = create_ast_tree(current, last);
    return root;
}