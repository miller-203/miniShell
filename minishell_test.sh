#!/bin/bash

MINISHELL=./minishell
VALGRIND="valgrind --leak-check=full --error-exitcode=42"
DEBUG_FILE=debug.log
EXPECTED_FILE=expected_outputs.txt

TESTS=(
  # Builtins
  'export FOO=bar'
  'echo $FOO'
  'unset FOO'
  'echo $FOO'
  'export A=1'
  'export B=2'
  'echo $A $B'
  'cd /tmp'
  'pwd'
  'env'
  'export X=hello'
  'env | grep X='
  # Expanding
  'export VAR=42'
  'echo $VAR'
  'echo "$VAR"'
  'export Q="quoted value"'
  'echo $Q'
  # Non built-in
  'ls / | grep bin'
  'cat /etc/passwd | grep root'
  # Redirections
  'echo test > ms_testfile'
  'cat < ms_testfile'
  'rm ms_testfile'
  'echo hello > ms_test1'
  'echo world >> ms_test1'
  'cat ms_test1'
  'rm ms_test1'
  # Pipes
  'echo foo | grep foo'
  'export P=abc'
  'echo $P | cat'
  'ls / | grep bin | sort'
)

# Clear debug and expected output files
echo "Running tests..." > "$DEBUG_FILE"
echo "# Fill in the expected output for each test below." > "$EXPECTED_FILE"

for i in "${!TESTS[@]}"; do
  CMD="${TESTS[$i]}"
  echo -e "\nTest $((i+1)): $CMD" >> "$DEBUG_FILE"
  printf "%s\nexit\n" "$CMD" | $MINISHELL > ms_out 2> ms_err
  ms_status=$?
  echo "# Test $((i+1)): $CMD" >> "$EXPECTED_FILE"
  echo "# Expected output (stdout):" >> "$EXPECTED_FILE"
  echo >> "$EXPECTED_FILE"
  echo "# Expected output (stderr):" >> "$EXPECTED_FILE"
  echo >> "$EXPECTED_FILE"
  echo "# Expected exit status:" >> "$EXPECTED_FILE"
  echo >> "$EXPECTED_FILE"
  echo "--- minishell stdout ---" >> "$DEBUG_FILE"; cat ms_out >> "$DEBUG_FILE"
  echo "--- minishell stderr ---" >> "$DEBUG_FILE"; cat ms_err >> "$DEBUG_FILE"
  echo "minishell status: $ms_status" >> "$DEBUG_FILE"
  # Leak check
  printf "%s\nexit\n" "$CMD" | $VALGRIND $MINISHELL > /dev/null 2> valgrind.log
  if grep -q "definitely lost: 0 bytes" valgrind.log && grep -q "indirectly lost: 0 bytes" valgrind.log; then
    echo "[OK] No memory leaks" >> "$DEBUG_FILE"
  else
    echo "[LEAK] Memory leak detected!" >> "$DEBUG_FILE"
    grep "definitely lost" valgrind.log >> "$DEBUG_FILE"
    grep "indirectly lost" valgrind.log >> "$DEBUG_FILE"
  fi
  # FD check (open fds after running)
  printf "%s\nexit\n" "$CMD" | $MINISHELL > /dev/null 2>&1 &
  ms_pid=$!
  sleep 0.2
  lsof -p $ms_pid > lsof_ms.txt
  kill $ms_pid 2>/dev/null
  open_fds=$(grep -c "/dev/" lsof_ms.txt)
  if [ $open_fds -le 3 ]; then
    echo "[OK] No fd leaks ($open_fds open)" >> "$DEBUG_FILE"
  else
    echo "[FD LEAK] $open_fds file descriptors open!" >> "$DEBUG_FILE"
    cat lsof_ms.txt >> "$DEBUG_FILE"
  fi
done

echo -e "\nTests complete. See $DEBUG_FILE for minishell output and $EXPECTED_FILE to fill in expected outputs."
rm -f ms_out ms_err valgrind.log lsof_ms.txt ms_testfile ms_test1 ms_test1 