public class Chassis extends Assembly {
	
	public Chassis()
	{
		parts.add(new Frame());
		parts.add(new Wheel());
		parts.add(new Wheel());
		parts.add(new Wheel());
		parts.add(new Wheel());
	}

}
