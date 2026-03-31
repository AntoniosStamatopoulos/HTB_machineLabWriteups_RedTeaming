# Findings - Eighteen HTB

##  Credentials

### MSSQL Access

* User: kevin
* Password: iNa2we6haRj2gaw!

---

### Database Extracted

#### admin

* Username: admin
* Hash: pbkdf2:sha256:600000$AMtzteQIG7yAbZIa$0673ad90a0b4afb19d662336f0fce3a9edd0b7b19193717be28ce4d66c887133

#### test

* Username: test
* Hash: pbkdf2:sha256:600000$G0LSuDQVocCk7q1Q$a73da7f910be5499c2f0fe81a1e913dc4e2d6b5184e7849a69c9fa7978166b8d

---

### Cracked Credentials

* Password: iloveyou1

---

### Valid Domain User

* User: adam.scott
* Password: iloveyou1

---

### Administrator Access

* Method: Pass-the-Hash
* User: administrator
* NTLM Hash:

  ```
  0b133be956bfaddf9cea56701affddec
  ```

---

##  Flags

### User Flag

```
169b80bc61c29856d692cfce273fae73
```

### Root Flag

```
e0ee9d43cf871f87<dynamic_root_flag>
```

---

##  Vulnerabilities Identified

* Exposed MSSQL service
* Weak password (despite PBKDF2)
* Credential reuse across services
* WinRM exposed externally
* Privilege escalation via NTLM hash

---

##  Attack Chain Summary

1. MSSQL login (kevin)
2. Impersonation (appdev)
3. Dump DB → hashes
4. Crack password
5. WinRM login (adam.scott)
6. Privilege escalation (Administrator via hash)
7. Root access

---
