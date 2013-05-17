import java.lang.Math;

/**
 * A Point is a set of coordinates on a Cartesian plane. It consists of two ordinates, x and y.
 * @author vtra143
 */
public class Point {
	private int x;
	private int y;
	
	/**
	 * Constructs a Point object.
	 * @param x the x value of the Point
	 * @param y the y value of the Point
	 */
	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	/**
	 * Returns the Manhattan distance between two points.
	 * @param to the Point that you calculate the distance to
	 * @return the Manhattan distance between two points
	 */
	public int distance(Point to) {
		return Math.abs(to.x-this.x) + Math.abs(to.y-this.y);
	}
	
	/**
	 * Returns the x ordinate of a Point.
	 * @return the x ordinate of a Point
	 */
	public int getX() {
		return this.x;
	}
	
	/**
	 * Returns the y ordinate of a Point.
	 * @return the y ordinate of a Point
	 */
	public int getY() {
		return this.y;
	}
}
