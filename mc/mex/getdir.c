#include <dir.h>
#include <dos.h>

int getfirst (path, attributes)
	char *path; int attributes;
{
return findfirst (path, getdta(), attributes);
}

int getnext (path, attributes)
	char *path; int attributes;
{
return findnext (getdta());
}
