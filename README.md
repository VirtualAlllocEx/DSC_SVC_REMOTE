# DSC_SVC_REMOTE
This code example allows you to create a **malware.exe** sample that can be run in the **context of a Windows system service**, and could be used for **local privilege escalation** in the context of e.g. **unquoted service path or file permisson vulnerability**, etc. The payload itself can be **remotely hosted**, downloaded via the wininet library and then executed via **direct system calls**. 

1. Replace the link to your hosted .bin payload 

![image](https://user-images.githubusercontent.com/50073731/236861981-678d0fa7-18e9-4318-95cd-346cb5464dec.png)

2. Change the name of the service to your service 

![image](https://user-images.githubusercontent.com/50073731/236862074-a8a46ee5-b97f-4a83-9e9d-e4b37d2e7ceb.png)
![image](https://user-images.githubusercontent.com/50073731/236862198-a3c5d566-9af1-4920-9860-7da461b8686f.png)

3. Start the respective service and retreive session in system context 

**cmd>**
```
sc start unquotedsvc
```
![image](https://user-images.githubusercontent.com/50073731/236866165-e061c6c0-2b47-41fc-a78a-edbcedc24719.png)

