# Message Format

An IRC message is a single line, delimited by a pair of CR ('\r', 0x0D) and LF ('\n', 0x0A) characters.

- When reading messages from a stream, read the incoming data into a buffer. Only parse and process a message once you encounter the \r\n at the end of it. If you encounter an empty message, silently ignore it.
- When sending messages, ensure that a pair of \r\n characters follows every single message your software sends out.

Messages have this format, as rough ABNF:
```
message         ::= ['@' <tags> SPACE] [':' <source> SPACE] <command> <parameters> <crlf>
SPACE           ::=  %x20 *( %x20 )   ; space character(s)
crlf            ::=  %x0D %x0A        ; "carriage return" "linefeed"
```
The specific parts of an IRC message are:

- `tags`: Optional metadata on a message, starting with ('@', 0x40).
- `source`: Optional note of where the message came from, starting with (':', 0x3A).
- `command`: The specific command this message represents.
- `parameters`: If it exists, data relevant to this specific command.
These message parts, and parameters themselves, are separated by one or more ASCII SPACE characters (' ', 0x20).

Most IRC servers limit messages to `512 bytes` in length, including the trailing CR-LF characters. Implementations which include message tags need to allow additional bytes for the tags section of a message; clients must allow 8191 additional bytes and servers must allow 4096 additional bytes.

## Tags

Basically, a series of `<key>[=<value>]` segments, separated by `(';', 0x3B)`.

The tags part is optional, and MUST NOT be sent unless explicitly enabled by a `capability`. This message part starts with a leading ('@', 0x40) character, which MUST be the first character of the message itself. The leading ('@', 0x40) is stripped from the value before it gets processed further.

## Source
```
source          ::=  <servername> / ( <nickname> [ "!" <user> ] [ "@" <host> ] )
nick            ::=  <any characters except NUL, CR, LF, chantype character, and SPACE> <possibly empty sequence of any characters except NUL, CR, LF, and SPACE>
user            ::=  <sequence of any characters except NUL, CR, LF, and SPACE>
```
The source (formerly known as prefix) is optional and starts with a (':', 0x3A) character (which is stripped from the value), and if there are no tags it MUST be the first character of the message itself.

The source indicates the true origin of a message. If the source is missing from a message, it’s is assumed to have originated from the client/server on the other end of the connection the message was received on.
```
Clients MUST NOT include a source when sending a message.
```
Servers MAY include a source on any message, and MAY leave a source off of any message. Clients MUST be able to process any given message the same way whether it contains a source or does not contain one.

## Command
```
command         ::=  letter* / 3digit
```
The `command` must either be a valid IRC command or a numeric (a three-digit number represented as text).


## Parameters

This is the format of the parameters part:
```
  parameter       ::=  *( SPACE middle ) [ SPACE ":" trailing ]
  nospcrlfcl      ::=  <sequence of any characters except NUL, CR, LF, colon (`:`) and SPACE>
  middle          ::=  nospcrlfcl *( ":" / nospcrlfcl )
  trailing        ::=  *( ":" / " " / nospcrlfcl )
```

```
parameter ::= *( SPACE middle ) [ SPACE ":" trailing ]
```

- This line defines the structure of a single parameter.
- `parameter`: This represents the complete parameter, which provides additional information within an IRC message.
- `::=`: This symbol signifies a definition.
- `*( SPACE middle )`: This part defines the mandatory section of a parameter:
    - `*`: This indicates zero or more repetitions of the following element.
    - `SPACE`: This refers to a space character, separating different parameters.
    - `middle`: This refers to the core content of the parameter, explained in the next line.
- `[ SPACE ":" trailing ]`: This defines an optional section of a parameter:
    - `[]`: This indicates the entire section within brackets is optional.
    - `SPACE`: This refers to a space character, separating the mandatory and optional parts.
    - `:`: This colon (":") acts as a separator between the mandatory and optional parts.
    - `trailing`: This refers to the optional additional details explained in the following line.

```
nospcrlfcl ::= <sequence of any characters except NUL, CR, LF, colon (:) and SPACE>
```

- This line defines what characters are allowed in the middle part of a parameter (essentially, the core information).
- `nospcrlfcl`: This is a name given to this group of allowed characters.
- `::=`: This symbol signifies a definition.
- `<sequence of any characters...>`: This indicates any sequence of characters is allowed, as long as they are:
    - `Not NUL` (null character): This is a special character that usually represents nothing.
    - `Not CR` (carriage return): This character typically moves the cursor to the beginning of the next line.
    - `Not LF` (line feed): This character typically moves the cursor to the next line.
    - `Not colon` (":"): This colon is reserved for separating the mandatory and optional parts.
    - `Not SPACE`: This space is already accounted for in the previous line.

```
middle ::= nospcrlfcl *( ":" / nospcrlfcl )
```
- This line further defines the structure of the middle part of a parameter.
- `middle`: This refers back to the core content section of a parameter.
- `::=`: This symbol signifies a definition.
- `nospcrlfcl`: This refers back to the group of allowed characters defined in line 2.
- `*`: This indicates zero or more repetitions of the following element.
- `( ":" / nospcrlfcl )`: This defines what can be repeated within the middle part:
    - `:`: This colon can be used to separate elements within the core content (similar to how a hyphen might separate parts of a compound word).
    - `/`: This indicates an "or" condition.
    - `nospcrlfcl`: This allows additional sequences of acceptable characters (like the first part).


```
trailing ::= *( ":" / " " / nospcrlfcl )
```

- This line defines the structure of the optional `trailing` part of a parameter.
- `trailing`: This refers to the additional details that can follow the colon in a parameter.
- `::=`: This symbol signifies a definition.
- `*`: This indicates zero or more repetitions of the following element.
- `( ":" / " " / nospcrlfcl )`: This defines what can be included in the optional trailing part:
    - `:`: This colon can be used to further separate elements within the optional details.
    - `" "`: This allows including spaces within the optional details.
    - `nospcrlfcl`: This allows additional sequences of acceptable characters (like the previous lines).



`Parameters` (or ‘params’) are extra pieces of information added to the end of a message. These parameters generally make up the ‘data’ portion of the message. What specific parameters mean changes for every single message.

Parameters are a series of values separated by one or more ASCII SPACE characters `(' ', 0x20)`. However, this syntax is insufficient in two cases: a parameter that contains one or more spaces, and an empty parameter. To permit such parameters, the final parameter can be prepended with a `(':', 0x3A)` character, in which case that character is stripped and the rest of the message is treated as the final parameter, including any spaces it contains. Parameters that contain spaces, are empty, or begin with a `':'` character MUST be sent with a preceding `':'`; in other cases the use of a preceding `':'` on the final parameter is OPTIONAL.

Software SHOULD AVOID sending more than 15 parameters, as older client protocol documents specified this was the maximum and some clients may have trouble reading more than this. However, clients MUST parse incoming messages with any number of them.