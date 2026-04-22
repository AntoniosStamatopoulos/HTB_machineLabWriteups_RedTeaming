# Silentium HTB Writeup

---

## Overview

"Silentium" is a Linux-based machine that simulates a modern enterprise environment combining web applications, containerized services, and internally exposed development tools. The lab focuses on real-world attack chains involving web exploitation, credential abuse, container interaction, and privilege escalation through internal services.

---

## Skills Developed

---

### Enumeration & Service Analysis

- Identifying exposed services such as HTTP and SSH  
- Discovering hidden assets like staging environments and virtual hosts  
- Understanding how external and internal services expand the attack surface  

---

### Web Exploitation & Access Control Bypass

- Exploiting insecure password reset mechanisms  
- Performing account takeover via logic flaws  
- Understanding authentication and authorization weaknesses in web applications  

---

### Remote Code Execution (RCE)

- Leveraging application functionality for code execution  
- Exploiting backend logic (e.g., custom integrations / APIs)  
- Gaining shell access through injection techniques  

---

### Container Enumeration & Breakout Concepts

- Enumerating environment variables within containers  
- Identifying sensitive data exposure (credentials, tokens)  
- Understanding the security boundary between containers and host systems  

---

### Credential Harvesting & Reuse

- Extracting credentials from environment variables  
- Identifying password reuse across services  
- Using harvested credentials for lateral movement  

---

### Lateral Movement (SSH Pivoting)

- Accessing the host system via SSH using valid credentials  
- Transitioning from container access to host-level access  
- Expanding control within the system  

---

### Internal Service Discovery

- Identifying services bound to localhost  
- Understanding the risks of internal-only applications  
- Accessing hidden services via port forwarding  

---

### Privilege Escalation (Gogs Exploitation)

- Exploiting vulnerable internal applications  
- Understanding misuse of development tools (Git services)  
- Achieving root access through chained vulnerabilities  

---

## Real-Life Scenario

---

### Example Use Case

In a real-world environment, an organization may deploy a staging platform for testing purposes alongside production systems.

- A vulnerability in the staging application allows unauthorized password resets  
- An attacker gains access to an administrative account  
- Sensitive credentials stored in environment variables are exposed  
- The attacker uses these credentials to access internal systems via SSH  
- Internal development tools (e.g., Git services) are accessible only locally and assumed to be secure  
- A vulnerability in such a service allows privilege escalation to full system control  

---

### Impact

- Full system compromise  
- Unauthorized access to sensitive internal data  
- Potential manipulation of development pipelines  
- Increased risk of lateral movement across infrastructure  

---

### Security Lessons

- Staging environments must be properly secured  
- Sensitive data should never be exposed via environment variables  
- Credentials should not be reused across services  
- Internal services must be hardened and monitored  
- Multiple low-risk vulnerabilities can lead to full compromise when chained together  

---
