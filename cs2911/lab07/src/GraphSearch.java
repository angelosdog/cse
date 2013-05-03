import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Queue;
public class GraphSearch extends Graph {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Graph g = constructRomania();
		
		String end = "Bucharest";
		System.out.println("Arad: " + Astar("Arad", end, g));
		System.out.println("Bucharest: " + Astar("Bucharest", end, g));
		System.out.println("Craiova: " + Astar("Craiova", end, g));
		System.out.println("Dobreta: " + Astar("Dobreta", end, g));
		System.out.println("Eforie: " + Astar("Eforie", end, g));
		System.out.println("Fagaras: " + Astar("Fagaras", end, g));
		System.out.println("Giurgiu: " + Astar("Giurgiu", end, g));
		System.out.println("Hirsova: " + Astar("Hirsova", end, g));
		System.out.println("Iasi: " + Astar("Iasi", end, g));
		System.out.println("Lugoj: " + Astar("Lugoj", end, g));
		System.out.println("Mehadia: " + Astar("Mehadia", end, g));
		System.out.println("Neamt: " + Astar("Neamt", end, g));
		System.out.println("Oradea: " + Astar("Oradea", end, g));
		System.out.println("Pitesti: " + Astar("Pitesti", end, g));
		System.out.println("Rimnicu Vilcea: " + Astar("Rimnicu Vilcea", end, g));
		System.out.println("Sibiu: " + Astar("Sibiu", end, g));
		System.out.println("Timisoara: " + Astar("Timisoara", end, g));
		System.out.println("Urziceni: " + Astar("Urziceni", end, g));
		System.out.println("Vaslui: " + Astar("Vaslui", end, g));
		System.out.println("Zerind: " + Astar("Zerind", end, g));
	
	}
	
	private static LinkedList<String> Astar(String start, String end, Graph g) {
		Queue<ASVertex> nextTowns = new LinkedList<ASVertex>();
		ArrayList<ASVertex> toAdd = new ArrayList<ASVertex>();
		ArrayList<Vertex> visited = new ArrayList<Vertex>();
		ASVertex from = new ASVertex(g.findVertex(start), 0, g.findVertex(start).getDistance(), null);
		ASVertex to = new ASVertex(g.findVertex(end), 0, g.findVertex(end).getDistance(), null);
		nextTowns.add(from);
		while(from != null) {
			from = nextTowns.poll();
			if (!visited.contains(from.getVertex())) {
				if (from.getVertex() == to.getVertex()) {
					return reconstructPath(from, new LinkedList<String>());
				}
				for(int i = 0; i < from.getVertex().getEdges().size(); i++){
					Vertex nextVertex = from.getVertex().getEdges().get(i).getTo();
					int costToNext = from.getCost() + from.getVertex().getEdges().get(i).getWeight();
					int heuristic = costToNext + nextVertex.getDistance();
					ASVertex next = new ASVertex(nextVertex, costToNext, heuristic, from);
					next.setCost(from.getCost() + from.getVertex().getEdges().get(i).getWeight());
					next.setHeuristic(next.getCost() + next.getVertex().getDistance());
					toAdd.add(next);
				}
				Collections.sort(toAdd, new CompareCost());
				nextTowns.addAll(toAdd);
				visited.add(from.getVertex());
			}
		} 
		return null;
	}

	private static LinkedList<String> reconstructPath(ASVertex from, LinkedList<String> path) {
		if (from == null) {
			return path;
		}
		else {
			path.addFirst(from.getVertex().getValue());
			return reconstructPath(from.getPrevious(), path);
		}
	}

	private static Graph constructRomania() {
		Graph g = new Graph();
		g.addVertex("Arad", 366);
		g.addVertex("Bucharest", 0);
		g.addVertex("Craiova", 160);
		g.addVertex("Dobreta", 242);
		g.addVertex("Eforie", 161);
		g.addVertex("Fagaras", 178);
		g.addVertex("Giurgiu", 77);
		g.addVertex("Hirsova", 151);
		g.addVertex("Iasi", 226);
		g.addVertex("Lugoj", 244);
		g.addVertex("Mehadia", 241);
		g.addVertex("Neamt", 234);
		g.addVertex("Oradea", 380);
		g.addVertex("Pitesti", 98);
		g.addVertex("Rimnicu Vilcea", 193);
		g.addVertex("Sibiu", 253);
		g.addVertex("Timisoara", 329);
		g.addVertex("Urziceni", 80);
		g.addVertex("Vaslui", 199);
		g.addVertex("Zerind", 374);
		
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
		g.addEdge("Rimnicu Vilcea", "Craiova", 146);
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
		g.addEdge("Iasi", "Neamt", 71);
		return g;
	}

}
