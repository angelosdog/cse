typedef struct _node* link;

void printNode (link ls);

void printList(link ls);

link newNode(Item item);

void insertNode (link ls, Item item);

void traverse(link ls, void (*visit)(link));

void traverseR(link ls, void(*visit)(link));

void square (link l);

void squareAll (link ls);

int count(link ls, int (*pred)(Item));

int isEven(Item n);

int isOdd(Item n);

int isZero(Item n);

int isGreaterThan3(Item n);

int isLessThan10(Item n);


