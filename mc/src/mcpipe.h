/* $Id: mcpipe.h,v 1.1 1995/02/19 09:10:00 axel Rel $
 */
int inpipe (int col, int row, char *cmd);
int outpipe (int col, int row, char *cmd);
void inpipeall (void);
void outpipeall (void);
