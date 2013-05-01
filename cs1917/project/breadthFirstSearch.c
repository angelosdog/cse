#include <stdio.h>
#define PATH_LIMIT 5

typedef struct _path{
    char path[PATH_LIMIT];
} pathing;

pathing generatePath(pathing tempPath, int pathSteps, pathing lastPath);


int main(int argc, char* argv[]){
    pathing lastPath;
    int i;
    for (i = 0; i < PATH_LIMIT-1; i++){
        lastPath.path[i] = 'R';
    }
    lastPath.path[i] = '\0';
    pathing tempPath;
    for (i = 0; i < PATH_LIMIT; i++){
        tempPath.path[i] = '\0';
    }
    int pathSteps = 0;
    tempPath = generatePath(tempPath, pathSteps, lastPath);
    printf("Final path = %s\n", tempPath.path);
    return 0;
}

pathing generatePath(pathing tempPath, int pathSteps, pathing lastPath){
    //printf("path = %s\n", tempPath.path);
    int i = 0;
    while (i < PATH_LIMIT && tempPath.path[i] == lastPath.path[i]){
        i++;    
    }
    //printf("There are %d R's\n", i);
    if (i != PATH_LIMIT){ 
        if (pathSteps == PATH_LIMIT-1){
            //printf("Path limit reached.\n");
            tempPath.path[pathSteps] = '\0';
        } else if (tempPath.path[pathSteps++] == '\0') {
             //printf("pathSteps is now %d\n", pathSteps);
             pathSteps--;
             //printf("pathSteps is now %d\n", pathSteps);
             tempPath.path[pathSteps] = 'L';
             printf("path = %s\n", tempPath.path);
             pathSteps++;
             i = 0;
             while (i < PATH_LIMIT && tempPath.path[i] == lastPath.path[i]){
                 i++;    
             }
             if (i != PATH_LIMIT){
                 pathSteps--;
                 //printf("pathSteps is now %d\n", pathSteps);
                 tempPath.path[pathSteps] = 'R';
                 printf("path = %s\n", tempPath.path);
                 pathSteps++;
                 i = 0;
                 while (i < PATH_LIMIT && tempPath.path[i] == lastPath.path[i]){
                     i++;    
                 }
                 if (i != PATH_LIMIT){
                    pathSteps--;
                    tempPath.path[pathSteps] = 'L';
                    pathSteps++;
                    tempPath = generatePath(tempPath, pathSteps, lastPath);
     
                    i = 0;
                    while (i < PATH_LIMIT && tempPath.path[i] == lastPath.path[i]){
                        i++;    
                    }
                    if (i != PATH_LIMIT){
                        pathSteps--;
                        tempPath.path[pathSteps] = 'R';
                        pathSteps++;
                        tempPath = generatePath(tempPath, pathSteps, lastPath);
     
                    }
                 }
             } 
             i = 0;
             while (i < PATH_LIMIT && tempPath.path[i] == lastPath.path[i]){
                 i++;    
             }
             if (i != PATH_LIMIT){
                 pathSteps--;
                 tempPath.path[pathSteps] = '\0';
             }
        }
    } else {
        printf("SUCCESS!\n");
    }
    //printf("path = %s\n", tempPath.path);
    return tempPath;
}

