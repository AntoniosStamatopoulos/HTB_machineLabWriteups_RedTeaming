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
