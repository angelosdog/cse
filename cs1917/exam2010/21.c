typedef struct node {
   int data;
   struct node *next;
};

Node* rotate(Node* first, int n){
   Node* second = first->next;
   Node* end = first->next;
   if (n != 0){
      while(end->next != NULL){
         end = end->next;
      }
      end->next = first;
      first->next = NULL;
      first = second;
      rotate(first, n-1);
   }
   return first;
}
