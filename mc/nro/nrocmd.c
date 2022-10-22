/*
 *	Command processor for NRO text processor
 *
 *	Stephen L. Browning
 *	5723 North Parker Avenue
 *	Indianapolis, Indiana 46220
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "nro.h"
#include "nrocom.c"

void put();
int ctod();
int getwrd();
int getlin();
void pfoot();
int itoda();
void skip();
void phead();

/*
 *	skip blanks and tabs in character buffer.
 *	return number of characters skipped.
 */

char *skipbl(p)
char *p;
{
	while (*p == ' ' || *p == '\t')
		++p;
	return(p);
}

/*
 *	end current filled line
 */
void
brek()
{
char	*op;

	if(co.outp > 0)
	{
		op	= &co.outbuf[co.outp];
#ifdef MSDOS
		*op++	= '\r';
#endif
		*op++	= '\n';
		*op++	= '\0';
		put(co.outbuf);
	}
	co.outp = 0;
	co.outw = 0;
	co.outwds = 0;
}

/*
 *	space vertically n lines
 */
void
space(n)
int n;
{
	brek();
	if (pg.lineno > pg.bottom)
		return;
	if (pg.lineno == 0)
		phead();
	skip(min(n,pg.bottom+1-pg.lineno));
	pg.lineno += n;
	if (pg.lineno > pg.bottom) pfoot();
}

/*
 *	skip over word and punctuation
 */

char *skipwd(p)
char *p;
{
	while (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' && *p != '\0')
		++p;
	return(p);
}

/*
 *	set parameter and check range
 */
void
set(param,val,type,defval,minval,maxval)
int *param;
int val;
char type;
int defval,minval,maxval;
{
	switch(type)
	{
	case '\r':
	case '\n':
		*param = defval;
		break;

	case '+':
		*param += val;
		break;

	case '-':
		*param -= val;
		break;

	default:
		*param = val;
		break;
	}
	*param = min(*param,maxval);
	*param = max(*param,minval);
}

/*
 *	Expand escape sequences
 */

expesc(p,q)
char *p;
char *q;
{
	char *s, *t;
	static enum Font {NORMAL, BOLD, ITALIC}	font[2] = {NORMAL, NORMAL};

	s = p;
	t = q;
	while (*s != '\0')
	{
		switch (*s)
		{
		case '\\':
			switch (*(s+1))
			{
			case '\\':
				++s;
				goto copychar;
			case 'n':
			case 'N':
				if (isalpha(*(s+2)))
				{
				  s += 2;
				  t += itoda(dc.nr[tolower(*s++)-'a'],t,6) - 1;
				  break;
				}
				else goto copychar;
			case 'f':
			case 'F':
				switch (*(s+2))
				{
				case 'B':
					font[0] = font[1];
					font[1] = BOLD;
					goto stackfont;
				case 'I':
					font[0] = font[1];
					font[1] = ITALIC;
					goto stackfont;
				case 'R':
					font[0] = font[1];
					font[1] = NORMAL;
					goto stackfont;
				case 'P':
					font[1] = font[0];
				stackfont:
					s += 3;
					break;
				default:
					goto copychar;
				}
				break;
			default:
				goto copychar;
			}
			break;
		default:
		copychar:
			switch (font[1])
			{
			case BOLD:
				*t++ = *s;
				goto normal;
			case ITALIC:
				*t++ = '_';
				goto normal;
			normal:
				*t++ = '\b';
			case NORMAL:
				*t++ = *s++;
			}
			break;
		}
	}
	*t = '\0';
	strcpy(p,q);
}

/*
 *	Push character back into input stream
 */
void
putbak(c)
char c;
{
	if (mac.ppb < &mac.pbb[0]) {
		mac.ppb = &mac.pbb[0];
		*mac.ppb = c;
	}
	else
	{
		if (mac.ppb >= &mac.pbb[MAXLINE-1])
		{
			puts("nro: push back buffer overflow\n");
			exit(15);
		}
		*++mac.ppb = c;
	}
}

/*
 *	Push back string into input stream
 */
void
pbstr(p)
char p[];
{
	int i;

	for (i=strlen(p)-1; i>=0; --i)
	{
		putbak(p[i]);
	}
}

/*
 *	Evaluate macro expansion
 */
void
maceval(p,m)
char *p;
char m[];
{
	int i, j;
	char *argp[10];
	char c;

	*p++ = '\0';		/* replace command char with '\0' */
	/*
	*	initialize argp array to substitute command
	*	string for any undefined argument
	*/
	for (i=0; i<10; ++i)
		argp[i] = p;
	p = skipwd(p);
	*p++ = '\0';
	for (i=0; i<10; ++i)
	{
		p = skipbl(p);
		if (*p == '\r' || *p == '\n' || *p == '\0')
			break;
		if (*p == '\'' || *p == '"')
		{
			c = *p++;
			argp[i] = p;
			while (*p != c && *p != '\r' && *p != '\n' && *p != '\0')
				++p;
			*p++ = '\0';
		}
		else
		{
			argp[i] = p;
			p = skipwd(p);
			*p++ = '\0';
		}
	}
	for (i=strlen(m)-1; i>=0; --i)
	{
		if (i > 0 && m[i-1] == '$')
		{
			if (!isdigit(m[i]))
			{
				putbak(m[i]);
			}
			else
			{
				pbstr(argp[m[i]-'0']);
				--i;
			}
		}
		else
		{
			putbak(m[i]);
		}
	}
}

/*
 *	get header or footer title
 */
void
gettl(p,q,limit)
char *p;
char *q;
int limit[];
{
	p = skipwd(p);
	p = skipbl(p);
	strcpy(q,p);
	limit[LEFT] = dc.inval;
	limit[RIGHT] = dc.rmval;
}

/*
 *	Put macro definition into table
 */
int
putmac(name,p)
char *name;
char *p;
{
	if (mac.lastp >= MXMDEF)
		return(ERR);
	if (mac.emb + strlen(name) + strlen(p) + 1 > &mac.mb[MACBUF])
	{
		return(ERR);
	}
	++mac.lastp;
	mac.mnames[mac.lastp] = mac.emb;
	strcpy(mac.emb,name);
	strcpy(mac.emb + strlen(name) + 1,p);
	mac.emb += strlen(name) + strlen(p) + 2;
	return(OK);
}

/*
 *	Collect macro definition from input stream
 */
int
colmac(p,d,i)
char *p, d[];
int i;
{
	while (*p != '\0')
	{
		if (i >= MXMLEN-1)
		{
			d[i-1] = '\0';
			return(ERR);
		}
		d[i++] = *p++;
	}
	d[i] = '\0';
	return(i);
}

/*
 *	Define a macro
 */
void
defmac(p,infp)
char *p;
FILE *infp;
{
	int i;
	char name[MNLEN];
	char defn[MXMLEN];
	char *q;

	q = skipwd(p);
	q = skipbl(q);
	i = getwrd(q,name);
	if (!isalpha(*name))
	{
		fprintf (stderr, "nro: missing or illegal macro definition name\n");
		exit(12);
	}
	if (i > 2) name[2] = '\0';
	i = 0;
	while (getlin(p,infp) != EOF)
	{
		if (p[0] == dc.cmdchr && tolower(p[1]) == 'e' && tolower(p[2]) == 'n')
		{
			break;
		}
		if ((i = colmac(p,defn,i)) == ERR)
		{
			fprintf (stderr, "nro: macro definition too long\n");
			exit(13);
		}
	}
	if (putmac(name,defn) == ERR)
	{
		fprintf (stderr, "nro: macro definition table full\n");
		exit(14);
	}
}

/*
 *	retrieves optional argument following nro command.
 *	returns positive integer value with sign (if any)
 *	saved in character addressed by p_argt.
 */
int
getval(p,p_argt)
char *p;
char *p_argt;
{
	p = skipwd(p);
	p = skipbl(p);
	*p_argt = *p;
	if((*p == '+') || (*p == '-'))
		++p;
	return(ctod(p));
}

/*
 *	decodes nro command and returns its associated
 *	value.
 */
int
comtyp(p,m)
char *p;
char *m;
{
	char c1, c2;
	char macnam[MNLEN];
	char *s;

	p++;
	/*
	*	First check to see if the command is a macro.
	*	If it is, truncate to two characters and return
	*	expansion in m.  Note that upper and lower case
	*	characters are handled differently for macro names,
	*	but not for normal command names.
	*/
	getwrd(p,macnam);
	macnam[2] = '\0';
	if ((s = getmac(macnam)) != NULL)
	{
		strcpy(m,s);
		return(MACRO);
	}
	c1 = tolower(*p);
	p++;
	c2 = tolower(*p);
	if (c1 == 'b' && c2 == 'o') return(BO);
	if (c1 == 'b' && c2 == 'p') return(BP);
	if (c1 == 'b' && c2 == 'r') return(BR);
	if (c1 == 'b' && c2 == 's') return(BS);
	if (c1 == 'c' && c2 == 'c') return(CC);
	if (c1 == 'c' && c2 == 'e') return(CE);
	if (c1 == 'c' && c2 == 'u') return(CU);
	if (c1 == 'd' && c2 == 'e') return(DE);
	if (c1 == 'e' && c2 == 'f') return(EF);
	if (c1 == 'e' && c2 == 'h') return(EH);
	if (c1 == 'e' && c2 == 'n') return(EN);
	if (c1 == 'f' && c2 == 'i') return(FI);
	if (c1 == 'f' && c2 == 'o') return(FO);
	if (c1 == 'h' && c2 == 'e') return(HE);
	if (c1 == 'i' && c2 == 'n') return(IN);
	if (c1 == 'j' && c2 == 'u') return(JU);
	if (c1 == 'l' && c2 == 's') return(LS);
	if (c1 == 'm' && c2 == '1') return(M1);
	if (c1 == 'm' && c2 == '2') return(M2);
	if (c1 == 'm' && c2 == '3') return(M3);
	if (c1 == 'm' && c2 == '4') return(M4);
	if (c1 == 'n' && c2 == 'e') return(NE);
	if (c1 == 'n' && c2 == 'f') return(NF);
	if (c1 == 'n' && c2 == 'j') return(NJ);
	if (c1 == 'n' && c2 == 'r') return(NR);
	if (c1 == 'o' && c2 == 'f') return(OF);
	if (c1 == 'o' && c2 == 'h') return(OH);
	if (c1 == 'p' && c2 == 'c') return(PC);
	if (c1 == 'p' && c2 == 'l') return(PL);
	if (c1 == 'p' && c2 == 'o') return(PO);
	if (c1 == 'r' && c2 == 'm') return(RM);
	if (c1 == 's' && c2 == 'o') return(SO);
	if (c1 == 's' && c2 == 'p') return(SP);
	if (c1 == 't' && c2 == 'i') return(TI);
	if (c1 == 'u' && c2 == 'l') return(UL);
	return(UNKNOWN);
}

void
comand(p)
char *p;
{
	int ct, val;
	int spval;
	int index;
	char argtyp;
	char name[MAXLINE];
	char macexp[MXMLEN];

	ct = comtyp(p,macexp);
	if (ct == UNKNOWN)
	{
		fprintf (stderr, "nro: unrecognized command %s\n",p);
		return;
	}
	expesc(p,name);
	val = getval(p,&argtyp);
	switch (ct)
	{
	case BO: /* bold face */
		set(&dc.boval,val,argtyp,1,0,HUGE);
		dc.cuval = dc.ulval = 0;
		break;

	case BP: /* begin page */
		if(pg.lineno > 0)
			space(HUGE);
		set(&pg.curpag,val,argtyp,pg.curpag+1,-HUGE,HUGE);
		pg.newpag = pg.curpag;
		break;

	case BR: /* break */
		brek();
		break;

	case BS: /* backspaces in output */
		set(&dc.bsflg,val,argtyp,1,0,1);
		break;

	case CC: /* command character */
		if (argtyp == '\r' || argtyp == '\n')
			dc.cmdchr = '.';
		else
			dc.cmdchr = argtyp;
		break;

	case CE: /* center */
		brek();
		set(&dc.ceval,val,argtyp,1,0,HUGE);
		break;

	case CU: /* continuous underline */
		set(&dc.cuval,val,argtyp,1,0,HUGE);
		dc.ulval = dc.boval = 0;
		break;

	case DE: /* define macro */
		defmac(p,sofile[dc.flevel]);
		break;

	case EF: /* even footer */
		gettl(p,pg.efoot,&pg.eflim[0]);
		break;

	case EH: /* even header */
		gettl(p,pg.ehead,&pg.ehlim[0]);
		break;

	case EN: /* end macro definition */
		fprintf (stderr, "***nro: missing .de command\n");
		break;

	case FI: /* fill */
		brek();
		dc.fill = YES;
		break;

	case FO: /* footer */
		gettl(p,pg.efoot,&pg.eflim[0]);
		gettl(p,pg.ofoot,&pg.oflim[0]);
		break;

	case HE: /* header */
		gettl(p,pg.ehead,&pg.ehlim[0]);
		gettl(p,pg.ohead,&pg.ohlim[0]);
		break;

	case IN: /* indenting */
		set(&dc.inval,val,argtyp,0,0,dc.rmval-1);
		dc.tival = dc.inval;
		break;

	case JU: /* justify */
		dc.juval = YES;
		break;

	case LS: /* line spacing */
		set(&dc.lsval,val,argtyp,1,1,HUGE);
		break;

	case M1: /* set topmost margin */
		set(&pg.m1val,val,argtyp,2,0,HUGE);
		break;

	case M2: /* set second top margin */
		set(&pg.m2val,val,argtyp,2,0,HUGE);
		break;

	case M3: /* set first bottom margin */
		set(&pg.m3val,val,argtyp,2,0,HUGE);
		pg.bottom = pg.plval - pg.m4val - pg.m3val;
		break;

	case M4: /* set bottom-most margin */
		set(&pg.m4val,val,argtyp,2,0,HUGE);
		pg.bottom = pg.plval - pg.m4val - pg.m3val;
		break;

	case MACRO: /* macro expansion */
		maceval(p,macexp);
		break;

	case NE: /* need n lines */
		brek();
		if ((pg.bottom-pg.lineno+1) < (val*dc.lsval))
		{
			space(HUGE);
		}
		break;

	case NF: /* no fill */
		brek();
		dc.fill = NO;
		break;

	case NJ: /* no justify */
		dc.juval = NO;
		break;

	case NR: /* set number register */
		p = skipwd(p);
		p = skipbl(p);
		if (!isalpha(*p))
		{
			fprintf (stderr, "nro: invalid or missing number register name\n");
		}
		else
		{
			index = tolower(*p) - 'a';
			p = skipwd(p);
			val = getval(p,&argtyp);
			set(&dc.nr[index],val,argtyp,0,-HUGE,HUGE);
		}
		break;

	case OF: /* odd footer */
		gettl(p,pg.ofoot,&pg.oflim[0]);
		break;

	case OH: /* odd header */
		gettl(p,pg.ohead,&pg.ohlim[0]);
		break;

	case PC: /* page number character */
		if (argtyp == '\r' || argtyp == '\n')
			dc.pgchr = '\0';
		else
			dc.pgchr = argtyp;
		break;

	case PL: /* page length */
		set(&pg.plval,val,argtyp,PAGELEN,
			pg.m1val+pg.m2val+pg.m3val+pg.m4val+1,HUGE);
		pg.bottom = pg.plval - pg.m3val - pg.m4val;
		break;

	case PO: /* page offset */
		set(&pg.offset,val,argtyp,0,0,HUGE);
		break;

	case RM: /* right margin */
		set(&dc.rmval,val,argtyp,PAGEWIDTH,dc.tival+1,HUGE);
		pg.ehlim[RIGHT] = pg.ohlim[RIGHT] = dc.rmval;
		pg.eflim[RIGHT] = pg.oflim[RIGHT] = dc.rmval;
		break;

	case SO: /* source file */
		p = skipwd(p);
		p = skipbl(p);
		if (getwrd(p,name) == 0) break;
		if (dc.flevel+1 >= NFILES)
		{
			fprintf (stderr, "nro: .so commands nested too deeply\n");
			exit(10);
		}
		if((sofile[dc.flevel+1] = fopen(name, "r")) == (FILE *)0)
		{
			fprintf (stderr,"***nro: unable to open %s\n",name);
			exit(11);
		}
		++dc.flevel;
		break;

	case SP: /* space */
		set(&spval,val,argtyp,1,0,HUGE);
		space(spval);
		break;

	case TI: /* temporary indent */
		brek();
		set(&dc.tival,val,argtyp,0,0,dc.rmval);
		break;

	case UL: /* underline */
		set(&dc.ulval,val,argtyp,0,1,HUGE);
		dc.cuval = dc.boval = 0;
		break;
	}
}



/*
 *	convert ascii character to decimal.
 */

atod(c)
char c;
{
	return(((c < '0') || (c > '9')) ? -1 : c-'0');
}

/*
 *	Get macro definition from table
 */
char *getmac(name)
char *name;
{
	int i;

	for (i = mac.lastp; i >= 0; --i)
	{
		if (mac.mnames[i] && !strcmp(name,mac.mnames[i]))
		{
			return(mac.mnames[i] + 3);
		}
	}
	return(NULL);
}

/*
 *	convert string to decimal.
 *	processes only positive values.
 */
int
ctod(p)
char *p;
{
	int val, d;

	val = 0;
	while(*p != '\0')
	{
		d = atod(*p++);
		if(d == -1) return(val);
		val = 10 * val + d;
	}
	return(val);
}

