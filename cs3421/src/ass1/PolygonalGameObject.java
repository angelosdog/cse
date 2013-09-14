package ass1;

import javax.media.opengl.GL2;

/**
 * A game object that has a polygonal shape.
 * 
 * This class extend GameObject to draw polygonal shapes.
 * 
 * @author malcolmr, vtra143
 */
public class PolygonalGameObject extends GameObject {

    private double[] myPoints;
    private double[] myFillColour;
    private double[] myLineColour;

    /**
     * Create a polygonal game object and add it to the scene tree
     * 
     * The polygon is specified as a list of doubles in the form:
     * 
     * [ x0, y0, x1, y1, x2, y2, ... ]
     * 
     * The line and fill colours can possibly be null, in which case that part of the object
     * should not be drawn.
     *
     * @param parent The parent in the scene tree
     * @param points A list of points defining the polygon
     * @param fillColour The fill colour in [r, g, b, a] form
     * @param lineColour The outline colour in [r, g, b, a] form
     */
    public PolygonalGameObject(GameObject parent, double points[],
            double[] fillColour, double[] lineColour) {
        super(parent);

        myPoints = points;
        myFillColour = fillColour;
        myLineColour = lineColour;
    }

    /**
     * Get the polygon
     * 
     * @return
     */
    public double[] getPoints() {        
        return myPoints;
    }

    /**
     * Set the polygon
     * 
     * @param points
     */
    public void setPoints(double[] points) {
        myPoints = points;
    }

    /**
     * Get the fill colour
     * 
     * @return
     */
    public double[] getFillColour() {
        return myFillColour;
    }

    /**
     * Set the fill colour.
     * 
     * Setting the colour to null means the object should not be filled.
     * 
     * @param fillColour The fill colour in [r, g, b, a] form 
     */
    public void setFillColour(double[] fillColour) {
        myFillColour = fillColour;
    }

    /**
     * Get the outline colour.
     * 
     * @return
     */
    public double[] getLineColour() {
        return myLineColour;
    }

    /**
     * Set the outline colour.
     * 
     * Setting the colour to null means the outline should not be drawn
     * 
     * @param lineColour
     */
    public void setLineColour(double[] lineColour) {
        myLineColour = lineColour;
    }

    @Override
    public void drawSelf(GL2 gl) {
        if (myLineColour != null) {
            gl.glColor4d(myLineColour[0], myLineColour[1], myLineColour[2], myLineColour[3]);
        } else {
            gl.glColor3d(0, 0, 0);
        }
        gl.glBegin(GL2.GL_LINE_LOOP);
        {
            for(int i = 0; i < myPoints.length; i+=2){
                gl.glVertex2d(myPoints[i], myPoints[i+1]);
            }
        }
        gl.glEnd();

        if (myFillColour != null) {
            gl.glColor4d(myFillColour[0], myFillColour[1], myFillColour[2], myFillColour[3]);
        } else {
            gl.glColor3d(1.0, 1.0, 1.0);
        }
        gl.glBegin(GL2.GL_POLYGON);
        {
            for(int i = 0; i < myPoints.length; i+=2){
                gl.glVertex2d(myPoints[i], myPoints[i+1]);
            }
        }
        gl.glEnd();
    }

    @Override
    public boolean containsPoint(double[] p) {
        // edge case if there is only one vertex (not really a polygon?)
        if(myPoints.length == 2) {
            // multiply the point by the model view transform to get the global co-ordinates
            double[] point = {myPoints[0],myPoints[1],1};
            point = MathUtil.multiply(this.getModelViewTransform(), point);
            return (p[0] == point[0] && p[1] == point[1]);
        }
        int intersections = 0; 
        
        for(int i = 0; i < myPoints.length; i+=2) {
            // multiply the points by the model view transform to get the global co-ordinates
            double[] p1 = {myPoints[i%myPoints.length], myPoints[(i+1)%myPoints.length], 1};
            p1 = MathUtil.multiply(this.getModelViewTransform(), p1);
            
            double[] p2 = {myPoints[(i+2)%myPoints.length], myPoints[(i+3)%myPoints.length], 1};
            p2 = MathUtil.multiply(this.getModelViewTransform(), p2);
            
            // edge case where p lies on the vertex of the polygon
            if((p[0] == p1[0] && p[1] == p1[1]) || (p[0] == p2[0] && p[1] == p2[1])) {
                return true;
            }
            
            //edge case where the line is vertical
            if(p1[0] == p2[0]) {
                // ensure that p1 is below p2
                if(p1[1] > p2[1]) {
                    double[] temp = p1;
                    p1 = p2;
                    p2 = temp;
                }
                // check if the x-value is the same and if it lies in the interval (p1[1],p2[1])
                if (p[0] != p1[0]) {
                    continue;
                }
                if (p[1] <= p2[1]) {
                    return true;
                }
            }
            // ensure that p1 is also "left" of p2
            if(p1[0] > p2[0]) {
                double[] temp = p1;
                p1 = p2;
                p2 = temp;
            }
            // calculate the point-gradient equation of p1,p2
            double m = (p2[1] - p1[1])/(p2[0] - p1[0]);
            double b = p1[1] - m*p1[0];
            
            // edge case where p is on the line p1,p2
            if (p[1] == m*p[0] + b) {
                // ensure that p is in the segment (p1,p2)
                
                if(m == 0 && p[0] > p1[0] && p[0] < p2[0]){
                    return true;
                } else if (m > 0 && p[0] > p1[0] && p[0] < p2[0] && p[1] > p1[1] && p[1] < p2[1]) {
                    return true;
                } else if (m < 0 && p[0] > p1[0] && p[0] < p2[0] && p[1] < p1[1] && p[1] > p2[1]) {
                    return true;
                }
            }
            
            // draw a ray from upwards from p to positive infinity and find the point of collision with the line
            // y is the point of collision with the line
            double y = m*p[0] + b;
            
            // if y < p[1], the line is opposite of the ray, in which case the ray can't intercept it
            if(y > p[1]) {
                // edge case where the ray intersects the vertex
                if(p[0] == p1[0] && y == p1[1]) {
                    // look ahead to the next line
                    double[] p3 = {myPoints[(i+4)%myPoints.length], myPoints[(i+5)%myPoints.length], 1};
                    
                    // if p2 and p3 lie on opposite sides of p1, then p is enclosed
                    if(p2[0] > p1[0] && p3[0] < p1[0]) {
                        return true;
                    } else if(p3[0] > p1[0] && p2[0] < p1[0]) {
                        return true;
                    }
                    intersections++;
                } else if (p[0] == p2[0] && y == p2[1]) {
                    double[] p3 = {myPoints[(i+4)%myPoints.length], myPoints[(i+5)%myPoints.length], 1};
                    
                    //if p1 and p3 lie on opposite sides of p2, then p is enclosed
                    if(p1[0] > p2[0] && p3[0] < p2[0]) {
                        return true;
                    } else if(p3[0] > p2[0] && p1[0] < p2[0]) {
                        return true;
                    }
                    intersections++;
                } else if (m == 0 && p[0] > p1[0] && p[0] < p2[0]) {
                    intersections++;
                } else if (m > 0 && p[0] > p1[0] && p[0] < p2[0] && y > p1[1] && y < p2[1]) {
                    intersections++;
                } else if (m < 0 && p[0] > p1[0] && y > p2[0] && p[1] < y && p[1] > p2[1]){
                    intersections++;
                }
            }
        }
        if (intersections % 2 == 0) {
            return false;
        }
        return true;
    }

}
