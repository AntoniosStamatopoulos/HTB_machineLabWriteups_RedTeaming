# HTB Machine: Eighteen

##  Overview

"Eighteen" is a Windows Active Directory-based machine that focuses on credential abuse, MSSQL exploitation, and lateral movement techniques commonly seen in real-world enterprise environments.

---

##  Skills Developed

###  Enumeration & Service Analysis

* Identifying exposed services such as IIS, MSSQL, and WinRM
* Understanding how service exposure impacts attack surface

---

###  MSSQL Exploitation

* Connecting to MSSQL using valid credentials
* Enumerating databases and tables
* Leveraging `EXECUTE AS LOGIN` for privilege escalation within SQL Server

---

###  Credential Extraction & Cracking

* Extracting password hashes from databases
* Identifying hash formats (PBKDF2)
* Cracking hashes using wordlists (rockyou)

---

###  Password Reuse & Credential Attacks

* Recognizing password reuse across services
* Performing password spraying against domain users
* Identifying valid domain credentials

---

###  Lateral Movement (WinRM)

* Gaining remote shell access via WinRM
* Understanding remote management services in Windows environments

---

###  Pass-the-Hash (PtH)

* Authenticating using NTLM hashes instead of plaintext passwords
* Bypassing standard authentication mechanisms
* Gaining administrative access without cracking credentials

---

###  Active Directory Fundamentals

* Understanding domain users, groups, and authentication mechanisms
* Recognizing common AD misconfigurations
* Chaining attacks across multiple services

---

##  Tools Used

* Nmap
* Gobuster
* Impacket
* Hashcat 
* Evil-WinRM

---

##  Key Takeaways

* Small misconfigurations can lead to full domain compromise
* Credential reuse is one of the most critical security weaknesses
* MSSQL servers can be a powerful pivot point in AD environments
* Pass-the-Hash remains a highly effective lateral movement technique

---

##  Repository Structure

* `nmap/` → Network scans
* `notes/` → Personal methodology & observations
* `findings/` → Credentials, hashes, and key discoveries
* `screenshots/` → Optional visual evidence

---

##  Disclaimer

This writeup is for educational purposes only and reflects techniques used in controlled lab environments such as Hack The Box.

---
