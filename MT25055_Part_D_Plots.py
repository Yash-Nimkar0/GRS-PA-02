# Roll No: MT25055
# File: MT25055_Part_D_Plots.py
# Description: Generates plots for PA02 using hardcoded experiment data
# AI Usage: Used AI to format raw CSV data into Python arrays for matplotlib.

import matplotlib.pyplot as plt

# =================================================================
# 1. HARDCODED DATA ARRAYS
# =================================================================
x_msg_sizes = ['1024', '4096', '16384', '65536']
# Helper integer array for math calculations
x_msg_ints = [1024, 4096, 16384, 65536] 
x_threads = ['1', '2', '4', '8']

# --- DATA FOR A1 (Two-Copy) ---
tput_A1_1T = [9.0440, 22.5305, 30.9946, 31.0731]
tput_A1_2T = [18.0641, 46.0754, 57.7659, 63.1129]
tput_A1_4T = [34.6203, 85.5743, 107.3823, 110.9362]
tput_A1_8T = [55.1827, 65.4610, 144.7238, 156.1455]

lat_A1_1T = [0.91, 1.45, 4.23, 16.87]
lat_A1_2T = [0.91, 1.42, 4.54, 16.62]
lat_A1_4T = [0.95, 1.54, 4.89, 18.91]
lat_A1_8T = [1.19, 4.01, 7.25, 26.87]

cache_A1_1T = [42598829, 206169446, 315913223, 316514691]
cache_A1_2T = [87685382, 325789459, 688124732, 554820928]
cache_A1_4T = [159012681, 689820243, 1095038017, 977198765]
cache_A1_8T = [244442533, 360928790, 1630893840, 1678622968]

cyc_A1_1T = [3267533829, 15247093411, 19198770329, 21592874864]
cyc_A1_2T = [6635152319, 25460608793, 40358755582, 36354325907]
cyc_A1_4T = [14355334358, 51213417947, 63940920465, 73112904054]
cyc_A1_8T = [39130079155, 93480442398, 127061651324, 114613573833]

# --- DATA FOR A2 (One-Copy) ---
tput_A2_1T = [9.1725, 26.6047, 35.7963, 38.8288]
tput_A2_2T = [16.2843, 51.9319, 70.5188, 79.3633]
tput_A2_4T = [33.4882, 86.0258, 126.2002, 147.2077]
tput_A2_8T = [54.7985, 66.4494, 170.6027, 239.7311]

lat_A2_1T = [0.89, 1.23, 3.66, 13.50]
lat_A2_2T = [1.01, 1.26, 3.72, 13.25]
lat_A2_4T = [0.98, 1.52, 4.16, 14.31]
lat_A2_8T = [1.20, 3.95, 6.15, 17.53]

cache_A2_1T = [34932202, 138657897, 352435289, 287480312]
cache_A2_2T = [61201811, 281467568, 639808297, 585896545]
cache_A2_4T = [147427670, 533782091, 1159558615, 1164502316]
cache_A2_8T = [180145330, 322375522, 1169781317, 1192042186]

cyc_A2_1T = [2239461023, 7015139865, 19805229443, 17768516892]
cyc_A2_2T = [5174585640, 14213804732, 32810802358, 37215969763]
cyc_A2_4T = [9865773037, 38944180083, 65954624896, 73869203751]
cyc_A2_8T = [30552157398, 87108507694, 129009614268, 117062655645]

# --- DATA FOR A3 (Zero-Copy) ---
tput_A3_1T = [5.8416, 21.2948, 21.9415, 35.2324]
tput_A3_2T = [11.3576, 42.6747, 42.4513, 69.4906]
tput_A3_4T = [20.4431, 73.9355, 77.9047, 126.7199]
tput_A3_8T = [37.4644, 90.2008, 105.6538, 172.8439]

lat_A3_1T = [1.40, 1.54, 5.97, 14.88]
lat_A3_2T = [1.44, 1.54, 6.18, 15.11]
lat_A3_4T = [1.60, 1.78, 6.73, 16.55]
lat_A3_8T = [1.76, 2.91, 9.93, 24.27]

cache_A3_1T = [19656930, 299106189, 343681382, 339492489]
cache_A3_2T = [26282614, 674167182, 614291659, 718561732]
cache_A3_4T = [67048432, 925094701, 1219265495, 1123815452]
cache_A3_8T = [166941534, 1029927908, 1400902812, 2029664875]

cyc_A3_1T = [1837874749, 16784993270, 20929070178, 19069650688]
cyc_A3_2T = [2525846368, 38567275421, 35200333399, 39763862225]
cyc_A3_4T = [6279243748, 54274764727, 69854860377, 64610169672]
cyc_A3_8T = [14260719734, 90771345160, 109756479706, 125694207982]

# =================================================================
# PLOTTING FUNCTIONS
# =================================================================

def plot_combined_4x(title, filename, y_label, 
                     data_a1, data_a2, data_a3, 
                     sub_titles, x_labels, use_threads_as_x=False):
    
    fig, axs = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle(title, fontsize=16)
    
    # Flatten axs for easy iteration
    axs = axs.flatten()
    
    x_data = x_threads if use_threads_as_x else x_msg_sizes
    
    for i in range(4):
        ax = axs[i]
        # Data is passed as list of lists: [ [1T data], [2T data], ... ]
        ax.plot(x_data, data_a1[i], marker='o', label='Two-Copy (A1)', linewidth=2)
        ax.plot(x_data, data_a2[i], marker='s', label='One-Copy (A2)', linewidth=2)
        ax.plot(x_data, data_a3[i], marker='^', label='Zero-Copy (A3)', linewidth=2)
        
        ax.set_title(sub_titles[i])
        ax.set_xlabel('Thread Count' if use_threads_as_x else 'Message Size (Bytes)')
        ax.set_ylabel(y_label)
        ax.grid(True, linestyle='--', alpha=0.7)
        ax.legend()

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig(filename)
    print(f"Generated {filename}")
    plt.show()

# -----------------------------------------------------------------
# 1. THROUGHPUT PLOTS (4 Subplots: 1T, 2T, 4T, 8T)
# -----------------------------------------------------------------
data_tput_a1 = [tput_A1_1T, tput_A1_2T, tput_A1_4T, tput_A1_8T]
data_tput_a2 = [tput_A2_1T, tput_A2_2T, tput_A2_4T, tput_A2_8T]
data_tput_a3 = [tput_A3_1T, tput_A3_2T, tput_A3_4T, tput_A3_8T]
sub_titles_tput = ['Threads = 1', 'Threads = 2', 'Threads = 4', 'Threads = 8']

plot_combined_4x('Throughput Comparison (Gbps)', 'MT25055_Throughput.png', 'Throughput (Gbps)',
                 data_tput_a1, data_tput_a2, data_tput_a3, 
                 sub_titles_tput, x_msg_sizes, use_threads_as_x=False)

# -----------------------------------------------------------------
# 2. LATENCY PLOTS (4 Subplots: 1024, 4096, 16384, 65536)
#    Note: We need to pivot the data to plot vs Thread Count
# -----------------------------------------------------------------
def pivot_for_latency(data_1t, data_2t, data_4t, data_8t, index):
    return [data_1t[index], data_2t[index], data_4t[index], data_8t[index]]

# Prepare pivoted data for each message size index
lat_sets_a1 = [pivot_for_latency(lat_A1_1T, lat_A1_2T, lat_A1_4T, lat_A1_8T, i) for i in range(4)]
lat_sets_a2 = [pivot_for_latency(lat_A2_1T, lat_A2_2T, lat_A2_4T, lat_A2_8T, i) for i in range(4)]
lat_sets_a3 = [pivot_for_latency(lat_A3_1T, lat_A3_2T, lat_A3_4T, lat_A3_8T, i) for i in range(4)]
sub_titles_lat = ['Msg Size = 1024', 'Msg Size = 4096', 'Msg Size = 16384', 'Msg Size = 65536']

plot_combined_4x('Latency Comparison (microseconds)', 'MT25055_Latency.png', 'Latency (us)',
                 lat_sets_a1, lat_sets_a2, lat_sets_a3, 
                 sub_titles_lat, x_threads, use_threads_as_x=True)

# -----------------------------------------------------------------
# 3. CACHE MISSES PLOTS (4 Subplots: 1T, 2T, 4T, 8T)
# -----------------------------------------------------------------
data_cache_a1 = [cache_A1_1T, cache_A1_2T, cache_A1_4T, cache_A1_8T]
data_cache_a2 = [cache_A2_1T, cache_A2_2T, cache_A2_4T, cache_A2_8T]
data_cache_a3 = [cache_A3_1T, cache_A3_2T, cache_A3_4T, cache_A3_8T]

plot_combined_4x('Cache Misses Comparison', 'MT25055_CacheMisses.png', 'Cache Misses',
                 data_cache_a1, data_cache_a2, data_cache_a3, 
                 sub_titles_tput, x_msg_sizes, use_threads_as_x=False)

# -----------------------------------------------------------------
# 4. CPU CYCLES PLOTS (Cycles Per Byte Calculation)
#    We perform [Total Cycles] / [Msg Size] before plotting
# -----------------------------------------------------------------

# Helper function to divide array by message sizes
def calc_per_byte(cyc_data, sizes):
    return [c / s for c, s in zip(cyc_data, sizes)]

# Convert A1
cyc_byte_A1_1T = calc_per_byte(cyc_A1_1T, x_msg_ints)
cyc_byte_A1_2T = calc_per_byte(cyc_A1_2T, x_msg_ints)
cyc_byte_A1_4T = calc_per_byte(cyc_A1_4T, x_msg_ints)
cyc_byte_A1_8T = calc_per_byte(cyc_A1_8T, x_msg_ints)

# Convert A2
cyc_byte_A2_1T = calc_per_byte(cyc_A2_1T, x_msg_ints)
cyc_byte_A2_2T = calc_per_byte(cyc_A2_2T, x_msg_ints)
cyc_byte_A2_4T = calc_per_byte(cyc_A2_4T, x_msg_ints)
cyc_byte_A2_8T = calc_per_byte(cyc_A2_8T, x_msg_ints)

# Convert A3
cyc_byte_A3_1T = calc_per_byte(cyc_A3_1T, x_msg_ints)
cyc_byte_A3_2T = calc_per_byte(cyc_A3_2T, x_msg_ints)
cyc_byte_A3_4T = calc_per_byte(cyc_A3_4T, x_msg_ints)
cyc_byte_A3_8T = calc_per_byte(cyc_A3_8T, x_msg_ints)

# Aggregate for plotting
data_cyc_a1 = [cyc_byte_A1_1T, cyc_byte_A1_2T, cyc_byte_A1_4T, cyc_byte_A1_8T]
data_cyc_a2 = [cyc_byte_A2_1T, cyc_byte_A2_2T, cyc_byte_A2_4T, cyc_byte_A2_8T]
data_cyc_a3 = [cyc_byte_A3_1T, cyc_byte_A3_2T, cyc_byte_A3_4T, cyc_byte_A3_8T]

# Plot with corrected Title and Label
plot_combined_4x('CPU Cycles per Byte Transferred', 'MT25055_Cycles.png', 'Cycles / Byte',
                 data_cyc_a1, data_cyc_a2, data_cyc_a3, 
                 sub_titles_tput, x_msg_sizes, use_threads_as_x=False)