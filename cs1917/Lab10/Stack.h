typedef struct _stack  *Stack;

//return the value at the top of the Stack
int top(Stack s);

//remove the top of the Stack
void pop(Stack s);

//push a value onto the top of the Stack
void push(Stack s, int value);

//create a new stack with nothing in it
Stack createStack(void);

//is the Stack empty?
int isEmpty(Stack s);

//dispose of a Stack we no longer need
void disposeStack(Stack s);
