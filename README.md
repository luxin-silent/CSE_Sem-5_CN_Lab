# 🌐 CSE Semester 5: Computer Networks (CN) Lab

> *"Sockets, Packets, and Zero Panic. Because copy-pasting code from random forums at 2 AM is a dangerous sport."*

Welcome, brave engineering soul! 🎓 If you've landed here, you are probably in **Semester 5**, staring down the barrel of **Computer Networks Lab**, wondering why TCP needs three handshakes when people barely get one, or why your C program throws `Segmentation fault (core dumped)` the second your lab evaluator walks by.

Fear not! This repository is your ultimate **plug-and-play reference guide** containing clean, well-commented, and lab-tested C implementations of socket programming cycles, network commands, and packet sniffers.

---

## 🎯 Purpose of This Repo

1. **Save Your Grades (and Sanity)**: Provide structured, working C implementations for all major CN Lab experiments (KTU Syllabus aligned).
2. **Understand the Mechanics**: Each experiment comes with code and an `Algorithm.md` so you actually understand *why* `bind()`, `listen()`, `accept()`, `send()`, and `recv()` exist.
3. **Open for University Sharing**: Built to be shared across campus so no student gets left behind in socket land.

---

## 📂 Repository Structure

Here is how the digital real estate is organized:

```text
CSE_Sem-5_CN_Lab/
├── KTU_CN_Lab_Cycle.pdf                     # 📄 Official lab syllabus & experiment prompts
├── LICENSE                                  # 📜 MIT License (Free as in freedom)
├── README.md                                # 📖 You are here!
└── Solutions/                               # 📁 The secret sauce (All working code)
```

### 🧪 Experiment Directory Summary

| # | Topic | Protocol | Key Concepts / Highlights |
|---|---|---|---|
| **01** | Network Commands | CLI | `ping`, `traceroute`, `netstat`, `ifconfig`, `nslookup` |
| **05** | Matrix Type Identification | **TCP** | Struct/Array transfer over reliable byte stream |
| **06** | Slang Translation | **UDP** | Connectionless datagram dictionary lookup |
| **07** | Multi-user Chat Room | **TCP** | Multi-threading (`pthread`), mutex locks, broadcasting |
| **08** | Daytime / Time Server | **UDP** | Connectionless time fetching (`ctime`, `time_t`) |
| **09** | File Transfer Server | **TCP** | Reading disk files & streaming chunks over sockets |
| **10** | Raw Socket Packet Sniffer | **Raw Sockets** | Kernel bypass, promiscuous packet capturing, IP/TCP headers |

---

## 🛠️ How to Use (The Survival Guide)

### 📋 Prerequisites

Before you start flexing in the terminal, make sure you have GCC installed (Linux or WSL recommended):

```bash
# Ubuntu / Debian / WSL
sudo apt update && sudo apt install build-essential git -y
```

---

### 1️⃣ Step 1: Clone the Repository

Open your favorite terminal and clone this repo onto your system:

```bash
git clone https://github.com/luxin-silent/CSE_Sem-5_CN_Lab.git
cd CSE_Sem-5_CN_Lab
```


---

### 2️⃣ Step 2: Navigate to an Experiment

Pick whichever experiment you need to run today. For example, to test the **Chat Server**:

```bash
cd "Solutions/7_Chat_Server[TCP_Connection]"
```

---

### 3️⃣ Step 3: The Two-Terminal Ritual (Compile & Execute)

Socket programming requires a **Server** to listen and a **Client** to connect. This means **you need two terminal windows open side-by-side**! 👯‍♂️

#### 🖥️ Terminal 1: Compile & Run the Server
```bash
# Compile server code (use -pthread for multi-threaded servers like Chat Server)
gcc server.c -o server -pthread

# Run the server (Must be started BEFORE the client!)
./server
```

#### 💻 Terminal 2: Compile & Run the Client
```bash
# Open a second terminal tab (Ctrl+Shift+T or Ctrl+Alt+T)
cd "Solutions/7_Chat_Server[TCP_Connection]"

# Compile client code
gcc client.c -o client -pthread

# Fire up the client!
./client
```

🎉 Boom! Your server and client are now talking to each other across local host sockets.

---

### 🔑 Special Case: Raw Socket Packet Capture (Ex 10)

Raw sockets talk directly to the network interface card, which means the Linux kernel demands **Superuser (Root) privileges**:

```bash
cd "Solutions/10_Raw_Socket_Packet_Capture"

gcc -Wall -o packet_capture packet_capture.c
sudo ./packet_capture

```

---

## ⚡ Pro-Tips & Common Pitfalls

| Error / Issue | Why it happens | Quick Fix 🔧 |
|---|---|---|
| `Address already in use` | The server crashed or was terminated, but port `8080` is still stuck in `TIME_WAIT`. | Change `PORT` in `server.c` to `8081` or kill the process using `fuser -k 8080/tcp`. |
| `Connection refused` | You launched `./client` before `./server` was running. | Start `./server` first! Sockets can't connect to ghosts. |
| `undefined reference to 'pthread_create'` | Forgot to link the POSIX threads library during compilation. | Add `-pthread` flag: `gcc server.c -o server -pthread` |
| `Operation not permitted` | Tried running raw socket packet sniffer without `sudo`. | Run with `sudo ./packet_capture`. |

---

## 🤝 Sharing with Classmates

Feel free to fork, clone, share, and star ⭐ this repo! If you find a bug or want to add cleaner algorithms, pull requests are warmly welcomed.

Good luck with your lab exams and viva! May your packets never drop and your handshakes always be three-way. 🤝✨

---

## 📜 License

Distributed under the **MIT License**. Free to use, modify, and share for educational purposes.
