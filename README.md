# fileless-winSCShell

This is small C code is designed to enable lateral movement during a red team engagement. It allows an attacker with valid credentials to impersonate a user and perform actions on a remote machine.

To use this tool, you must have valid credentials for a user on the target machine. The tool takes the following command-line arguments:

## Usage:
```
PS C:\Users\ieuser> .\scshell.exe user password DOMAIN MachineIP XblAuthManager "C:\windows\system32\cmd.exe /C echo Heheheboooy > C:\yesitwork.txt"
```
    username: The username of the user to impersonate.
    password: The password of the user to impersonate.
    domain: The domain of the target machine.
    target: The name of the target machine.
    service: The name of the service to modify.
    payload: The path to the payload to use.
    
This C code used to performs the following actions:

    Logs on as the specified user using LogonUserA.
    Impersonates the logged-on user using ImpersonateLoggedOnUser.
    Opens a handle to the service control manager on the target machine using OpenSCManagerA.
    Opens a handle to the specified service using OpenServiceA.
    Queries the service configuration using QueryServiceConfigA.
    Changes the service configuration to use the specified payload using ChangeServiceConfigA.
    Starts the service using StartServiceA.
    Reverts the service configuration back to its original state using ChangeServiceConfigA.
