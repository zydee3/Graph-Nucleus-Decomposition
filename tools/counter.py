import re
import networkx as nx
import matplotlib.pyplot as plt

source_dir = "data/input/facebook"
pettern_head = r"% (\d+) (\d+)"
pattern_edge = r"(\d+)\s(\d+)"

def get_graph(source_dir):
    with open(source_dir, "r") as file:
        is_undirected = True
        num_vertices = -1
        num_edges = -1
        
        graph = nx.Graph()
        
        for line in file:
            if line.startswith("%"):
                if line == "% undirected": 
                    is_undirected = True
                elif line == "% directed":
                    is_undirected = False
            
                match = re.match(pettern_head, line)
                if match:
                    num_vertices = int(match.group(1))
                    num_edges = int(match.group(2))
            else:   
                match = re.match(pattern_edge, line)
                if match:
                    if num_vertices == -1 or num_edges == -1:
                        raise Exception("No graph size found")
                    
                    
                    u = int(match.group(1))
                    v = int(match.group(2))
                    
                    graph.add_edge(u, v)
                    # print(num1, num2)
                else:
                    raise Exception("Invalid edge line")
                
        return graph
    
graph = get_graph(source_dir)

print("Number of vertices: ", graph.number_of_nodes())
print("Number of edges: ", graph.number_of_edges())
print("is directed: ", graph.is_directed())
