# Reconnaissance - WingData HTB

This folder contains the enumeration phase of the WingData machine.

---

##  Nmap Scan

### Command

```bash
nmap -sC -sV -Pn -p- <TARGET_IP>
```

### Findings

* **Port 22 (SSH)**

  * OpenSSH 9.2p1 (Debian)
* **Port 80 (HTTP)**

  * Apache 2.4.66
  * Web title: *WingData Solutions*

### Observations

* Only two exposed services → reduced attack surface
* Web server is the primary entry point
* SSH likely requires valid credentials

---

##  Web Enumeration

### Initial Discovery

Using Burp Suite, a subdomain was identified:

```bash
ftp.wingdata.htb
```

This suggests the presence of an FTP-related web interface.

---

##  Directory Fuzzing

### Command

```bash
ffuf -u http://ftp.wingdata.htb/FUZZ -w <WORDLIST> -fc 404
```

### Findings

| Path            | Status |
| --------------- | ------ |
| crossdomain.xml | 200    |
| css             | 200    |
| favicon.ico     | 200    |
| help            | 500    |
| icons           | 500    |
| images          | 500    |
| include         | 500    |
| language        | 500    |
| plugins         | 500    |

### Observations

* Multiple endpoints return **500 Internal Server Error**
* Indicates:

  * Potential misconfigurations
  * Possible backend processing issues
* Strong hint that the application may be vulnerable

---

##  Exploit Research

### Command

```bash
searchsploit wing ftp
```

### Key Finding

```bash
Wing FTP Server 7.4.3 - Unauthenticated Remote Code Execution (RCE)
```

### Observations

* Target appears to be running **Wing FTP Server**
* Public exploit available for **unauthenticated RCE**
* This becomes the main attack vector

---

##  Summary

* Web service is the primary attack surface
* Subdomain reveals **Wing FTP interface**
* Fuzzing indicates unstable backend (500 errors)
* Public RCE exploit available → high impact vulnerability

 Leads directly to exploitation phase
