package sailing;

import java.awt.BorderLayout;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLJPanel;
import javax.swing.JFrame;

import sailing.objects.CameraHarness;
import sailing.objects.Pirate;

import ass1.Camera;
import ass1.GameEngine;
import ass1.Mouse;

import com.jogamp.opengl.util.FPSAnimator;

/**
 * COMMENT: Comment Triangle 
 *
 * @author malcolmr
 */
public class SailingGame extends JFrame {

    private static final long serialVersionUID = 1L;
    private static final float[] WATER_COLOR = { 0.0f, 0.0f, 1.0f, 0.0f };
    private static final String MAP_FILE = "C:\\Users\\vincent\\Desktop\\ass1-sailing\\map.json";

    private Map myMap;
    private GLJPanel myPanel;

    public static final SailingGame theGame = new SailingGame();
    
    private SailingGame() {
        super("Sailing Game");
    }
    
    private void init()  throws IOException {
        GLProfile glprofile = GLProfile.getDefault();
        GLCapabilities glcapabilities = new GLCapabilities(glprofile);
        myPanel = new GLJPanel(glcapabilities);

        readMap(new File(MAP_FILE));

        // attach the camera to the player
        Pirate player = myMap.player();
        Camera camera = new Camera(new CameraHarness(player));
        camera.scale(20);
        camera.setBackground(WATER_COLOR);

        GameEngine engine = new GameEngine(camera);
        myPanel.addGLEventListener(engine);

        // Add an animator to call 'display' at 60fps        
        FPSAnimator animator = new FPSAnimator(60);
        animator.add(myPanel);
        animator.start();

        // Add a mouse event handler        
        myPanel.addMouseListener(Mouse.theMouse);
        myPanel.addMouseMotionListener(Mouse.theMouse);

        getContentPane().add(myPanel, BorderLayout.CENTER);
        setSize(1024, 768);
        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    public void readMap(File mapFile) throws IOException {
        InputStream in = new FileInputStream(mapFile);
        myMap = Map.read(in);
        in.close();
    }

    public static void main(String[] args) throws IOException {
        theGame.init();
    }

}
