int stringLength(char* string){
    int length = 0;
    int i = 0;
    while (string[i] != '\0'){
        length++;
        i++;
    }
    return length-1;
}
