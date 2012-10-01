Description
===========
In computer science, a sparse file is a type of computer file that attempts to use file system space more efficiently when blocks allocated to the file are mostly empty. This is achieved by writing brief information (metadata) representing the empty blocks to disk instead of the actual "empty" space which makes up the block, using less disk space. The full block size is written to disk as the actual size only when the block contains "real" (non-empty) data.

Wikipedia (http://en.wikipedia.org/wiki/Sparse_file)

How to use?
-----------
`SparseFile.exe C:\testfile 1024`

creates C:\testfile with 1024 bytes.

Could be used to create fake files.