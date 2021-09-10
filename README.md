# Cache_Simulator
Program that simulates Reads and Writes to a cache. The program also keeps count of all hits and misses of the addresses provided.
The program takes a first.txt file with R or W which defines the action of read or write and then a hex address. The program uses the tag to recognize if the cache memory is taken and records if its a hit or a miss. The process goes such as,
read->hit
read->miss->write
write->hit
write->miss
