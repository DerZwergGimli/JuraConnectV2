# JuraConnect V2
This is a simple project for communication with a Jura Impressa Coffee Maker. 
This project is using a ESP8266 (NodeMCU).

### Tested with:
- Jura Impressa S70
- Jura Impressa S95

### Communication:

- UART (TTL 5V) [9600 8N1]

### Connection:

| Pin           | Signal        | GPIO  |
|:------------- |:-------------:| -----:|
| 5             | +5V           | V(in) |
| 4             | n.c.          |  n.c. |
| 3             | RxD           |    D1 |
| 2             | GND           |   GND |
| 1             | TxD           |    D2 |

### Commands:

| Function                           | Command |
|:-----------------------------------|---------|
| machine on                         | AN:01   |
| machine off                        | AN:02   |
| test display                       | AN:03   |
| key test                           | AN:05   |
| tests via keys on coffeemaker      | AN:06   |
| ERASE EEPROM (NEVER DO THIS!!!)    | AN:0A   |
| test UCHI                          | AN:0C   |
| Date?                              | AN:0D   |
| make product 1                     | FA:01   |
| make product 2                     | FA:02   |
| make product 3                     | FA:03   |
| make product 4                     | FA:04   |
| make product 5                     | FA:05   |
| make product 6                     | FA:06   |
| make product 7                     | FA:07   |
| make hot water                     | FA:08   |
| make steam                         | FA:09   |
| flushing                           | FA:0B   |
| pump coffee on                     | FN:01   |
| pump coffee off                    | FN:02   |
| coffee heating on                  | FN:03   |
| coffee heating off                 | FN:04   |
| steam heating on                   | FN:05   |
| steam heating off                  | FN:06   |
| grinder left on                    | FN:07   |
| grinder left off                   | FN:08   |
| grinder right on                   | FN:09   |
| grinder right off                  | FN:0A   |
| pump steam on                      | FN:0B   |
| pump steam off                     | FN:0C   |
| init brewgroup                     | FN:0D   |
| brewgroup in grinding position     | FN:0F   |
| brewgroup?                         | FN:11   |
| brewgroup?                         | FN:12   |
| brewgroup in brewing position      | FN:13   |
| product 1 grind                    | FN:14   |
| product 1 pump                     | FN:15   |
| product 3 grind                    | FN:16   |
| product 3 pump                     | FN:17   |
| drainage valve on                  | FN:1D   |
| drainage valve off                 | FN:1E   |
| brewgroup?                         | FN:22   |
| emptying of steam valve on         | FN:24   |
| emptying of steam valve off        | FN:25   |
| steam valve on                     | FN:26   |
| steam valve off                    | FN:27   |
| cappuuccino valve o                | FN:28   |
| cappuuccino valve of               | FN:29   |
| brewgroup put trester of           | FN:0E   |
| read word in EEPROM (type address) | RE:     |
| read line in EEPROM (type address) | RT:     |
| type of machine?                   | TY:     |
| write word in EEPROM               | WE:     |
| read machine type                  | RE:31   |
| set machine type                   | WE:31   |
| read inputs                        | IC:     |
| read RAM                           | RR:     |

# Useful Links:

- <http://protocoljura.wiki-site.com/index.php/Commands_for_coffeemaker>