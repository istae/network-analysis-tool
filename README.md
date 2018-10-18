# Network Analysis Tool
The tool constructs unweighed networks and performs analysis on the network such as diameter of the network or the shortest path between two nodes.

## Input
The tool can construct networks from files or stdin. The input must be formatted in the following way:
- Each node label must be placed in a newline.
- Connection between nodes are done by placing neighbors nodes one after another in newlines.
- To start a new branch in the network, a single '.' is placed in a new line. 

Example:
```
love
finds
a
way
.
love
in
first
sight
. 
```
In the above example, the word 'love' has two neighbors: 'finds' and 'in'. 'Finds' is connected to 'a' which then connects to 'way'.
In the other branch, 'in' connects to 'first' and 'first' to 'sight'. Notice that '.' is used in a new line to start a new brach.

## Flags

- '-f filename':  file input formatted with the rules above. (see 'old.txt' adopted from Old Man and the Sea by Hemingway)
- '-b': prints the hash balance of the hash table used to assign unique node lables and ids.

## Commands
  ```
  [Commands] type 'commands' for the list
  top=(number),
  total=('e' or 'n'),
  shortest path=(label, label),
  shortest path all=(label, label),
  neighbors=(label),
  count=(label),
  is neighbor=(label, label),
  betweenness=(label),
  close cent=(label),
  diameter
  edge density
  print all
  commands
  clear
  hash balance
  exit
in 0.0100 (sec)
  ```
  
After each '=' put a label name (or two in some cases) to run the commands

Example:

  ```
  demo -f old.txt
  
  betweenness=the
calculating all shortest paths...
[99%] 1907 of 1926
done
total shortest paths: 3442602
'the' is in 1351146 shortest paths
betweenness score: 0.392
in 19.4540 (sec)
  ```
  
