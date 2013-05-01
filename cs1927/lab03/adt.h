void LISTinit (void);                  // initialise the list

int  LISTempty (void);                 // is the list empty?

int  LISTmove (int n);             // move current position

Item LISTcurrent (void);               // return element at current position

void LISTbefore (Item newItem);    // insert element before current

void LISTafter (Item newItem);     // insert element after current

Item LISTdelete (void);                // delete current element
