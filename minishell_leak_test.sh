#!/bin/bash
# minishell_leak_test.sh
# Automated leak test for minishell using valgrind

MINISHELL=./minishell
VALGRIND="valgrind --leak-check=full --show-leak-kinds=definite --errors-for-leak-kinds=definite --track-origins=yes"

# Create a temporary file with commands to run
CMDS=$(mktemp)
cat > "$CMDS" <<EOF
echo hello
ls > /dev/null
cat Makefile > /dev/null
clear
env
pwd
export TESTVAR=leaktest
unset TESTVAR
exit
EOF

# Run minishell with valgrind and the test commands
$VALGRIND $MINISHELL < "$CMDS" 2>&1 | tee valgrind_leak_test.log

# Clean up
echo "\nFull valgrind log saved to valgrind_leak_test.log"
rm "$CMDS" 