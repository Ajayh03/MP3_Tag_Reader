#include "types.h"
#include<stdio.h>

typedef struct view
{
    char *file_name;
    FILE *fptr_view;
    
}viewdata;

status read_and_validate_view(viewdata *view,char **argv);
status open_file(viewdata *view);
status check_ID3_tag(viewdata *view);
status skip_header(viewdata *view);
status read_print_tag(viewdata *view);
