#include<stdio.h>
#include<string.h>
#include "types.h"
#include "view.h"
#include "edit.h"

operation check_operation_type(char **argv)
{
    if(strcmp(argv[1],"-v") == 0)
    {
        return e_write;
    }
    else if(strcmp(argv[1],"-e") == 0)
    {
        return e_edit;
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        return e_help;
    }
    else
    {
        return e_unsupport;
    } 
}

int main(int argc,char *argv[])
{

    viewdata view;
    editdata edit;

    if(argc == 1)
    {
        printf("ERROR\n");
        return 0;
    }

    int ret = check_operation_type(argv);

    if(ret == e_write)
    {
        if(argc == 3)
        {
            if(read_and_validate_view(&view,argv) == e_success)
            {
               if(open_file(&view) == e_success)
               {
                    if(check_ID3_tag(&view) == e_success)
                    {
                        if(skip_header(&view) == e_success)
                        {
                            if(read_print_tag(&view) == e_success)
                            {
                               return 0; 
                            }
                            else
                            {
                                printf("Tag is NOT Proper\n");
                                return 0;
                            }   
                        }
                        else
                        {
                            printf("ERROR in skipping header\n");
                            return 0;
                        }
                    }
                    else
                    {
                        printf("ERROR in the ID3 tag\n");
                        return 0;
                    }
               }
               else
               {
                    printf("ERROR in opening file !\n");
                    return 0;
               }
            }
            else
            {
                printf("ERROR in passing the audio file !\n");
                return 0;
            }
        }
        else
        {
            printf("ERROR in passing write arguments\n");
            return 0;
        }
    }
    else if(ret == e_edit)
    {
        if(argc == 5)
        {
            if(read_and_validate_edit(&edit,argv) == e_success)
            {
                if(open_files_edit(&edit) == e_success)
                {
                    if(copy_header(&edit) == e_success)
                    {
                        if(do_edit(&edit) == e_success)
                        {
                            printf("EDITED SUCCESSFULLY !\n");
                            return 0;
                        }
                        else
                        {
                            printf("EDITING UNSUCCESSFULL !\n");
                            return 0;
                        }
                    }
                    else
                    {
                        printf("ERROR in Copying header\n");
                        return 0;
                    }
                }
                else
                {
                    printf("ERROR in opening file\n");
                    return 0;
                }
            }
            else
            {
                printf("ERROR in passing edit arguments\n");
                return 0;
            }
        }
        else
        {
            printf("ERROR in passing edit1 arguments\n");
            return 0;
        }
    }
    else if(ret == e_help)
    {
        printf("Usage:   mp3 -[tTaAycg] 'value' file 1\n\t mp3tag -v\n");
        printf("-t\t Modifies a Title tag\n-g\t Modifies a Genre tag\n");
        printf("-a\t Modifies an Artist tag\n-A\t Modifies an Album tag\n");
        printf("-y\t Modifies a Year tag\n-c\t Modifies Comment tag\n");

        return 0;    
    }
    else
    {
        printf("ERROR IN PASSING ARGUMENT\n");
        return 0;
    }
}


