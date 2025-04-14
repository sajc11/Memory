#!/bin/bash
echo "========== Running All Tests =========="

for script in test_*.sh; do
    echo "[RUNNING] $script"
    bash "$script"
    echo "----------------------------------------"
done

echo "========== Done =========="
