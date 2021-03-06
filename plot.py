import matplotlib.pyplot as plt
import networkx as nx
import sys
import re

def plot_agent(filename):
    plt.close()
    plt.ion()
    plt.show()
    
    G = nx.DiGraph()
    F = nx.DiGraph()
    
    inputSize = 0
    outputSize = 0
    
    with open(filename, 'r') as file:
        for line in file.readlines():
            info = re.findall("-*\d+\.\d*|\d+",line)
            if len(info) == 2:
                inputSize = int(info[0])
                outputSize = int(info[1])
            if len(info) == 3:
                F.add_edge(int(info[0]), int(info[1]), weight=info[2][:4] if info[2][0] != '-' else info[2][:5] )
                print(f'{info[0]} {info[1]} {info[2]}')

    for i in range(1, inputSize+1):
        G.add_node(i, layer=1)
    
    for i in range(inputSize+1, inputSize+outputSize+1):
        G.add_node(i, layer=2)

    posG = nx.multipartite_layout(G, subset_key="layer")
    posF = nx.spring_layout(F)
    posF.update(posG)
    
    nx.draw(F, posF, with_labels=True)
    labels = nx.get_edge_attributes(F,'weight')
    nx.draw_networkx_edge_labels(F,posF,edge_labels=labels)
    plt.show()
    plt.pause(0)
    # plt.pause(0.001)

if __name__ == '__main__':
    if (len(sys.argv) < 2):
        exit(0)
    plot_agent(sys.argv[1])
