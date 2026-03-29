# Findings – DevArea HTB

##  Initial Observations

- Multiple services exposed: FTP, SSH, HTTP (Apache), Jetty, and Hoverfly
- Anonymous FTP access enabled file retrieval
- Presence of a Java-based backend service using Apache CXF

---

##  Finding 1: Anonymous FTP Access

**Severity:** Medium  

**Description:**  
The FTP service allows anonymous authentication, exposing internal application files.

**Impact:**  
- Unauthorized access to internal resources  
- Disclosure of application components  

**Evidence:**
- ftp-anon: Anonymous FTP login allowed
- employee-service.jar


---

##  Finding 2: Exposure of Backend Application (JAR)

**Severity:** Medium  

**Description:**  
The application JAR file was accessible via FTP and could be extracted and analyzed.

**Impact:**  
- Reverse engineering of application logic  
- Identification of underlying technologies and vulnerabilities  

---

##  Finding 3: Local File Inclusion (CVE-2022-46364)

**Severity:** High  

**Description:**  
The Apache CXF service is vulnerable to Local File Inclusion via XOP include functionality.

**Impact:**  
- Arbitrary file read  
- Exposure of sensitive system and configuration files  

**Files accessed:**
- `/etc/passwd`
- `/proc/self/environ`
- `/etc/systemd/system/*.service`

**Evidence:**
```bash
./exploit.sh file:///etc/passwd
