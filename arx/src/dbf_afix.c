/* $Id: dbf_afix.c,v 1.6 2020/12/07 11:06:07 axel Exp $
 * fix dbf record for advantage distinct values
 *
 * these cannot be found by ADSlocal driver:
 *   [158, 169, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
 *    199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 222, 223, 224,
 *    226, 227, 228, 229, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 242, 243, 244, 245, 247, 249, 251, 252, 254]
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <str_def.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_afix (struct DBF *td)
{
unsigned char fld;
struct FIELD_RECORD *tf;
char *tt;
char buff;
int status;
char noiso8859[] = "\x9e\x9f\xa9\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xde\xdf\xe0\xe2\xe3\xe4\xe5\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf2\xf3\xf4\xf5\xf7\xf9\xfb\xfc\xfe";
#ifdef FIND_THEM_ALL
char codepg437[] = "\x50\x66\xaa\xdd\xdd\xdd\xdd\xdd\xdd\xdd\x2b\x2b\xdd\xdd\x2b\x2b\x2b\x2b\x2b\x2b\x2d\x2d\x2b\x2d\x2b\xdd\xdd\x2b\x2b\x2d\x2d\xdd\x2d\x2b\x2d\x2d\x2d\x2d\x2b\x2b\x2b\x2b\x2b\x2b\x2b\x2b\xdd\x5f\xdd\x2d\x61\x47\x70\x53\x73\x74\x46\x54\x4f\x64\x38\x66\x65\x6e\x3d\x3d\x3d\x28\x29\x7e\xfa\x76\x6e\xdd";
#endif
char codepg850[] = "\x78\x66\xaa\x7c\x7c\x7c\x7c\x7c\x7c\x7c\x2b\x2b\x7c\x7c\x2b\x2b\x2b\x2b\x2b\x2b\x2d\x2d\x2b\x2d\x2b\x7c\x7c\x2b\x2b\x2d\x2d\x7c\x2d\x2b\x2d\x2d\x2d\x2d\x2b\x2b\x2b\x2b\x2b\x2b\x2b\x2b\x7c\x5f\x7c\x2d\x4f\x4f\x70\x53\x73\x74\x46\x54\x4f\x64\x38\x66\x2d\x60\x3d\x3d\x3d\xb6\xa7\x7e\xfa\x76\x6e\x7c";
#ifdef FIND_THEM_ALL
char iso885915[] = "\x78\x66\xac\xa6\xa6\xa6\xa6\xa6\xa6\xa6\x2b\x2b\xa6\xa6\x2b\x2b\x2b\x2b\x2b\x2b\x2d\x2d\x2b\x2d\x2b\xa6\xa6\x2b\x2b\x2d\x2d\xa6\x2d\x2b\x2d\x2d\x2d\x2d\x2b\x2b\x2b\x2b\x2b\x2b\x2b\x2b\xa6\x5f\xa6\xaf\x4f\x4f\x70\x53\x73\x74\x46\x54\x4f\x64\x38\x66\x65\x60\x3d\x3d\x3d\xb6\xa7\x98\xb7\x76\x6e\xa6";
#endif
status = 0;
for (fld=0; fld<td->fields; fld++)
	{
	tf = &td->fields_ptr[fld];
	tf->reserved_bytes[0] = 'u';
	switch (tf->typ)
	 {
	 case 'C':
		tt = td->record_ptr + tf->field_data_address;
		buff = tt[tf->len];
		tt[tf->len] = '\0';
		if (str_repl(tt, noiso8859, codepg850
#ifdef FIND_THEM_ALL
		                                     , iso885915 /* ??? */
#endif
                                                                ) != NULL) {
		  status = 1;
		  tf->reserved_bytes[0] = 'm';
		}
		tt[tf->len] = buff;
		break;
	 }
	}
#ifdef FIND_THEM_ALL
if (status && td->towhat == CAPITALIZE) {
  fprintf(stderr, "GO %ld\n", td->current_record);
  for (fld=0; fld<td->fields; fld++)
	{
	tf = &td->fields_ptr[fld];
	if (tf->reserved_bytes[0] == 'm') {
		tt = td->record_ptr + tf->field_data_address;
		buff = tt[tf->len];
		tt[tf->len] = '\0';
		fprintf(stderr, "REPLACE %.*s WITH '%.*s'\n", MAX_FIELDNAME, tf->name, str_tlen(tt), tt);
		tt[tf->len] = buff;
	}
  }
}
#endif
return status;
}

/*
# Encoding: ascii-8bit
class Employee < ActiveResource::Base
  self.site = 'http://reposit/dbfx/'
  self.element_name = 'emp'
  self.format = :xml

  def Employee.dbpath
    '/us2/data/ks/Kasch_32/Data'
  end

  def Employee.alsql(cmd)
    xql = open("|tee -a /tmp/xql.log|/opt/xbase/bin/xql #{Employee.dbpath} >&2", "w")
    xql.puts(cmd.force_encoding('iso-8859-15'))
    xql.close
  end

# find all key-incompatible char vals by write/read/find
def Employee.cforhigh128
  isomap = Hash.new
  (128..255).to_a.each do |i|
    e = Employee.find(13)
    e.ename = "i am #{i}"
    e.save
    Employee.alsql("update emps set job = '#{i.chr}' where empno = 7902;")
    c = Emp.find_by_EMPNO(7902)
    raise "not written" unless c.ENAME == "i am #{i}"
    d = Emp.find_by_JOB(c.JOB)
    j = d ? d.JOB[0].ord : 256
    k = c.JOB[0].ord
    if d.nil? or d.ENAME != "i am #{i}" or c.JOB.length != 1
      if k==256
      notfound = 1
      else
      notfound = 0
      end
    else
      isomap[j] = i
    end
  end
  isomap[175] = 45
  isomap[152] = 126
  illegalvals = Array.new
  workingvals = Array.new
  codepage437 = Array.new
  iso885915   = Array.new
  (128..255).to_a.each do |i|
    e = Employee.find(13)
    e.ename = "i am #{i}"
    e.save
    Employee.alsql("update emps set job = '#{i.chr}' where empno = 7902;")
    c = Emp.find_by_EMPNO(7902)
    raise "not written" unless c.ENAME == "i am #{i}"
    d = Emp.find_by_JOB(c.JOB)
    j = d ? d.JOB[0].ord : 256
    k = c.JOB[0].ord
    if d.nil? or d.ENAME != "i am #{i}" or c.JOB.length != 1
      codepage437.push(i)
      x = c.JOB[0].ord
      iso885915.push(x<128 ? x : isomap[x])
      illegalvals.push(i)
      if k==256
      workingvals.push("#{i} \\#{i.to_s(8)} \\x#{i.to_s(16)} => ")
      else
      workingvals.push("#{i} \\#{i.to_s(8)} \\x#{i.to_s(16)} => #{"%3d"%k} \\#{"%-3s"%k.to_s(8)} \\x#{"%-3s"%k.to_s(16)} #{c.JOB}")
      end
    else
      workingvals.push("#{i} \\#{i.to_s(8)} \\x#{i.to_s(16)} => #{j} \\#{j.to_s(8)} \\x#{j.to_s(16)} #{d.JOB}")
    end
  end
  codepage437.each {|i| print "\\x#{i.to_s(16)}"}
  puts ''
  iso885915.each {|i| print "\\x#{i.to_s(16)}"}
  puts ''
  illegalvals.each {|i| print "\\x#{i.to_s(16)}"}
  puts ''
  puts 'Windows-1251  => ISO-8859-15   c'
  workingvals.each {|i| puts i}
  illegalvals
end


DBF-binary       ANSI          
CP437/850     => ISO-8859-1    5 R
128 \200 \x80 => 199 \307 \xc7 Ç
129 \201 \x81 => 252 \374 \xfc ü
130 \202 \x82 => 233 \351 \xe9 é
131 \203 \x83 => 226 \342 \xe2 â
132 \204 \x84 => 228 \344 \xe4 ä
133 \205 \x85 => 224 \340 \xe0 à
134 \206 \x86 => 229 \345 \xe5 å
135 \207 \x87 => 231 \347 \xe7 ç
136 \210 \x88 => 234 \352 \xea ê
137 \211 \x89 => 235 \353 \xeb ë
138 \212 \x8a => 232 \350 \xe8 è
139 \213 \x8b => 239 \357 \xef ï
140 \214 \x8c => 238 \356 \xee î
141 \215 \x8d => 236 \354 \xec ì
142 \216 \x8e => 196 \304 \xc4 Ä
143 \217 \x8f => 197 \305 \xc5 Å
144 \220 \x90 => 201 \311 \xc9 É
145 \221 \x91 => 230 \346 \xe6 æ
146 \222 \x92 => 198 \306 \xc6 Æ
147 \223 \x93 => 244 \364 \xf4 ô
148 \224 \x94 => 246 \366 \xf6 ö
149 \225 \x95 => 242 \362 \xf2 ò
150 \226 \x96 => 251 \373 \xfb û
151 \227 \x97 => 249 \371 \xf9 ù
152 \230 \x98 => 255 \377 \xff ÿ
153 \231 \x99 => 214 \326 \xd6 Ö
154 \232 \x9a => 220 \334 \xdc Ü
155 \233 \x9b => 162 \242 \xa2 ¢
156 \234 \x9c => 163 \243 \xa3 £
157 \235 \x9d => 165 \245 \xa5 ¥
158 \236 \x9e =>  80 \120 \x50  P
158 \236 \x9e => 120 \    \x78   x
159 \237 \x9f => 102 \146 \x66  f
160 \240 \xa0 => 225 \341 \xe1 á
161 \241 \xa1 => 237 \355 \xed í
162 \242 \xa2 => 243 \363 \xf3 ó
163 \243 \xa3 => 250 \372 \xfa ú
164 \244 \xa4 => 241 \361 \xf1 ñ
165 \245 \xa5 => 209 \321 \xd1 Ñ
166 \246 \xa6 => 170 \252 \xaa ª
167 \247 \xa7 => 186 \272 \xba º
168 \250 \xa8 => 191 \277 \xbf ¿
169 \251 \xa9 => 172 \254 \xac  ¬
170 \252 \xaa => 172 \254 \xac ¬
171 \253 \xab => 189 \275 \xbd .
172 \254 \xac => 188 \274 \xbc .
173 \255 \xad => 161 \241 \xa1 ¡
174 \256 \xae => 171 \253 \xab «
175 \257 \xaf => 187 \273 \xbb »
176 \260 \xb0 => 124 \174 \x7c  |
177 \261 \xb1 => 124 \174 \x7c  |
178 \262 \xb2 => 124 \174 \x7c  |
179 \263 \xb3 => 124 \174 \x7c  |
180 \264 \xb4 => 124 \174 \x7c  |
181 \265 \xb5 => 124 \174 \x7c  |
182 \266 \xb6 => 124 \174 \x7c  |
183 \267 \xb7 =>  43 \53  \x2b  +
184 \270 \xb8 =>  43 \53  \x2b  +
185 \271 \xb9 => 124 \174 \x7c  |
186 \272 \xba => 124 \174 \x7c  |
187 \273 \xbb =>  43 \53  \x2b  +
188 \274 \xbc =>  43 \53  \x2b  +
189 \275 \xbd =>  43 \53  \x2b  +
190 \276 \xbe =>  43 \53  \x2b  +
191 \277 \xbf =>  43 \53  \x2b  +
192 \300 \xc0 =>  43 \53  \x2b  +
193 \301 \xc1 =>  45 \55  \x2d  -
194 \302 \xc2 =>  45 \55  \x2d  -
195 \303 \xc3 =>  43 \53  \x2b  +
196 \304 \xc4 =>  45 \55  \x2d  -
197 \305 \xc5 =>  43 \53  \x2b  +
198 \306 \xc6 => 124 \174 \x7c  |
199 \307 \xc7 => 124 \174 \x7c  |
200 \310 \xc8 =>  43 \53  \x2b  +
201 \311 \xc9 =>  43 \53  \x2b  +
202 \312 \xca =>  45 \55  \x2d  -
203 \313 \xcb =>  45 \55  \x2d  -
204 \314 \xcc => 124 \174 \x7c  |
205 \315 \xcd =>  45 \55  \x2d  -
206 \316 \xce =>  43 \53  \x2b  +
207 \317 \xcf =>  45 \55  \x2d  -
208 \320 \xd0 =>  45 \55  \x2d  -
209 \321 \xd1 =>  45 \55  \x2d  -
210 \322 \xd2 =>  45 \55  \x2d  -
211 \323 \xd3 =>  43 \53  \x2b  +
212 \324 \xd4 =>  43 \53  \x2b  +
213 \325 \xd5 =>  43 \53  \x2b  +
214 \326 \xd6 =>  43 \53  \x2b  +
215 \327 \xd7 =>  43 \53  \x2b  +
216 \330 \xd8 =>  43 \53  \x2b  +
217 \331 \xd9 =>  43 \53  \x2b  +
218 \332 \xda =>  43 \53  \x2b  +
219 \333 \xdb => 124 \174 \x7c  |
220 \334 \xdc =>  95 \137 \x5f  _
221 \335 \xdd => 166 \246 \xa6 ¦
222 \336 \xde => 124 \174 \x7c  |
223 \337 \xdf => 175 \257 \xaf  ¯
224 \340 \xe0 =>  97 \141 \x61  a
224 \340 \xe0 =>  79 \117 \x4f   O
225 \341 \xe1 => 223 \337 \xdf ß
226 \342 \xe2 =>  71 \107 \x47  G
226 \342 \xe2 =>  79 \117 \x4f   O
227 \343 \xe3 => 112 \160 \x70  p
228 \344 \xe4 =>  83 \123 \x53  S
229 \345 \xe5 => 115 \163 \x73  s
230 \346 \xe6 => 181 \265 \xb5 µ
231 \347 \xe7 => 116 \164 \x74  t
232 \350 \xe8 =>  70 \106 \x46  F
233 \351 \xe9 =>  84 \124 \x54  T
234 \352 \xea =>  79 \117 \x4f  O
235 \353 \xeb => 100 \144 \x64  d
236 \354 \xec =>  56 \70  \x38  8
237 \355 \xed => 102 \146 \x66  f
238 \356 \xee => 101 \145 \x65  e
238 \356 \xee =>  45 \55  \x2d   -
239 \357 \xef => 110 \156 \x6e  n
239 \357 \xef =>  96 \140 \x60   `
240 \360 \xf0 =>  61 \75  \x3d  =
241 \361 \xf1 => 177 \261 \xb1 ±
242 \362 \xf2 =>  61 \75  \x3d  =
243 \363 \xf3 =>  61 \75  \x3d  =
244 \364 \xf4 =>  40 \50  \x28  (
244 \364 \xf4 => 182 \266 \xb6   ¶
245 \365 \xf5 =>  41 \51  \x29  )
245 \365 \xf5 => 167 \247 \xa7   §
246 \366 \xf6 => 247 \367 \xf7 ÷
247 \367 \xf7 => 152 \230 \x98  ~
248 \370 \xf8 => 176 \260 \xb0 °
249 \371 \xf9 => 183 \267 \xb7  ·
250 \372 \xfa => 183 \267 \xb7 ·
251 \373 \xfb => 118 \166 \x76  v
252 \374 \xfc => 110 \156 \x6e  n
253 \375 \xfd => 178 \262 \xb2 ²
254 \376 \xfe => 124 \174 \x7c  |
255 \377 \xff => 160 \240 \xa0  

all replaced
176 \260 \xb0 => 166 \246 \xa6  ¦
176 \260 \xb0 => 124 \174 \x7c  |

178 ¦ |
180 ¦ |   !!!!!!!!!!!!!!!!!
182 ¦ |
210 - -
223 ¯ ¯
226 G O
239 n `
245 ) ¶

 */
