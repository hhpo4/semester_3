#include <windows.h>
#include <stdio.h>

#define MAX_TIMES 100

int g_nIndex = 0;
DWORD g_dwTimes[MAX_TIMES];

DWORD WINAPI Thread1(LPVOID lpParam) {
    while (g_nIndex < MAX_TIMES) {
        g_dwTimes[g_nIndex] = GetTickCount();
        Sleep(10);
        g_nIndex++;
    }
    return 0;
}

DWORD WINAPI Thread2(LPVOID lpParam) {
    while (g_nIndex < MAX_TIMES) {
        g_nIndex++;
        g_dwTimes[g_nIndex] = GetTickCount();
        Sleep(10);
    }
    return 0;
}

int main() {
    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    for (int i = 0; i < MAX_TIMES; i++) {
        printf("%d: %lu\n", i, g_dwTimes[i]);
    }

    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);
    return 0;
}