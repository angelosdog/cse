
public class Job {

	private Point from;
	private Point to;
	
	Job(Point from, Point to) {
		this.from = from;
		this.to = to;
	}
	public Point getFrom() {
		return this.from;
	}
	
	public Point getTo() {
		return this.to;
	}
	
}