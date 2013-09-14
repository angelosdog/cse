package ass1.tests;

import static org.junit.Assert.*;

import org.junit.Test;

import ass1.Camera;
import ass1.GameEngine;
import ass1.GameObject;
import ass1.PolygonalGameObject;

public class ConvexCollisionTest {
    @Test
    public void test0() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 1, 1, 2, 4, 3, 9, 4, 16, 5, 25, 6, 36, 7, 49};
        double[] pt = new double[]{0, 100};
        PolygonalGameObject boat = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(0, engine.collision(pt).size());
        boat.destroy();
    }

    @Test
    public void test1() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 1, 1, 2, 4, 3, 9, 4, 16, 5, 25, 6, 36, 7, 49};
        double[] pt = new double[]{5, 35};
        PolygonalGameObject boat = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(boat));
        boat.destroy();
    }

    @Test
    public void test2() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 1, 1, 2, 4, 3, 9, 4, 16, 5, 25, 6, 36, 7, 49};
        double[] pt = new double[]{3, 18};
        PolygonalGameObject boat = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(boat));
        boat.destroy();
    }

    @Test
    public void test3() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 20, 10, 30, 10, 40, 10, 50, 10, 60, 10, 70, 10, 80, 10, 90, 10, 100, 10, 110, 10, 120, 10, 130, 10, 140, 10, 150, 10, 160, 10, 170, 10, 180, 10, 190, 10, 200, 10, 210, 10, 220, 10, 230, 10, 240, 10, 250, 10, 260, 10, 270, 10, 280, 10, 290, 10, 300, 10, 310, 10, 320, 10, 330, 10, 340, 10, 350, 10, 360, 10, 370, 10, 380, 10, 390, 10, 400, 10, 410, 10, 420, 10, 430, 10, 440, 10, 450, 10, 460, 10, 470, 10, 480, 10, 490, 10, 490, 0, 480, 0, 470, 0, 460, 0, 450, 0, 440, 0, 430, 0, 420, 0, 410, 0, 400, 0, 390, 0, 380, 0, 370, 0, 360, 0, 350, 0, 340, 0, 330, 0, 320, 0, 310, 0, 300, 0, 290, 0, 280, 0, 270, 0, 260, 0, 250, 0, 240, 0, 230, 0, 220, 0, 210, 0, 200, 0, 190, 0, 180, 0, 170, 0, 160, 0, 150, 0, 140, 0, 130, 0, 120, 0, 110, 0, 100, 0, 90, 0, 80, 0, 70, 0, 60, 0, 50, 0, 40, 0, 30, 0, 20, 0, 10, 0};
        double[] pt = new double[]{255, 0};
        PolygonalGameObject max_collinear = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(max_collinear));
        max_collinear.destroy();
    }

    @Test
    public void test4() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 20, 10, 30, 10, 40, 10, 50, 10, 60, 10, 70, 10, 80, 10, 90, 10, 100, 10, 110, 10, 120, 10, 130, 10, 140, 10, 150, 10, 160, 10, 170, 10, 180, 10, 190, 10, 200, 10, 210, 10, 220, 10, 230, 10, 240, 10, 250, 10, 260, 10, 270, 10, 280, 10, 290, 10, 300, 10, 310, 10, 320, 10, 330, 10, 340, 10, 350, 10, 360, 10, 370, 10, 380, 10, 390, 10, 400, 10, 410, 10, 420, 10, 430, 10, 440, 10, 450, 10, 460, 10, 470, 10, 480, 10, 490, 10, 490, 0, 480, 0, 470, 0, 460, 0, 450, 0, 440, 0, 430, 0, 420, 0, 410, 0, 400, 0, 390, 0, 380, 0, 370, 0, 360, 0, 350, 0, 340, 0, 330, 0, 320, 0, 310, 0, 300, 0, 290, 0, 280, 0, 270, 0, 260, 0, 250, 0, 240, 0, 230, 0, 220, 0, 210, 0, 200, 0, 190, 0, 180, 0, 170, 0, 160, 0, 150, 0, 140, 0, 130, 0, 120, 0, 110, 0, 100, 0, 90, 0, 80, 0, 70, 0, 60, 0, 50, 0, 40, 0, 30, 0, 20, 0, 10, 0};
        double[] pt = new double[]{250, 0};
        PolygonalGameObject max_collinear = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(max_collinear));
        max_collinear.destroy();
    }

    @Test
    public void test5() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 20, 10, 30, 10, 40, 10, 50, 10, 60, 10, 70, 10, 80, 10, 90, 10, 100, 10, 110, 10, 120, 10, 130, 10, 140, 10, 150, 10, 160, 10, 170, 10, 180, 10, 190, 10, 200, 10, 210, 10, 220, 10, 230, 10, 240, 10, 250, 10, 260, 10, 270, 10, 280, 10, 290, 10, 300, 10, 310, 10, 320, 10, 330, 10, 340, 10, 350, 10, 360, 10, 370, 10, 380, 10, 390, 10, 400, 10, 410, 10, 420, 10, 430, 10, 440, 10, 450, 10, 460, 10, 470, 10, 480, 10, 490, 10, 490, 0, 480, 0, 470, 0, 460, 0, 450, 0, 440, 0, 430, 0, 420, 0, 410, 0, 400, 0, 390, 0, 380, 0, 370, 0, 360, 0, 350, 0, 340, 0, 330, 0, 320, 0, 310, 0, 300, 0, 290, 0, 280, 0, 270, 0, 260, 0, 250, 0, 240, 0, 230, 0, 220, 0, 210, 0, 200, 0, 190, 0, 180, 0, 170, 0, 160, 0, 150, 0, 140, 0, 130, 0, 120, 0, 110, 0, 100, 0, 90, 0, 80, 0, 70, 0, 60, 0, 50, 0, 40, 0, 30, 0, 20, 0, 10, 0};
        double[] pt = new double[]{103, 7};
        PolygonalGameObject max_collinear = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(max_collinear));
        max_collinear.destroy();
    }

    @Test
    public void test6() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 20, 10, 30, 10, 40, 10, 50, 10, 60, 10, 70, 10, 80, 10, 90, 10, 100, 10, 110, 10, 120, 10, 130, 10, 140, 10, 150, 10, 160, 10, 170, 10, 180, 10, 190, 10, 200, 10, 210, 10, 220, 10, 230, 10, 240, 10, 250, 10, 260, 10, 270, 10, 280, 10, 290, 10, 300, 10, 310, 10, 320, 10, 330, 10, 340, 10, 350, 10, 360, 10, 370, 10, 380, 10, 390, 10, 400, 10, 410, 10, 420, 10, 430, 10, 440, 10, 450, 10, 460, 10, 470, 10, 480, 10, 490, 10, 490, 0, 480, 0, 470, 0, 460, 0, 450, 0, 440, 0, 430, 0, 420, 0, 410, 0, 400, 0, 390, 0, 380, 0, 370, 0, 360, 0, 350, 0, 340, 0, 330, 0, 320, 0, 310, 0, 300, 0, 290, 0, 280, 0, 270, 0, 260, 0, 250, 0, 240, 0, 230, 0, 220, 0, 210, 0, 200, 0, 190, 0, 180, 0, 170, 0, 160, 0, 150, 0, 140, 0, 130, 0, 120, 0, 110, 0, 100, 0, 90, 0, 80, 0, 70, 0, 60, 0, 50, 0, 40, 0, 30, 0, 20, 0, 10, 0};
        double[] pt = new double[]{6, -3};
        PolygonalGameObject max_collinear = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(0, engine.collision(pt).size());
        max_collinear.destroy();
    }

    @Test
    public void test7() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 10, 0};
        double[] pt = new double[]{10, 5};
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(square));
        square.destroy();
    }

    @Test
    public void test8() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 10, 0};
        double[] pt = new double[]{6, 3};
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(square));
        square.destroy();
    }

    @Test
    public void test9() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 10, 0};
        double[] pt = new double[]{20, 20};
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(0, engine.collision(pt).size());
        square.destroy();
    }

    @Test
    public void test10() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 10, 0};
        double[] pt = new double[]{-4, 5};
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(0, engine.collision(pt).size());
        square.destroy();
    }

    @Test
    public void test11() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{0, 0, 0, 10, 10, 10, 10, 0};
        double[] pt = new double[]{0, 10};
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(square));
        square.destroy();
    }

    @Test
    public void test12() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{10, 10, 30, 20, 20, 40};
        double[] pt = new double[]{25, 30};
        PolygonalGameObject tri = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(tri));
        tri.destroy();
    }

    @Test
    public void test13() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{1, 1, 3, 2, 2, 4};
        double[] pt = new double[]{2, 3};
        PolygonalGameObject tri = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(tri));
        tri.destroy();
    }

    @Test
    public void test14() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{1, 1, 3, 2, 2, 4};
        double[] pt = new double[]{4, 3};
        PolygonalGameObject tri = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(0, engine.collision(pt).size());
        tri.destroy();
    }

    @Test
    public void test15() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);

        double[] shape = new double[]{1, 1, 3, 2, 2, 4};
        double[] pt = new double[]{2, 4};
        PolygonalGameObject tri = new PolygonalGameObject(GameObject.ROOT, shape, null, null);
        assertEquals(1, engine.collision(pt).size());
        assertTrue(engine.collision(pt).contains(tri));
        tri.destroy();
    }


}