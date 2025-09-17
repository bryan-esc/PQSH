#!/bin/sh

LOGFILE=tests/test_rdrn_checklist.log

# Functions

pqsh_test_commands() {
    echo add bin/worksim 3
    sleep 1
    echo add bin/worksim 2
    sleep 1 
    echo add bin/worksim 1
    sleep 6
    echo "status"
}

pqsh_check_output() {
    python3 <<EOF
import re
import sys

status_rx  = r'Running =\\s+([0-9]+), Waiting =\\s+([0-9]+), Finished =\\s+([0-9]+), Turnaround =\\s+([0-9\\.]+), Response =\\s+([0-9\\.]+)'
stdout_txt = open("$LOGFILE").read()
try:
    running, waiting, finished, turnaround, response = \
	map(float, re.findall(status_rx, stdout_txt)[-1])
except ValueError:
    sys.exit(1)

if running != 0 or waiting != 0 or finished != 3:
    sys.exit(2)

if not (4.75 <= turnaround <= 5.75):
    sys.exit(3)

if not (0.00 <= response <= 0.75):
    sys.exit(4)
EOF
}

# Main Execution

printf "Testing %s ...\n" "$(basename $LOGFILE .log)"

if [ ! -x bin/pqsh ]; then
    echo "ERROR: Please build bin/pqsh"
    exit 1
fi

printf " %-60s ... " "Running PQSH commands"
if pqsh_test_commands | valgrind --leak-check=full bin/pqsh -p rdrn > $LOGFILE 2> $LOGFILE.valgrind; then
    echo "Success"
else
    echo "Failure"
    exit 2
fi

printf " %-60s ... " "Verifying PQSH output"
if pqsh_check_output; then
    echo "Success"
else
    echo "Failure"
    exit 3
fi

printf " %-60s ... " "Verifying PQSH memory"
if [ $(awk '/ERROR SUMMARY:/ {print $4}' $LOGFILE.valgrind) -eq 0 ]; then
    echo "Success"
else
    echo "Failure"
    exit 4
fi

echo
