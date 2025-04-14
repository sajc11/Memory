#!/bin/bash
# test_lru_eviction.sh — Forces LRU eviction by loading more than 128 pages

echo "[TEST] Triggering LRU Eviction with 130+ unique pages..."

# Generate 130+ addresses to unique pages
> test_eviction.txt
for i in {0..130}; do
    addr=$((i << 8))  # i * 256 gives offset 0 of page i
    echo $addr >> test_eviction.txt
done

make > /dev/null
./vm test_eviction.txt > eviction_output.txt

# Extract just the first number after "Page faults:"
faults=$(grep "Page faults:" eviction_output.txt | awk '{print $3}')

echo "[CHECK] Page fault count >= 129..."
if [ "$faults" -ge 129 ] 2>/dev/null; then
  echo "[PASS] Likely eviction occurred (Page faults: $faults)"
else
  echo "[FAIL] Not enough page faults for eviction (Page faults: $faults)"
fi

rm -f test_eviction.txt eviction_output.txt
