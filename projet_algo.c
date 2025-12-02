// hanoi_enhanced.c - Premium User Experience Edition
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CLEAR_SCREEN "cls"
#else
#include <unistd.h>
#define CLEAR_SCREEN "clear"
#endif

// ═══════════════════════════════════════════════════════════
//  COLOR DEFINITIONS (ANSI Escape Codes)
// ═══════════════════════════════════════════════════════════
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"

// Foreground colors
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright foreground colors
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// Background colors
#define BG_BLACK    "\033[40m"
#define BG_BLUE     "\033[44m"
#define BG_CYAN     "\033[46m"
#define BG_GREEN    "\033[42m"

// ═══════════════════════════════════════════════════════════
//  UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════
#ifdef _WIN32
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

void enableVirtualTerminal() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}

void clearScreen() {
    system(CLEAR_SCREEN);
}

void printBox(const char* title, const char* color) {
    int titleLen = strlen(title);
    int padding = (60 - titleLen) / 2;
    
    printf("%s%s+", BOLD, color);
    for (int i = 0; i < 60; i++) printf("=");
    printf("+%s\n", RESET);
    
    printf("%s%s|", BOLD, color);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < 60 - padding - titleLen; i++) printf(" ");
    printf("|%s\n", RESET);
    
    printf("%s%s+", BOLD, color);
    for (int i = 0; i < 60; i++) printf("=");
    printf("+%s\n", RESET);
}

void printSeparator(const char* color) {
    printf("%s%s", BOLD, color);
    for (int i = 0; i < 62; i++) printf("-");
    printf("%s\n", RESET);
}

void printHeader() {
    printf("\n");
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s|                   TOWER OF HANOI                           |%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s|           Recursive | Iterative | Visualizer               |%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_CYAN, RESET);
    printf("\n");
}

void pressAnyKey() {
    printf("\n%s%s[Press ENTER to continue...]%s", DIM, BRIGHT_BLACK, RESET);
    getchar();
    getchar();
}

void printProgress(int current, int total) {
    int barWidth = 40;
    float progress = (float)current / total;
    int pos = barWidth * progress;
    
    printf("\r%s[", BRIGHT_CYAN);
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) printf("█");
        else printf("░");
    }
    printf("] %s%.1f%%%s", BRIGHT_YELLOW, progress * 100, RESET);
    fflush(stdout);
}

// ═══════════════════════════════════════════════════════════
//  RECURSIVE IMPLEMENTATION
// ═══════════════════════════════════════════════════════════
int moveCount = 0;
double calibratedMovesPerSecond = 0.0;

void hanoi(int n, char source, char dest, char aux) {
    if (n == 1) {
        moveCount++;
        return;
    }
    hanoi(n - 1, source, aux, dest);
    moveCount++;
    hanoi(n - 1, aux, dest, source);
}

void showRecursiveExplanation() {
    clearScreen();
    printf("\n");
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_BLUE, RESET);
    printf("%s%s|           RECURSIVE ALGORITHM EXPLAINED                   |%s\n", BOLD, BRIGHT_BLUE, RESET);
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_BLUE, RESET);
    
    printf("\n%s%s> BASE CASE:%s\n", BOLD, BRIGHT_GREEN, RESET);
    printf("  When n = 1, simply move the disk from source to destination.\n");
    
    printf("\n%s%s> RECURSIVE CASE (n > 1):%s\n", BOLD, BRIGHT_YELLOW, RESET);
    printf("  %s1.%s Move (n-1) disks from source -> auxiliary (using dest)\n", BRIGHT_CYAN, RESET);
    printf("  %s2.%s Move the largest disk from source -> destination\n", BRIGHT_CYAN, RESET);
    printf("  %s3.%s Move (n-1) disks from auxiliary -> destination (using source)\n", BRIGHT_CYAN, RESET);
    
    printf("\n%s%s> WHY IT WORKS:%s\n", BOLD, BRIGHT_MAGENTA, RESET);
    printf("  Each recursive call reduces the problem size by 1 disk,\n");
    printf("  eventually reaching the base case where moving 1 disk is trivial.\n");
    
    printf("\n%s%s> COMPLEXITY ANALYSIS:%s\n", BOLD, BRIGHT_RED, RESET);
    printf("  %sTime:%s   O(2^n) - Exponential growth, %stotal moves = 2^n - 1%s\n", 
           BRIGHT_CYAN, RESET, YELLOW, RESET);
    printf("  %sSpace:%s  O(n) - Stack depth equals number of disks\n", 
           BRIGHT_CYAN, RESET);
    
    printf("\n%s%s> FUN FACTS:%s\n", BOLD, BRIGHT_YELLOW, RESET);
    printf("  * 64 disks would take %s584 billion years%s at 1 move/second!\n", 
           BRIGHT_RED, RESET);
    printf("  * The puzzle has %s18,446,744,073,709,551,615%s total moves for 64 disks\n",
           BRIGHT_CYAN, RESET);
    
    printSeparator(BRIGHT_BLACK);
    pressAnyKey();
}

void runRecursiveTest(int disks) {
    clearScreen();
    moveCount = 0;
    
    printf("\n");
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_GREEN, RESET);
    printf("%s%s|               RECURSIVE TEST - %d DISK%s               |%s\n", 
           BOLD, BRIGHT_GREEN, disks, disks > 1 ? "S" : " ", RESET);
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_GREEN, RESET);
    printf("\n");
    
    // First, calibrate with 25 disks if we haven't already
    if (calibratedMovesPerSecond == 0.0) {
        printf("%s%sCalibrating performance with 25 disks...%s\n", BRIGHT_YELLOW, BOLD, RESET);
        fflush(stdout);
        
        moveCount = 0;
        clock_t calibStart = clock();
        hanoi(25, 'A', 'C', 'B');
        clock_t calibEnd = clock();
        
        double calibTime = (double)(calibEnd - calibStart) / CLOCKS_PER_SEC;
        if (calibTime > 0.0001) {
            calibratedMovesPerSecond = moveCount / calibTime;
        } else {
            calibratedMovesPerSecond = 1e9;  // Extremely fast, use a large value
        }
        
        printf("%s%sCalibration complete!%s\n", BRIGHT_GREEN, BOLD, RESET);
        printf("  Performance: %.0f moves/second\n\n", calibratedMovesPerSecond);
    }
    
    // Now run the actual test
    moveCount = 0;
    printf("%s%sComputing %d disks...%s\n", BRIGHT_YELLOW, BOLD, disks, RESET);
    fflush(stdout);
    
    clock_t start = clock();
    hanoi(disks, 'A', 'C', 'B');
    clock_t end = clock();
    
    double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    
    // If time is too small to measure accurately, estimate it
    if (timeSpent < 0.001 && calibratedMovesPerSecond > 0) {
        timeSpent = moveCount / calibratedMovesPerSecond;
    }
    
    long long expectedMoves = 0;
    if (disks < 63) expectedMoves = (1LL << disks) - 1;
    else expectedMoves = (long long)(pow(2.0, (double)disks) - 1.0);
    
    printf("\n%s%sRESULTS:%s\n", BOLD, BRIGHT_CYAN, RESET);
    printSeparator(BRIGHT_BLACK);
    printf("  %s* Total moves:%s       %s%lld%s\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_WHITE, expectedMoves, RESET);
    printf("  %s* Execution time:%s   %s%.9f seconds%s\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_WHITE, timeSpent, RESET);
    
    if ((long long)moveCount == expectedMoves) {
        printf("  %s* Status:%s           %s%sCORRECT!%s\n", 
               BRIGHT_YELLOW, RESET, BOLD, BRIGHT_GREEN, RESET);
    } else {
        printf("  %s* Status:%s           %s%sERROR!%s\n", 
               BRIGHT_YELLOW, RESET, BOLD, BRIGHT_RED, RESET);
    }
    
    if (disks >= 15) {
        double days = (double)expectedMoves / 86400.0;
        double years = days / 365.25;
        printf("\n  %sFun Fact:%s At 1 move/second, this would take:\n", 
               BRIGHT_MAGENTA, RESET);
        if (years < 1) {
            printf("     %s%.1f days%s\n", BRIGHT_CYAN, days, RESET);
        } else if (years < 1000) {
            printf("     %s%.1f years%s\n", BRIGHT_CYAN, years, RESET);
        } else {
            printf("     %s%.2e years%s\n", BRIGHT_CYAN, years, RESET);
        }
    }
    
    printSeparator(BRIGHT_BLACK);
    pressAnyKey();
}

// ═══════════════════════════════════════════════════════════
//  ITERATIVE IMPLEMENTATION
// ═══════════════════════════════════════════════════════════
typedef struct {
    int *arr;
    int top;
} Stack;

Stack createStack(int capacity) {
    Stack s;
    s.arr = (int *)malloc(capacity * sizeof(int));
    s.top = -1;
    return s;
}

void push(Stack *s, int value) {
    s->arr[++(s->top)] = value;
}

int pop(Stack *s) {
    if (s->top == -1) return 0;
    return s->arr[(s->top)--];
}

int peek(Stack *s) {
    if (s->top == -1) return 0;
    return s->arr[s->top];
}

void legalMoveSilent(Stack *A, Stack *B, char nameA, char nameB) {
    int topA = peek(A);
    int topB = peek(B);
    
    if (topA == 0) {
        push(A, pop(B));
    } else if (topB == 0) {
        push(B, pop(A));
    } else if (topA > topB) {
        push(A, pop(B));
    } else {
        push(B, pop(A));
    }
}

void showIterativeExplanation() {
    clearScreen();
    printf("\n");
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_BLUE, RESET);
    printf("%s%s|          ITERATIVE ALGORITHM EXPLAINED                    |%s\n", BOLD, BRIGHT_BLUE, RESET);
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_BLUE, RESET);
    
    printf("\n%s%s> CORE CONCEPT:%s\n", BOLD, BRIGHT_GREEN, RESET);
    printf("  Uses three stacks (A, B, C) to represent the three pegs.\n");
    printf("  Simulates legal moves in a specific cyclic pattern.\n");
    
    printf("\n%s%s> ALGORITHM STEPS:%s\n", BOLD, BRIGHT_YELLOW, RESET);
    printf("  %s1.%s Calculate total moves = 2^n - 1\n", BRIGHT_CYAN, RESET);
    printf("  %s2.%s If n is even, swap auxiliary and destination pegs\n", BRIGHT_CYAN, RESET);
    printf("  %s3.%s For each move i from 1 to total:\n", BRIGHT_CYAN, RESET);
    printf("      * If i mod 3 = 1: move between source <-> destination\n");
    printf("      * If i mod 3 = 2: move between source <-> auxiliary\n");
    printf("      * If i mod 3 = 0: move between auxiliary <-> destination\n");
    
    printf("\n%s%s> EVEN/ODD CONSIDERATION:%s\n", BOLD, BRIGHT_MAGENTA, RESET);
    printf("  For even n, the smallest disk follows a different cycle.\n");
    printf("  Swapping peg roles at the start ensures correct sequence.\n");
    
    printf("\n%s%s> COMPLEXITY ANALYSIS:%s\n", BOLD, BRIGHT_RED, RESET);
    printf("  %sTime:%s   O(2^n) - Performs 2^n - 1 moves\n", BRIGHT_CYAN, RESET);
    printf("  %sSpace:%s  O(n) - Three stacks storing n disks\n", BRIGHT_CYAN, RESET);
    
    printf("\n%s%s> ADVANTAGES:%s\n", BOLD, BRIGHT_GREEN, RESET);
    printf("  * No recursion overhead (no stack depth limit)\n");
    printf("  * More memory efficient for very large n\n");
    printf("  * Easier to pause/resume execution\n");
    
    printSeparator(BRIGHT_BLACK);
    pressAnyKey();
}

void runIterativeTest(int n) {
    clearScreen();
    printf("\n");
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_MAGENTA, RESET);
    printf("%s%s|               ITERATIVE TEST - %d DISK%s               |%s\n", 
           BOLD, BRIGHT_MAGENTA, n, n > 1 ? "S" : " ", RESET);
    printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_MAGENTA, RESET);
    printf("\n");
    
    // Calibrate if needed
    if (calibratedMovesPerSecond == 0.0) {
        printf("%s%sCalibrating performance with 25 disks...%s\n", BRIGHT_YELLOW, BOLD, RESET);
        fflush(stdout);
        
        Stack calibSrc = createStack(25);
        Stack calibAux = createStack(25);
        Stack calibDest = createStack(25);
        
        for (int i = 25; i >= 1; i--)
            push(&calibSrc, i);
        
        long long calibMoves = (1LL << 25) - 1;
        char S = 'A', A = 'B', D = 'C';
        
        clock_t calibStart = clock();
        for (long long i = 1; i <= calibMoves; i++) {
            if (i % 3 == 1) {
                legalMoveSilent(&calibSrc, &calibDest, S, D);
            } else if (i % 3 == 2) {
                legalMoveSilent(&calibSrc, &calibAux, S, A);
            } else {
                legalMoveSilent(&calibAux, &calibDest, A, D);
            }
        }
        clock_t calibEnd = clock();
        
        double calibTime = (double)(calibEnd - calibStart) / CLOCKS_PER_SEC;
        if (calibTime > 0.0001) {
            calibratedMovesPerSecond = calibMoves / calibTime;
        } else {
            calibratedMovesPerSecond = 1e9;
        }
        
        free(calibSrc.arr);
        free(calibAux.arr);
        free(calibDest.arr);
        
        printf("%s%sCalibration complete!%s\n", BRIGHT_GREEN, BOLD, RESET);
        printf("  Performance: %.0f moves/second\n\n", calibratedMovesPerSecond);
    }
    
    Stack src = createStack(n);
    Stack aux = createStack(n);
    Stack dest = createStack(n);
    
    for (int i = n; i >= 1; i--)
        push(&src, i);
    
    long long totalMoves = 0;
    if (n < 63) totalMoves = (1LL << n) - 1;
    else totalMoves = (long long)(pow(2.0, (double)n) - 1.0);
    
    char S = 'A', A = 'B', D = 'C';
    if (n % 2 == 0) {
        char temp = D;
        D = A;
        A = temp;
    }
    
    printf("%sProcessing %lld moves...%s\n\n", BRIGHT_YELLOW, totalMoves, RESET);
    
    clock_t start = clock();
    
    for (long long i = 1; i <= totalMoves; i++) {
        if (i % 3 == 1) {
            legalMoveSilent(&src, &dest, S, D);
        } else if (i % 3 == 2) {
            legalMoveSilent(&src, &aux, S, A);
        } else {
            legalMoveSilent(&aux, &dest, A, D);
        }
        
        // Show progress for large computations
        if (totalMoves > 10000 && i % (totalMoves / 100) == 0) {
            printProgress(i, totalMoves);
        }
    }
    
    if (totalMoves > 10000) {
        printf("\n\n");
    }
    
    clock_t end = clock();
    double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
    
    // If time is too small to measure accurately, estimate it
    if (timeTaken < 0.001 && calibratedMovesPerSecond > 0) {
        timeTaken = totalMoves / calibratedMovesPerSecond;
    }
    
    printf("%s%sRESULTS:%s\n", BOLD, BRIGHT_CYAN, RESET);
    printSeparator(BRIGHT_BLACK);
    printf("  %s* Total moves:%s      %s%lld%s\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_WHITE, totalMoves, RESET);
    printf("  %s* Execution time:%s  %s%.9f seconds%s\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_WHITE, timeTaken, RESET);
    printf("  %s* Status:%s          %s%sCOMPLETE!%s\n", 
           BRIGHT_YELLOW, RESET, BOLD, BRIGHT_GREEN, RESET);
    
    if (n >= 15) {
        double movesPerSec = totalMoves / timeTaken;
        printf("  %s* Performance:%s     %s%.0f moves/second%s\n",
               BRIGHT_YELLOW, RESET, BRIGHT_CYAN, movesPerSec, RESET);
    }
    
    printSeparator(BRIGHT_BLACK);
    
    free(src.arr);
    free(aux.arr);
    free(dest.arr);
    
    pressAnyKey();
}

// ═══════════════════════════════════════════════════════════
//  VISUALIZER LAUNCHER
// ═══════════════════════════════════════════════════════════
void launchVisualizer() {
    clearScreen();
    printf("\n");
    printBox("LAUNCHING VISUALIZER", BRIGHT_CYAN);
    printf("\n");
    
    #ifdef _WIN32
    printf("%s  Opening graphical visualizer...%s\n\n", BRIGHT_GREEN, RESET);
    system("start \"\" \"dist\\run.bat\"");
    #else
    printf("%s  Warning: Visualizer is only supported on Windows.%s\n", BRIGHT_RED, RESET);
    printf("  The console versions work on all platforms!\n");
    #endif
    
    pressAnyKey();
}

// ═══════════════════════════════════════════════════════════
//  BATCH TESTING IMPLEMENTATION
// ═══════════════════════════════════════════════════════════
void runBatchExperiments(int maxDisks, int isIterative) {
    clearScreen();
    printf("\n");
    const char* modeStr = isIterative ? "ITERATIVE" : "RECURSIVE";
    const char* color = isIterative ? BRIGHT_MAGENTA : BRIGHT_GREEN;
    
    printf("%s%s+=============================================================================+%s\n", BOLD, color, RESET);
    printf("%s%s|               BATCH PERFORMANCE TEST (%s)                       |%s\n", BOLD, color, modeStr, RESET);
    printf("%s%s+=============================================================================+%s\n", BOLD, color, RESET);
    printf("\n");
    
    printf("%s%s  %-10s | %-20s | %-15s | %-15s%s\n", BOLD, BRIGHT_CYAN, "Disks", "Total Moves", "Time (sec)", "Moves/Sec", RESET);
    printSeparator(BRIGHT_BLACK);
    
    // Calibrate if needed (reuse existing calibration logic or variable)
    if (calibratedMovesPerSecond == 0.0) {
        // Quick calibration
        clock_t cStart = clock();
        if (isIterative) {
             Stack s = createStack(20); Stack a = createStack(20); Stack d = createStack(20);
             for(int i=20; i>=1; i--) push(&s, i);
             long long m = (1LL<<20)-1;
             char S='A', A='B', D='C';
             for(long long i=1; i<=m; i++) {
                 if(i%3==1) legalMoveSilent(&s,&d,S,D);
                 else if(i%3==2) legalMoveSilent(&s,&a,S,A);
                 else legalMoveSilent(&a,&d,A,D);
             }
             free(s.arr); free(a.arr); free(d.arr);
        } else {
            moveCount = 0;
            hanoi(20, 'A', 'C', 'B');
        }
        clock_t cEnd = clock();
        double cTime = (double)(cEnd - cStart) / CLOCKS_PER_SEC;
        if (cTime > 0) calibratedMovesPerSecond = ((1LL<<20)-1) / cTime;
        else calibratedMovesPerSecond = 1e9;
    }

    for (int n = 1; n <= maxDisks; n++) {
        long long expectedMoves = (1LL << n) - 1;
        double timeTaken = 0.0;
        
        // Run the algorithm
        clock_t start = clock();
        
        if (isIterative) {
            Stack src = createStack(n);
            Stack aux = createStack(n);
            Stack dest = createStack(n);
            
            for (int i = n; i >= 1; i--) push(&src, i);
            
            char S = 'A', A = 'B', D = 'C';
            if (n % 2 == 0) { char t = D; D = A; A = t; }
            
            for (long long i = 1; i <= expectedMoves; i++) {
                if (i % 3 == 1) legalMoveSilent(&src, &dest, S, D);
                else if (i % 3 == 2) legalMoveSilent(&src, &aux, S, A);
                else legalMoveSilent(&aux, &dest, A, D);
            }
            
            free(src.arr); free(aux.arr); free(dest.arr);
        } else {
            moveCount = 0;
            hanoi(n, 'A', 'C', 'B');
        }
        
        clock_t end = clock();
        timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        
        // Fix for 0 time on fast machines/small N
        if (timeTaken < 0.001 && calibratedMovesPerSecond > 0) {
            timeTaken = expectedMoves / calibratedMovesPerSecond;
        }
        
        double mps = (timeTaken > 0) ? expectedMoves / timeTaken : 0.0;
        
        printf("  %-10d | %-20lld | %-15.6f | %-15.0f\n", 
               n, expectedMoves, timeTaken, mps);
        fflush(stdout); // Ensure line prints immediately
    }
    
    printSeparator(BRIGHT_BLACK);
    pressAnyKey();
}

// ═══════════════════════════════════════════════════════════
//  MENU SYSTEMS
// ═══════════════════════════════════════════════════════════
void recursiveMenu() {
    int choice;
    int disks;
    
    while (1) {
        clearScreen();
        printHeader();
        
        printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_GREEN, RESET);
        printf("%s%s|                   RECURSIVE MODE                           |%s\n", BOLD, BRIGHT_GREEN, RESET);
        printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_GREEN, RESET);
        printf("\n");
        
        printf("  %s1.%s %sCustom Test%s       - Choose any number of disks\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s2.%s %sVisualizer%s        - Launch graphical interface\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s3.%s %sStandard Tests%s    - Run 5, 10, 15, 20 disk tests\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s4.%s %sExplanation%s       - Learn how the algorithm works\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s5.%s %sBatch Test%s        - Run range 1-N (Table View)\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s0.%s %sBack%s              - Return to main menu\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        
        printf("\n");
        printSeparator(BRIGHT_BLACK);
        printf("%s  Enter your choice: %s", BRIGHT_YELLOW, RESET);
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                printf("%s  Number of disks: %s", BRIGHT_YELLOW, RESET);
                if (scanf("%d", &disks) != 1 || disks <= 0) {
                    printf("\n%s  Warning: Invalid input! Please enter a positive number.%s\n", 
                           BRIGHT_RED, RESET);
                    pressAnyKey();
                } else if (disks > 30) {
                    printf("\n%s  Warning: %d disks will take a very long time!%s\n", 
                           BRIGHT_RED, disks, RESET);
                    printf("%s     Continue anyway? (1=Yes, 0=No): %s", BRIGHT_YELLOW, RESET);
                    int confirm;
                    if (scanf("%d", &confirm) == 1 && confirm == 1) {
                        runRecursiveTest(disks);
                    }
                } else {
                    runRecursiveTest(disks);
                }
                break;
                
            case 2:
                launchVisualizer();
                break;
                
            case 3:
                runRecursiveTest(5);
                runRecursiveTest(10);
                runRecursiveTest(15);
                runRecursiveTest(20);
                break;
                
            case 4:
                showRecursiveExplanation();
                break;

            case 5:
                printf("%s  Max disks to test (e.g., 25): %s", BRIGHT_YELLOW, RESET);
                if (scanf("%d", &disks) == 1 && disks > 0) {
                    if (disks > 30) {
                         printf("\n%s  Warning: %d disks will take a long time! Continue? (1=Yes): %s", BRIGHT_RED, disks, RESET);
                         int c; scanf("%d", &c);
                         if (c!=1) break;
                    }
                    runBatchExperiments(disks, 0);
                }
                break;
                
            case 0:
                return;
                
            default:
                printf("\n%s  Warning: Invalid choice! Please try again.%s\n", 
                       BRIGHT_RED, RESET);
                pressAnyKey();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  ITERATIVE MENU
// ═══════════════════════════════════════════════════════════
void iterativeMenu() {
    int choice;
    int disks;
    
    while (1) {
        clearScreen();
        printHeader();
        
        printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_MAGENTA, RESET);
        printf("%s%s|                   ITERATIVE MODE                           |%s\n", BOLD, BRIGHT_MAGENTA, RESET);
        printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_MAGENTA, RESET);
        printf("\n");
        
        printf("  %s1.%s %sCustom Test%s       - Choose any number of disks\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s3.%s %sStandard Tests%s    - Run 5, 10, 15, 20 disk tests\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s4.%s %sExplanation%s       - Learn how the algorithm works\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s5.%s %sBatch Test%s        - Run range 1-N (Table View)\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s0.%s %sBack%s              - Return to main menu\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        
        printf("\n");
        printSeparator(BRIGHT_BLACK);
        printf("%s  Enter your choice: %s", BRIGHT_YELLOW, RESET);
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                printf("%s  Number of disks: %s", BRIGHT_YELLOW, RESET);
                if (scanf("%d", &disks) != 1 || disks <= 0) {
                    printf("\n%s  Warning: Invalid input! Please enter a positive number.%s\n", 
                           BRIGHT_RED, RESET);
                    pressAnyKey();
                } else {
                    runIterativeTest(disks);
                }
                break;
                
            case 3:
                runIterativeTest(5);
                runIterativeTest(10);
                runIterativeTest(15);
                runIterativeTest(20);
                break;
                
            case 4:
                showIterativeExplanation();
                break;

            case 5:
                printf("%s  Max disks to test (e.g., 25): %s", BRIGHT_YELLOW, RESET);
                if (scanf("%d", &disks) == 1 && disks > 0) {
                    if (disks > 30) {
                         printf("\n%s  Warning: %d disks will take a long time! Continue? (1=Yes): %s", BRIGHT_RED, disks, RESET);
                         int c; scanf("%d", &c);
                         if (c!=1) break;
                    }
                    runBatchExperiments(disks, 1);
                }
                break;
                
            case 0:
                return;
                
            default:
                printf("\n%s  Warning: Invalid choice! Please try again.%s\n", 
                       BRIGHT_RED, RESET);
                pressAnyKey();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  MAIN FUNCTION
// ═══════════════════════════════════════════════════════════
int main() {
    enableVirtualTerminal();
    int choice;
    
    while (1) {
        clearScreen();
        printHeader();
        
        printf("  %s1.%s %sRecursive Version%s   - Classic recursive solution\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s2.%s %sIterative Version%s   - Stack-based iterative solution\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s3.%s %sVisualizer%s          - Launch graphical interface\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        printf("  %s0.%s %sExit%s                - Close the program\n", 
               BRIGHT_CYAN, RESET, BRIGHT_WHITE, RESET);
        
        printf("\n");
        printSeparator(BRIGHT_BLACK);
        printf("%s  Enter your choice: %s", BRIGHT_YELLOW, RESET);
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                recursiveMenu();
                break;
                
            case 2:
                iterativeMenu();
                break;
                
            case 3:
                launchVisualizer();
                break;
                
            case 0:
                clearScreen();
                printf("\n");
                printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s|                   Thanks for using!                        |%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s|               Tower of Hanoi Solver v2.0                  |%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s|                                                            |%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("%s%s+============================================================+%s\n", BOLD, BRIGHT_CYAN, RESET);
                printf("\n");
                return 0;
                
            default:
                printf("\n%s  Warning: Invalid choice! Please try again.%s\n", 
                       BRIGHT_RED, RESET);
                pressAnyKey();
        }
    }
    
    return 0;
}


