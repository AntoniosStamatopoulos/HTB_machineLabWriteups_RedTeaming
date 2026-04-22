Silentium Lab Overview
• Lab Description

This lab simulates a realistic enterprise environment where multiple technologies coexist, including a public-facing web application, a staging environment, containerized services, and internally exposed development tools.

The main objective is to demonstrate how an attacker can progress from initial access to full system compromise by chaining together multiple vulnerabilities and misconfigurations across different layers of the infrastructure.

• What This Lab Covers

• Reconnaissance and service enumeration
• Discovery of hidden assets (e.g., staging environments)
• Exploitation of web application vulnerabilities
• Unauthorized access and account takeover
• Remote Code Execution (RCE)
• Container compromise and environment enumeration
• Credential harvesting from environment variables
• Lateral movement to the host system via SSH
• Discovery of internal-only services
• Privilege escalation through vulnerable internal applications
• Full system compromise (root access)

• Key Concepts Demonstrated

• Misconfigured authentication and password reset mechanisms
• Exposure of sensitive data through environment variables
• Credential reuse across services
• Security risks of staging environments
• Importance of internal service hardening
• Container vs host security boundaries
• Chaining multiple low/medium vulnerabilities into critical impact

Real-Life Scenario
• Example Use Case

A similar attack could occur in a real-world organization that maintains a staging environment for testing new features.

• A vulnerable staging application exposes a password reset flaw
• An attacker gains administrative access without proper authorization
• Sensitive credentials are stored in environment variables and become accessible
• The attacker uses these credentials to access internal systems via SSH
• Internal tools (e.g., Git servers) are assumed to be secure because they are only locally accessible
• A vulnerability in such a tool allows privilege escalation to root

• Impact

• Full system compromise
• Unauthorized access to sensitive data
• Potential code injection into production pipelines
• Risk of lateral movement within the internal network

• Security Lesson

This lab highlights how modern infrastructures can be compromised when:

• Development convenience overrides security best practices
• Internal services are not properly secured
• Sensitive data is improperly stored
• Multiple small vulnerabilities are chained together
