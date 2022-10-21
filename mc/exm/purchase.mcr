# purchase.mcr
# sample mc-macro
# goto ordernumber cell and input given ordernumber
{goto}c5~
$ORDER~

# mark the range for printing
{goto}i1~{mark}{goto}q18~{mark}

# print the output part
/p~n

# quit without save
/qn
