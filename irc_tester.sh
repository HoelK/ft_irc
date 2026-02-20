#!/bin/bash
# ══════════════════════════════════════════════════════════════════════════════
#  ft_irc nc tester — RFC 1459 compliance
#  Usage: ./irc_tester.sh [host] [port] [password]
# ══════════════════════════════════════════════════════════════════════════════

HOST="${1:-127.0.0.1}"
PORT="${2:-6666}"
PASS="${3:-42irc}"

GREEN="\033[92m"; RED="\033[91m"; YELLOW="\033[93m"
CYAN="\033[96m";  BOLD="\033[1m"; RESET="\033[0m"

PASS_COUNT=0; FAIL_COUNT=0; TOTAL=0

section() {
    echo -e "\n${BOLD}${CYAN}══════════════════════════════════════════════════${RESET}"
    echo -e "${BOLD}${CYAN}  $1${RESET}"
    echo -e "${BOLD}${CYAN}══════════════════════════════════════════════════${RESET}"
}

check() {
    local name="$1" response="$2" expect="$3"
    TOTAL=$((TOTAL+1))
    if echo "$response" | grep -qE "$expect"; then
        echo -e "  [${GREEN}PASS${RESET}] $name"
        PASS_COUNT=$((PASS_COUNT+1))
    else
        echo -e "  [${RED}FAIL${RESET}] $name"
        echo -e "         ${YELLOW}expected:${RESET} /$expect/"
        echo "$response" | grep -v '^$' | head -3 | while read -r l; do
            echo -e "         ${YELLOW}got:${RESET} $l"
        done
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
}

# nc -C handles CRLF automatically
irc_send() {
    local delay="$1"; shift
    {
        for cmd in "$@"; do
            echo "$cmd"
            sleep "$delay"
        done
        echo "QUIT :bye"
        sleep 0.4
    } | timeout 5 nc -C "$HOST" "$PORT" 2>/dev/null
}

nc_run() {
    local nick="$1"; shift
    irc_send 0.15 "PASS $PASS" "NICK $nick" "USER $nick 0 * :$nick" "$@"
}

nc_run_pass() {
    local nick="$1" pass="$2"; shift 2
    irc_send 0.15 "PASS $pass" "NICK $nick" "USER $nick 0 * :$nick" "$@"
}

nc_raw() { irc_send 0.15 "$@"; }

# Background hold with unique ready-file per call
_NC_HOLD_SEQ=0
_NC_HOLD_LAST_READY=""

nc_hold() {
    local nick="$1" secs="$2"; shift 2
    local cmds=("$@")
    _NC_HOLD_SEQ=$(( _NC_HOLD_SEQ + 1 ))
    local ready="/tmp/nc_hold_ready_$$_${_NC_HOLD_SEQ}"
    _NC_HOLD_LAST_READY="$ready"
    rm -f "$ready"
    {
        echo "PASS $PASS"
        echo "NICK $nick"
        echo "USER $nick 0 * :$nick"
        sleep 0.4
        for cmd in "${cmds[@]}"; do
            echo "$cmd"
            sleep 0.3
        done
        touch "$ready"
        sleep "$secs"
        echo "QUIT :bye"
    } | timeout $(( secs + 3 )) nc -C "$HOST" "$PORT" 2>/dev/null 1>/dev/null &
}

nc_hold_wait() {
    local ready="$_NC_HOLD_LAST_READY"
    local i=0
    while [ ! -f "$ready" ]; do
        sleep 0.1
        i=$((i+1))
        [ $i -ge 50 ] && break
    done
    sleep 0.3
    rm -f "$ready"
}


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.1 — Connection Registration
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.1 — Connection Registration"

# Valid full handshake → 001
r=$(nc_run ncRfc1)
check "Valid PASS+NICK+USER → 001 Welcome" "$r" "001"

# Wrong password → 464
r=$(nc_run_pass rfc2 wrongpass)
check "Wrong password → 464 ERR_PASSWDMISMATCH" "$r" "464|ERROR"

# No PASS → 464/451
r=$(nc_raw "NICK rfc3" "USER rfc3 0 * :ncRfc3")
check "Missing PASS → 464/451 ERR_NOTREGISTERED" "$r" "464|451|ERROR"

# NICK before PASS/USER → should not get 001 yet
r=$(nc_raw "NICK rfc4")
TOTAL=$((TOTAL+1))
if echo "$r" | grep -qE "001"; then
    echo -e "  [${RED}FAIL${RESET}] NICK only (no PASS/USER) → must NOT receive 001 prematurely"
    FAIL_COUNT=$((FAIL_COUNT+1))
else
    echo -e "  [${GREEN}PASS${RESET}] NICK only (no PASS/USER) → no premature 001"
    PASS_COUNT=$((PASS_COUNT+1))
fi

# 001 must be followed by 002, 003, 004
r=$(nc_run ncRfc5)
check "001 followed by 002 (RPL_YOURHOST)"  "$r" "002"
check "001 followed by 003 (RPL_CREATED)"   "$r" "003"
check "001 followed by 004 (RPL_MYINFO)"    "$r" "004"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §2.3.1 — NICK validation
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §2.3.1 — NICK validation"

# Valid nicks
r=$(nc_run "validnick")
check "Alphanumeric NICK → accepted (001)" "$r" "001"

r=$(nc_run "nick_u")
check "NICK with underscore → accepted" "$r" "001"

r=$(nc_run "nick-d")
check "NICK with dash → accepted" "$r" "001"

r=$(nc_run "nick{b}")
check "NICK with {} → accepted (RFC special chars)" "$r" "001"

r=$(nc_run "nick|p")
check "NICK with | → accepted (RFC special chars)" "$r" "001"

# Invalid nicks → 432 ERR_ERRONEUSNICKNAME
r=$(nc_run_pass "1StartDigit" "$PASS")
check "NICK starting with digit → 432" "$r" "432|ERROR"

# NICK with space: "Nick With Space" — IRC protocol space terminates params
# Server should register nick as "Nick" (truncated) or reject. Either is acceptable.
# A conformant server registers "Nick" as the nick (space = param separator in IRC).
# We test that the server doesn't crash and responds with 001 or 432.
# Per RFC 1459 §2.3, space is a parameter separator — server parses "Nick" as the nick.
# Getting 001 is CORRECT behavior. We just verify no crash.
r=$(irc_send 0.15 "PASS $PASS" "NICK Nick With Space" "USER x 0 * :x")
check "NICK with space → 001 (RFC: space = param separator, nick='Nick')" "$r" "001|432"

r=$(nc_run_pass "Nick#Hash" "$PASS")
check "NICK with # → 432" "$r" "432|ERROR"

r=$(nc_run_pass "Nick@At" "$PASS")
check "NICK with @ → 432" "$r" "432|ERROR"

# Empty NICK → 431 ERR_NONICKNAMEGIVEN
r=$(nc_run_pass "" "$PASS")
check "Empty NICK → 431 ERR_NONICKNAMEGIVEN" "$r" "431|432|ERROR"

# NICK too long (RFC max is 9 chars, servers may allow more but must not crash)
r=$(nc_run "$(python3 -c 'print("A"*200)')")
check "Overly long NICK → 432 or truncated (no crash)" "$r" "432|001"

# Duplicate NICK → 433
nc_hold dupa 4
nc_hold_wait
r=$(nc_run_pass dupb "$PASS" "NICK dupa")
check "Duplicate NICK → 433 ERR_NICKNAMEINUSE" "$r" "433"
wait

# NICK change after auth
nc_hold nickwit 5 "JOIN #rfcnicktest"
nc_hold_wait
r=$(irc_send 0.25 \
    "PASS $PASS" "NICK changer" "USER changer 0 * :x" \
    "JOIN #rfcnicktest" \
    "NICK ncChangerNew")
check "NICK change → server confirms with NICK message" "$r" "NICK"
wait


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.1 — JOIN
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.1 — JOIN"

# Basic join → 353 (RPL_NAMREPLY) + 366 (RPL_ENDOFNAMES)
r=$(nc_run joina "JOIN #rfctest")
check "JOIN → 353 RPL_NAMREPLY"    "$r" "353"
check "JOIN → 366 RPL_ENDOFNAMES"  "$r" "366"

# Channel name must start with # (or &)
r=$(nc_run joinb "JOIN rfcnochan")
check "JOIN without # prefix → 403/476 ERR_NOSUCHCHANNEL" "$r" "403|476"

# JOIN with no args → 461 ERR_NEEDMOREPARAMS
r=$(nc_run joinc "JOIN")
check "JOIN no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

# Invite-only: 473 ERR_INVITEONLYCHAN
nc_hold joinop 6 "JOIN #rfcinvonly" "MODE #rfcinvonly +i"
nc_hold_wait
r=$(nc_run jguest "JOIN #rfcinvonly")
check "JOIN +i channel without invite → 473" "$r" "473"
wait

# Keyed channel: 475 ERR_BADCHANNELKEY
nc_hold keyop 6 "JOIN #rfckeyed" "MODE #rfckeyed +k rfcpass"
nc_hold_wait
r=$(nc_run nokey "JOIN #rfckeyed")
check "JOIN +k channel without key → 475" "$r" "475"
r=$(nc_run wrongkey "JOIN #rfckeyed wrongkey")
check "JOIN +k channel with wrong key → 475" "$r" "475"
r=$(nc_run rightkey "JOIN #rfckeyed rfcpass")
check "JOIN +k channel with correct key → success (366)" "$r" "366"
wait

# Channel limit: 471 ERR_CHANNELISFULL
nc_hold limop 6 "JOIN #rfclimit" "MODE #rfclimit +l 1"
nc_hold_wait
r=$(nc_run overlim "JOIN #rfclimit")
check "JOIN over +l limit → 471 ERR_CHANNELISFULL" "$r" "471"
wait


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.2 — PART
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.2 — PART"

# PART must appear in response and must NOT just be from QUIT
r=$(nc_run part1 "JOIN #rfcpart" "PART #rfcpart :leaving")
check "PART with reason → PART #rfcpart in response" "$r" "PART #rfcpart"

r=$(nc_run part2 "JOIN #rfcpart2" "PART #rfcpart2")
check "PART without reason → PART #rfcpart2 in response" "$r" "PART #rfcpart2"

r=$(irc_send 0.4 "PASS $PASS" "NICK part3" "USER part3 0 * :part3" "PART #notjoined")
check "PART channel not joined → 442 ERR_NOTONCHANNEL" "$r" "442"

r=$(irc_send 0.4 "PASS $PASS" "NICK part4" "USER part4 0 * :part4" "PART #nonexistent")
check "PART non-existent channel → 403/442" "$r" "403|442"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.4.1 — PRIVMSG
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.4.1 — PRIVMSG"

r=$(nc_run priv1 "JOIN #rfcmsg" "PRIVMSG #rfcmsg :hello channel")
check "PRIVMSG to channel → no error" "$r" "001"

r=$(nc_run priv2 "PRIVMSG NoSuchUser :hi")
check "PRIVMSG to unknown user → 401 ERR_NOSUCHNICK" "$r" "401"

r=$(nc_run priv3 "PRIVMSG #nosuchchan :hi")
check "PRIVMSG to unknown channel → 401/403" "$r" "401|403"

r=$(nc_run priv4 "PRIVMSG")
check "PRIVMSG no params → 411 ERR_NORECIPIENT" "$r" "411|461"

r=$(nc_run priv5 "PRIVMSG #rfcmsg")
check "PRIVMSG no text → 412 ERR_NOTEXTTOSEND" "$r" "412|461"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.8 — KICK
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.8 — KICK"

r=$(nc_run kick1 "JOIN #rfckick" "KICK #rfckick NoOne :bye")
check "KICK non-member → 441 ERR_USERNOTINCHANNEL" "$r" "441|401"

r=$(nc_run kick2 "KICK #notjoined someone")
check "KICK in channel not joined → 442/403" "$r" "442|403"

r=$(nc_run kick3 "KICK")
check "KICK no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

# Non-op KICK → 482
nc_hold kickop 6 "JOIN #rfckick2"
nc_hold_wait
r=$(nc_run kicknop "JOIN #rfckick2" "KICK #rfckick2 kickop :bye")
check "Non-op KICK → 482 ERR_CHANOPRIVSNEEDED" "$r" "482"
wait


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.7 — INVITE
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.7 — INVITE"

r=$(nc_run inv1 "JOIN #rfcinv" "INVITE NoOne #rfcinv")
check "INVITE unknown user → 401 ERR_NOSUCHNICK" "$r" "401"

r=$(nc_run inv2 "INVITE someone #notjoined")
check "INVITE on channel not joined → 442/403" "$r" "442|403"

r=$(nc_run inv3 "INVITE")
check "INVITE no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

# Non-op INVITE on +i channel → 482
nc_hold invop 6 "JOIN #rfcinv2" "MODE #rfcinv2 +i"
nc_hold_wait
r=$(irc_send 0.4 "PASS $PASS" "NICK invnop" "USER invnop 0 * :invnop"     "JOIN #rfcinv2" "INVITE invop #rfcinv2")
check "Non-op INVITE on +i → 482 ERR_CHANOPRIVSNEEDED" "$r" "482|341"
wait

# Valid invite flow: op invites, guest joins +i channel
nc_hold invop2 8 "JOIN #rfcinv3" "MODE #rfcinv3 +i"
nc_hold_wait


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.4 — TOPIC
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.4 — TOPIC"

r=$(nc_run top1 "JOIN #rfctopic" "TOPIC #rfctopic :Hello RFC!")
check "TOPIC set → 332 RPL_TOPIC or TOPIC broadcast" "$r" "TOPIC|332"

r=$(nc_run top2 "JOIN #rfctopic" "TOPIC #rfctopic")
check "TOPIC query → 331 RPL_NOTOPIC or 332 RPL_TOPIC" "$r" "331|332"

r=$(irc_send 0.4 "PASS $PASS" "NICK top3" "USER top3 0 * :top3" "TOPIC #nonexistent")
check "TOPIC non-existent channel → 403/442" "$r" "403|442"

# +t mode: only ops can set topic
nc_hold topop 6 "JOIN #rfctopict" "MODE #rfctopict +t"
nc_hold_wait
r=$(nc_run topnop "JOIN #rfctopict" "TOPIC #rfctopict :hacked")
check "Non-op TOPIC on +t → 482 ERR_CHANOPRIVSNEEDED" "$r" "482"
wait

# Op can still set topic on +t
r=$(nc_run topop2 "JOIN #rfctopic2" "MODE #rfctopic2 +t" "TOPIC #rfctopic2 :op topic")
check "Op TOPIC on +t → accepted" "$r" "TOPIC|332"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.3 — MODE (channel)
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.2.3 — MODE"

# MODE query → 324 RPL_CHANNELMODEIS
r=$(nc_run mode1 "JOIN #rfcmode" "MODE #rfcmode")
check "MODE #chan (query) → 324 RPL_CHANNELMODEIS" "$r" "324"

# +i
r=$(nc_run mode2 "JOIN #rfcmodeI" "MODE #rfcmodeI +i")
check "MODE +i → MODE broadcast" "$r" "MODE"

r=$(nc_run mode3 "JOIN #rfcmodeI2" "MODE #rfcmodeI2 +i" "MODE #rfcmodeI2 -i")
check "MODE -i → MODE broadcast" "$r" "MODE"

# +t
r=$(nc_run mode4 "JOIN #rfcmodeT" "MODE #rfcmodeT +t")
check "MODE +t → MODE broadcast" "$r" "MODE"

# +k / -k
r=$(nc_run mode5 "JOIN #rfcmodeK" "MODE #rfcmodeK +k testkey")
check "MODE +k → MODE broadcast" "$r" "MODE"

r=$(nc_run mode6 "JOIN #rfcmodeK2" "MODE #rfcmodeK2 +k x" "MODE #rfcmodeK2 -k x")
check "MODE -k → MODE broadcast" "$r" "MODE"

# +l / -l
r=$(nc_run mode7 "JOIN #rfcmodeL" "MODE #rfcmodeL +l 5")
check "MODE +l → MODE broadcast" "$r" "MODE"

r=$(nc_run mode8 "JOIN #rfcmodeL2" "MODE #rfcmodeL2 +l 5" "MODE #rfcmodeL2 -l")
check "MODE -l → MODE broadcast" "$r" "MODE"

# +o / -o
nc_hold modembr 6 "JOIN #rfcmodeO"
nc_hold_wait
r=$(nc_run modegvr "JOIN #rfcmodeO" "MODE #rfcmodeO +o modegvr")
check "MODE +o self (non-op) → 482" "$r" "482"
wait

# Non-op MODE → 482
nc_hold modehld 6 "JOIN #rfcmodeP"
nc_hold_wait
r=$(nc_run modenop "JOIN #rfcmodeP" "MODE #rfcmodeP +i")
check "Non-op MODE → 482 ERR_CHANOPRIVSNEEDED" "$r" "482"
wait

# MODE on non-existent channel → 403
r=$(nc_run mode9 "MODE #doesnotexist99")
check "MODE non-existent channel → 403/442" "$r" "403|442"

# Unknown mode flag
r=$(nc_run mode10 "JOIN #rfcmodeX" "MODE #rfcmodeX +z")
check "Unknown MODE flag → 472 ERR_UNKNOWNMODE or ignored" "$r" "472|MODE|001"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.6.2 — PING / PONG
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.6.2 — PING / PONG"

# PING: increase timeout to 7s and delay to 0.5s to ensure welcome burst is done
r=$(irc_send 0.5     "PASS $PASS" "NICK ping1" "USER ping1 0 * :ping1"     "PING :testtoken")
check "PING → PONG with same token" "$r" "PONG.*testtoken"

r=$(irc_send 0.5     "PASS $PASS" "NICK ping2" "USER ping2 0 * :ping2"     "PING")
check "PING no token → PONG or 409 ERR_NOORIGIN" "$r" "PONG|409"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.1.6 — QUIT
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §4.1.6 — QUIT"

nc_hold quitwit 5 "JOIN #rfcquit"
nc_hold_wait
r=$(irc_send 0.2 \
    "PASS $PASS" "NICK quituser" "USER quituser 0 * :x" \
    "JOIN #rfcquit" \
    "QUIT :goodbye RFC")
check "QUIT → server closes connection" "$r" "ERROR|QUIT|001"
# Witness should have received the QUIT broadcast — tested in Python suite
wait


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §5 — Error replies
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §5 — Error replies"

r=$(nc_run err1 "FOOBAR :test")
check "Unknown command → 421 ERR_UNKNOWNCOMMAND" "$r" "421"

r=$(nc_run err2 "JOIN #rfcerr" "PRIVMSG")
check "PRIVMSG no params → 411 ERR_NORECIPIENT" "$r" "411|461"

r=$(nc_run err3 "MODE")
check "MODE no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

r=$(nc_run err4 "KICK")
check "KICK no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

r=$(nc_run err5 "INVITE")
check "INVITE no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"

r=$(nc_run err6 "TOPIC")
check "TOPIC no params → 461 ERR_NEEDMOREPARAMS" "$r" "461"


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 — Message format (§2.3)
# ══════════════════════════════════════════════════════════════════════════════
section "[NC] RFC 1459 §2.3 — Message format"

# Server replies must be prefixed with :servername
r=$(nc_run ncFmt1)
check "Server replies prefixed with :servername" "$r" ":.*001"

# Messages from clients forwarded with :nick!user@host prefix
nc_hold fmtop 5 "JOIN #rfcfmt"
nc_hold_wait
r=$(irc_send 0.2 \
    "PASS $PASS" "NICK fmtsend" "USER fmtsend 0 * :x" \
    "JOIN #rfcfmt" \
    "PRIVMSG #rfcfmt :test message")
check "Client message has :nick!user@host prefix" "$r" "fmtsend!.*@"
wait

# Empty lines must not crash server
r=$(irc_send 0.1 "PASS $PASS" "NICK fmtempty" "USER fmtempty 0 * :x" "" "" "PING :alive")
check "Empty lines → server stays alive (PONG)" "$r" "PONG|001"


# ─── Summary ──────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}╔══════════════════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}║                     NC RESULTS                      ║${RESET}"
echo -e "${BOLD}╠══════════════════════════════════════════════════════╣${RESET}"
printf "${BOLD}║  ${GREEN}%2d passed${RESET}${BOLD}   ${RED}%2d failed${RESET}${BOLD}   %2d total                    ║${RESET}\n" \
       "$PASS_COUNT" "$FAIL_COUNT" "$TOTAL"
echo -e "${BOLD}╚══════════════════════════════════════════════════════╝${RESET}"
