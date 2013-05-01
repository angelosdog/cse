typedef struct _stack* Stack;

int top(Stack s);
void pop(Stack s);
void push(Stack s, int value);
Stack createStack(void);
int isEmpty(Stack s);
void disposeStack(Stack s);
