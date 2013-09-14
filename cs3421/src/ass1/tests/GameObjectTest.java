package ass1.tests;

import org.junit.Test;

import ass1.GameObject;

import junit.framework.TestCase;

/**
 * COMMENT: Comment GameObjectTest 
 *
 * @author malcolmr, vtra143
 */
public class GameObjectTest extends TestCase {

    // to account for rounding errors on doubles, we will
    // test to within epsilon of the correct answer:
 
    private static final double EPSILON = 0.001;
    
    @Test
    public void testGlobal0() {
        GameObject obj = new GameObject(GameObject.ROOT);
        
        double[] p = obj.getGlobalPosition();
        double r = obj.getGlobalRotation();
        double s = obj.getGlobalScale();
        
        assertEquals(0, p[0], EPSILON);
        assertEquals(0, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
    }

    @Test
    public void testGlobal1() {
        GameObject obj = new GameObject(GameObject.ROOT);
        
        obj.translate(-2, 3);
        obj.rotate(90);
        obj.scale(2);
        
        double[] p = obj.getGlobalPosition();
        double r = obj.getGlobalRotation();
        double s = obj.getGlobalScale();
        
        assertEquals(-2, p[0], EPSILON);
        assertEquals(3, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);
    }

    @Test
    public void testGlobal2() {
        GameObject parent = new GameObject(GameObject.ROOT);
        GameObject child = new GameObject(parent);
        
        parent.translate(-2, 3);
        parent.rotate(90);
        parent.scale(2);
        
        // the child is also moved:
        
        double[] p = child.getGlobalPosition();
        double r = child.getGlobalRotation();
        double s = child.getGlobalScale();
        
        assertEquals(-2, p[0], EPSILON);
        assertEquals(3, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);        
        
        // now move the child in its new coordinate frame
        child.translate(1, 0);
        child.rotate(-90);
        child.scale(0.5);

        p = child.getGlobalPosition();
        r = child.getGlobalRotation();
        s = child.getGlobalScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);

        // the parent is not affected
        
        p = parent.getGlobalPosition();
        r = parent.getGlobalRotation();
        s = parent.getGlobalScale();
        
        assertEquals(-2, p[0], EPSILON);
        assertEquals(3, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);

    }

    @Test
    public void testSetParent0() {
        GameObject obj1 = new GameObject(GameObject.ROOT);
        GameObject obj2 = new GameObject(GameObject.ROOT);
        
        assertSame(GameObject.ROOT, obj1.getParent());
        assertTrue(obj1.getChildren().isEmpty());
        
        assertSame(GameObject.ROOT, obj2.getParent());
        assertTrue(obj1.getChildren().isEmpty());
        
        obj1.translate(1, 1);
        obj1.rotate(90);
        obj1.scale(2);
        
        obj2.setParent(obj1);

        // obj2's global coordinate frame should not be changed
        
        double[] p = obj2.getGlobalPosition();
        double r = obj2.getGlobalRotation();
        double s = obj2.getGlobalScale();

        assertEquals(0, p[0], EPSILON);
        assertEquals(0, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);        

        // obj2's local coordinate frame is adjusted to suit
        
        p = obj2.getPosition();
        r = obj2.getRotation();
        s = obj2.getScale();
        
        assertEquals(-0.5, p[0], EPSILON);
        assertEquals(0.5, p[1], EPSILON);
        //assertEquals(-90, r, EPSILON);
        assertEquals(0.5, s, EPSILON);        

        // obj1's local coordinate frame is not affected
        
        p = obj1.getPosition();
        r = obj1.getRotation();
        s = obj1.getScale();
        
        assertEquals(1, p[0], EPSILON);
        assertEquals(1, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);        

    }
    
    @Test
    public void testSetParent1() {
        GameObject obj1 = new GameObject(GameObject.ROOT);
        GameObject obj2 = new GameObject(obj1);
        GameObject obj3 = new GameObject(GameObject.ROOT);
        
        assertSame(GameObject.ROOT, obj1.getParent());
        assertTrue(obj1.getChildren().size() == 1);
        
        assertSame(obj1, obj2.getParent());
        assertTrue(obj2.getChildren().isEmpty());
        
        assertSame(GameObject.ROOT, obj3.getParent());
        assertTrue(obj3.getChildren().isEmpty());
        
        obj1.translate(1, 1);
        obj1.rotate(0);
        obj1.scale(1);

        // obj2's global coordinate frame should not be changed
        
        double[] p = obj1.getGlobalPosition();
        double r = obj1.getGlobalRotation();
        double s = obj1.getGlobalScale();
        
        assertEquals(1, p[0], EPSILON);
        assertEquals(1, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);        

        p = obj2.getGlobalPosition();
        r = obj2.getGlobalRotation();
        s = obj2.getGlobalScale();
        
        assertEquals(1, p[0], EPSILON);
        assertEquals(1, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        obj2.translate(0, 1);
        obj2.rotate(0);
        obj2.scale(3);
        
        p = obj2.getGlobalPosition();
        r = obj2.getGlobalRotation();
        s = obj2.getGlobalScale();
        
        assertEquals(1, p[0], EPSILON);
        assertEquals(2, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(3, s, EPSILON);
        
        obj3.translate(-1, -1);
        obj3.rotate(90);
        obj3.scale(2);
        
        p = obj3.getGlobalPosition();
        r = obj3.getGlobalRotation();
        s = obj3.getGlobalScale();
        
        assertEquals(-1, p[0], EPSILON);
        assertEquals(-1, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);
        
        obj2.setParent(obj3);
        
        p = obj2.getGlobalPosition();
        r = obj2.getGlobalRotation();
        s = obj2.getGlobalScale();
        
        
        assertEquals(1, p[0], EPSILON);
        assertEquals(2, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(3, s, EPSILON);
        
        p = obj2.getPosition();
        r = obj2.getRotation();
        s = obj2.getScale();
        
        assertEquals(1.5, p[0], EPSILON);
        assertEquals(-1, p[1], EPSILON);
        assertEquals(-90, r, EPSILON);
        assertEquals(1.5, s, EPSILON);
    }
    
    @Test
    public void testSetParent2(){
        GameObject parent = new GameObject(GameObject.ROOT);
        GameObject child = new GameObject(parent);
        
        parent.translate(-2, 3);
        parent.rotate(90);
        parent.scale(2);
        
        // the child is also moved:
        
        double[] p = child.getGlobalPosition();
        double r = child.getGlobalRotation();
        double s = child.getGlobalScale();
        
        assertEquals(-2, p[0], EPSILON);
        assertEquals(3, p[1], EPSILON);
        assertEquals(90, r, EPSILON);
        assertEquals(2, s, EPSILON);        
        
        // now move the child in its new coordinate frame
        child.translate(1, 0);
        child.rotate(-90);
        child.scale(0.5);

        p = child.getGlobalPosition();
        r = child.getGlobalRotation();
        s = child.getGlobalScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        // set child's parent to root
        
        child.setParent(GameObject.ROOT);
        
        // global co-ordinates should not change
        
        p = child.getGlobalPosition();
        r = child.getGlobalRotation();
        s = child.getGlobalScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        // local co-ordinates should not change either
        
        p = child.getPosition();
        r = child.getRotation();
        s = child.getScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        // transform the old parent object
        
        parent.scale(0.5);
        parent.rotate(-90);
        parent.translate(2,-3);
        
        p = parent.getPosition();
        r = parent.getRotation();
        s = parent.getScale();

        assertEquals(0, p[0], EPSILON);
        assertEquals(0, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        // child is not affected the old parent's transformations
        
        p = child.getGlobalPosition();
        r = child.getGlobalRotation();
        s = child.getGlobalScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
        
        p = child.getPosition();
        r = child.getRotation();
        s = child.getScale();

        assertEquals(-2, p[0], EPSILON);
        assertEquals(5, p[1], EPSILON);
        assertEquals(0, r, EPSILON);
        assertEquals(1, s, EPSILON);
    }
}
