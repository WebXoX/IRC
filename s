<< CAP LS
<< NICK joe
<< USER joe joe irc.libera.chat :joe
>> :iridium.libera.chat NOTICE * :*** Checking Ident
>> :iridium.libera.chat NOTICE * :*** Looking up your hostname...
>> :iridium.libera.chat NOTICE * :*** Couldn't look up your hostname
>> :iridium.libera.chat NOTICE * :*** No Ident response
>> :iridium.libera.chat CAP * LS :account-notify away-notify chghost extended-join multi-prefix sasl tls account-tag cap-notify echo-message server-time solanum.chat/identify-msg solanum.chat/oper solanum.chat/realhost
<< CAP REQ :multi-prefix
>> :iridium.libera.chat CAP joe ACK :multi-prefix
<< CAP END
>> :iridium.libera.chat 001 joe :Welcome to the Libera.Chat Internet Relay Chat Network joe
<< MODE joe +i
>> :iridium.libera.chat 002 joe :Your host is iridium.libera.chat[188.240.145.100/6697], running version solanum-1.0-dev
>> :iridium.libera.chat 003 joe :This server was created Thu Nov 16 2023 at 06:25:11 UTC
>> :iridium.libera.chat 004 joe iridium.libera.chat solanum-1.0-dev DGIMQRSZaghilopsuwz CFILMPQRSTbcefgijklmnopqrstuvz bkloveqjfI
>> :iridium.libera.chat 005 joe ACCOUNTEXTBAN=a KNOCK SAFELIST ELIST=CMNTU MONITOR=100 CALLERID=g FNC WHOX ETRACE CHANTYPES=# EXCEPTS INVEX :are supported by this server
>> :iridium.libera.chat 005 joe CHANMODES=eIbq,k,flj,CFLMPQRSTcgimnprstuz CHANLIMIT=#:250 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=Libera.Chat STATUSMSG=@+ CASEMAPPING=rfc1459 NICKLEN=16 MAXNICKLEN=16 CHANNELLEN=50 TOPICLEN=390 :are supported by this server
>> :iridium.libera.chat 005 joe DEAF=D TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,agjrxz :are supported by this server
>> :iridium.libera.chat 251 joe :There are 68 users and 33274 invisible on 28 servers
>> :iridium.libera.chat 252 joe 40 :IRC Operators online
>> :iridium.libera.chat 253 joe 138 :unknown connection(s)
>> :iridium.libera.chat 254 joe 22819 :channels formed
>> :iridium.libera.chat 255 joe :I have 2376 clients and 1 servers
>> :iridium.libera.chat 265 joe 2376 2939 :Current local users 2376, max 2939
>> :iridium.libera.chat 266 joe 33342 37286 :Current global users 33342, max 37286
>> :iridium.libera.chat 250 joe :Highest connection count: 2940 (2939 clients) (358030 connections received)
>> :iridium.libera.chat 375 joe :- iridium.libera.chat Message of the Day - 
>> :iridium.libera.chat 372 joe :- This server provided by NORDUnet/SUNET
>> :iridium.libera.chat 372 joe :- Welcome to Libera Chat, the IRC network for
>> :iridium.libera.chat 372 joe :- free & open-source software and peer directed projects.
>> :iridium.libera.chat 372 joe :-  
>> :iridium.libera.chat 372 joe :- Use of Libera Chat is governed by our network policies.
>> :iridium.libera.chat 372 joe :-  
>> :iridium.libera.chat 372 joe :- To reduce network abuses we perform open proxy checks
>> :iridium.libera.chat 372 joe :- on hosts at connection time.
>> :iridium.libera.chat 372 joe :-  
>> :iridium.libera.chat 372 joe :- Please visit us in #libera for questions and support.
>> :iridium.libera.chat 372 joe :-  
>> :iridium.libera.chat 372 joe :- Website and documentation:  https://libera.chat
>> :iridium.libera.chat 372 joe :- Webchat:                    https://web.libera.chat
>> :iridium.libera.chat 372 joe :- Network policies:           https://libera.chat/policies
>> :iridium.libera.chat 372 joe :- Email:                      support@libera.chat
>> :iridium.libera.chat 376 joe :End of /MOTD command.
>> :joe MODE joe :+Ziw
>> :NickServ!NickServ@services.libera.chat NOTICE joe :This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY joe_oblivian <password>
>> :NickServ!NickServ@services.libera.chat NOTICE joe :You have 30 seconds to identify to your nickname before it is changed.
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< JOIN #aa
>> :joe!~joe@94.205.23.240 JOIN #aa
>> :iridium.libera.chat MODE #aa +Cnst
>> :iridium.libera.chat 353 joe @ #aa :@joe
>> :iridium.libera.chat 366 joe #aa :End of /NAMES list.
<< MODE #aa
>> :services. MODE #aa -o joe   
>> :iridium.libera.chat NOTICE #aa :*** Notice -- TS for #aa changed from 1712988169 to 1623864597
>> :services. MODE #aa +p-C 
>> :ChanServ!ChanServ@services.libera.chat JOIN #aa
>> :services. MODE #aa +o ChanServ   
>> :iridium.libera.chat 324 joe #aa +npst
--> chanquery mode
<< WHO #aa
>> :iridium.libera.chat 329 joe #aa 1623864597
--> event 329
>> :iridium.libera.chat 352 joe #aa ChanServ services.libera.chat services. ChanServ H@ :0 Channel Services
--> silent event who
>> :iridium.libera.chat 352 joe #aa ~joe 94.205.23.240 iridium.libera.chat joe H :0 joe
--> silent event who
>> :iridium.libera.chat 315 joe #aa :End of /WHO list.
--> chanquery who end
<< MODE #aa b
>> :iridium.libera.chat 368 joe #aa :End of Channel Ban List
--> chanquery ban end
>> :NickServ!NickServ@services.libera.chat NOTICE joe :You failed to identify in time for the nickname joe
>> :joe!~joe@94.205.23.240 NICK :Guest6319
<< MODE o+i
>> :iridium.libera.chat 403 Guest6319 o+i :No such channel
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< MODE #aa +i
>> :iridium.libera.chat 482 Guest6319 #aa :You're not a channel operator
<< MODE #aa -i
>> :iridium.libera.chat 482 Guest6319 #aa :You're not a channel operator
<< PART #aa :aa
>> :Guest6319!~joe@94.205.23.240 PART #aa
<< JOIN #jowa
>> :Guest6319!~joe@94.205.23.240 JOIN #jowa
>> :iridium.libera.chat MODE #jowa +Cnst
>> :iridium.libera.chat 353 Guest6319 @ #jowa :@Guest6319
>> :iridium.libera.chat 366 Guest6319 #jowa :End of /NAMES list.
<< MODE #jowa
>> :iridium.libera.chat 324 Guest6319 #jowa +Cnst
--> chanquery mode
<< WHO #jowa
>> :iridium.libera.chat 329 Guest6319 #jowa 1712988288
--> event 329
>> :iridium.libera.chat 352 Guest6319 #jowa ~joe 94.205.23.240 iridium.libera.chat Guest6319 H@ :0 joe
--> silent event who
>> :iridium.libera.chat 315 Guest6319 #jowa :End of /WHO list.
--> chanquery who end
<< MODE #jowa b
>> :iridium.libera.chat 368 Guest6319 #jowa :End of Channel Ban List
--> chanquery ban end
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< MODE #jowa +oi
>> :iridium.libera.chat 461 Guest6319 MODE :Not enough parameters
<< MODE #jowa +i
>> :Guest6319!~joe@94.205.23.240 MODE #jowa +i 
<< MODE +o+i
>> :iridium.libera.chat 403 Guest6319 +o+i :No such channel
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
<< MODE +ti
>> :iridium.libera.chat 403 Guest6319 +ti :No such channel
<< MODE #jowa -i
>> :Guest6319!~joe@94.205.23.240 MODE #jowa -i 
<< MODE #jowa +ti
>> :Guest6319!~joe@94.205.23.240 MODE #jowa +i 
<< MODE #jowa +tiafa
>> :iridium.libera.chat 472 Guest6319 a :is an unknown mode char to me
<< MODE #jowa +tafa
>> :iridium.libera.chat 472 Guest6319 a :is an unknown mode char to me
<< MODE #jowa +tafawoasdf
>> :iridium.libera.chat 472 Guest6319 a :is an unknown mode char to me
<< MODE #jowa +tafawoasdf adsf
>> :iridium.libera.chat 472 Guest6319 a :is an unknown mode char to me
<< PING iridium.libera.chat
>> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
--> lag pong
