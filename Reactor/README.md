# HTB Reactor

This repository contains my writeup for the Hack The Box machine **Reactor**.

The lab demonstrates a full attack chain starting from a vulnerable Next.js application affected by **React2Shell / CVE-2025-55182**, leading to remote command execution as the `node` user.

After initial access, the writeup covers SQLite database enumeration, MD5 hash cracking, SSH access as `engineer`, and privilege escalation through a locally exposed **Node.js Inspector** process running as `root`.

Sensitive information such as flags and personal IP addresses has been redacted and replaced with placeholders.
