package ass1.tests;
/**
 * @author Luke Esther
 */
import static org.junit.Assert.*;

import org.junit.Test;

import ass1.GameObject;
import ass1.PolygonalGameObject;

public class CollisionTest {

    @Test
    public void squareTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,0, 0,10, 10,10, 10,0}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        assertTrue(p1.containsPoint(new double[]{5,5}));
        assertTrue(p1.containsPoint(new double[]{9,1}));
        assertTrue(!p1.containsPoint(new double[]{5,15}));
        assertTrue(!p1.containsPoint(new double[]{-5,-2}));
    }
    
    @Test
    public void triangleTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,0}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});

        assertTrue(p1.containsPoint(new double[]{0.5,2}));
        assertTrue(!p1.containsPoint(new double[]{0.1,0.1}));
        assertTrue(!p1.containsPoint(new double[]{0,0}));
    }
    
    @Test
    public void polygonTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,1, 2,2, 2,0, 1,0,}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        assertTrue(p1.containsPoint(new double[]{1,0.5}));
        assertTrue(p1.containsPoint(new double[]{0.8,1}));
        assertTrue(!p1.containsPoint(new double[]{0.1,0.1}));
        assertTrue(!p1.containsPoint(new double[]{0,3}));
        assertTrue(!p1.containsPoint(new double[]{0,0}));
        assertTrue(!p1.containsPoint(new double[]{-5,-2}));
    }
    
    @Test
    public void polygonVertexTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,1, 2,2, 2,0, 1,0,}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        assertTrue(p1.containsPoint(new double[]{0,2}));
        assertTrue(p1.containsPoint(new double[]{2,2}));
        assertTrue(p1.containsPoint(new double[]{2,0}));
        assertTrue(p1.containsPoint(new double[]{1,3}));
        assertTrue(p1.containsPoint(new double[]{1,0}));
    }
    
    @Test
    public void polyEdgeTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,1, 2,2, 2,0, 1,0,}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        assertTrue(p1.containsPoint(new double[]{1,2}));
        assertTrue(p1.containsPoint(new double[]{2,1}));
        assertTrue(p1.containsPoint(new double[]{1.5,0}));
        assertTrue(p1.containsPoint(new double[]{0.5,2.5}));
        assertTrue(p1.containsPoint(new double[]{1.5,1.5}));
    }
    
    
    @Test
    public void singleVertexTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{1,1}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        assertTrue(p1.containsPoint(new double[]{1,1}));
        assertTrue(!p1.containsPoint(new double[]{1,0}));
        assertTrue(!p1.containsPoint(new double[]{0,1}));
        assertTrue(!p1.containsPoint(new double[]{0,0}));
    }
    
    @Test
    public void singleLineTest() {
        PolygonalGameObject p1 = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{1,1, 2,1}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        //assertTrue(p1.containsPoint(new double[]{1,1}));
        //assertTrue(p1.containsPoint(new double[]{1.5,1}));
        assertTrue(!p1.containsPoint(new double[]{1,0}));
        assertTrue(!p1.containsPoint(new double[]{0,1}));
        assertTrue(!p1.containsPoint(new double[]{3,1}));
    }

}