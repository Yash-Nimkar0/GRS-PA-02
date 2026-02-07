#!/bin/bash
# Roll No: MT25055
# File: MT25055_Part_C_Experiment.sh

# 1. Compile
make clean
make

# 2. Setup CSV
CSV_FILE="MT25055_results.csv"
echo "Implementation,Threads,MsgSize,Throughput_Gbps,Latency_us,Cycles,CacheMisses,ContextSwitches" > $CSV_FILE

# Parameters
MSG_SIZES=(1024 4096 16384 65536)
THREADS=(1 2 4 8)
IMPLS=("A1" "A2" "A3")

echo "Starting Experiments (Hybrid CPU Supported)..."

for IMPL in "${IMPLS[@]}"; do
    for SIZE in "${MSG_SIZES[@]}"; do
        for THREAD in "${THREADS[@]}"; do
            echo "Running $IMPL | Threads: $THREAD | Size: $SIZE"
            
            # --- START SERVER ---
            sudo ip netns exec ns_server ./MT25055_Part_${IMPL}_Server > /dev/null 2>&1 &
            sleep 1
            SERVER_PID=$(pgrep -n -f "MT25055_Part_${IMPL}_Server")
            
            if [ -z "$SERVER_PID" ]; then
                echo "Error: Server failed to start."
                continue
            fi

            # --- START PERF ---
            # Using -e cycles,cache-misses (Generic events)
            # The script logic below will handle the Hybrid CPU output
            sudo ip netns exec ns_server perf stat -p $SERVER_PID -e cycles,cache-misses,context-switches -x, -o perf_out.txt sleep 5 &
            PERF_PID=$!

            # --- START CLIENT ---
            CLIENT_OUTPUT=$(sudo ip netns exec ns_client ./MT25055_Part_${IMPL}_Client $THREAD $SIZE)
            
            # Parse Client Output (TPUT LATENCY)
            TPUT=$(echo $CLIENT_OUTPUT | awk '{print $1}')
            LAT=$(echo $CLIENT_OUTPUT | awk '{print $2}')
            
            # Wait for perf
            wait $PERF_PID 2>/dev/null

            # --- SMART PARSE PERF (Fix for Hybrid CPUs) ---
            # awk checks if the 1st column ($1) is a Number.
            # It ignores "<not counted>" or "<not supported>" lines.
            
            CYCLES=$(awk -F, '/cycles/ && $1 ~ /^[0-9]+$/ {print $1; exit}' perf_out.txt)
            CACHE_MISS=$(awk -F, '/cache-misses/ && $1 ~ /^[0-9]+$/ {print $1; exit}' perf_out.txt)
            CS=$(awk -F, '/context-switches/ && $1 ~ /^[0-9]+$/ {print $1; exit}' perf_out.txt)
            
            # Fallbacks (should not be needed now)
            CYCLES=${CYCLES:-0}
            CACHE_MISS=${CACHE_MISS:-0}
            CS=${CS:-0}
            TPUT=${TPUT:-0}
            LAT=${LAT:-0}

            # --- SAVE RESULTS ---
            echo "$IMPL,$THREAD,$SIZE,$TPUT,$LAT,$CYCLES,$CACHE_MISS,$CS" >> $CSV_FILE
            
            # Cleanup
            sudo kill -9 $SERVER_PID 2>/dev/null
            sudo killall -9 MT25055_Part_${IMPL}_Client 2>/dev/null
            wait $SERVER_PID 2>/dev/null
            sleep 0.5
        done
    done
done

echo "Experiments Completed. Data saved to $CSV_FILE"
cat $CSV_FILE