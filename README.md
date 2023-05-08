# DSC_SVC_REMOTE
This code example allows you to create a **malware.exe** sample that can be run in the **context of a system service**, and could be used for local privilege escalation in the context of an unquoted service path, etc. The payload itself can be remotely hosted, downloaded via the wininet library and then executed via direct system calls. 

1. Replace the link to your hosted .bin payload 

![image](https://user-images.githubusercontent.com/50073731/236861981-678d0fa7-18e9-4318-95cd-346cb5464dec.png)

2. Change the name of the service to your service 

![image](https://user-images.githubusercontent.com/50073731/236862074-a8a46ee5-b97f-4a83-9e9d-e4b37d2e7ceb.png)
![image](https://user-images.githubusercontent.com/50073731/236862198-a3c5d566-9af1-4920-9860-7da461b8686f.png)
