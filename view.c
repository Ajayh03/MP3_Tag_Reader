#include<stdio.h>
#include<string.h>
#include "types.h"
#include "view.h"

status read_and_validate_view(viewdata *view,char **argv)
{
    if(argv[2][0] != '.')
    {
        if(strstr(argv[2],".mpeg") || strstr(argv[2],".mp3"))
        {
            if(strlen(strchr(argv[2],'.')) >=4  && strlen(strchr(argv[2],'.')) <=5)
            {
                view->file_name = argv[2];
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

status open_file(viewdata *view)
{
    view->fptr_view = fopen(view->file_name,"r");
    if (view->fptr_view == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", view->file_name);
    	return e_failure;
    }
    return e_success;
}

status check_ID3_tag(viewdata *view)
{
    char arr[3];
    fread(arr,1,3,view->fptr_view);
    arr[3] = '\0';
    if(strcmp(arr,"ID3") == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

status skip_header(viewdata *view)
{
    fseek(view->fptr_view,10,SEEK_SET);
    return e_success;
}

status read_print_tag(viewdata *view)
{
    char arr[5];
    char *tag_arr[6] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};
    printf("--------------------------------------------------------------------------\n");
    printf("\t\t\tMP3 Tag Reader and Editor\n");
    printf("--------------------------------------------------------------------------\n\n");
    for(int k=0;k<6;k++)
    {
        fread(arr,1,4,view->fptr_view);
        arr[4] = '\0';
        int i;
        for(i=0;i<6;i++)
        {
            if(strcmp(tag_arr[i],arr) == 0)
            {
                //printf("TAG : %s\n",arr);
                fseek(view->fptr_view,3,SEEK_CUR);//skip 3 bytes of flag
                // char arr_size[4];
                int size;
                fread(&size,1,4,view->fptr_view);
                //printf("--_->%d",size);
                //bigendian_to_little((char *)&size);
                //printf("--->%d",size);
               // memcpy(&size, size, 4);


                //size = (int)arr_size;

                //sscanf(arr_size,"%d",&size);
                //printf("size : %d\n",size);

                char arr_data[size-1];
                fread(arr_data,size-1,1,view->fptr_view);
                arr_data[size-1] = '\0';

                if(strcmp(arr,"TIT2") == 0)
                    printf("TITLE   \t:\t %s \n\n",arr_data);
                
                if(strcmp(arr,"TPE1") == 0)
                    printf("ARTIST  \t:\t %s \n\n",arr_data);
                
                if(strcmp(arr,"TALB") == 0)    
                    printf("ALBUM   \t:\t %s \n\n",arr_data);

                if(strcmp(arr,"TYER") == 0)    
                    printf("YEAR    \t:\t %s \n\n",arr_data);

                if(strcmp(arr,"TCON") == 0)    
                    printf("MUSIC   \t:\t %s \n\n",arr_data);

                if(strcmp(arr,"COMM") == 0)    
                    printf("COMMENT \t:\t %s \n\n",arr_data);
    
                break;
            }
        }
        if(i>=6)
        {
            return e_failure;
        }
    }
    printf("--------------------------------------------------------------------------\n");
    return e_success; 
}


















