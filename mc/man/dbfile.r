.SH "Dbase III file format"
dBASE III stores all records in a mixed binary/ASCII format.
Character fields are stored padded with spaces (20H rather than nulls).
Numbers are right justified within their field and again padded with spaces.
Logical fields are single bytes containing T,t,y,F,f,n.
.P
The structure of dBASE III files is as follows:
.DS
dBASE III DATABASE FILE HEADER:
+--------+----------------+-------------------------------+
| BYTE   |    CONTENTS    |          MEANING              |
+--------+----------------+-------------------------------+
| 0      | 1 byte         | dBASE III version number      |
|        |                |  (03H without a .DBT file)    |
|        |                |  (83H with a .DBT file)       |
+--------+----------------+-------------------------------+
| 1-3    | 3 bytes        | date of last update           |
|        |                |  (YY MM DD) in binary format  |
+--------+----------------+-------------------------------+
| 4-7    | 32 bit number  | number of records in data file|
+--------+----------------+-------------------------------+
| 8-9    | 16 bit number  | length of header structure    |
+--------+----------------+-------------------------------+
| 10-11  | 16 bit number  | length of the record          |
+--------+----------------+-------------------------------+
| 12-31  | 20 bytes       | reserved bytes (version 1.00) |
+--------+----------------+-------------------------------+
| 32-n   | 32 bytes each  | field descriptor array        |
|        |                |  (see below)                  |-+
+--------+----------------+-------------------------------+ |
| n+1    | 1 byte         | 0DH as the field terminator   | |
+--------+----------------+-------------------------------+ |
|                                                           |
|                                                           |
A FIELD DESCRIPTOR:      <----------------------------------+
+--------+----------------+-------------------------------+
| BYTE   |    CONTENTS    |          MEANING              |
+--------+----------------+-------------------------------+
| 0-10   | 11 bytes       | field name in ASCII zero-fill |
+--------+----------------+-------------------------------+
| 11     | 1 byte         | field type in ASCII           |
|        |                |  (C N F L D or M)             |
+--------+----------------+-------------------------------+
| 12-15  | 32 bit number  | field data address            |
|        |                |  (address is set in memory)   |
+--------+----------------+-------------------------------+
| 16     | 1 byte         | field length in binary        |
+--------+----------------+-------------------------------+
| 17     | 1 byte         | field decimal count in binary |
+--------+----------------+-------------------------------+
| 18-31  | 14 bytes       | reserved bytes (version 1.00) |
+--------+----------------+-------------------------------+
.DE
.P
The data records are layed out as follows:
.IP 1.
Data records are preceeded by one byte that is a
space (20H) if the record is not deleted and an
asterisk (2AH) if it is deleted.
.IP 2.
Data fields are packed into records with no field
separators or record terminators.
.IP 3.
Data types are stored in ASCII format as follows:
.DS
DATA TYPE	DATA RECORD STORAGE
---------	--------------------------------------------
Character	(ASCII characters)
Numeric	- . 0 1 2 3 4 5 6 7 8 9
Logical	? Y y N n T t F f  (? when not initialized)
Memo	(10 digits representing a .DBT block number)
Date	(8 digits in YYYYMMDD format, such as
	19840704 for July 4, 1984)
.DE
.P
This information came directly from the Ashton-Tate Forum.
It can also be found in the Advanced Programmer's Guide
available from Ashton-Tate.
.P
One slight difference occurs between files created by dBASE III and those
created by dBASE III Plus.
In the earlier files, there is an ASCII NUL
character between the $0D end of header indicator and the start of the
data.
This NUL is no longer present in Plus, making a Plus header one byte
smaller than an identically structured III file.
.B db2rdb
will work with either version of dBASE III and writes files which may
be used by either (see Plus mode option).
.SH "/rdb file format"
/rdb stores all records in a plain ASCII format.
It uses a fully transparent and easy-to-understand
file format to store databases.
It is organized like a table.
The columns of the table are separated with
.B <tab>
(09H),
the rows with
.B <newline>
(0CH).
At the top of each column is a column name and a dashed row to separate the 
names from the data.
