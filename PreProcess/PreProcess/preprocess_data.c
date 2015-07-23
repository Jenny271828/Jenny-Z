#include "preprocess_data.h"


int preprocess_data(int start, int end)
{
    int right_rows = del_overrange__data(start,end);
    printf("preprocess_data() is done!!\n\n");
    return right_rows;
}

int del_overrange__data(int start, int end)
{
    FILE *fp_r = fopen(data_original_filename, "r");
    FILE *fp_w = fopen(data_preprocess_filename, "w");
    FILE *fp_log = fopen(data_preprocess_log_filename, "w");
    if (fp_r == NULL || fp_w == NULL || fp_log == NULL)
    {
        printf("Can't open files in preprocess_data().\n");
        return 0;
    }
    
    int all_rows = 0;
    int right_rows = 0;
    int wrong_rows = 0;
    int tmp_citing,tmp_cited;
    char line_feed;
    
    while (fscanf(fp_r, "%d,%d",&tmp_citing,&tmp_cited) != EOF)
    {
        line_feed = getc(fp_r);
        all_rows++;
        
        if (tmp_citing > start && tmp_citing < end && tmp_cited > start && tmp_cited < end)
        {
            fprintf(fp_w, "%d %d\n",tmp_citing,tmp_cited);
            right_rows++;
        }
        else
        {
            wrong_rows++;
        }
    }
    
    fprintf(fp_log, "all_rows   = %d\n",all_rows);
    fprintf(fp_log, "right_rows = %d\n",right_rows);
    fprintf(fp_log, "wrong_rows = %d\n",wrong_rows);
    
    fclose(fp_r);
    fclose(fp_w);
    fclose(fp_log);
    
    printf("del_overrange__data() ==> %d -> %d.\n",all_rows,right_rows);
    return right_rows;
}

