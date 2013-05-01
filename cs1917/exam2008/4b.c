Lnode *list_insert(Lnode *head, Lnode *newn){
    Lnode *previous = *head->next;
    if (*head->next == NULL || *head->next->price > *newn->price){
        *newn->next = *head->next;
        *head->next = *newn;
    } else {
        while(*previous != NULL && *previous->next->price > *newn->price){
           *newn->next = *previous->next;
           *previous->next = *newn;
        }
    }

    return *head;
}

void print_quotes(Lnode *curr){
    *curr = *curr->next;
    while(*curr->next != NULL){
        printf("%s\t%d\n", *curr->name, *curr->price);
        *curr = *curr->next;
    }
}

void free_listmem(Lnode *head){
    Lnode *current = *head
    while (*head->next != NULL){
        *head = *head->next;
        free(*current);
    }
    free(*head);
}
