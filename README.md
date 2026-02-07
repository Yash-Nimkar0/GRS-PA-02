# Analysis of Network I/O Primitives (PA02)

**Course:** Graduate Systems (CSE638)  
**Student Roll No:** MT25055  
**Assignment:** Programming Assignment 02  

---

## 📌 Project Overview
This project analyzes the performance overhead of data movement in network I/O by comparing three different socket implementation strategies. The goal is to measure the cost of User-to-Kernel and Kernel-to-Device copies using hardware counters.

### Implementations Compared:
1.  **A1: Two-Copy (Baseline):**
    * Uses standard `malloc`, `memcpy`, and `send()`.
    * **Data Path:** Application Buffer → Kernel Socket Buffer → NIC.
    * **Overhead:** High CPU usage due to double copying.

2.  **A2: One-Copy (Scatter-Gather):**
    * Uses `sendmsg()` with `struct iovec`.
    * **Data Path:** Scattered Application Data → Kernel Socket Buffer → NIC.
    * **Overhead:** Eliminates the User-Space assembly copy.

3.  **A3: Zero-Copy (MSG_ZEROCOPY):**
    * Uses `sendmsg()` with `MSG_ZEROCOPY` flag.
    * **Data Path:** Application Buffer → NIC (via DMA).
    * **Overhead:** No data copying, but incurs MMU/TLB overhead (page pinning).

---

## 📂 Repository Structure

```text<br>
├── MT25055_Part_A1_Server.c / Client.c  # Two-Copy Source Code<br>
├── MT25055_Part_A2_Server.c             # One-Copy Source Code (Server only)<br>
├── MT25055_Part_A3_Server.c             # Zero-Copy Source Code (Server only)<br>
├── MT25055_Part_A_Common.h              # Shared Header (Port/IP definitions)<br>
├── MT25055_Part_C_Experiment.sh         # Bash Automation Script (Namespaces + Perf)<br>
├── MT25055_Part_D_Plots.py              # Python Plotting Script (Hardcoded Data)<br>
├── Makefile                             # Compilation Rules<br>
├── README.md                            # Project Documentation<br>
└── MT25055_results.csv                  # (Optional) Raw Experiment Data<br>
⚙️ Prerequisites
OS: Linux (Kernel 4.14+ required for MSG_ZEROCOPY).

Tools: gcc, make, python3, matplotlib.

System Privileges: Root access (sudo) is required to:

Create network namespaces (ip netns).

Access hardware performance counters (perf).

Enabling Perf Counters (Important)
Before running experiments, ensure kernel restrictions are lifted:

Bash

sudo sh -c 'echo -1 > /proc/sys/kernel/perf_event_paranoid'
sudo sh -c 'echo 0 > /proc/sys/kernel/kptr_restrict'
🚀 How to Build & Run
1. Compilation
Use the provided Makefile to compile all server/client binaries.

Bash

make all
2. Running Experiments
The MT25055_Part_C_Experiment.sh script automates the entire process:

Creates virtual network namespaces (Server/Client).

Runs each implementation (A1, A2, A3) across multiple message sizes and thread counts.

Captures perf stats (Cycles, Cache Misses, Context Switches).

Saves output to MT25055_results.csv.

Bash

chmod +x MT25055_Part_C_Experiment.sh
sudo ./MT25055_Part_C_Experiment.sh
3. Generating Plots
Once the CSV is ready (or using the hardcoded data), run the Python script to generate the 4 required analysis graphs:

Throughput vs Message Size

Latency vs Thread Count

Cache Misses vs Message Size

Cycles per Byte vs Message Size

Bash

python3 MT25055_Part_D_Plots.py
Note: The script saves images as PNG files (e.g., MT25055_Cycles.png).

📊 Key Results
Throughput: One-Copy (A2) generally outperforms Zero-Copy (A3) on this testbed. Zero-Copy only becomes competitive at very large message sizes (64KB+) due to the high fixed cost of page pinning.

Latency: Two-Copy (A1) suffers from significant latency spikes at high thread counts (8 threads) due to cache contention caused by memcpy.

Efficiency: A2 (One-Copy) demonstrates the lowest "Cycles per Byte", making it the most CPU-efficient strategy for this specific workload.

🤖 AI Usage Declaration
In compliance with assignment guidelines, Generative AI tools were used for:

Debugging: Diagnosing perf counter failures in namespaces (Prompt: "Why does perf stat show 0 cycles inside a network namespace?").

Boilerplate Code: Generating initial sendmsg struct skeletons.

Data Formatting: Converting raw CSV data into Python arrays for plotting.

Scripting: Logic for capturing PIDs inside namespaces using pgrep.

Logic verification and final implementation were performed manually.