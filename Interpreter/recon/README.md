# Recon - Nmap Scan

## Command Used

```bash
sudo nmap <Target_IP> -p- -sV -sC
```

## Target

* Host: interpreter.htb (10.129.244.184)
* OS: Linux

## Open Ports & Services

| Port | Service | Details                             |
| ---- | ------- | ----------------------------------- |
| 22   | SSH     | OpenSSH 9.2p1 (Debian)              |
| 80   | HTTP    | Jetty – Mirth Connect Administrator |
| 443  | HTTPS   | Jetty – Mirth Connect (SSL)         |
| 6661 | Unknown | Likely custom/internal service      |

## Observations

* Web service running **Mirth Connect Administrator** (potential attack surface)
* HTTP methods include **TRACE** (potentially risky)
* SSL certificate suggests self-hosted service
* Additional unknown service on port **6661** worth further investigation

## Next Steps

* Enumerate web application (80/443)
* Identify version & vulnerabilities of Mirth Connect
* Investigate service on port 6661
* Perform directory/content discovery
