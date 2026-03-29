# DevArea HTB Writeup

## Overview
This repository documents my approach to solving the **DevArea** machine from Hack The Box.  
The objective was to perform a full penetration testing workflow, from enumeration to privilege escalation.

---

## Objectives
- Identify exposed services
- Exploit vulnerabilities for initial access
- Escalate privileges to root
- Analyze the attack chain

---

## Enumeration

### Nmap Scan
Multiple services were identified:
- FTP (Anonymous access enabled)
- SSH
- HTTP (Apache)
- Jetty (port 8080)
- Hoverfly services (ports 8500 & 8888)

### FTP Access
Anonymous login allowed access to a file:
- `employee-service.jar`

---

## Application Analysis

The retrieved JAR file was extracted and analyzed, revealing a backend service using **Apache CXF**.

---

## Vulnerability Exploitation

### CVE-2022-46364 – Apache CXF (LFI)

A Local File Inclusion vulnerability was exploited to read sensitive files:

- `/etc/passwd`
- `/proc/self/environ`
- systemd service files

---

## Credential Discovery

From system files, credentials were discovered:

- Service: Hoverfly
- Username: `admin`
- Password: (redacted)

---

##  Initial Access

The Hoverfly API was abused to:
- Authenticate via API
- Inject middleware
- Execute a reverse shell

Result:
```bash
dev_ryan@devarea
