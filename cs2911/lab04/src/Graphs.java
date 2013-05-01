import java.util.LinkedList;

public interface Graphs {
	void addVertex(Object o);
	void addEdge(Object from, Object to);
	void removeVertex(Object o);
	void removeEdge(Object from, Object to);
	boolean isConnected(Object from, Object to);
	boolean isVertex(Object o);
	LinkedList<Object> connectedNodes(Object o);
}
