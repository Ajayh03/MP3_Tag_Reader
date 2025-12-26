#include<stdio.h>
#include<string.h>
#include"types.h"
#include"edit.h"

status read_and_validate_edit(editdata *edit,char **argv)
{
    if((strcmp(argv[2],"-t") == 0) || (strcmp(argv[2],"-T") == 0) || (strcmp(argv[2],"-a") == 0) || (strcmp(argv[2],"-A") == 0) || (strcmp(argv[2],"-y")== 0) || (strcmp(argv[2],"-c")== 0) || (strcmp(argv[2],"-g") == 0))
    {
        if(strcmp(argv[2],"-t") == 0)
            strcpy(edit->edit_tag,"TIT2");
    
        else if(strcmp(argv[2],"-a") == 0)
            strcpy(edit->edit_tag,"TPE1");
        
        else if(strcmp(argv[2],"-A") == 0)
            strcpy(edit->edit_tag,"TALB");
        
        else if(strcmp(argv[2],"-y") == 0)
            strcpy(edit->edit_tag,"TYER");
        
        else if(strcmp(argv[2],"-c") == 0)
            strcpy(edit->edit_tag,"COMM");
        
        else if(strcmp(argv[2],"-g") == 0)
            strcpy(edit->edit_tag,"TCON");
           

        edit->edit_tag[4]='\0';    

        if(argv[4][0] != '.')
        {
            if(strstr(argv[4],".mpeg") || strstr(argv[4],".mp3"))
            {
                if(strlen(strchr(argv[4],'.')) >=4  && strlen(strchr(argv[4],'.')) <=5)
                {
                    if(strlen(strchr(argv[4],'.')) == 4)
                    {
                        edit->extn = ".mp3";
                    }
                    else
                    {
                        edit->extn = ".mpeg";
                    }
                    edit->edit_content = argv[3];
                    edit->file_org_edit = argv[4];
                    edit->demo_name = argv[4];
                    return e_success;
                }
                else
                {
                    return e_failure;
                }
            }
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }

}

status open_files_edit(editdata *edit)
{
    edit->fptr_edit = fopen(edit->file_org_edit,"r");
    if(edit->fptr_edit == NULL)
    {
        printf("Opening orgfile unsuccessfull\n");
        return e_failure;
    }
    if(strlen(edit->extn) == 4)
    {
        edit->file_temp_edit = "temp.mp3";
    }
    else
    {
        edit->file_temp_edit = "temp.mpeg";
    }
    edit->fptr_temp = fopen(edit->file_temp_edit,"w");

    if(edit->fptr_temp == NULL)
    {
        printf("Opening tempfile unsuccessfull\n");
        return e_failure;
    }
    
    return e_success; 
}

status copy_header(editdata *edit)
{
    char arr[10];
    fread(arr,1,10,edit->fptr_edit);
    fwrite(arr,1,10,edit->fptr_temp);

    return e_success; 
}

status do_edit(editdata *edit)
{
    char arr[5];
    for(int i=0;i<6;i++)
    {

        fread(arr,1,4,edit->fptr_edit);
        arr[4] = '\0';
        if(strcmp(arr,edit->edit_tag) == 0)
        {
            fwrite(arr,1,4,edit->fptr_temp);
            edit_data(edit);
        }
        else
        {
            fwrite(arr,1,4,edit->fptr_temp);
            copy_data(edit);
        }
    }

    copy_remaining_data(edit);

    return e_success;
}

status copy_data(editdata *edit)
{
    char arr_flag[3];
    fread(arr_flag,1,3,edit->fptr_edit);
    fwrite(arr_flag,1,3,edit->fptr_temp);
    
    char arr_size[4];
    fread(arr_size,1,4,edit->fptr_edit);

    //bigendian_to_little_edit(arr_size);
    
    fwrite(arr_size,1,4,edit->fptr_temp);

    int value;
    memcpy(&value, arr_size, 4);

    char arr_data[value];
    fread(arr_data,1,value-1,edit->fptr_edit);
    fwrite(arr_data,1,value-1,edit->fptr_temp);

    return e_success;

}

status edit_data(editdata *edit)
{
    char arr_flag1[3];
    fread(arr_flag1,1,3,edit->fptr_edit);
    fwrite(arr_flag1,1,3,edit->fptr_temp);

    //char arr_size[4];
    int size;
    fread(&size,1,4,edit->fptr_edit);

    //bigendian_to_little_edit(arr_size);

    //memcpy(&size, arr_size, 4);
    edit->size = size;

    int cont_size = strlen(edit->edit_content)+1;
    char str1[4];
    memcpy(str1,&cont_size,4);

    //bigendian_to_little_edit(str1);
    
    fwrite(str1,1,4,edit->fptr_temp);

    fwrite(edit->edit_content,1,cont_size-1,edit->fptr_temp);
    fseek(edit->fptr_edit,edit->size-1,SEEK_CUR);

    return e_success;
}


status copy_remaining_data(editdata *edit)
{
    int cur_size  = ftell(edit->fptr_edit);
    fseek(edit->fptr_edit,0,SEEK_END);
    int total_size = ftell(edit->fptr_edit);
    int remaining = total_size - cur_size;

    char arr_rem[1];

    fseek(edit->fptr_edit,cur_size,SEEK_SET);

    for(int i=0;i<remaining;i++)
    {
        fread(arr_rem,1,1,edit->fptr_edit);
        fwrite(arr_rem,1,1,edit->fptr_temp);
    }

    remove(edit->file_org_edit);
    rename(edit->file_temp_edit,edit->demo_name);
      
    fclose(edit->fptr_edit);
    fclose(edit->fptr_temp);

    return e_success;
}

