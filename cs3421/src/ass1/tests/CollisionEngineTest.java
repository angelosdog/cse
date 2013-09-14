package ass1.tests;
import static org.junit.Assert.*;

import org.junit.Test;

import ass1.Camera;
import ass1.GameEngine;
import ass1.GameObject;
import ass1.PolygonalGameObject;

public class CollisionEngineTest {

    @Test
    public void test() {
        Camera camera = new Camera();
        GameEngine engine = new GameEngine(camera);
        
        PolygonalGameObject square = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,0, 0,10, 10,10, 10,0}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});

        PolygonalGameObject triangle = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,0}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});

        PolygonalGameObject porygon = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{0,2, 1,3, 1,1, 2,2, 2,0, 1,0,}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        
        PolygonalGameObject point = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{1,1}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        
        PolygonalGameObject line = new PolygonalGameObject(GameObject.ROOT, 
                new double[]{1,1, 2,1}, 
                new double[]{0,0,0,0}, new double[]{0,0,0,0});
        
        assertTrue(engine.collision(new double[]{5,5}).size() == 1); //square
        assertTrue(engine.collision(new double[]{5,5}).contains(square));
        
        assertTrue(engine.collision(new double[]{9,1}).size() == 1); //square
        assertTrue(engine.collision(new double[]{9,1}).contains(square));
        
        assertTrue(engine.collision(new double[]{5,15}).size() == 0);
        assertTrue(engine.collision(new double[]{-5,-2}).size() == 0);
        
        assertTrue(engine.collision(new double[]{0.5,2}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{0.5,2}).contains(square));
        assertTrue(engine.collision(new double[]{0.5,2}).contains(triangle));
        assertTrue(engine.collision(new double[]{0.5,2}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{0.1,0.1}).size() == 1); //square
        assertTrue(engine.collision(new double[]{0.1,0.1}).contains(square));
        
        assertTrue(engine.collision(new double[]{0,0}).size() == 1); //square
        assertTrue(engine.collision(new double[]{0,0}).contains(square)); //square
        
        assertTrue(engine.collision(new double[]{1,0.5}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{1,0.5}).contains(square));
        assertTrue(engine.collision(new double[]{1,0.5}).contains(triangle));
        assertTrue(engine.collision(new double[]{1,0.5}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{0.8,1}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{0.8,1}).contains(square));
        assertTrue(engine.collision(new double[]{0.8,1}).contains(triangle));
        assertTrue(engine.collision(new double[]{0.8,1}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{0,3}).size() == 1); //square
        assertTrue(engine.collision(new double[]{0,3}).contains(square));
        
        assertTrue(engine.collision(new double[]{0,2}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{0,2}).contains(square));
        assertTrue(engine.collision(new double[]{0,2}).contains(triangle));
        assertTrue(engine.collision(new double[]{0,2}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{2,2}).size() == 2); //square, polygon
        assertTrue(engine.collision(new double[]{2,2}).contains(square));
        assertTrue(engine.collision(new double[]{2,2}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{2,0}).size() == 2); //square, polygon
        assertTrue(engine.collision(new double[]{2,0}).contains(square));
        assertTrue(engine.collision(new double[]{2,0}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{1,3}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{1,3}).contains(square));
        assertTrue(engine.collision(new double[]{1,3}).contains(triangle));
        assertTrue(engine.collision(new double[]{1,3}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{1,0}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{1,0}).contains(square));
        assertTrue(engine.collision(new double[]{1,0}).contains(triangle));
        assertTrue(engine.collision(new double[]{1,0}).contains(porygon));

        assertTrue(engine.collision(new double[]{1,2}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{1,2}).contains(square));
        assertTrue(engine.collision(new double[]{1,2}).contains(triangle));
        assertTrue(engine.collision(new double[]{1,2}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{2,1}).size() == 3); //square, polygon, line
        assertTrue(engine.collision(new double[]{2,1}).contains(square));
        assertTrue(engine.collision(new double[]{2,1}).contains(porygon));
        assertTrue(engine.collision(new double[]{2,1}).contains(line));
        
        assertTrue(engine.collision(new double[]{1.5, 0}).size() == 2); //square, polygon
        assertTrue(engine.collision(new double[]{1.5, 0}).contains(square));
        assertTrue(engine.collision(new double[]{1.5, 0}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{0.5, 2.5}).size() == 3); //square, triangle, polygon
        assertTrue(engine.collision(new double[]{0.5, 2.5}).contains(square));
        assertTrue(engine.collision(new double[]{0.5, 2.5}).contains(triangle));
        assertTrue(engine.collision(new double[]{0.5, 2.5}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{1.5,1.5}).size() == 2); //square, polygon
        assertTrue(engine.collision(new double[]{1.5,1.5}).contains(square));
        assertTrue(engine.collision(new double[]{1.5,1.5}).contains(porygon));
        
        assertTrue(engine.collision(new double[]{1,1}).size() == 5); //square, triangle, polygon, line, point
        assertTrue(engine.collision(new double[]{1,1}).contains(square));
        assertTrue(engine.collision(new double[]{1,1}).contains(triangle));
        assertTrue(engine.collision(new double[]{1,1}).contains(porygon));
        assertTrue(engine.collision(new double[]{1,1}).contains(line));
        assertTrue(engine.collision(new double[]{1,1}).contains(point));
        
        
        assertTrue(engine.collision(new double[]{0,1}).size() == 1); //square
        assertTrue(engine.collision(new double[]{0,1}).contains(square));
        
        assertTrue(engine.collision(new double[]{1.5,1}).size() == 3); //square, polygon, line
        assertTrue(engine.collision(new double[]{1.5,1}).contains(square));
        assertTrue(engine.collision(new double[]{1.5,1}).contains(porygon));
        assertTrue(engine.collision(new double[]{1.5,1}).contains(line));
        
        assertTrue(engine.collision(new double[]{3,1}).size() == 1); //square
        assertTrue(engine.collision(new double[]{3,1}).contains(square));
    }
}