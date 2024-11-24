```markdown
# K-Means Clustering Algorithm with K-Means++ Initialization

This repository contains an implementation of the **K-Means clustering algorithm**, including the **K-Means++ initialization method**, with both Python and C extensions for performance optimization. The implementation is designed to handle large datasets efficiently.

---

## Features

- Implements the K-Means clustering algorithm with optimized **K-Means++** initialization.
- Hybrid implementation using Python for flexibility and C for computational efficiency.
- Handles input data from `.txt` or `.csv` files.
- Convergence based on user-defined epsilon (`eps`) threshold.

---

## Repository Structure

```
root/
├── main.py                 # Python driver for K-Means++ initialization and clustering
├── kmeansmodule.c          # C implementation of core K-Means functionality
├── setup.py                # Build script for the Python C extension
├── README.md               # Project documentation
├── mykmeanssp.so           # Compiled Python C extension
└── utils.py                # Python utilities for K-Means
```

---

## Usage

### Dependencies

- Python 3.7 or higher
- NumPy
- GCC (for compiling the C extension)

### Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-repo-url>/kmeans-clustering.git
   cd kmeans-clustering
   ```
2. Compile the C extension:
   ```bash
   python setup.py build_ext --inplace
   ```
3. Ensure the `mykmeanssp.so` file is in the root directory.

---

### Running the Algorithm

#### Command-Line Interface

Run the algorithm using:
```bash
python main.py <k> <max_iter> <eps> <file1> <file2>
```

- `<k>`: Number of clusters (must be between 1 and the number of data points).
- `<max_iter>`: Maximum number of iterations (default: 300).
- `<eps>`: Convergence threshold for centroids (default: 0.001).
- `<file1>` and `<file2>`: Paths to `.csv` or `.txt` files containing the input datasets.

#### Example

```bash
python main.py 3 300 0.001 data1.csv data2.csv
```

---

## How It Works

### K-Means Algorithm

1. **Initialization**:  
   The algorithm uses K-Means++ for optimized initial centroid selection.
2. **Assignment Step**:  
   Each data point is assigned to the nearest cluster based on Euclidean distance.
3. **Update Step**:  
   Centroids are recomputed as the mean of all points in their cluster.
4. **Convergence Check**:  
   The algorithm stops if the centroids' movements are below `eps` or the maximum number of iterations is reached.

---

## Code Details

### Python Script: `main.py`

- Handles input validation and pre-processing using pandas and numpy.
- Implements K-Means++ initialization.
- Delegates computationally intensive tasks to the C extension `mykmeanssp`.

### C Extension: `kmeansmodule.c`

- Optimized implementation for:
  - Euclidean distance computation.
  - Cluster assignment.
  - Centroid updates.
  - Convergence checks.
- Uses linked lists and arrays for efficient memory management.
- Provides the `fit` function, integrating seamlessly with Python.

### Setup Script: `setup.py`

- Compiles the C extension using Python's setuptools.
- Links the C implementation with Python.

---

## Algorithm Parameters

| Parameter    | Description                                |
|--------------|--------------------------------------------|
| `k`          | Number of clusters                        |
| `max_iter`   | Maximum iterations (default: 300)         |
| `eps`        | Convergence threshold (default: 0.001)    |
| `file1`      | Path to the first input dataset           |
| `file2`      | Path to the second input dataset          |

---

## Example Input and Output

### Input
- `data1.csv`:
    ```
    1.0, 2.0
    2.0, 3.0
    3.0, 4.0
    ```
- `data2.csv`:
    ```
    1.1, 2.1
    2.1, 3.1
    3.1, 4.1
    ```

### Command
```bash
python main.py 2 300 0.001 data1.csv data2.csv
```

### Output
```plaintext
1,3
1.0000,2.0000
2.5000,3.5000
```

---

## Performance Optimization

- **C Extension**:  
  Computationally expensive tasks, such as distance calculations and centroid updates, are implemented in C for improved performance.
- **Efficient Memory Management**:  
  Linked list and array structures are used for handling large datasets efficiently.

---

## Testing

1. Compile the C extension using `setup.py`.
2. Use test datasets provided in the repository.
3. Run:
   ```bash
   python main.py <k> <max_iter> <eps> <file1> <file2>
   ```

---

## Contact

For any questions or contributions, feel free to open an issue or submit a pull request.
```
