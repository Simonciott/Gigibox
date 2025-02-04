# String
The string is a type of data that can be stored inside the Data memory of the system. Imagine the string as an array of characters: The first element is the size of the string and that the size of the string is equal to `size + 1`. After that the array is filled with the characters of the string

# Data formatting
| index  |  0  | 1  |  2  |  3  | ... |  [size]  |
|--------|-----|----|-----|-----|-----|----------|
|property|size|char1|char2|char3|char[n]|char[size]|

- size: How many cells/addresses the string occupies (excluding the cell storing the size)
- char[n]: the [n] character if the string