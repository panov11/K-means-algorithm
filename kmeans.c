# include <stdio.h>
# include <stdlib.h>
#include <math.h>

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};
/*
 reciving 2 arrays and returns euclidic distance
*/
double d_new(double *a,double *b,int size){
    double sum=0;
    double tmp=0;
    int i;
    for(i=0;i<size;i++){
        tmp=a[i]-b[i];
        sum=sum+pow(tmp,2);
    }

    return sqrt(sum);
}
/*
reciving vector and centroids and returns the min index
*/
int find_min(struct vector *v1,double **u,int size,int k){
    double *vec;
    struct cord *curr;
    int i, min_index=0;
    double min,tmp;
    vec=calloc(size,sizeof(double));
    if(vec==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    curr=v1->cords;
    for(i=0;i<size;i++){
        vec[i]=curr->value;
        curr=curr->next;
    }

    min=d_new(vec,u[0],size);

    for(i=0;i<k;i++){
        tmp=d_new(vec,u[i],size);

        if(tmp<=min){
            min=tmp;
            min_index=i;
        }
    }
    free(vec);

    return min_index;
}

/*
checks if Eclidean Distance between the updated centroid to the previous one is less then epsilon for all centroids
*/
int epsilon_check(double **old,double **new,int k,int size){
    double e=0.001,tmp;
    int i;
    for(i=0;i<k;i++){
        tmp=d_new(old[i],new[i],size);
            if(tmp>=e){
                return 0;
            }
    }
    return 1;
}

    


int main(int argc, char **argv)
{
    int k,iter;
    struct vector *head_vec, *curr_vec,*tmp_vec,*next_vec;
    struct cord *head_cord, *curr_cord,*tmp_cord,*next_cord;
    int i, j,num_cords=0,check_epsilon=0,iterations=0,num_vec=0,first=1;
    double n;
    char c;
    double **u,**new_u,**clusters;
    int *size_cluster;
    int min_i;


    if(argc!=3){
        
        iter=200;
    }
    else{
        iter = atoi(argv[2]);

    }
    k=atoi(argv[1]);


    

    head_cord = malloc(sizeof(struct cord));
    if(head_cord==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if(head_vec==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    curr_vec = head_vec;
    curr_vec->next = NULL;


    /*
    reding the file to vectors
    */
    while (scanf("%lf%c", &n, &c) == 2)
    {
        if(first==1){
           num_cords=num_cords+1; 
        }

        if (c == '\n')
        {
            first=0;
            num_vec=num_vec+1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if(curr_vec->next==NULL){
                printf("An Error Has Occurred");
                exit(1);
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if(head_cord==NULL){
                printf("An Error Has Occurred");
                exit(1);
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        if(curr_cord->next==NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;

    }
    free(curr_cord);
    if(k<=1||k>=num_vec){
        printf("invalid number of clusters!\n");
        exit(1);
    }
    if(iter<=1||iter>=1000)
    {
        printf("invalid maximum iteration!\n");
        exit(1);
    }


    /*
    setting up clusters and centroids arrays
    */
    u=calloc(k,sizeof(double *));
    if(u==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    new_u=calloc(k,sizeof(double *));
    if(new_u==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    clusters=calloc(k,sizeof(double *));
    if(clusters==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    size_cluster=calloc(k,sizeof(double));
    if(size_cluster==NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    for(i=0;i<k;i++){
        u[i]=calloc(num_cords,sizeof(double));
        if(u[i]==NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        new_u[i]=calloc(num_cords,sizeof(double));
        if(new_u[i]==NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
        clusters[i]=calloc(num_cords,sizeof(double));
        if(clusters[i]==NULL){
            printf("An Error Has Occurred");
            exit(1);
        }
    }

    /*
    intiailizng first k centroids to k vectors and clusters to 0
    */
    tmp_vec=head_vec;
    for(i=0;i<k;i++){
        tmp_cord=tmp_vec->cords;
        for(j=0;j<num_cords;j++){
            u[i][j]=tmp_cord->value;
            tmp_cord=tmp_cord->next;
            clusters[i][j]=0.0;

        }
        size_cluster[i]=0.0;
        tmp_vec=tmp_vec->next;
    }
    

    while(iterations<iter&&check_epsilon!=1){
        curr_vec=head_vec;
        iterations=iterations+1;
        /*
        checking each vector his cluster
        */

        for(j=0;j<num_vec;j++){
            curr_cord=curr_vec->cords;
            min_i=find_min(curr_vec,u,num_cords,k);

            size_cluster[min_i]=size_cluster[min_i]+1;
            for(i=0;i<num_cords;i++){
                clusters[min_i][i]=clusters[min_i][i]+curr_cord->value;
                curr_cord=curr_cord->next;
            }
            curr_vec=curr_vec->next;
        } 

        /*
        caulclating the new centriods
        */
        for(i=0;i<k;i++){
            for(j=0;j<num_cords;j++){
                new_u[i][j]=clusters[i][j]/size_cluster[i];


            }
        }

        if(epsilon_check(u,new_u,k,num_cords)==1){
            break;
        }
        /*
        coping new centriod to old centriod
        */
        for(i=0;i<k;i++){
            for(j=0;j<num_cords;j++){
                u[i][j]=new_u[i][j];
            }
        } 

        /*
        initializing to 0 the clusters and new centroid
        */
        for(i=0;i<k;i++){
            for(j=0;j<num_cords;j++){
                new_u[i][j]=0;
                clusters[i][j]=0;
            }
            size_cluster[i]=0;
        }  
    }

    for(i=0;i<k;i++){
        for(j=0;j<num_cords;j++){
            if(j==num_cords-1){
                printf("%.4f\n",u[i][j]);

            }
            else{
                printf("%.4f, ",u[i][j]);
            }

        }
    }
    /*
    free memory alocated
    */

    for(i=0;i<k;i++){
        free(u[i]);
        free(new_u[i]);
        free(clusters[i]);
    }

    for(i=0;i<num_vec;i++){
        head_cord=head_vec->cords;
        for(j=0;j<num_cords;j++){
            if(j==num_cords-1){
                free(head_cord);
            }
            else{
                next_cord=head_cord->next;
                free(head_cord);
                head_cord=next_cord;
            }
            
        }
        if(i==num_vec-1){
            free(head_vec);
        }
        else{
            next_vec=head_vec->next;
            free(head_vec);
            head_vec=next_vec;

        }
        
    }

    
    free(u);
    free(new_u);
    free(clusters);
    free(size_cluster);
    free(curr_vec);





    return 0;
}

