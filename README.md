# DSC_SVC_REMOTE
This code example allows you to create a malware.exe sample that can be run in the context of a system service, and could be used for local privilege escalation in the context of an unquoted service path, etc. The payload itself can be remotely hosted, downloaded via the wininet library and then executed via direct system calls. 
