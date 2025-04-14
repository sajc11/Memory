#!/bin/bash
# test_dirty_eviction.sh — Confirms dirty page is written back

echo "[TEST] Verifying dirty page write-back during eviction"

# Create address list that:
# - writes to page 0
# - loads 130 more pages to force eviction of page 0
echo 65537 > test_dirty.txt   # Write to page 0, offset 1 (bit 16 set)
for i in {1..130}; do
    echo $((i << 8)) >> test_dirty.txt
done

make > /dev/null
./vm test_dirty.txt > dirty_output.txt

echo "[CHECK] Dirty page tracking..."
grep -q "WRITE" dirty_output.txt && echo "[PASS] Dirty page detected." || echo "[FAIL] No dirty page detected."

grep -q "Dirty pages written back:" dirty_output.txt && echo "[PASS] Dirty page stats reported." || echo "[FAIL] Dirty pages missing."

rm -f test_dirty.txt dirty_output.txt
