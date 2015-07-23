#include "preprocess_attributes.h"

int preprocess_attributes(int start, int end)
{
    int right_rows;
    right_rows = del_overrange_attributes(start, end);
    quicksort_attributes(right_rows);
    
    right_rows = del_duplicate_attributes();
    
    right_rows = del_dontmatch_attributes(right_rows);
    
    printf("preprocess_attributes() is done!!\n\n");
    return right_rows;
}

int del_overrange_attributes(int start, int end)
{
    FILE *fp_r = fopen(original_attributes_filename, "r");
    FILE *fp_w = fopen(del_overrange_atttibutes_filename, "w");
    FILE *fp_log = fopen(del_overrange_atttibutes_log_filename, "w");
    if (fp_r == NULL || fp_w == NULL || fp_log == NULL)
    {
        printf("Can't open file in del_overrange_attributes().\n");
        return 0;
    }
    int tmp_pid,tmp_unknown;
    char tmp_cid[cid_size];
    char line_feed;
    int all_rows = 0;
    int right_rows = 0;
    int wrong_rows = 0;
    
    while (fscanf(fp_r, "%d,%c%c%c,%d",&tmp_pid,
                  &tmp_cid[0],&tmp_cid[1],&tmp_cid[2],
                  &tmp_unknown) != EOF)
    {
        line_feed = getc(fp_r);
        all_rows++;
        
        if (tmp_pid > start && tmp_pid < end)
        {
            fprintf(fp_w,"%d %c%c%c\n",tmp_pid,tmp_cid[0],tmp_cid[1],tmp_cid[2]);
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
    
    printf("del_overrange_attributes() ==> %d -> %d.\n",all_rows,right_rows);
    
    return right_rows;
}

void swap(int *pid_left, char *cid_left, int *pid_right, char *cid_right)
{
    int tmp = *pid_left;
    *pid_left = *pid_right;
    *pid_right = tmp;
    
    for (int i = 0; i < cid_size; i++)
    {
        char tmpc = cid_left[i];
        cid_left[i] = cid_right[i];
        cid_right[i] = tmpc;
    }
}
void quick_sort(int *pid,char **cid, int left,int right)
{
    if (right <= left) return;
    int lt = left, gt = right;
    int r = (rand() % (right - left + 1)) + left;
    swap(&pid[left], cid[left], &pid[r], cid[r]);
    
    int v = pid[left];
    int i = left;
    while (i <= gt)
    {
        if (pid[i] < v)
        {
            swap(&pid[lt], cid[lt], &pid[i], cid[i]);
            lt++;
            i++;
        }
        else if (pid[i] > v)
        {
            swap(&pid[i], cid[i], &pid[gt], cid[gt]);
            gt--;
        }
        else
        {
            i++;
        }
    }
    
    quick_sort(pid, cid, left, lt - 1);
    quick_sort(pid, cid, gt + 1, right);
}
void quicksort_attributes(int N)
{
    FILE *fp_r = fopen(del_overrange_atttibutes_filename, "r");
    FILE *fp_w = fopen(quicksort_attributes_filename, "w");
    if (fp_r == NULL || fp_w == NULL)
    {
        printf("Can't open file in quicksort_attributes().\n");
        exit(1);
    }
    char line_feed;
    int *pid = malloc(N * sizeof(int));
    char **cid = malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++)
    {
        cid[i] = malloc(cid_size * sizeof(char));
    }
    for (int i = 0 ; i < N; i++)
    {
        fscanf(fp_r, "%d %c%c%c",&pid[i],&cid[i][0],&cid[i][1],&cid[i][2]);
        line_feed = getc(fp_r);
    }
    /*printf("last one=%d %c%c%c\n",pid[ATTR_SIZE-1],cid[ATTR_SIZE-1][0],
     cid[ATTR_SIZE-1][1],cid[ATTR_SIZE-1][2]);*/
    
    quick_sort(pid, cid, 0, N-1);
    
    for (int i = 0; i < N; i++)
    {
        fprintf(fp_w, "%d %c%c%c\n",pid[i],cid[i][0],cid[i][1],cid[i][2]);
    }
    fclose(fp_r);
    fclose(fp_w);
    
    free(pid);
    for (int i = 0; i < N; i++)
    { free(cid[i]); }
    free(cid);
    
    //printf("qsort_attribus() is done.\n");

}

int del_duplicate_attributes()
{
    FILE *fp_r = fopen(quicksort_attributes_filename, "r");
    FILE *fp_w = fopen(del_duplicate_attributes_filename, "w");
    FILE *fp_log = fopen(del_duplicate_attributes_log_filename, "w");
    
    int pid;
    char cid[cid_size];
    int pre_pid;
    char pre_cid[cid_size];
    char line_feed;
    
    int all_rows = 0;
    int distinct_rows = 0;
    int duplicate_rows = 0;
    
    //Read the first line into tep_pid_last and tmp_cid_last
    fscanf(fp_r, "%d %c%c%c",&pre_pid,&pre_cid[0],&pre_cid[1],&pre_cid[2]);
    line_feed = getc(fp_r);
    
    //默认第一行为不同项
    all_rows++;
    distinct_rows++;
    fprintf(fp_w,"%d %c%c%c\n",pre_pid,pre_cid[0],
            pre_cid[1],pre_cid[2]);
    
    //Compare to the next line
    while (fscanf(fp_r, "%d %c%c%c",&pid,&cid[0],&cid[1],&cid[2]) != EOF)
    {
        line_feed = getc(fp_r);
        all_rows++;
        //Current line equal to last line
        if (pid == pre_pid && pre_cid[0] == cid[0]
            && pre_cid[1] == cid[1] && pre_cid[2] == cid[2])
        {
            duplicate_rows++;
        }
        else
        {
            fprintf(fp_w,"%d %c%c%c\n",pid,cid[0],cid[1],cid[2]);
            distinct_rows++;
            pre_pid = pid;
            pre_cid[0] = cid[0];
            pre_cid[1] = cid[1];
            pre_cid[2] = cid[2];
        }
        
    }
    fprintf(fp_log, "all_rows       = %d\n",all_rows);
    fprintf(fp_log, "distinct_rows  = %d\n",distinct_rows);
    fprintf(fp_log, "duplicate_rows = %d\n",duplicate_rows);
    
    fclose(fp_r);
    fclose(fp_w);
    fclose(fp_log);
    
    printf("del_duplicate_attributes() ==> %d -> %d.\n",all_rows,distinct_rows);
    return distinct_rows;
}

int binary_search(int array[], int n, int value)
{
    int left = 0;
    int right = n - 1;
    
    while (left <= right)
    {
        int middle = left + ((right - left) >> 1);
        
        if (array[middle] > value)
        {
            right = middle - 1;
        }
        else if (array[middle] < value)
        {
            left = middle + 1;
        }
        else
        {
            return middle;
        }
    }
    
    return -1;
}

int del_dontmatch_attributes(int N)
{
    FILE *fp_r_data = fopen(data_preprocess_filename, "r");
    FILE *fp_r_atts = fopen(del_duplicate_attributes_filename, "r");
    FILE *fp_w = fopen(attributes_preprocess_filename, "w");
    FILE *fp_log = fopen(attributes_preprocess_log_filename, "w");
    
    int tmp_citing,tmp_ctied;
    int bs_citing = 0;
    int bs_cited = 0;
    
    int match_rows = 0;
    int dontmatch_rows = 0;
    
    char line_feed;
    int *pid = malloc(N * sizeof(int));
    int *pid_mark = malloc(N * sizeof(int));
    char **cid = malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++)
    {
        cid[i] = malloc(cid_size * sizeof(char));
        pid_mark[i] = 0;
    }
    
    for (int i = 0 ;i < N; i++)
    {
        fscanf(fp_r_atts,"%d %c%c%c",&pid[i],&cid[i][0],&cid[i][1],&cid[i][2]);
    }
    
    while ((fscanf(fp_r_data, "%d %d",&tmp_citing,&tmp_ctied)) != EOF)
    {
        line_feed = getc(fp_r_data);
        
        bs_citing = binary_search(pid, N, tmp_citing);
        bs_cited = binary_search(pid, N, tmp_ctied);
        
        int index = 0;
        if (bs_citing != -1)
        {
            if (pid_mark[bs_citing] != 1)
            {
                pid_mark[bs_citing]  = 1;
                index = 1;
                while (pid[bs_citing+index] == pid[bs_citing] && (bs_cited+index) < N)
                {
                    pid_mark[bs_citing+index] = 1;
                    index++;
                }
                index = 1;
                while (pid[bs_citing-index] == pid[bs_citing] && (bs_cited-index) >= 0)
                {
                    pid_mark[bs_citing-index] = 1;
                    index++;
                }
            }
        }
        if (bs_cited != -1)
        {
            if (pid_mark[bs_cited] != 1)
            {
                pid_mark[bs_cited] = 1;
                
                index = 1;
                while (pid[bs_cited+index] == pid[bs_cited] && (bs_cited+index) < N)
                {
                    pid_mark[bs_cited+index] = 1;
                    index++;
                }
                index = 1;
                while (pid[bs_cited-index] == pid[bs_cited] && (bs_cited-index) >= 0)
                {
                    pid_mark[bs_cited-index] = 1;
                    index++;
                }
            }
        }
        
    }
    
    for (int i = 0; i < N; i++)
    {
        if (pid_mark[i] == 1)
        {
            fprintf(fp_w, "%d %c%c%c\n",pid[i],cid[i][0],cid[i][1],cid[i][2]);
            match_rows++;
        }
        else
        {
            dontmatch_rows++;
        }
    }
    
    fprintf(fp_log,"all_rows       = %d\n",N);
    fprintf(fp_log,"match_rows     = %d\n",match_rows);
    fprintf(fp_log,"dontmatch_rows = %d\n",dontmatch_rows);
    
    
    fclose(fp_r_data);
    fclose(fp_w);
    fclose(fp_log);
    printf("del_dontmatch_attributes() ==> %d -> %d.\n",N,match_rows);
    return match_rows;
}
