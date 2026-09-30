# Raw Socket Packet Capture Application

A simple C application using Linux **Raw Sockets** (`AF_PACKET`) to capture, parse, and analyze network packets at Layer 2 (Ethernet), Layer 3 (IPv4), and Layer 4 (TCP / UDP / ICMP), including raw payload data.

---

## Files in this Directory

- [`packet_capture.c`](file:///home/luxin_silent/Lab_Works/CSE_Sem-5_CN_Lab/Solutions/10_Raw_Socket_Packet_Capture/packet_capture.c) - C source code for the raw socket packet capturer.
- [`README.md`](file:///home/luxin_silent/Lab_Works/CSE_Sem-5_CN_Lab/Solutions/10_Raw_Socket_Packet_Capture/README.md) - Step-by-step execution and analysis instructions.

---

## Step 1: Open Terminal & Navigate to Workspace

Navigate to the project directory:

```bash
cd /home/luxin_silent/Lab_Works/CSE_Sem-5_CN_Lab/Solutions/10_Raw_Socket_Packet_Capture
```

---

## Step 2: Compile the Code

```bash
gcc -Wall -o packet_capture packet_capture.c
```

---

## Step 3: Execute the Packet Capturer

Raw sockets capture low-level network interface frames and require **root / administrative privileges**.

Run the program using `sudo`:

```bash
sudo ./packet_capture
```

You will see:

```text
====================================================
        RAW SOCKET PACKET CAPTURE APPLICATION       
====================================================
Capturing packets... Press Ctrl+C to stop.
```

---

## Step 4: Generate Network Traffic (In a Second Terminal)

Open a **second terminal window** and run any of the commands below to generate traffic that will be captured in real-time:

1. **ICMP Traffic (Ping):**
   ```bash
   ping -c 2 8.8.8.8
   ```

2. **TCP Traffic (HTTP Web Request):**
   ```bash
   curl http://example.com
   ```

3. **UDP Traffic (DNS Query):**
   ```bash
   nslookup google.com
   ```

---

## Step 5: How to Analyze Captured Output

When network traffic is captured, the application formats the output into 4 detailed layers:

### Sample Captured Output (ICMP Echo Request):

```text
====================================================
 PACKET #1 | Total Size: 98 Bytes
====================================================

--- [ ETHERNET HEADER ] ---
 Destination MAC : 00:15:5D:01:02:03
 Source MAC      : 9C:7B:EF:12:34:56
 Protocol        : 0x0800 (IP)

--- [ IP HEADER ] ---
 IP Version        : 4
 Header Length     : 20 Bytes (5 DWORDS)
 Type of Service   : 0
 Total Length      : 84 Bytes
 Identification    : 45210
 Time To Live (TTL): 64
 Protocol          : 1
 Checksum          : 0x4A2B
 Source IP         : 192.168.1.15
 Destination IP    : 8.8.8.8

--- [ ICMP HEADER ] ---
 Type     : 8 (Echo Request)
 Code     : 0
 Checksum : 0x3E12

--- [ PAYLOAD DATA (56 Bytes) ] ---
 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70  | abcdefghijklmnop
 71 72 73 74 75 76 77 61 62 63 64 65 66 67 68 69  | qrstuvwabcdefghi

----------------------------------------------------
 Stats -> Total: 1 | TCP: 0 | UDP: 0 | ICMP: 1 | Other: 0
```

---

### Field Analysis Reference

| Header | Key Field | Explanation |
| :--- | :--- | :--- |
| **Overview** | `Total Size` | Size of the entire captured frame in bytes. |
| **Ethernet** | `Source / Dest MAC` | Layer 2 hardware MAC addresses of sender & receiver. |
| | `Protocol` | `0x0800` indicates IPv4 payload. |
| **IP Header** | `Source / Dest IP` | Sender and Receiver IPv4 addresses. |
| | `TTL` | Time to Live (decremented by each router hop). |
| | `Protocol` | Protocol number (`1` = ICMP, `6` = TCP, `17` = UDP). |
| **TCP Header** | `Ports` | Source and Destination TCP ports (e.g. 80 = HTTP). |
| | `Flags` | Active control flags (`SYN`, `ACK`, `FIN`, `PSH`, `RST`, `URG`). |
| **UDP Header** | `Ports & Length` | Source/Destination ports and UDP payload length. |
| **ICMP Header** | `Type & Code` | Control message type (`8` = Echo Request, `0` = Echo Reply). |
| **Payload** | `Hex & ASCII` | Raw packet data formatted in Hex (Left) and ASCII text (Right). |

---

## Step 6: Stop Execution

To stop packet capturing, press **`Ctrl + C`** in the capturer terminal window.
