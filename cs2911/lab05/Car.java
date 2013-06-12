
public class Car extends Assembly{
	public Car()
	{
		parts.add(new Body());
		parts.add(new Engine());
		parts.add(new Chassis());
	}
}
