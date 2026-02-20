#!/bin/bash
# ══════════════════════════════════════════════════════════════════════════════
#  ft_irc — Master tester with valgrind + auto-restart on crash
#  Usage: ./run_tests.sh [port] [password] [path/to/ft_irc]
#
#  If the server crashes mid-suite, it is restarted on the next available port
#  so the remaining test suite can continue.
# ══════════════════════════════════════════════════════════════════════════════

BASE_PORT="${1:-6666}"
PASS="${2:-42irc}"
HOST="127.0.0.1"

BOLD="\033[1m"; CYAN="\033[96m"; GREEN="\033[92m"
RED="\033[91m"; YELLOW="\033[93m"; RESET="\033[0m"

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VALGRIND_LOG="${DIR}/valgrind.log"
VALGRIND_LOG_PY="${DIR}/valgrind_py.log"

SERVER_PID=""
CURRENT_PORT="$BASE_PORT"
VALGRIND_FAIL=0
CRASH_COUNT=0

# ─── Find binary ──────────────────────────────────────────────────────────────
if [ -n "$3" ]; then
    BINARY="$3"
elif [ -x "${DIR}/ft_irc" ]; then
    BINARY="${DIR}/ft_irc"
elif [ -x "${DIR}/../ft_irc" ]; then
    BINARY="$(cd "${DIR}/.." && pwd)/ft_irc"
else
    BINARY=""
fi

# ─── Banner ───────────────────────────────────────────────────────────────────
echo -e "${BOLD}${CYAN}"
echo "╔══════════════════════════════════════════════════════╗"
echo "║              ft_irc — IRC Tester                    ║"
echo "║      valgrind + nc (netcat) + Python socket         ║"
echo "╚══════════════════════════════════════════════════════╝"
echo -e "${RESET}"
echo -e "  ${BOLD}Password:${RESET} ${PASS}"
echo -e "  ${BOLD}Binary  :${RESET} ${BINARY:-not found — must be pre-running}"
echo -e "  ${BOLD}Logs    :${RESET} ${VALGRIND_LOG} / ${VALGRIND_LOG_PY}"
echo ""

# ─── Dependency check ─────────────────────────────────────────────────────────
ERR=0
command -v nc       &>/dev/null || { echo -e "  ${RED}✗ nc not found${RESET}";       ERR=1; }
command -v python3  &>/dev/null || { echo -e "  ${RED}✗ python3 not found${RESET}";  ERR=1; }
command -v valgrind &>/dev/null || { echo -e "  ${RED}✗ valgrind not found${RESET}"; ERR=1; }
[ $ERR -ne 0 ] && exit 1

# ─── Find a free port starting from $1 ───────────────────────────────────────
find_free_port() {
    local p="$1"
    while timeout 1 bash -c "echo >/dev/tcp/${HOST}/${p}" 2>/dev/null; do
        p=$((p + 1))
    done
    echo "$p"
}

# ─── Kill current server (silent) ────────────────────────────────────────────
kill_server() {
    if [ -n "$SERVER_PID" ]; then
        kill -INT "$SERVER_PID" 2>/dev/null
        wait "$SERVER_PID" 2>/dev/null
        SERVER_PID=""
    fi
}

# ─── Start server under valgrind on $CURRENT_PORT ────────────────────────────
start_server() {
    local log="${1:-$VALGRIND_LOG}"

    if [ -z "$BINARY" ]; then
        echo -e "  ${YELLOW}⚠ No binary — assuming server already running on ${HOST}:${CURRENT_PORT}${RESET}"
        return
    fi
    [ ! -x "$BINARY" ] && { echo -e "  ${RED}✗ Binary not executable: $BINARY${RESET}"; exit 1; }

    echo -e "  ${BOLD}▶ Starting server on port ${CURRENT_PORT} under valgrind...${RESET}"

    valgrind \
        --leak-check=full \
        --track-origins=yes \
        --show-leak-kinds=all \
        --error-exitcode=42 \
        --log-file="$log" \
        "$BINARY" "$CURRENT_PORT" "$PASS" &>/dev/null &

    SERVER_PID=$!

    local tries=0
    while ! timeout 1 bash -c "echo >/dev/tcp/${HOST}/${CURRENT_PORT}" 2>/dev/null; do
        sleep 0.2
        tries=$((tries+1))
        if ! kill -0 "$SERVER_PID" 2>/dev/null; then
            echo -e "  ${RED}✗ Server exited immediately — check binary${RESET}"
            exit 1
        fi
        if [ $tries -ge 25 ]; then
            echo -e "  ${RED}✗ Server did not start within 5s on port ${CURRENT_PORT}${RESET}"
            kill "$SERVER_PID" 2>/dev/null
            exit 1
        fi
    done
    echo -e "  ${GREEN}✓ Server started (PID $SERVER_PID) on port ${CURRENT_PORT}${RESET}\n"
}

# ─── Check if server is alive; restart on next port if not ───────────────────
ensure_server() {
    local suite="$1" log="${2:-$VALGRIND_LOG}"
    if [ -n "$SERVER_PID" ] && ! kill -0 "$SERVER_PID" 2>/dev/null; then
        CRASH_COUNT=$((CRASH_COUNT + 1))
        VALGRIND_FAIL=1
        local new_port
        new_port=$(find_free_port $((CURRENT_PORT + 1)))
        echo -e "\n  ${RED}${BOLD}⚠ Server crashed during ${suite}! (was port ${CURRENT_PORT})${RESET}"
        echo -e "  ${YELLOW}↻ Restarting on port ${new_port}...${RESET}"
        CURRENT_PORT="$new_port"
        SERVER_PID=""
        start_server "$log"
        return 1   # signal: was restarted
    fi
    return 0
}

# ─── Parse valgrind log and print report ─────────────────────────────────────
parse_valgrind() {
    local log="$1" label="$2"
    local fail=0

    echo -e "\n${BOLD}${CYAN}══════════════════════════════════════════════════${RESET}"
    echo -e "${BOLD}${CYAN}  Valgrind report — ${label}${RESET}"
    echo -e "${BOLD}${CYAN}══════════════════════════════════════════════════${RESET}"

    if [ ! -f "$log" ]; then
        echo -e "  ${YELLOW}⚠ Log not found: $log${RESET}"; return
    fi

    if grep -q "Process terminating with default action of signal 11" "$log"; then
        echo -e "  [${RED}SEGFAULT${RESET}] Server crashed with SIGSEGV"
        fail=1
    elif grep -q "Process terminating with default action of signal" "$log"; then
        local sig
        sig=$(grep "default action of signal" "$log" | grep -oE 'signal [0-9]+' | head -1)
        echo -e "  [${RED}CRASH${RESET}]   Server killed by $sig"
        fail=1
    fi

    local invalid
    invalid=$(grep -E "Invalid (read|write)" "$log" 2>/dev/null | wc -l | tr -d ' ')
    invalid=${invalid:-0}
    if [ "$invalid" -gt 0 ] 2>/dev/null; then
        echo -e "  [${RED}FAIL${RESET}]    $invalid invalid read/write error(s)"
        fail=1
    else
        echo -e "  [${GREEN}PASS${RESET}]    No invalid reads/writes"
    fi

    local uaf
    uaf=$(grep -E "Invalid read.*(recently freed)" "$log" 2>/dev/null | wc -l | tr -d ' ')
    uaf=${uaf:-0}
    if [ "$uaf" -gt 0 ] 2>/dev/null; then
        echo -e "  [${RED}FAIL${RESET}]    Use-after-free detected"
        fail=1
    else
        echo -e "  [${GREEN}PASS${RESET}]    No use-after-free"
    fi

    local def_lost
    def_lost=$(grep "definitely lost:" "$log" | grep -oE '[0-9,]+ bytes' | head -1)
    if [ -n "$def_lost" ] && [ "$def_lost" != "0 bytes" ]; then
        echo -e "  [${RED}FAIL${RESET}]    Definite leak: $def_lost"
        fail=1
    else
        echo -e "  [${GREEN}PASS${RESET}]    No definite memory leaks"
    fi

    local ind_lost
    ind_lost=$(grep "indirectly lost:" "$log" | grep -oE '[0-9,]+ bytes' | head -1)
    if [ -n "$ind_lost" ] && [ "$ind_lost" != "0 bytes" ]; then
        echo -e "  [${YELLOW}WARN${RESET}]    Indirect leak: $ind_lost"
    else
        echo -e "  [${GREEN}PASS${RESET}]    No indirect memory leaks"
    fi

    local err_summary
    err_summary=$(grep "ERROR SUMMARY:" "$log" | tail -1)
    if echo "$err_summary" | grep -q "0 errors"; then
        echo -e "  [${GREEN}PASS${RESET}]    $err_summary"
    else
        echo -e "  [${RED}FAIL${RESET}]    $err_summary"
        fail=1
    fi

    if [ "$fail" -eq 1 ]; then
        VALGRIND_FAIL=1
        echo -e "\n  ${YELLOW}── Excerpt ──${RESET}"
        grep -v "^==[0-9]*==\s*$" "$log" | head -35 \
            | sed 's/^/  | /' 
        echo -e "  ${YELLOW}Full log: $log${RESET}"
    fi
}

# ─── Cleanup on Ctrl-C ────────────────────────────────────────────────────────
cleanup() {
    echo -e "\n${YELLOW}Interrupted — shutting down...${RESET}"
    kill_server
    exit 130
}
trap cleanup INT TERM

# ══════════════════════════════════════════════════════════════════════════════
# RUN
# ══════════════════════════════════════════════════════════════════════════════

# Find a free starting port
CURRENT_PORT=$(find_free_port "$BASE_PORT")
[ "$CURRENT_PORT" != "$BASE_PORT" ] && \
    echo -e "  ${YELLOW}⚠ Port $BASE_PORT in use — using $CURRENT_PORT${RESET}\n"

echo -e "  ${BOLD}Target  :${RESET} ${HOST}:${CURRENT_PORT}"

rm -f "$VALGRIND_LOG" "$VALGRIND_LOG_PY"
start_server "$VALGRIND_LOG"

extract() {
    # Try plain "N passed/failed/total" first, then try inside box lines
    local val
    val=$(echo "$1" | grep -oE "[0-9]+[[:space:]]+$2" | grep -oE '[0-9]+' | tail -1)
    echo "${val:-0}"
}

# ─── nc tests ─────────────────────────────────────────────────────────────────
echo -e "${BOLD}${CYAN}▶ Running nc tests... (port ${CURRENT_PORT})${RESET}"
NC_OUT=$(bash "${DIR}/irc_tester.sh" "$HOST" "$CURRENT_PORT" "$PASS" 2>&1)
echo "$NC_OUT"
NC_P=$(extract "$NC_OUT" passed)
NC_F=$(extract "$NC_OUT" failed)
NC_T=$(extract "$NC_OUT" total)

ensure_server "nc tests" "$VALGRIND_LOG"
NC_CRASHED=$?

sleep 0.5

# ─── Python tests — always on current (possibly new) port ────────────────────
echo -e "\n${BOLD}${CYAN}▶ Running Python socket tests... (port ${CURRENT_PORT})${RESET}"
PY_OUT=$(python3 "${DIR}/irc_tester.py" "$HOST" "$CURRENT_PORT" "$PASS" 2>&1)
echo "$PY_OUT"
PY_P=$(extract "$PY_OUT" passed)
PY_F=$(extract "$PY_OUT" failed)
PY_T=$(extract "$PY_OUT" total)

ensure_server "Python tests" "$VALGRIND_LOG_PY"

# ─── Shut down and report valgrind ────────────────────────────────────────────
kill_server
wait 2>/dev/null

parse_valgrind "$VALGRIND_LOG"    "nc suite"
[ -f "$VALGRIND_LOG_PY" ] && parse_valgrind "$VALGRIND_LOG_PY" "Python suite"

# ─── Combined summary ─────────────────────────────────────────────────────────
TP=$(( ${NC_P:-0} + ${PY_P:-0} ))
TF=$(( ${NC_F:-0} + ${PY_F:-0} ))
TT=$(( ${NC_T:-0} + ${PY_T:-0} ))

echo -e "\n${BOLD}╔══════════════════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}║                  COMBINED RESULTS                   ║${RESET}"
echo -e "${BOLD}╠══════════════════════════════════════════════════════╣${RESET}"
printf "${BOLD}║  %-8s  %3s passed  %3s failed  %3s total           ║${RESET}\n" "[nc]"      "${NC_P:-0}" "${NC_F:-0}" "${NC_T:-0}"
printf "${BOLD}║  %-8s  %3s passed  %3s failed  %3s total           ║${RESET}\n" "[python]"  "${PY_P:-0}" "${PY_F:-0}" "${PY_T:-0}"
echo -e "${BOLD}╠══════════════════════════════════════════════════════╣${RESET}"
printf "${BOLD}║  %-8s  %3s passed  %3s failed  %3s total           ║${RESET}\n" "[IRC]"     "$TP"        "$TF"        "$TT"
if [ "$CRASH_COUNT" -gt 0 ]; then
    printf "${BOLD}║  %-8s  ${RED}%s${RESET}${BOLD}                           ║${RESET}\n" "[crashes]" "${CRASH_COUNT}x server crash (restarted)"
fi
if [ "${VALGRIND_FAIL:-0}" -eq 1 ]; then
    printf "${BOLD}║  %-8s  ${YELLOW}%-40s${RESET}${BOLD}  ║${RESET}\n" "[valgrind]" "⚠ warnings (see logs)"
else
    printf "${BOLD}║  %-8s  ${GREEN}%-40s${RESET}${BOLD}  ║${RESET}\n" "[valgrind]" "CLEAN"
fi
echo -e "${BOLD}╚══════════════════════════════════════════════════════╝${RESET}"

if [ "$TF" -eq 0 ] && [ "$CRASH_COUNT" -eq 0 ]; then
    echo -e "\n  ${GREEN}${BOLD}All IRC tests passed ✓${RESET}"
    [ "${VALGRIND_FAIL:-0}" -eq 1 ] && \
        echo -e "  ${YELLOW}Valgrind reported warnings — see logs above${RESET}"
    echo ""
    exit 0
else
    [ "$TF"           -gt 0 ] && echo -e "\n  ${RED}${BOLD}${TF} IRC test(s) failed${RESET}"
    [ "$CRASH_COUNT"  -gt 0 ] && echo -e "  ${RED}${BOLD}Server crashed ${CRASH_COUNT} time(s) during testing${RESET}"
    [ "${VALGRIND_FAIL:-0}" -eq 1 ] && \
        echo -e "  ${YELLOW}Valgrind reported warnings — see logs above${RESET}"
    echo ""
    exit 1
fi
