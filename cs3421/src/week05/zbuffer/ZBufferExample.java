package week05.zbuffer;

import java.awt.Color;
import java.awt.GridLayout;
import java.util.ArrayList;
import java.util.List;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLJPanel;
import javax.swing.JFrame;

import com.jogamp.opengl.util.FPSAnimator;

import week05.Point;
import week05.Polygon;


/**
 * COMMENT: Comment ZBuffer 
 *
 * @author malcolmr
 */
public class ZBufferExample extends JFrame {

    private List<Polygon> myPolygons;   
    private GLJPanel[] myPanels;

    private Point[] myRedPoints;
    private Point[] myBluePoints;
    private Point[] myGreenPoints;

    public ZBufferExample() {
        super("ZBuffer example");
        init();
    }

    public List<Polygon> getPolygons() {
        return myPolygons;
    }
    
    public void init() {
        // initialisation
        GLProfile glp = GLProfile.getDefault();
        GLCapabilities caps = new GLCapabilities(glp);

        initPolygons();
        setSize(1200, 400);
        
        initPanels(caps);

        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    /**
     * Create three quads
     */
    private void initPolygons() {
        myPolygons = new ArrayList<Polygon>();
        
        myRedPoints = new Point[4];
        myRedPoints[0] = new Point(1,1,-5);
        myRedPoints[1] = new Point(-1,1,-5);
        myRedPoints[2] = new Point(-1,-1,-5);
        myRedPoints[3] = new Point(1,-1,-5);
        
        Polygon p1 = new Polygon();
        p1.setColor(Color.RED);
        for (int i = 0; i < myRedPoints.length; i++) {
            p1.addPoint(myRedPoints[i]);            
        }
        myPolygons.add(p1);

        myBluePoints = new Point[4];
        myBluePoints[0] = new Point(0,1,-5);
        myBluePoints[1] = new Point(-2,1,-5);
        myBluePoints[2] = new Point(-2,-1,-5);
        myBluePoints[3] = new Point(0,-1,-5);
        
        Polygon p2 = new Polygon();
        p2.setColor(Color.BLUE);
        for (int i = 0; i < myBluePoints.length; i++) {
            p2.addPoint(myBluePoints[i]);            
        }
        myPolygons.add(p2);
        
        myGreenPoints = new Point[4];
        myGreenPoints[0] = new Point(2,1,-5);
        myGreenPoints[1] = new Point(0,1,-5);
        myGreenPoints[2] = new Point(0,-1,-5);
        myGreenPoints[3] = new Point(2,-1,-5);

        Polygon p3 = new Polygon();
        p3.setColor(Color.GREEN);
        for (int i = 0; i < myGreenPoints.length; i++) {
            p3.addPoint(myGreenPoints[i]);            
        }
        myPolygons.add(p3);
        

    }
    
    private void initPanels(GLCapabilities caps) {
        // create a panel to draw on
        myPanels = new GLJPanel[2];
        myPanels[0] = new GLJPanel(caps);
        myPanels[1] = new GLJPanel(caps);

        setLayout(new GridLayout(1, 2));
        add(myPanels[0]);
        add(myPanels[1]);

        // the top-down view
        TopView view0 = new TopView(this);
        myPanels[0].addGLEventListener(view0);
//        myPanels[0].addMouseMotionListener(view0);

        // the front view
        MainView view1 = new MainView(this);
        myPanels[1].addGLEventListener(view1);
        myPanels[1].addMouseMotionListener(view1);
        myPanels[1].addKeyListener(view1);
        myPanels[1].setFocusable(true);
        
        FPSAnimator animator = new FPSAnimator(60);
        animator.add(myPanels[0]);
        animator.add(myPanels[1]);
        animator.start();
    }

    
    
    public static void main(String[] args) {
        ZBufferExample zBufferExample = new ZBufferExample();
    }
}
