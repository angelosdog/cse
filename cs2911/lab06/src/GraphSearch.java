import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Queue;
public class GraphSearch {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Graph g = constructRomania();
		
		String start = "Arad";
		String end = "Bucharest";
		System.out.println(bfs(start, end, g));
		System.out.println(ucs(start, end, g));
	
	}
	
	private static boolean bfs(String start, String end, Graph g) {
		Queue<Vertex> nextTowns = new LinkedList<Vertex>();
		ArrayList<Vertex> toAdd = new ArrayList<Vertex>();
		ArrayList<Vertex> visited = new ArrayList<Vertex>();
		Vertex from = g.findVertex(start);
		Vertex to = g.findVertex(end);
		nextTowns.add(from);
		from = nextTowns.poll();
		do {
			if(from == to) {
				return true;
			}
			
			if(!visited.contains(from)) {
				for(int i = 0; i < from.getEdges().size(); i++){
					if(!visited.contains(from.getEdges().get(i).getTo())){
						toAdd.add(from.getEdges().get(i).getTo());
					} 
				}
			}
			Collections.sort(toAdd, new CompareVerticesName());
			nextTowns.addAll(toAdd);
			visited.add(from);
			from = nextTowns.poll();
		} while (from != null);
		return false;
	}
	
	private static Integer ucs(String start, String end, Graph g) {
		Queue<Vertex> nextTowns = new LinkedList<Vertex>();
		ArrayList<Vertex> toAdd = new ArrayList<Vertex>();
		ArrayList<Vertex> visited = new ArrayList<Vertex>();
		Vertex from = g.findVertex(start);
		Vertex to = g.findVertex(end);
		nextTowns.add(from);
		from = nextTowns.poll();
		do {
			if (from == to) {
				return from.getCost();
			}
			if(!visited.contains(from)) {
				for(int i = 0; i < from.getEdges().size(); i++){
					if(visited.contains(from.getEdges().get(i).getTo()) == false){
						Vertex next = from.getEdges().get(i).getTo();
						toAdd.add(next);
						next.setCost(from.getCost() + from.getEdges().get(i).getWeight());
					}
				}
			}
			Collections.sort(toAdd, new CompareVerticesCost());
			nextTowns.addAll(toAdd);
			visited.add(from);
			from = nextTowns.poll();
		} while(from != null);
		return null;
	}

	private static Graph constructRomania() {
		Graph g = new Graph();
		g.addVertex("Arad");
		g.addVertex("Bucharest");
		g.addVertex("Craiova");
		g.addVertex("Dobreta");
		g.addVertex("Eforie");
		g.addVertex("Fagaras");
		g.addVertex("Giurgiu");
		g.addVertex("Hirsova");
		g.addVertex("Iasi");
		g.addVertex("Lugoj");
		g.addVertex("Mehadia");
		g.addVertex("Neamt");
		g.addVertex("Oradea");
		g.addVertex("Pitesti");
		g.addVertex("Rimnicu Vilcea");
		g.addVertex("Sibiu");
		g.addVertex("Timisoara");
		g.addVertex("Urziceni");
		g.addVertex("Vaslui");
		g.addVertex("Zerind");
		
		g.addEdge("Oradea", "Zerind", 71);
		g.addEdge("Zerind", "Arad", 75);
		g.addEdge("Oradea", "Sibiu", 151);
		g.addEdge("Arad", "Sibiu", 140);
		g.addEdge("Arad", "Timisoara", 118);
		g.addEdge("Timisoara", "Lugoj", 111);
		g.addEdge("Lugoj", "Mehadia", 70);
		g.addEdge("Mehadia", "Dobreta", 75);
		g.addEdge("Dobreta", "Craiova", 71);
		g.addEdge("Sibiu", "Rimnicu Vilcea", 80);
		g.addEdge("Rimnicu", "Craiova", 146);
		g.addEdge("Sibiu", "Fagaras", 99);
		g.addEdge("Rimnicu Vilcea", "Pitesti", 97);
		g.addEdge("Craiova", "Pitesti", 138);
		g.addEdge("Pitesti", "Bucharest", 101);
		g.addEdge("Fagaras", "Bucharest", 211);
		g.addEdge("Bucharest", "Giurgiu", 90);
		g.addEdge("Bucharest", "Urziceni", 85);
		g.addEdge("Urziceni", "Hirsova", 98);
		g.addEdge("Hirsova", "Eforie", 86);
		g.addEdge("Urziceni", "Vaslui", 142);
		g.addEdge("Vaslui", "Iasi", 92);
		g.addEdge("Iasi", "Nearnt", 71);
		return g;
	}

}
