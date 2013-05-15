import java.awt.*;
import java.awt.geom.*;

/**
   A shape that manages its selection state.
 */
public abstract class SelectableShape implements SceneShape
{
	private boolean selected;
	protected int x;
	protected int y;
	
	public void setSelected(boolean b)
	{
		selected = b;
	}

	public boolean isSelected()
	{
		return selected;
	}

	public void drawSelection(Graphics2D g2)
	{
		translate(1, 1);
		draw(g2);
		translate(1, 1);
		draw(g2);
		translate(-2, -2);
	}

	public void translate(int dx, int dy)
	{
		x += dx;
		y += dy;
	}
}
