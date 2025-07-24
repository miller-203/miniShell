#!/bin/bash
# minishell_map_leak_test.sh
# Run all test commands from Minishell Map CSV with valgrind leak checking

MINISHELL=./minishell
VALGRIND="valgrind --leak-check=full --show-leak-kinds=definite --errors-for-leak-kinds=definite --track-origins=yes"
CSV="Minishell Map - GitHub _ @vietdu91 @QnYosa - Feuille 1.csv"
LOG=valgrind_map_leak_test.log

# Extract test commands from the CSV (lines starting with $> or ,$>)
grep -E '^\$>|,\$>' "$CSV" | sed 's/^\$>//;s/^,\$>//;s/,$>//' | while read -r CMD
 do
    # Skip empty lines
    if [[ -z "$CMD" ]]; then continue; fi
    # Prepare a temp file with the command and exit
    CMDFILE=$(mktemp)
    echo "$CMD" > "$CMDFILE"
    echo "exit" >> "$CMDFILE"
    echo "\n===== Testing: $CMD =====" | tee -a "$LOG"
    $VALGRIND $MINISHELL < "$CMDFILE" >> "$LOG" 2>&1
    rm "$CMDFILE"
done

echo "\nFull valgrind log saved to $LOG" 