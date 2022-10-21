/* my2rdb - mysql to /rdb dumper */
char *my2rdb_version = "v1.0 $Id: my2rdb.c,v 1.3 2022/10/02 15:39:24 axel Exp $";
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "strhlp.c"

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, char **argv)
{
  MYSQL *con;
  MYSQL_RES *result;
  MYSQL_ROW row;
  MYSQL_FIELD *field;
  MYSQL_FIELD tab_fields[255];
  int num_fields;
  int i, j, k;
  char query[80];

  if (argc!=6)
  {
      fprintf(stderr, "usage: my2rdb host user passwd db table\n%s\n", my2rdb_version);
      exit(1);
  }

  con = mysql_init(NULL);
  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, argv[1], argv[2], argv[3], argv[4], 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }

  snprintf(query, 80, "select * from %s", argv[5]);
  if (mysql_query(con, query))
  {
      finish_with_error(con);
  }
  
  result = mysql_store_result(con);
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  num_fields = mysql_num_fields(result);
  k = 0;
  while(field = mysql_fetch_field(result)) 
    {
      tab_fields[k] = *field;
      uc(tab_fields[k].name);
      tab_fields[k].def_length = 0;
      switch(tab_fields[k].type) {
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_SHORT:
        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONGLONG:
          tab_fields[k].length = 11;
          tab_fields[k].decimals = 0;
          break;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
          tab_fields[k].length -= tab_fields[k].decimals==0 ? 1 : tab_fields[k].decimals;
          break;
        case MYSQL_TYPE_FLOAT:
          break;
        case MYSQL_TYPE_DOUBLE:
          break;
        case MYSQL_TYPE_BIT:
          tab_fields[k].length = 1;
          break;
        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_YEAR:
          tab_fields[k].def_length = 1; /* use as date flag */
          tab_fields[k].length = 8;
          break;
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
          tab_fields[k].length /= 3;
          break;
        case MYSQL_TYPE_BLOB:
          break;
        case MYSQL_TYPE_SET:
          break;
        case MYSQL_TYPE_ENUM:
          break;
        case MYSQL_TYPE_GEOMETRY:
          break;
        case MYSQL_TYPE_NULL:
          break;
      }
      k++;
    }
  for(k = 0; k < num_fields; k++)
    { 
      printf("%s%c", tab_fields[k].name, k==num_fields-1 ? '\n' : '\t');
    }
  for(k = 0; k < num_fields; k++)
    { 
      for(j = 0; j < tab_fields[k].length; j++) printf("-");
      printf("%c", k==num_fields-1 ? '\n' : '\t');
    }
  while ((row = mysql_fetch_row(result))) 
  { 
      for(i = 0; i < num_fields; i++) 
      {
        if (row[i] && strlen(row[i])>=10 && tab_fields[i].def_length == 1)
          {
            strncpy(row[i]+4, row[i]+5, 2);
            strncpy(row[i]+6, row[i]+8, 2);
            *(row[i]+8) = '\0';
          }
        printf("%s%c", row[i] ? row[i] : "", i==num_fields-1 ? '\n' : '\t'); 
      }
  }
  
  mysql_free_result(result);
  mysql_close(con);
  
  exit(0);
}
