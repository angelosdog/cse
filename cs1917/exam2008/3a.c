int test_sort(int nPtr[], int n){
   int i = 0;
   int returnValue;
   if (n == 1){
       returnValue = 1;
   } else if (nPtr[1] < nPtr[0]){
       while (i < n-1 && nPtr[i+1] < nPtr[i]){
           i++;
       }
       if (i == n){
           returnValue = 0;
       } else {
           returnValue = 1;
       }
   } else if (nPtr[1] > nPtr[0]){
       while (i < n-1 && nPtr[i+1] > nPtr[i]){
           i++;
       }
       if (i == n){
           returnValue = 0;
       } else {
           returnValue = 1;
       }
   } else {
       returnValue = 0;
   }
}
