#include <stdio.h>

#define BLACK   "\x1b[40m"
#define RED     "\x1b[41m"
#define GREEN   "\x1b[42m"
#define YELLOW  "\x1b[43m"
#define BLUE    "\x1b[44m"
#define MAGENTA "\x1b[45m"
#define CYAN    "\x1b[46m"
#define DEFAULT  "\x1B[0m"
int main(int argc, char* argv[]){
    // for the final map, instead of just saying RED or CYAN, we'll use
    // getDiscipline (I think that's the funciton)
    printf(       "               ____               \n");

    printf(DEFAULT"              /");
    printf(RED                   "    ");
    printf(DEFAULT                    "\\              \n");

    printf(DEFAULT"         ____/");
    printf(RED                   "  ");
    printf(BLACK                    "11");
    printf(RED                         "  ");
    printf(DEFAULT                        "\\____         \n");

    printf(DEFAULT"        /");
    printf(CYAN            "    ");
    printf(DEFAULT             "\\");
    printf(RED                   "      ");
    printf(DEFAULT                    "/");
    printf(MAGENTA                     "    ");
    printf(DEFAULT                         "\\    \n");

    printf(DEFAULT"   ____/");
    printf(CYAN           "  ");
    printf(BLACK            "3 ");
    printf(CYAN              "  ");
    printf(DEFAULT             "\\");
    printf(RED                   "____");
    printf(DEFAULT                   "/");
    printf(MAGENTA                    "  ");
    printf(BLACK                        "5 ");
    printf(MAGENTA                        "  ");
    printf(DEFAULT                          "\\____   \n");

    printf(DEFAULT"  /");
    printf(MAGENTA   "    ");
    printf(DEFAULT       "\\");
    printf(CYAN            "      ");
    printf(DEFAULT               "/");
    printf(GREEN                  "    ");
    printf(DEFAULT                    "\\");
    printf(MAGENTA                      "      ");
    printf(DEFAULT                            "/");
    printf(CYAN                                "    ");
    printf(DEFAULT                                 "\\  \n");

    printf(DEFAULT" /");
    printf(MAGENTA  "  ");
    printf(BLACK      "6 ");
    printf(MAGENTA     "  ");
    printf(DEFAULT       "\\");
    printf(CYAN            "    ");
    printf(DEFAULT             "/");
    printf(GREEN                "  ");
    printf(BLACK                  "11");
    printf(GREEN                    "  ");
    printf(DEFAULT                    "\\");
    printf(MAGENTA                      "    ");
    printf(DEFAULT                          "/");
    printf(CYAN                              "  ");
    printf(BLACK                               "9 ");
    printf(CYAN                                 "  ");
    printf(DEFAULT                                "\\  \n");

    printf(DEFAULT" \\");
    printf(MAGENTA    "      ");
    printf(DEFAULT           "/");
    printf(GREEN               "    ");
    printf(DEFAULT                  "\\");
    printf(GREEN                       "      ");
    printf(DEFAULT                            "/");
    printf(YELLOW                               "    ");
    printf(DEFAULT                                   "\\");
    printf(CYAN                                         "      ");
    printf(DEFAULT                                             "/ \n");

    printf(DEFAULT"  \\");
    printf(MAGENTA     "____");
    printf(DEFAULT          "/");
    printf(GREEN              "  ");
    printf(BLACK                "4 ");
    printf(GREEN                 "  ");
    printf(DEFAULT                 "\\");
    printf(GREEN                     "____");
    printf(DEFAULT                       "/");
    printf(YELLOW                         "  ");
    printf(BLACK                            "2 ");
    printf(YELLOW                            "  ");
    printf(DEFAULT                             "\\");
    printf(CYAN                                  "____");
    printf(DEFAULT                                   "/  \n");
    
    printf(DEFAULT"  /");
    printf(YELLOW    "    ");
    printf(DEFAULT       "\\");
    printf(GREEN             "      ");
    printf(DEFAULT                    "/");
    printf(BLUE                        "    ");
    printf(DEFAULT                           "\\");
    printf(YELLOW                             "      ");
    printf(DEFAULT                                    "/");
    printf(CYAN                                         "    ");
    printf(DEFAULT                                           "\\  \n");
    
    printf(DEFAULT" /");
    printf(YELLOW   "  ");
    printf(BLACK      "10");
    printf(YELLOW       "  ");
    printf(DEFAULT        "\\");
    printf(GREEN            "    ");
    printf(DEFAULT              "/");
    printf(BLUE                  "  ");
    printf(BLACK                   "7 ");
    printf(BLUE                     "  ");
    printf(DEFAULT                    "\\");
    printf(YELLOW                       "    ");
    printf(DEFAULT                          "/");
    printf(CYAN                              "  ");
    printf(BLACK                               "4 ");
    printf(CYAN                                  "  ");
    printf(DEFAULT                                 "\\  \n");
 
    printf(DEFAULT" \\");
    printf(YELLOW    "      ");
    printf(DEFAULT         "/");
    printf(YELLOW           "    ");
    printf(DEFAULT              "\\");
    printf(BLUE                   "      ");
    printf(DEFAULT                      "/");
    printf(MAGENTA                       "    ");
    printf(DEFAULT                           "\\");
    printf(CYAN                                "      ");
    printf(DEFAULT                                   "/ \n");
 
    printf(DEFAULT"  \\");
    printf(YELLOW     "____");
    printf(DEFAULT        "/");
    printf(YELLOW          "  ");
    printf(BLACK             "9 ");
    printf(YELLOW              "  ");
    printf(DEFAULT               "\\");
    printf(BLUE                    "____");
    printf(DEFAULT                     "/");
    printf(MAGENTA                      "  ");
    printf(BLACK                          "5 ");
    printf(MAGENTA                          "  ");
    printf(DEFAULT                            "\\");
    printf(CYAN                                 "____");
    printf(DEFAULT                                  "/  \n");
 
    printf(DEFAULT"  /");
    printf(YELLOW    "    ");
    printf(DEFAULT       "\\");
    printf(YELLOW          "      ");
    printf(DEFAULT               "/");
    printf(CYAN                   "    ");
    printf(DEFAULT                     "\\");
    printf(MAGENTA                       "      ");
    printf(DEFAULT                             "/");
    printf(GREEN                                "    ");
    printf(DEFAULT                                   "\\  \n");
  
    printf(DEFAULT" /");
    printf(YELLOW    "  ");
    printf(BLACK       "8 ");
    printf(YELLOW       "  ");
    printf(DEFAULT        "\\");
    printf(YELLOW           "    ");
    printf(DEFAULT              "/");
    printf(CYAN                  "  ");
    printf(BLACK                   "12");
    printf(CYAN                      "  ");
    printf(DEFAULT                     "\\");
    printf(MAGENTA                       "    ");
    printf(DEFAULT                           "/");
    printf(GREEN                              "  ");
    printf(BLACK                                "8 ");
    printf(GREEN                                 "  ");
    printf(DEFAULT                                 "\\  \n");
  
    printf(DEFAULT" \\");
    printf(YELLOW    "      ");
    printf(DEFAULT         "/");
    printf(RED              "    ");
    printf(DEFAULT              "\\");
    printf(CYAN                   "      ");
    printf(DEFAULT                      "/");
    printf(GREEN                         "    ");
    printf(DEFAULT                           "\\");
    printf(GREEN                               "      ");
    printf(DEFAULT                                   "/ \n");

    printf(DEFAULT"  \\");
    printf(YELLOW     "____");
    printf(DEFAULT        "/");
    printf(RED             "  ");
    printf(BLACK             "6 ");
    printf(RED                 "  ");
    printf(DEFAULT               "\\");
    printf(CYAN                    "____");
    printf(DEFAULT                     "/");
    printf(GREEN                         "  ");
    printf(BLACK                           "10");
    printf(GREEN                             "  ");
    printf(DEFAULT                             "\\");
    printf(GREEN                                 "____");
    printf(DEFAULT                                   "/  \n");

    printf(DEFAULT"       \\");
    printf(RED             "      ");
    printf(DEFAULT               "/");
    printf(RED                    "    ");
    printf(DEFAULT                    "\\");
    printf(GREEN                        "      ");
    printf(DEFAULT                            "/       \n");

    printf(DEFAULT"        \\");
    printf(RED              "____");
    printf(DEFAULT              "/");
    printf(RED                   "  ");
    printf(BLACK                   "3 ");
    printf(RED                       "  ");
    printf(DEFAULT                     "\\");
    printf(GREEN                         "____");
    printf(DEFAULT                           "/        \n");
    
    printf(DEFAULT"             \\");
    printf(RED                   "      ");
    printf(DEFAULT                     "/             \n");

    printf(DEFAULT"              \\");
    printf(RED                    "____");
    printf(DEFAULT                    "/              \n");
  
    return 0;
}
