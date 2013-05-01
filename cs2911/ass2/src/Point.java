import java.lang.Math;
public class Point {
	private int x;
	private int y;
	
	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public int distance(Point to) {
		return Math.abs(to.x-this.x) + Math.abs(to.y-this.y);
	}
	
	public int getX() {
		return this.x;
	}
	
	public int getY() {
		return this.y;
	}
}
