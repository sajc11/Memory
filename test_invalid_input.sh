#!/bin/bash
echo "[TEST] Rejecting invalid or out-of-range input"

cat <<EOL > bad_input.txt
-1
4294967296  # UINT32_MAX + 1
hello
32768text
EOL

make > /dev/null
./vm bad_input.txt > /dev/null 2> warn_output.txt

echo "[CHECK] Detected invalid input lines..."
grep -q "\[WARN\]" warn_output.txt && echo "[PASS] Warnings shown for invalid input." || echo "[FAIL] No warning shown."

rm -f bad_input.txt warn_output.txt
