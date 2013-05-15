import java.util.Iterator;
import java.util.LinkedList;

/**
 * Example Directed graph implementation for Strings for COMP2911 Lab Week 7
 * @author Timothy Wiley
 */
public class Graph {
	/**
	 * The directed graph is represented by a list of nodes
	 */
	protected LinkedList<Vertex> vertices;
	
	/**
	 * Construct an empty Directed String Graph
	 */
	public Graph() {
		vertices = new LinkedList<Vertex>();
	}
	
	/**
	 * Add the given string to the graph, creating a new vertex.
	 * Strings compared for uniqueness by their equals() method
	 * @param s String to add to the graph
	 */
	public void addVertex(String s) {
		if (!containsVertex(s)) {
			Vertex node = new Vertex(s);
			vertices.add(node);
		}
	}
	
	/**
	 * Removes the given string from the graph, and all outgoing/incoming connections
	 * Strings compared for uniqueness by their equals() method
	 * @param s String to remove from the graph
	 */
	public void deleteVertex(String s) {
		// Get the node
		Vertex toDelete = findVertex(s);
		
		if (toDelete != null) {
			// Remove node
			vertices.remove(toDelete);
			
			// Remove all incoming connections
			for (Vertex from : vertices) {
				for(Edge edge : from.getEdges())
					if(edge.getTo().equals(toDelete)){
						Edge e = new Edge(from, toDelete, edge.getWeight());
						deleteEdge(e);
					}
			}
			for(Edge edge : toDelete.getEdges()){
				Edge e = new Edge(toDelete, edge.getTo(), edge.getWeight());
				deleteEdge(e);
			}
		}
	}

	/**
	 * Adds an directed edge from the node sFrom to the node sTo
	 * @param sFrom From vertex
	 * @param sTo To vertex
	 */
	public void addEdge(String sFrom, String sTo, int weight) {
		Vertex from = findVertex(sFrom);
		Vertex to = findVertex(sTo);
		
		if (from != null && to != null) {
			Edge edge = new Edge(from, to, weight);
			if (!from.getEdges().contains(edge)) {
				from.getEdges().add(edge);
				edge = new Edge(to, from, weight);
				to.getEdges().add(edge);
			}	
		}
	}
	
	/**
	 * Removes the given edge from the graph, if the edge exists
	 * @param sFrom From vertex
	 * @param sTo To vertex
	 */
	public void deleteEdge(String sFrom, String sTo, int weight) {
		Vertex from = findVertex(sFrom);
		Vertex to = findVertex(sTo);
		
		if (from != null && to != null) {
			Edge edge = new Edge(from, to, weight);
			deleteEdge(edge);
			edge = new Edge(to, from, weight);
			deleteEdge(edge);
		}
	}
	
	/**
	 * Removes the given edge from the graph, if the edge exists
	 * @param edge Edge to remove from the graph
	 */
	private void deleteEdge(Edge edge) {
		edge.getFrom().getEdges().remove(edge);
	}
	
	/**
	 * True if the graph contains the given node.
	 * Strings compared for uniqueness by their equals() method 
	 * @param s String to find
	 * @return True if the vertex is contained in the graph, false otherwise
	 */
	public boolean containsVertex(String s) {
		Iterator<Vertex> nit = vertices.iterator();
		while (nit.hasNext()) {
			Vertex next = nit.next();
			if (next.equals(s)) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Number of vertices in the graph
	 * @return Number of vertices in the graph
	 */
	public int nVertices() {
		return vertices.size();
	}
	
	/**
	 * Retrieve all of the nodes stored in this graph
	 * @return Array of vertices in the graph
	 */
	public String[] getVertex() {
		String[] sVertices = new String[vertices.size()];
		int i = 0;
		for (Vertex vertex : vertices) {
			sVertices[i] = vertex.getValue();
			++i;
		}
		return sVertices;
	}
	
	/**
	 * Determines if the directed edge exists
	 * @param sFrom From vertex
	 * @param sTo To vertex
	 */
	public boolean isConnected(String sFrom, String sTo) {
		Vertex from = findVertex(sFrom);
		Vertex to = findVertex(sTo);
		boolean connected = false;
		if (from != null && to != null) {
			for(Edge edge : from.getEdges()){
				if(edge.getTo() == to) connected = true;
			}
		}
		return connected;
	}
	
	/**
	 * Returns all of the nodes connected by an edge to the given node
	 * @param s Vertex to get connections of
	 * @return Array of nodes x such that there exists an edge s -> x
	 */
	public String[] getEdges(String s) {
		String[] edges = null;
		Vertex node = new Vertex(s);
		
		if (node != null) {
			edges = new String[node.getEdges().size()];
			int i = 0;
			for(Edge edge : node.getEdges()) {
				edges[i] = edge.getTo().getValue();
				++i;
			}
		}
		
		return edges;
	}
	
	/**
	 * Finds the vertex for the given string s
	 * @param s String to find
	 * @return Vertex corresponding to s
	 */
	public Vertex findVertex(String s) {
		Vertex vertex = null;
		Iterator<Vertex> nit = vertices.iterator();
		while (nit.hasNext() && vertex == null) {
			Vertex next = nit.next();
			if (next.equals(s)) {
				vertex = next;
			}
		}
		return vertex;
	}
	
	@Override
	public String toString() {
		StringBuffer string = new StringBuffer();
		for (Vertex vertex : vertices) {
			string.append(vertex.toString());
		}
		return string.toString();
	}
	
	public LinkedList<Vertex> getVertices() {
		return this.vertices;
	}
}
