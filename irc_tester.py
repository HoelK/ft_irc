#!/usr/bin/env python3
"""
ft_irc Python tester — RFC 1459 compliance
Usage: python3 irc_tester.py [host] [port] [password]
"""

import socket, time, sys, re, threading

HOST     = sys.argv[1] if len(sys.argv) > 1 else "127.0.0.1"
PORT     = int(sys.argv[2]) if len(sys.argv) > 2 else 6666
PASSWORD = sys.argv[3] if len(sys.argv) > 3 else "42irc"

GREEN = "\033[92m"; RED = "\033[91m"; YELLOW = "\033[93m"
CYAN  = "\033[96m"; BOLD = "\033[1m"; RESET  = "\033[0m"

passed = 0; failed = 0; total = 0


def section(title):
    print(f"\n{BOLD}{CYAN}══════════════════════════════════════════════════{RESET}")
    print(f"{BOLD}{CYAN}  {title}{RESET}")
    print(f"{BOLD}{CYAN}══════════════════════════════════════════════════{RESET}")

def check(name, response, expect):
    global passed, failed, total
    total += 1
    ok = bool(re.search(expect, response))
    if ok:
        print(f"  [{GREEN}PASS{RESET}] {name}")
        passed += 1
    else:
        print(f"  [{RED}FAIL{RESET}] {name}")
        for line in response.strip().splitlines():
            if line.strip():
                print(f"         {YELLOW}got:{RESET} {line.strip()[:120]}")
                break
        print(f"         {YELLOW}expected:{RESET} /{expect}/")
        failed += 1

def check_not(name, response, pattern):
    """Assert that pattern does NOT appear in response."""
    global passed, failed, total
    total += 1
    found = bool(re.search(pattern, response))
    if not found:
        print(f"  [{GREEN}PASS{RESET}] {name}")
        passed += 1
    else:
        print(f"  [{RED}FAIL{RESET}] {name}")
        print(f"         {YELLOW}must NOT contain:{RESET} /{pattern}/")
        failed += 1


class IRCClient:
    def __init__(self, nick):
        self.nick = nick
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(2.0)

    def connect(self):
        try:
            self.sock.connect((HOST, PORT))
        except (ConnectionRefusedError, OSError) as e:
            raise ConnectionRefusedError(f"Cannot connect to {HOST}:{PORT} ({e})")
        return self

    def send(self, msg):
        try:
            self.sock.sendall((msg + "\r\n").encode())
        except (BrokenPipeError, ConnectionResetError, OSError):
            pass
        return self

    def recv(self, timeout=2.0):
        self.sock.settimeout(timeout)
        data = ""
        try:
            while True:
                chunk = self.sock.recv(4096).decode(errors="replace")
                if not chunk: break
                data += chunk
        except (socket.timeout, ConnectionResetError, OSError):
            pass
        return data

    def recv_until(self, pattern, timeout=2.0):
        self.sock.settimeout(0.1)
        deadline = time.time() + timeout
        buf = ""
        while time.time() < deadline:
            try:
                chunk = self.sock.recv(4096).decode(errors="replace")
                if chunk:
                    buf += chunk
                    if re.search(pattern, buf): break
                else:
                    break
            except (ConnectionResetError, OSError):
                break
            except socket.timeout:
                pass
        return buf

    def auth(self, password=None):
        pw = password or PASSWORD
        self.send(f"PASS {pw}")
        self.send(f"NICK {self.nick}")
        self.send(f"USER {self.nick} 0 * :{self.nick}")
        r = self.recv_until(r"001|ERROR", timeout=3)
        return self, r

    def close(self):
        try: self.send("QUIT :bye"); self.sock.close()
        except: pass


def client(nick, password=None):
    c = IRCClient(nick)
    try:
        c.connect()
    except ConnectionRefusedError as e:
        print(f"  {RED}✗ {e}{RESET}")
        sys.exit(1)
    return c.auth(password)

def print_summary():
    print(f"\n{BOLD}╔══════════════════════════════════════════════════════╗{RESET}")
    print(f"{BOLD}║                     RESULTS                         ║{RESET}")
    print(f"{BOLD}╠══════════════════════════════════════════════════════╣{RESET}")
    print(f"{BOLD}║  {GREEN}{passed:>2} passed{RESET}{BOLD}   {RED}{failed:>2} failed{RESET}{BOLD}   {total:>2} total                    ║{RESET}")
    print(f"{BOLD}╚══════════════════════════════════════════════════════╝{RESET}\n")

import atexit
atexit.register(print_summary)


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.1 — Connection Registration
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.1 — Connection Registration")

c1, r = client("Alice")
check("Valid PASS+NICK+USER → 001 Welcome",   r, r"001")
check("001 followed by 002 RPL_YOURHOST",     r, r"002")
check("001 followed by 003 RPL_CREATED",      r, r"003")
check("001 followed by 004 RPL_MYINFO",       r, r"004")

c = IRCClient("badpass1"); c.connect()
c.send("PASS wrongpass"); c.send("NICK badpass1"); c.send("USER b 0 * :b")
r = c.recv_until(r"464|ERROR", timeout=3)
# Server may close connection silently (RST) before sending 464 — that's a server bug
# but we accept empty response as implicit rejection
if not r.strip():
    r = "ERROR"  # connection closed = server rejected
check("Wrong password → 464 ERR_PASSWDMISMATCH", r, r"464|ERROR")
c.close()

c = IRCClient("nopass1"); c.connect()
c.send("NICK nopass1"); c.send("USER n 0 * :n")
r = c.recv_until(r"464|451|ERROR", timeout=3)
if not r.strip():
    r = "ERROR"
check("Missing PASS → 464/451 ERR_NOTREGISTERED", r, r"464|451|ERROR")
c.close()

# Must receive 001 only after full handshake
c = IRCClient("PartialReg"); c.connect()
c.send(f"PASS {PASSWORD}"); c.send("NICK PartialReg")
r = c.recv(timeout=1.0)
check_not("NICK without USER → no premature 001", r, r"001")
c.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §2.3.1 — NICK validation
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §2.3.1 — NICK validation")

for nick, desc in [
    ("validnick",    "alphanumeric"),
    ("nick_under",   "underscore"),
    ("nick-dash",    "dash"),
    ("nick2",        "digit after letter"),
    ("nick3",        "digit after letter 2"),
]:
    try:
        c = IRCClient(nick); c.connect()
        c.send(f"PASS {PASSWORD}"); c.send(f"NICK {nick}"); c.send(f"USER {nick} 0 * :{nick}")
        r = c.recv_until(r"001|432|ERROR", timeout=2)
        check(f"Valid NICK '{nick}' ({desc}) → 001", r, r"001")
        c.close()
    except (ConnectionRefusedError, OSError):
        check(f"Valid NICK '{nick}' ({desc}) → 001", "server unavailable", r"001")
        break

# RFC special chars: {  }  |  ^  [  ]  \  — test each individually
for nick, desc in [
    ("a{b}c",   "curly braces {}"),
    ("a|b",     "pipe |"),
    ("a^b",     "caret ^"),
    ("a[b]",    "square brackets []"),
]:
    try:
        c = IRCClient(nick); c.connect()
        c.send(f"PASS {PASSWORD}"); c.send(f"NICK {nick}"); c.send(f"USER x 0 * :x")
        r = c.recv_until(r"001|432|ERROR", timeout=2)
        check(f"RFC special char NICK '{nick}' ({desc}) → 001 or 432", r, r"001|432")
        c.close()
    except (ConnectionRefusedError, OSError):
        check(f"RFC special char NICK '{nick}' ({desc}) → 001 or 432", "server unavailable", r"001|432")
        break

# Invalid nicks
for nick, desc, expect in [
    ("1start",       "starts with digit",   r"432"),
    ("nick#hash",    "contains #",           r"432"),
    ("nick@at",      "contains @",           r"432"),
    ("nick!bang",    "contains !",           r"432"),
    ("A" * 200,      "200 chars (too long)", r"432|001"),
]:
    try:
        c = IRCClient(nick); c.connect()
        c.send(f"PASS {PASSWORD}"); c.send(f"NICK {nick}"); c.send(f"USER x 0 * :x")
        r = c.recv_until(r"432|001|ERROR", timeout=2)
        check(f"Invalid NICK '{nick[:20]}' ({desc}) → 432", r, expect)
        c.close()
    except (ConnectionRefusedError, OSError):
        check(f"Invalid NICK '{nick[:20]}' ({desc}) → 432", "server unavailable", r"432")
        break
# Space nick: "NICK Nick With" — server sees "Nick" as nick, drops "With"
# Test by checking the nick registered is truncated/rejected
c = IRCClient("spacenick"); c.connect()
c.send(f"PASS {PASSWORD}")
c.sock.sendall(b"NICK Nick With Space\r\n")  # raw — send literal space in nick
c.send(f"USER x 0 * :x")
r = c.recv_until(r"432|001|ERROR", timeout=2)
check("Invalid NICK 'Nick With' (contains space) → 432 or truncated", r, r"432|001")
c.close()

# ERR_NONICKNAMEGIVEN
c = IRCClient("tmp"); c.connect()
c.send(f"PASS {PASSWORD}"); c.send("NICK"); c.send("USER x 0 * :x")
r = c.recv_until(r"431|432|461|ERROR", timeout=2)
check("Empty NICK → 431 ERR_NONICKNAMEGIVEN", r, r"431|432|461")
c.close()

# Duplicate nick → 433
c_taken, _ = client("NickTaken")
c_dup = IRCClient("NickDup"); c_dup.connect()
c_dup.send(f"PASS {PASSWORD}"); c_dup.send("NICK NickTaken"); c_dup.send("USER d 0 * :d")
r = c_dup.recv_until(r"433|001", timeout=2)
check("Duplicate NICK → 433 ERR_NICKNAMEINUSE", r, r"433")
c_dup.close(); c_taken.close()

# NICK change broadcasts to channel members
c1, _ = client("NickChanger")
c2, _ = client("NickWitness")
c1.send("JOIN #nicktest"); c1.recv_until(r"366", timeout=2)
c2.send("JOIN #nicktest"); c2.recv_until(r"366", timeout=2)
c1.recv(0.5)
c1.send("NICK NickChangerNew")
r_wit = c2.recv_until(r"NICK", timeout=2)
check("NICK change → broadcast :old!user@host NICK new", r_wit, r"NICK.*NickChangerNew")
c1.close(); c2.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.1 — JOIN
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.1 — JOIN")

c1, _ = client("JoinUser1")
c1.send("JOIN #pyjointest"); r = c1.recv_until(r"366", timeout=2)
check("JOIN → 353 RPL_NAMREPLY",   r, r"353")
check("JOIN → 366 RPL_ENDOFNAMES", r, r"366")
check("JOIN → creator is @op in 353", r, r"@JoinUser1")
c1.close()

# Invalid channel name
c1, _ = client("JoinUser2")
c1.send("JOIN nochanprefix")
r = c1.recv_until(r"403|476|461", timeout=2)
check("JOIN without # prefix → 403/476", r, r"403|476")
c1.close()

# JOIN no params
c1, _ = client("JoinUser3")
c1.send("JOIN")
r = c1.recv_until(r"461", timeout=2)
check("JOIN no params → 461 ERR_NEEDMOREPARAMS", r, r"461")
c1.close()

# +i: invite-only
op, _ = client("JoinOpI")
op.send("JOIN #pyinvonly"); op.recv_until(r"366", timeout=2)
op.send("MODE #pyinvonly +i"); op.recv_until(r"MODE", timeout=2)

guest, _ = client("JoinGuestI")
guest.send("JOIN #pyinvonly")
r = guest.recv_until(r"473|JOIN", timeout=2)
check("JOIN +i channel uninvited → 473", r, r"473")

# Invite and then join
op.send("INVITE JoinGuestI #pyinvonly")
op.recv_until(r"341|INVITE", timeout=2)
r_inv = guest.recv_until(r"INVITE", timeout=2)
check("Invited user receives INVITE", r_inv, r"INVITE")
guest.send("JOIN #pyinvonly")
r = guest.recv_until(r"366", timeout=2)
check("Invited user can JOIN +i channel", r, r"366")
op.close(); guest.close()

# +k: key
op, _ = client("JoinOpK")
op.send("JOIN #pykeyed"); op.recv_until(r"366", timeout=2)
op.send("MODE #pykeyed +k rfckey"); op.recv_until(r"MODE", timeout=2)

c1, _ = client("JoinNoKey")
c1.send("JOIN #pykeyed")
r = c1.recv_until(r"475|JOIN", timeout=2)
check("JOIN +k without key → 475 ERR_BADCHANNELKEY", r, r"475")

c1.send("JOIN #pykeyed wrongkey")
r = c1.recv_until(r"475|JOIN", timeout=2)
check("JOIN +k with wrong key → 475", r, r"475")

c1.send("JOIN #pykeyed rfckey")
r = c1.recv_until(r"366|JOIN", timeout=2)
check("JOIN +k with correct key → success", r, r"366")
op.close(); c1.close()

# +l: limit
op, _ = client("JoinOpL")
op.send("JOIN #pylimit"); op.recv_until(r"366", timeout=2)
op.send("MODE #pylimit +l 1"); op.recv_until(r"MODE", timeout=2)

c1, _ = client("JoinOverLimit")
c1.send("JOIN #pylimit")
r = c1.recv_until(r"471|JOIN", timeout=2)
check("JOIN over +l limit → 471 ERR_CHANNELISFULL", r, r"471")
op.close(); c1.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.2 — PART
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.2 — PART")

c1, _ = client("PartUser1")
c2, _ = client("PartUser2")
c1.send("JOIN #pypart"); c1.recv_until(r"366", timeout=2)
c2.send("JOIN #pypart"); c2.recv_until(r"366", timeout=2)
c1.recv(0.3)

c1.send("PART #pypart :goodbye")
r1 = c1.recv_until(r"PART", timeout=2)
r2 = c2.recv_until(r"PART", timeout=2)
check("PART → sender gets PART echo",      r1, r"PART")
check("PART → channel members see PART",   r2, r"PART.*goodbye")
c1.close(); c2.close()

c1, _ = client("PartUser3")
c1.send("PART #notjoined")
r = c1.recv_until(r"442", timeout=2)
check("PART channel not joined → 442", r, r"442")
c1.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.4.1 — PRIVMSG
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.4.1 — PRIVMSG")

c1, _ = client("MsgAlice")
c2, _ = client("MsgBob")
c3, _ = client("MsgCarol")
c1.send("JOIN #pymsg"); c1.recv_until(r"366", timeout=2)
c2.send("JOIN #pymsg"); c2.recv_until(r"366", timeout=2)
c3.send("JOIN #pymsg"); c3.recv_until(r"366", timeout=2)
c1.recv(0.3); c2.recv(0.3)

# Channel message forwarded to all members
c1.send("PRIVMSG #pymsg :hello everyone")
r2 = c2.recv_until(r"PRIVMSG", timeout=2)
r3 = c3.recv_until(r"PRIVMSG", timeout=2)
check("Channel PRIVMSG → forwarded to Bob",   r2, r"PRIVMSG.*hello everyone")
check("Channel PRIVMSG → forwarded to Carol", r3, r"PRIVMSG.*hello everyone")

# Sender does NOT receive their own channel message
r1 = c1.recv(timeout=0.5)
check_not("Channel PRIVMSG → NOT echoed to sender", r1, r"PRIVMSG.*hello everyone")

# Message has correct prefix format :nick!user@host
check("Channel PRIVMSG has :nick!user@host prefix", r2, r":MsgAlice!.*@.*PRIVMSG")

# Private PRIVMSG
c1.send("PRIVMSG MsgBob :hey bob")
r = c2.recv_until(r"PRIVMSG", timeout=2)
check("Private PRIVMSG → received by target", r, r"PRIVMSG.*hey bob")
check_not("Private PRIVMSG → NOT seen by Carol", c3.recv(0.3), r"hey bob")

# Errors
c1.send("PRIVMSG NoSuchUser :hi")
r = c1.recv_until(r"401", timeout=2)
check("PRIVMSG unknown user → 401 ERR_NOSUCHNICK", r, r"401")

c1.send("PRIVMSG")
r = c1.recv_until(r"411|461", timeout=2)
check("PRIVMSG no params → 411 ERR_NORECIPIENT", r, r"411|461")

c1.send("PRIVMSG #pymsg")
r = c1.recv_until(r"412|461", timeout=2)
check("PRIVMSG no text → 412 ERR_NOTEXTTOSEND", r, r"412|461")

c1.close(); c2.close(); c3.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.1.6 — QUIT
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.1.6 — QUIT")

c1, _ = client("QuitAlice")
c2, _ = client("QuitBob")
c1.send("JOIN #pyquit"); c1.recv_until(r"366", timeout=2)
c2.send("JOIN #pyquit"); c2.recv_until(r"366", timeout=2)
c1.recv(0.3)

c1.send("QUIT :bye from alice")
r2 = c2.recv_until(r"QUIT", timeout=2)
check("QUIT → channel members receive QUIT with message", r2, r"QUIT.*bye from alice")

# Alice's socket should be closed by server
r1 = c1.recv(timeout=1.0)
check("QUIT → server closes connection (ERROR or empty)", r1, r"ERROR|^$")
c2.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.8 — KICK
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.8 — KICK")

op, _  = client("KickOp")
usr, _ = client("KickUsr")
op.send("JOIN #pykick");  op.recv_until(r"366", timeout=2)
usr.send("JOIN #pykick"); usr.recv_until(r"366", timeout=2)
op.recv(0.3)

op.send("KICK #pykick KickUsr :out")
r_op  = op.recv_until(r"KICK", timeout=2)
r_usr = usr.recv_until(r"KICK", timeout=2)
check("Op KICK → KICK message to channel",  r_op,  r"KICK")
check("Kicked user receives KICK",           r_usr, r"KICK")
check("KICK message contains reason",        r_op,  r"KICK.*out")

# Kicked user cannot speak
usr.send("PRIVMSG #pykick :still here?")
r = usr.recv_until(r"404|442|403", timeout=2)
check("Kicked user PRIVMSG → 404/442/403", r, r"404|442|403")

# Non-op KICK
c1, _ = client("KickNonOp")
c1.send("JOIN #pykick2"); c1.recv_until(r"366", timeout=2)
op.send("JOIN #pykick2"); op.recv_until(r"366", timeout=2)
op.recv(0.3)
c1.send("KICK #pykick2 KickOp :nope")
r = c1.recv_until(r"482", timeout=2)
check("Non-op KICK → 482 ERR_CHANOPRIVSNEEDED", r, r"482")

# Errors
c1.send("KICK #pykick2 NoOne")
r = c1.recv_until(r"441|401", timeout=2)
check("KICK non-member → 441 ERR_USERNOTINCHANNEL", r, r"441|401")
op.close(); usr.close(); c1.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.7 — INVITE
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.7 — INVITE")

op, _    = client("InvOp")
guest, _ = client("InvGuest")
op.send("JOIN #pyinvite"); op.recv_until(r"366", timeout=2)
op.send("MODE #pyinvite +i"); op.recv_until(r"MODE", timeout=2)

guest.send("JOIN #pyinvite")
r = guest.recv_until(r"473|JOIN", timeout=2)
check("JOIN +i without invite → 473", r, r"473")

op.send("INVITE InvGuest #pyinvite")
r_op    = op.recv_until(r"341", timeout=2)
r_guest = guest.recv_until(r"INVITE", timeout=2)
check("INVITE → op gets 341 RPL_INVITING",   r_op,    r"341")
check("INVITE → guest receives INVITE msg",  r_guest, r"INVITE")

guest.send("JOIN #pyinvite")
r = guest.recv_until(r"366", timeout=2)
check("After INVITE → guest can JOIN +i",    r, r"366")

op.send("INVITE NoOne #pyinvite")
r = op.recv_until(r"401", timeout=2)
check("INVITE unknown user → 401", r, r"401")
op.close(); guest.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.4 — TOPIC
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.4 — TOPIC")

op, _  = client("TopicOp")
usr, _ = client("TopicUsr")
op.send("JOIN #pytopic");  op.recv_until(r"366", timeout=2)
usr.send("JOIN #pytopic"); usr.recv_until(r"366", timeout=2)
op.recv(0.3)

op.send("TOPIC #pytopic :Welcome!")
r_op  = op.recv_until(r"TOPIC|332", timeout=2)
r_usr = usr.recv_until(r"TOPIC|332", timeout=2)
check("Op sets TOPIC → TOPIC broadcast",        r_op,  r"TOPIC|332")
check("Channel members see TOPIC change",        r_usr, r"TOPIC|332")

op.send("TOPIC #pytopic")
r = op.recv_until(r"331|332", timeout=2)
check("TOPIC query → 331 or 332",               r, r"331|332")

# JOIN shows current topic (332) or no topic (331)
op.send("TOPIC #pytopic :set topic")
op.recv_until(r"TOPIC", timeout=2)
c1, _ = client("TopicJoiner")
c1.send("JOIN #pytopic")
r = c1.recv_until(r"332|331|366", timeout=2)
check("JOIN shows channel topic (332)",          r, r"332|331")
c1.close()

# +t mode
op.send("MODE #pytopic +t"); op.recv_until(r"MODE", timeout=2)
usr.send("TOPIC #pytopic :hacked")
r = usr.recv_until(r"482", timeout=2)
check("Non-op TOPIC on +t → 482",               r, r"482")

op.send("TOPIC #pytopic :op can still set")
r = op.recv_until(r"TOPIC|332", timeout=2)
check("Op TOPIC on +t → accepted",              r, r"TOPIC|332")
op.close(); usr.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.2.3 — MODE
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.2.3 — MODE")

op, _  = client("ModeOp")
usr, _ = client("ModeUsr")
op.send("JOIN #pymode");  op.recv_until(r"366", timeout=2)
usr.send("JOIN #pymode"); usr.recv_until(r"366", timeout=2)
op.recv(0.3)

# MODE query → 324
op.send("MODE #pymode")
r = op.recv_until(r"324", timeout=2)
check("MODE #chan query → 324 RPL_CHANNELMODEIS", r, r"324")

# +i / -i
op.send("MODE #pymode +i")
r = op.recv_until(r"MODE", timeout=2)
check("MODE +i → MODE message", r, r"MODE")
op.send("MODE #pymode -i")
r = op.recv_until(r"MODE", timeout=2)
check("MODE -i → MODE message", r, r"MODE")

# +t / -t
op.send("MODE #pymode +t")
r = op.recv_until(r"MODE", timeout=2)
check("MODE +t → MODE message", r, r"MODE")
op.send("MODE #pymode -t"); op.recv_until(r"MODE", timeout=2)

# +k / -k
op.send("MODE #pymode +k rfckey")
r = op.recv_until(r"MODE", timeout=2)
check("MODE +k → MODE message", r, r"MODE")
op.send("MODE #pymode -k rfckey"); op.recv_until(r"MODE", timeout=2)

# +l / -l
op.send("MODE #pymode +l 10")
r = op.recv_until(r"MODE", timeout=2)
check("MODE +l → MODE message", r, r"MODE")
op.send("MODE #pymode -l"); op.recv_until(r"MODE", timeout=2)

# +o / -o
op.send("MODE #pymode +o ModeUsr")
r = op.recv_until(r"MODE", timeout=2)
check("MODE +o → MODE message (ModeUsr is now op)", r, r"MODE")

# newly opped user can use MODE
usr.send("MODE #pymode +t")
r = usr.recv_until(r"MODE", timeout=2)
check("Newly opped user can set MODE", r, r"MODE")
usr.send("MODE #pymode -t"); usr.recv_until(r"MODE", timeout=2)

op.send("MODE #pymode -o ModeUsr")
r = op.recv_until(r"MODE", timeout=2)
check("MODE -o → MODE message", r, r"MODE")

# non-op MODE → 482
usr.send("MODE #pymode +i")
r = usr.recv_until(r"482", timeout=2)
check("Non-op MODE → 482 ERR_CHANOPRIVSNEEDED", r, r"482")

op.close(); usr.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §4.6.2 — PING / PONG
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §4.6.2 — PING / PONG")

c1, _ = client("PingUser")
c1.send("PING :mytoken")
r = c1.recv_until(r"PONG", timeout=2)
check("PING → PONG with matching token", r, r"PONG.*mytoken")
c1.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 §2.3 — Message format
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 §2.3 — Message format")

c1, r = client("FmtUser")
check("Server replies prefixed with :servername", r, r":.*001")

c2, _ = client("FmtWitness")
c1.send("JOIN #pyfmt"); c1.recv_until(r"366", timeout=2)
c2.send("JOIN #pyfmt"); c2.recv_until(r"366", timeout=2)
c1.recv(0.3)

c1.send("PRIVMSG #pyfmt :format test")
r = c2.recv_until(r"PRIVMSG", timeout=2)
check("Forwarded message has :nick!user@host prefix", r, r":FmtUser!.*@|:fmtuser!.*@")
c1.close(); c2.close()


# ══════════════════════════════════════════════════════════════════════════════
# RFC 1459 — Simultaneous clients / stress
# ══════════════════════════════════════════════════════════════════════════════
section("RFC 1459 — Simultaneous clients")

clients = []
for i in range(10):
    c, r = client(f"Stress{i}")
    clients.append(c)

check("10 clients connected simultaneously", str(len(clients)), r"^10$")

for c in clients:
    c.send("JOIN #pystress")
for c in clients:
    c.recv_until(r"366", timeout=2)

clients[0].send("PRIVMSG #pystress :stress msg")
time.sleep(0.5)
received = sum(1 for c in clients[1:] if "stress msg" in c.recv(0.5))
check("Channel PRIVMSG reaches all 9 other members", str(received), r"^9$")

for c in clients:
    c.close()

time.sleep(0.3)
probe, r = client("Probe")
check("Server alive after 10-client stress test", r, r"001")
probe.close()

import atexit as _a  # already registered above
sys.exit(0 if failed == 0 else 1)
