[33mcommit 088c062b1022ede958d601a7fa0a6a6c98c186ad[m[33m ([m[1;36mHEAD -> [m[1;32mmain[m[33m, [m[1;31morigin/main[m[33m, [m[1;31morigin/HEAD[m[33m)[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 17 19:50:47 2026 +0100

    fixed Privw

[33mcommit 414a6696dae458783d46e9b708090a8bf049d9d7[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 17 01:48:18 2026 +0100

    added more security

[33mcommit ae12d8a0d2b1b24da314e4657f93d484f7e009d5[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 17 01:20:47 2026 +0100

    fixed topic and small fixes

[33mcommit 05695fa48b8b794c3c10e30e695752b7412eef12[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 17 00:53:07 2026 +0100

    small fix

[33mcommit a0417195713dc608701f975e59be6239052a7358[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 23:44:12 2026 +0100

    added real timestamp for RPL_CREATED

[33mcommit b9f68d61b8e1e99cc19a207f6f94f79299798924[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 23:35:05 2026 +0100

    deleted trailling '\r\n' in cmdData

[33mcommit 89abd3eba4b15bf4047f373407cedc201f980f76[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 20:31:53 2026 +0100

    replace bool op in clients by vector of clients in channels fixing major bug

[33mcommit f2eba73ebafd090644581ed2b2bd7e99eb2e09a2[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 20:16:53 2026 +0100

    fixed invite

[33mcommit 620c26797a770e88467bf0ec018c234ecb4c8d82[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 19:15:42 2026 +0100

    fixed auth before using pass

[33mcommit 1690b5ca51424dfcd33dd239e720e2d09dcbbcd9[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 19:08:45 2026 +0100

    added ERR_PASSWDMISMATCH when aptempting connection with wrong password

[33mcommit b46517019aaada8a6cd09337dccb07c4d108b803[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 19:02:49 2026 +0100

    fixed segfault with mode ' '

[33mcommit 0dd4cf7752b747f33182bf061f41a57fe8f792ff[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 16:41:22 2026 +0100

    updated todo.txt

[33mcommit 878ad996b2bc3a2f22fd83d1806bbf31e8a4435c[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 16:13:17 2026 +0100

    stupidity

[33mcommit bbd673419b9b0aed69cff56951d4444f6a40bebd[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 15:20:46 2026 +0100

    refactor rpl::priv

[33mcommit acfac232e7c52f84759fca8f1977e8965005034f[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 15:11:29 2026 +0100

    re fixed a my mistakes

[33mcommit 1e60d96fb198a45f4cba8fa0f502cb1fd0799e28[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 14:46:40 2026 +0100

    allowed non op to see channel modes

[33mcommit 3d2f8718d99ef27ed3b68e2f1ed9da834d5b839b[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 14:38:50 2026 +0100

    repaired another bug i created

[33mcommit 0d241dddac572e9dffa0aaffc1b2262fa46a0224[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 11:31:37 2026 +0100

    Fixed segfault

[33mcommit 8528ea44f8385a79de33866240657b2a40628893[m
Author: Derhen David <dedavid@c1r3p9.42lehavre.fr>
Date:   Mon Feb 16 11:21:34 2026 +0100

    Changed cmd_data to cmdData and rpl_data to rplData

[33mcommit 2388a80d8364a0e456ac2c62c26a0d644aae68f3[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 06:31:38 2026 +0100

    added todos

[33mcommit 0cfd88d66aa13af17693dabbe44ec38301d9d6e8[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 16 06:06:52 2026 +0100

    fixed Kick command, cleared code

[33mcommit 32679ad503a9e296140a4bf6c3e81f9165fe194f[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 21:56:25 2026 +0100

    fixed Nick cmd, stopped rpl_data dependencie

[33mcommit 0d97d949fe062ccf0d6820ab1d207ffb1cbe6a99[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 20:41:56 2026 +0100

    fixed wrong pass not disconnecting

[33mcommit af7dd640afe9e59b42c115f041f961626e77d00d[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 19:05:19 2026 +0100

    fixed leaks and small issue

[33mcommit 520597c98a9841d9cd3d3135856027106e27908f[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 17:55:25 2026 +0100

    reordered arguements

[33mcommit a68a2849ef0c62cab43b951279f324558357f9bd[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 17:46:36 2026 +0100

    added function to valid channel names

[33mcommit 9593cf6a1674e890110e7cefe23624bb21de9288[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 17:38:29 2026 +0100

    fixed headers directory

[33mcommit 6ba95aefe239528f28e4d3e1eb57d96e1dc1e644[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 17:25:17 2026 +0100

    added auto response 'feur' from 'quoi'

[33mcommit fee0d0a1a0ec7b9d5dc6a70a8ff5ada73fa08086[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 05:21:22 2026 +0100

    Improved Quit command

[33mcommit 3453ce528b43bc84865554e457ed8db263fa2c3d[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 05:08:50 2026 +0100

    added User class

[33mcommit d1c40b528fc1374b65c927415091948370ae213f[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 04:50:06 2026 +0100

    added Nick class

[33mcommit 7ebdb5c6ec19bec05d8b9b53e906699156462acf[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 15 04:36:49 2026 +0100

    added privmsg class

[33mcommit c50a75ec2272675cb6aba70a36b89220ab975116[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 20:11:50 2026 +0100

    added Topic class

[33mcommit 2fcc710b947ed3beaf81eb8193abf3cc91c7a23b[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 19:40:45 2026 +0100

    compilation fix

[33mcommit 8029bb21601d1589ec30cba9af34290260656141[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:57:52 2026 +0100

    small fixe

[33mcommit 25af9975425b8a82239f0913e94ccdf25d71c022[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:57:14 2026 +0100

    started reworking Kick command (does not compile)

[33mcommit 1ee95be700aa1d5220580eedbf23bad59c8b4618[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:42:08 2026 +0100

    small fixes

[33mcommit 373963c20f616ef2a49fee1ae70f6c9c19754706[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:14:00 2026 +0100

    small fixes

[33mcommit 1fd3c3e8aad0ce4808a63b613fb7c8e27e9f9dad[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:13:42 2026 +0100

    added Invite and Join classes

[33mcommit c3def18839153b91cfddb75ef3d9fd777948375e[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:12:46 2026 +0100

    Added Invite Class, improving readability and bugs by alot

[33mcommit 0e84e748d2f1c38d25ed6b6ee43426b7f9ddb668[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 06:11:52 2026 +0100

    small fixes

[33mcommit cdf08931954b27c32bd178c67a22e0caf76d3de4[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 05:54:54 2026 +0100

    improved join clarity by alot fixing bugs on the way

[33mcommit b71102eaa281a7816e31f7063eca9950382ad526[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 01:01:26 2026 +0100

    updated readme

[33mcommit 94c9652a6cf729acd1e38cace387142809691803[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 14 01:00:23 2026 +0100

    fixed nick in channels and basic mode functionnality

[33mcommit e4932490b9c0a5b7f84e3e3d9c34a1ac8f5baad9[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Thu Feb 12 21:38:37 2026 +0100

    small fixes

[33mcommit 39e73bfd3c3d68b77569123e4c35f77934042e9f[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Thu Feb 12 18:57:14 2026 +0100

    small fixes

[33mcommit 014170897b4359016c8fdc2602fc00c1f933f651[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Thu Feb 12 06:12:25 2026 +0100

    started adding modes, probably doesn't work (didn't test)

[33mcommit d5dae9f98710c879e92e823f7e3095dfa64adee5[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 20:49:45 2026 +0100

    started implementing invites

[33mcommit f4ef156159047cf36b4ecded1762ff9895385b15[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 19:34:54 2026 +0100

    added security for accept limit

[33mcommit cfb659cd6f7ec5fcc61714fe4cb8b1136af4893e[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 08:09:58 2026 +0100

    deleted nonsense while true causing unexpected disconnections

[33mcommit 5947821ded82f70e4a53c6522776fcad7d51d818[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 07:59:52 2026 +0100

    fixed unexpected disconnection fails

[33mcommit 13173983f20db27092ce1f1466f7bbeef0823f30[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 05:06:26 2026 +0100

    added README.md

[33mcommit a86861a446708430122c12ae1ab822546db33862[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 04:47:37 2026 +0100

    started adding errors to commandfs

[33mcommit fae14fd1ab8efbdcc34f51269d205bc8d1b22159[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 03:44:30 2026 +0100

    added errors

[33mcommit e644802c2d878c8a8ed065a254ea210a41d43999[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 11 00:03:39 2026 +0100

    added signal

[33mcommit 77f273fd4291e1983630c4413271cb27a21b7380[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 10 22:55:12 2026 +0100

    I don't remember what ive done

[33mcommit dbfe4e0cbeba7a46e530da884aed234ddc615230[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 10 07:14:17 2026 +0100

    started implementing errors

[33mcommit 68f3f3f7fe7f62415fd9725c6b16921d5a3ce26d[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 10 05:43:23 2026 +0100

    improved readability of RPL, maybe broke a shit or two

[33mcommit 3cc6443ca5bab81c91752b438d99a3a805747fb4[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 10 03:21:09 2026 +0100

    started adding topic command

[33mcommit 7e80f7b29c28db8006b61e258bafd9bbcd9735c2[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Tue Feb 10 01:39:50 2026 +0100

    started implementing kick command

[33mcommit 47a24001f93c16fbb879d5f48c060e01f97c5cc4[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 9 23:02:03 2026 +0100

    added channel communication

[33mcommit 30b11c922a817a238220b32a638a65efc7ab795b[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Mon Feb 9 06:07:34 2026 +0100

    fixed channels in client class

[33mcommit dfafa1fca3b7d225d7c49676fd0775bd80290244[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Mon Feb 9 05:44:03 2026 +0100

    fixed channels

[33mcommit 2d58bef5c15df0ac95dd9a4d9eb232773f00253b[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Mon Feb 9 03:38:21 2026 +0100

    added join

[33mcommit fa43c1a92e128007f65391bfb658b6b7e1afcc8e[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 23:44:08 2026 +0100

    removed vgcores

[33mcommit 006622ad13deac5579ec14844a22bdcfc77c0731[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 23:41:27 2026 +0100

    removed id system to identify clients with fd

[33mcommit d625f7099505275937f520f2b76865f5819477a3[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 20:26:53 2026 +0100

    changes reply system and cleaned code

[33mcommit 3d659e2106f84000073d08be63378b8a9ae29fcc[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 17:38:51 2026 +0100

    reworked client map fixing major bugs

[33mcommit 9fd531c36696d4a5d3b6bc08cc9cf00ab7900d5e[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 04:56:09 2026 +0100

    removed unecessary map causing issues

[33mcommit d55ef58d52a1359f1de89dc161ccf08db5013716[m
Author: HoelK <KeromnesHoel@gmail.com>
Date:   Sun Feb 8 02:09:37 2026 +0100

    fixed connection and disconnections

[33mcommit 608bc900532a5556551aea5e8b9bbcdfc7d01e84[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 7 22:24:12 2026 +0100

    replace pollfd array by vector

[33mcommit f143030c8e9d68f7ca47d025cd615fb50490a8fe[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 7 20:31:12 2026 +0100

    structured project working directory

[33mcommit 344522deb207166fa787c60a6ecacdef4bd07b5f[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 7 20:26:58 2026 +0100

    temporary parsing fix

[33mcommit ffda5c017953966e48c51a54940f9e0ae284965d[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 7 18:45:42 2026 +0100

    implemented authentification

[33mcommit c3c49339489ceb00b2ddcbd2ab05b05b0760239b[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Feb 7 04:54:47 2026 +0100

    added parsing and logic pipeline

[33mcommit 5dcfe3f2863eeb9fdb501f4c8bcac473b2378f38[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Wed Feb 4 01:54:43 2026 +0100

    added basic socket connection

[33mcommit 741dc64f87673f8aa9b76d8290e39f6fa88a7237[m
Author: Samuel Bonneau <sbonneau@c1r4p11.42lehavre.fr>
Date:   Mon Feb 2 16:08:08 2026 +0100

    tmrlaputehoel

[33mcommit adf83b1178c93e871d0c89d25c34f177aabca550[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Jan 31 22:57:01 2026 +0100

    added Server Class

[33mcommit ea1cc944863b73ddc91e5034b9b1c9392da2005f[m
Author: Hoël <KeromnesHoel@gmail.com>
Date:   Sat Jan 31 21:44:38 2026 +0100

    first commit
