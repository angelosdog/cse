import java.util.LinkedList;

public class dGraph implements Graphs {
	
	private LinkedList<LinkedList<Integer>> adjList;
	private LinkedList<Integer> vertices;
	
	public dGraph()
	{
		adjList = new LinkedList<LinkedList<Integer>>();
		vertices = new LinkedList<Integer>();
	}
	
	public void addVertex(Object o)
	{
		if(o instanceof Integer){
			int i = 0;
			vertices.add((Integer) o);
			LinkedList<Integer> newRow = new LinkedList<Integer>();
			LinkedList<Integer> currentRow;
			for(i=0; i <= adjList.size(); i++)
			{
				newRow.add(0);
			}
			for(i=0; i < adjList.size(); i++)
			{
				currentRow = adjList.get(i);
				currentRow.add(0);
			}
			adjList.add(newRow);
		}
	}
	
	public void addEdge(Object from, Object to)
	{
		if(from instanceof Integer && to instanceof Integer)
		{
			if(vertices.contains(from) && vertices.contains(to))
			{
				LinkedList<Integer> row = adjList.get(vertices.indexOf(from));
				int currEdges = row.get(vertices.indexOf(from));
				currEdges++;
				row.set(vertices.indexOf(from),currEdges);
			}
		}
	}
	public void removeVertex(Object o)
	{
		if(o instanceof Integer && vertices.contains(o))
		{
			int i = 0;
			int index = vertices.indexOf(o);
			LinkedList<Integer> currentRow;
			
			for(i=0; i < adjList.size(); i++)
			{
				currentRow = adjList.get(i);
				currentRow.remove(index);
			}
			adjList.remove(index);
			vertices.remove(o);
		}
	}
	public void removeEdge(Object from, Object to)
	{
		if(from instanceof Integer && to instanceof Integer )
		{
			if(vertices.contains(from) && vertices.contains(to))
			{
				LinkedList<Integer> row = adjList.get(vertices.indexOf(from));
				if(row.get(vertices.indexOf(from)) > 0)
				{
					row.set(vertices.indexOf(from),row.get(vertices.indexOf(from))-1);
				}
			}
		}
	}
	public boolean isConnected(Object from, Object to)
	{
		if(from instanceof Integer && to instanceof Integer)
		{
			if(vertices.contains(from) && vertices.contains(to))
			{
				LinkedList<Integer> row = adjList.get(vertices.indexOf(from));
				return row.get(vertices.indexOf(from)) > 0;
					
			}
		}
		return false;
	}
	public boolean isVertex(Object o)
	{
		return o instanceof Integer && vertices.contains(o);
	}
	public LinkedList<Object> connectedNodes(Object o)
	{
		LinkedList<Object> connected = new LinkedList<Object>();
		if(o instanceof Integer)
		{
			int i;
			int index = vertices.indexOf(o);
			LinkedList<Integer> row = adjList.get(index);
			for(i = 0; i < adjList.size(); i++)
			{	
				if(row.get(i) > 0)
				{
					connected.add(row.get(i));
				}
			}
		}
		return connected;
	}	
}
