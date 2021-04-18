# Readme

Use command
```
git clone https://github.com/pavel-collab/Hash-Table.git
``` 
to clone this project from Github to your computer. Compile with command:

```
make hash_table
```
and start with command 
```
./ht
```
___
This version of program use lists as a variant of resolving collisions. This mean, that pairs with the same indexes (idx = hash(key) % table_size) construct a list in the hash table's cell.
___
There are some simple actions, that you can do with Hash Table:
- use "Exit" to finish working
- use "MENU" to show the list of avaliable actions in the consol
- use "SET" to push a pair key-value to the hash table; after the command SET input _key_ and _value_ separating them with a spaces; for example:
```
Enter action: SET key value
```
- use "GET" to get a value that connect with given key from the hash table; after the command GET input _key_; for example:
```
Enter action: GET key
```
- use "REMOVE" to delete a pair key-value; example:
```
Enter action: REMOVE key
```
- use "HASH" to show the hash of given string; example:
```
Enter action: HASH string
```
- use "LIST" to get the list with given number; example:
```
Enter action: LIST 4
```
- use "DUMP" to output all information about hash table; The infirmation will be output to the log named "log.txt" in the curent folder.

## Protection mode.
There is a protection mode in the programm. This mode switch on verefication and help to find errors such as:
* Hash Table pointer address is NULL
* Lists pointer address is NULL
* Amount of elements in the table more than table capacity
* Hash Table capacity is 0.
If you want to switch on this mode, open file "hash_table.h" and uncomment line 4.
```
//#define DEBUG
```
```
#define DEBUG
```
After that, you will receive messages about error's presence in the consol.

## Debugging
If you have some problems with the program and want to debbug it, use command:
```
make debug
``` 
this command will compile program with additional information for gdb, after that you can use gdb to debbuging.