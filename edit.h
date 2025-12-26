#include<stdio.h>
#include"types.h"

typedef struct edit
{
    char *file_org_edit;
    char *demo_name;
    FILE *fptr_edit;
    char edit_tag[5];
    char *extn;

    char *edit_content;
    FILE *fptr_temp;
    char *file_temp_edit;
    int size;
}editdata;

status read_and_validate_edit(editdata *edit , char **argv);
status open_files_edit(editdata *edit);
status copy_header(editdata *edit);
status do_edit(editdata *edit);
status edit_data(editdata *edit);
status copy_data(editdata *edit);
status copy_remaining_data(editdata *edit);
