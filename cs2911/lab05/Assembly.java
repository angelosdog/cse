import java.util.LinkedList;

public class Assembly implements Part {
	protected LinkedList<Part> parts;
	
	public int getWeight()
	{
		int weight = 0;
		for(int i=0; i < parts.size(); i++)
		{
			weight = weight + parts.get(i).getWeight();
		}
		return weight;
	}
}
