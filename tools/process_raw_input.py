import os
import shutil
import numpy as np

def clear_directory_contents(directory):
    if os.path.exists(directory):
        shutil.rmtree(directory)
    os.makedirs(directory)

def process_raw_input(source_path, target_path, file_name):
    def parse_is_directed(line):
        values = line.split(" ")

        directed_value = values[1]
        weighted_value = values[2].strip('\n')
        
        is_graph_directed = None
        is_graph_weighted = None
        
        if directed_value == "directed":
            is_graph_directed = True
        elif directed_value == "undirected":
            is_graph_directed = False
        else:
            raise Exception("Invalid graph direction: ", directed_value)
        
        if weighted_value == "weighted":
            is_graph_weighted = True
        elif weighted_value == "unweighted":
            is_graph_weighted = False
        else:
            raise Exception("Invalid graph weight: ", weighted_value)
        
        return is_graph_directed, is_graph_weighted
    
    def parse_num_vertices_and_edges(line):
        values = line.split(" ")
        return int(values[1]), int(values[2].strip('\n'))
    
    def parse_edge_line(line, is_graph_weighted):
        values = line.split(" ")
        if is_graph_weighted:
            return int(values[0]), int(values[1]), float(values[2].strip('\n'))
        else:
            return int(values[0]), int(values[1].strip('\n')), 1
    
    is_graph_directed = None
    is_graph_weighted = None
    num_vertices = None
    num_edges = None
    matrix = None
    
    skip = 0
    
    unique_vertices = set()
    # Read the raw input
    # If the graph is undirected then each entry is added twice to 
    # the matrix as (u,v) and (v,u). Otherwise, if the graph is 
    # directed then the edges are added normally.
    with open(source_path + file_name, "r") as file:
        for line in file:
            if is_graph_directed is None:
                is_graph_directed, is_graph_weighted = parse_is_directed(line)
                
            elif num_vertices is None:
                num_vertices, num_edges = parse_num_vertices_and_edges(line)
                
            else:
                if matrix is None:
                    matrix = np.zeros((num_vertices, num_vertices))
                
                skip += 1
                

                    
                source, target, weight = parse_edge_line(line, is_graph_weighted)          
                matrix[source, target] = weight
                
                try:
                    if skip % 2 == 0:
                        matrix[source + 1, target + 1] = weight
                except:
                    pass
                        
                unique_vertices.add(source)
                unique_vertices.add(target)
                
                if not is_graph_directed:
                    matrix[target, source] = weight
    
    num_vertices = max(unique_vertices)
    
    # Compute the number of edges in the graph to write to the file.
    # If the graph is undirected then the number of edges is half the
    # number of non-zero entries in the matrix. In other words, we 
    # only write the upper triangle of the matrix.
    num_edges = int(np.count_nonzero(matrix))
    
    # Write the processed output
    with open(target_path + file_name, "w") as file:
        file.write(f"% {'directed' if is_graph_directed else 'undirected'}\n")
        file.write(f"% {num_vertices} {num_edges}\n") 
        
        for row_idx in range(num_vertices):
            for col_idx in range(num_vertices):   
                weight = int(matrix[row_idx, col_idx])
                if weight == 0:
                    continue
                
                file.write(f"{row_idx} {col_idx} {weight}\n")

source_dir = "../data/input_raw/"
target_dir = "../data/input/"

clear_directory_contents(target_dir)

for file_name in os.listdir(source_dir):
    process_raw_input(source_dir, target_dir, file_name)
    