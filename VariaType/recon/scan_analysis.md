# Nmap Scan Analysis

## Scan Command

```bash
nmap 10.129.43.164 -A
```

Overview

The scan identifies a Linux-based target exposing two services:

SSH (port 22)
HTTP (port 80)

All other ports are closed, indicating a limited attack surface.

Open Ports & Services
Port 22 — SSH
Service: OpenSSH 9.2p1 (Debian)
Purpose: Remote system access
Note: Useful after obtaining valid credentials
Port 80 — HTTP
Service: nginx 1.22.1
Title: VariaType Labs — Variable Font Generator
Purpose: Web application (primary attack vector)
System Information
Operating System: Linux (Kernel 5.x)
Device Type: General-purpose system
Network Distance: 2 hops
Key Takeaways
Limited exposed services reduce attack surface
Web application is the main entry point
SSH is likely used post-exploitation
Conclusion

The target appears to be a web-focused system.
Initial access is expected through the HTTP service, with SSH access potentially leveraged after gaining credentials.
