#include <windows.h>
#include <stdio.h>

#define MAX_TIMES 100 

int g_nIndex = 0;
DWORD g_dwTimes[MAX_TIMES]; 
CRITICAL_SECTION g_CriticalSection;

DWORD WINAPI Thread1(LPVOID lpParam) {
    BOOL fDone = FALSE;
    while (!fDone) {
        EnterCriticalSection(&g_CriticalSection);
        if (g_nIndex >= MAX_TIMES) {
            fDone = TRUE;
        } else {
            g_dwTimes[g_nIndex] = GetTickCount();
            Sleep(10);
            g_nIndex++;
        }
        LeaveCriticalSection(&g_CriticalSection);
    }
    return 0;
}

DWORD WINAPI Thread2(LPVOID lpParam) {
    BOOL fDone = FALSE;
    while (!fDone) {
        EnterCriticalSection(&g_CriticalSection);
        if (g_nIndex >= MAX_TIMES) {
            fDone = TRUE;
        } else {
            g_nIndex++;
            g_dwTimes[g_nIndex] = GetTickCount();
            Sleep(10);
        }
        LeaveCriticalSection(&g_CriticalSection);
    }
    return 0;
}

int main() {
    InitializeCriticalSection(&g_CriticalSection); //инициализация синхронизации

    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, Thread2, NULL, 0, NULL); //создание потоков

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE); // Ожидание завершения

    for (int i = 0; i < MAX_TIMES; i++) {
        printf("g_dwTimes[%d] = %lu\n", i, g_dwTimes[i]);
    }

    DeleteCriticalSection(&g_CriticalSection); // очищение критической секции
    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);

    return 0;
}