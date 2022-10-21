Cell	Attrib	Format	Value	Text
------	-	----	----------------------	--------------------------------------------------------------------------------
C5	0	0	1.0000000000000000e+00	windowdef
H	0	16	0.0000000000000000e+00	coldef
L1	8	127	0.0000000000000000e+00	'Purchase Order
A1	1	127	0.0000000000000000e+00	'mcsample/purchase.mc
L2	8	127	0.0000000000000000e+00	'--------------
A3	1	127	0.0000000000000000e+00	'here are the statements, the output is on the right --->
C5	2	127	1.0000000000000000e+00	1
A5	1	127	0.0000000000000000e+00	'Order number:
A7	6	127	0.0000000000000000e+00	i3<cat $RDB/act/Company
A8	6	127	0.0000000000000000e+00	a15<echo &c5 | search -ms $RDB/act/pur/purchaseorder Number
A9	6	127	0.0000000000000000e+00	i7<echo &b17 | search -ms $RDB/act/pur/vendor Number | report $RDB/act/pur/mail.f
A10	6	127	0.0000000000000000e+00	i15<echo &c5 | search -ms $RDB/act/pur/purchaseitem Order
