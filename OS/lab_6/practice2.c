#include <windows.h>
#include <stdio.h>

#define MAX_TIMES 100  

int g_nIndex = 0;
DWORD g_dwTimes[MAX_TIMES];

HANDLE g_hMutex = NULL;

DWORD WINAPI Thread1(LPVOID lpParam) {
    BOOL fDone = FALSE;
    DWORD dw;
    while (!fDone) {
        dw = WaitForSingleObject(g_hMutex, INFINITE);
        if (dw == WAIT_OBJECT_0) {
            if (g_nIndex >= MAX_TIMES) {
                fDone = TRUE;
            } else {
                g_dwTimes[g_nIndex] = GetTickCount();
                Sleep(10);
                g_nIndex++;
            }
            ReleaseMutex(g_hMutex);
        } else {
            break;
        }
    }
    return 0;
}

DWORD WINAPI Thread2(LPVOID lpParam) {
    BOOL fDone = FALSE;
    DWORD dw;
    while (!fDone) {
        dw = WaitForSingleObject(g_hMutex, INFINITE);
        if (dw == WAIT_OBJECT_0) {
            if (g_nIndex >= MAX_TIMES) {
                fDone = TRUE;
            } else {
                g_dwTimes[g_nIndex] = GetTickCount();
                Sleep(10);
                g_nIndex++;
            }
            ReleaseMutex(g_hMutex);
        } else {
            break;
        }
    }
    return 0;
}

int main() {
    g_hMutex = CreateMutex(NULL, FALSE, NULL);

    HANDLE hThreads[2];
    hThreads[0] = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    for (int i = 0; i < MAX_TIMES; i++) {
        printf("g_dwTimes[%d] = %lu\n", i, g_dwTimes[i]);
    }

    CloseHandle(g_hMutex);
    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);

    return 0;
}