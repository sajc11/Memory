#!/bin/bash
# test_basic.sh — Runs basic test with a small custom input

echo "[TEST] Basic logical address translation with writes"

# Create a small addresses.txt file
cat <<EOL > test_addresses.txt
1
256
32768
98305   
131330  
EOL

# Build and run
make > /dev/null
./vm test_addresses.txt > test_output.txt

# Check for expected lines
echo "[CHECK] Output contains expected memory translation..."
grep -q "WRITE" test_output.txt && echo "[PASS] Write handled." || echo "[FAIL] Write not handled."

echo "[CHECK] Page fault count..."
grep -q "Page faults:" test_output.txt && echo "[PASS] Page faults tracked." || echo "[FAIL] Page fault count missing."

echo "[CHECK] TLB tracking..."
grep -q "TLB hits:" test_output.txt && echo "[PASS] TLB tracked." || echo "[FAIL] TLB stats missing."

# Clean up
rm -f test_addresses.txt test_output.txt
