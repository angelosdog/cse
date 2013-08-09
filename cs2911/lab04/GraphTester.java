import org.junit.*;

public class GraphTester {

	@Test
	public void test() {
		dGraph graph = new dGraph();
		graph.addVertex(1);
		graph.addVertex(2);
		assert graph.isVertex(1) == true;
		assert graph.isVertex(2) == true;
		graph.addEdge(1, 2);
		assert graph.isConnected(1, 2) == true;
		assert graph.isConnected(1, 2) == false;
		graph.removeEdge(1, 2);
		assert (graph.isConnected(1, 2)) == false;
		graph.removeVertex(1);
		graph.removeVertex(2);
		assert (graph.isVertex(1)) == false;
		assert (graph.isVertex(2)) == false;
	}

}