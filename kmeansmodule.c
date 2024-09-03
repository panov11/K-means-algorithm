#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdlib.h>

struct cord {
    double value;
    struct cord *next;
};

struct vector {
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
int epsilon_check(double **old,double **new,int k,int size,double epsilon){
    double e=epsilon;
    double tmp;
    int i;
    for(i=0;i<k;i++){
        tmp=d_new(old[i],new[i],size);
            if(tmp>=e){
                return 0;
            }
    }
    return 1;
}
static struct cord* create_cord_list(double *data, int length) {
    struct cord *head = NULL, *current = NULL;
    for (int i = 0; i < length; i++) {
        struct cord *new_cord = (struct cord*) malloc(sizeof(struct cord));
        if (!new_cord) {
            return NULL;
        }
        new_cord->value = data[i];
        new_cord->next = NULL;
        if (current) {
            current->next = new_cord;
        } else {
            head = new_cord;
        }
        current = new_cord;
    }
    return head;
}

static void free_vectors(struct vector *head) {
    struct vector *current_vector = head;
    while (current_vector) {
        struct cord *current_cord = current_vector->cords;
        while (current_cord) {
            struct cord *temp_cord = current_cord;
            current_cord = current_cord->next;
            free(temp_cord);
        }
        struct vector *temp_vector = current_vector;
        current_vector = current_vector->next;
        free(temp_vector);
    }
}

static PyObject* fit(PyObject *self, PyObject *args) {
    int K, iter, N, cord_count;
    double epsilon;
    PyObject *data_points_list, *initial_centroids_list;

    if (!PyArg_ParseTuple(args, "iiiidOO", &K, &iter, &N, &cord_count, &epsilon, &data_points_list, &initial_centroids_list)) {
        return NULL;
    }


    struct vector *head_vector = NULL, *current_vector = NULL;
    for (int i = 0; i < N; i++) {
        PyObject *vector_item = PyList_GetItem(data_points_list, i);
        double *data = (double*) malloc(cord_count * sizeof(double));
        if (!data) {
            PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
            free_vectors(head_vector);
            return NULL;
        }
        for (int j = 0; j < cord_count; j++) {
            data[j] = PyFloat_AsDouble(PyList_GetItem(vector_item, j));
        }
        struct cord *head_cord = create_cord_list(data, cord_count);
        free(data);
        if (!head_cord) {
            PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
            free_vectors(head_vector);
            return NULL;
        }

        struct vector *new_vector = (struct vector*) malloc(sizeof(struct vector));
        if (!new_vector) {
            PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
            free_vectors(head_vector);
            return NULL;
        }
        new_vector->next = NULL;
        new_vector->cords = head_cord;

        if (current_vector) {
            current_vector->next = new_vector;
        } else {
            head_vector = new_vector;
        }
        current_vector = new_vector;
    }

    double **centroids = (double**) malloc(K * sizeof(double*));
    double **new_centroids = (double**) malloc(K * sizeof(double*));
    double **clusters = (double**) malloc(K * sizeof(double*));

    if (!centroids || !new_centroids || !clusters) {
        PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
        free_vectors(head_vector);
        return NULL;
    }
    
    for (int i = 0; i < K; i++) {
        centroids[i] = (double*) malloc(cord_count * sizeof(double));
        new_centroids[i] = (double*) malloc(cord_count * sizeof(double));
        clusters[i] = (double*) malloc(cord_count * sizeof(double));

        if (!centroids[i] || !new_centroids[i] || !clusters[i]) {
            PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
            free_vectors(head_vector);
            return NULL;
        }
        PyObject *centroid_item = PyList_GetItem(initial_centroids_list, i);
        for (int j = 0; j < cord_count; j++) {
            centroids[i][j] = PyFloat_AsDouble(PyList_GetItem(centroid_item, j));
            new_centroids[i][j] = 0.0;
            clusters[i][j]=0.0;
        }
    }

    int *cluster_count = (int*) calloc(K, sizeof(int));
    if (!cluster_count) {
        PyErr_SetString(PyExc_MemoryError, "An Error Has Occurred");
        free_vectors(head_vector);
        return NULL;
    }

    int iterations=0;
    int check_epsilon=0;
    struct cord *curr_cord = NULL;
    int min_i;
    while(iterations<iter&&check_epsilon!=1){
        current_vector=head_vector;
        iterations=iterations+1;
        /*
        checking each vector his cluster
        */

        for(int j=0;j<N;j++){
            curr_cord=current_vector->cords;
            min_i=find_min(current_vector,centroids,cord_count,K);
            cluster_count[min_i]=cluster_count[min_i]+1;
            for(int i=0;i<cord_count;i++){
                clusters[min_i][i]=clusters[min_i][i]+curr_cord->value;
                curr_cord=curr_cord->next;
            }
            current_vector=current_vector->next;
        } 

        /*
        caulclating the new centriods
        */
        for(int i=0;i<K;i++){
            for(int j=0;j<cord_count;j++){
                new_centroids[i][j]=clusters[i][j]/cluster_count[i];


            }
        }

        if(epsilon_check(centroids,new_centroids,K,cord_count,epsilon)==1){
            check_epsilon=1;
            break;
        }
        /*
        coping new centriod to old centriod
        */
        for(int i=0;i<K;i++){
            for(int j=0;j<cord_count;j++){
                centroids[i][j]=new_centroids[i][j];
            }
        } 

        /*
        initializing to 0 the clusters and new centroid
        */
        for(int i=0;i<K;i++){
            for(int j=0;j<cord_count;j++){
                new_centroids[i][j]=0;
                clusters[i][j]=0;
            }
            cluster_count[i]=0;
        }  
    }

   
    // Free the linked list memory
    PyObject *result = PyList_New(K);
    for (int i = 0; i < K; i++) {
        PyObject *centroid = PyList_New(cord_count);
        for (int j = 0; j < cord_count; j++) {
            PyList_SetItem(centroid, j, PyFloat_FromDouble(centroids[i][j]));
        }
        PyList_SetItem(result, i, centroid);
    }

    for (int i = 0; i < K; i++) {
        free(centroids[i]);
        free(new_centroids[i]);
        free(clusters[i]);
    }
    free(centroids);
    free(new_centroids);
    free(cluster_count);
    free(clusters);
    free_vectors(head_vector);

    return result;
}

static PyMethodDef MyKMeansMethods[] = {
    {"fit", fit, METH_VARARGS, "Fit K-means clustering algorithm.\n\n"
                               "Arguments:\n"
                               "centroids -- Initial centroids (list of lists).\n"
                               "data_points -- Data points (list of lists).\n"
                               "K -- Number of clusters.\n"
                               "N -- Number of data points.\n"
                               "dim -- Dimension of each data point.\n"
                               "max_iter -- Maximum number of iterations.\n"
                               "epsilon -- Convergence threshold.\n"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mykmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1,
    MyKMeansMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    return PyModule_Create(&mykmeansmodule);
}