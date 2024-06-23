# Extensible Hash Table

The current implementation doesn't store a <Key, Value> pair but only a Integer which acts as a key and value both. The removal function doesn't merge and coalese the buckets back when removing.

Report:

-   Insertion is the slowest among insert, remove, find functions taking 2 seconds for 5 million insertions
-   Removal taking 1 second for 5 million removals
-   Find is the fastest taking only 0.4 seconds
