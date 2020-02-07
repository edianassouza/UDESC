package engineTester;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import models.RawModel;
import models.TexturedModel;

import org.lwjgl.opengl.Display;
import org.lwjgl.util.vector.Vector3f;

import renderEngine.DisplayManager;
import renderEngine.Loader;
import renderEngine.MasterRenderer;
import renderEngine.OBJLoader;
import terrains.Terrain;
import textures.ModelTexture;
import entities.Camera;
import entities.Entity;
import entities.Light;
import normalMappingObjConverter.NormalMappedObjLoader;
import objConverter.OBJFileLoader;
import org.lwjgl.util.vector.Vector4f;

public class MainGameLoop {

	public static void main(String[] args) {

		DisplayManager.createDisplay();
		Loader loader = new Loader();
		
		List<Entity> entities = new ArrayList<Entity>();
		List<Entity> normalMapEntities = new ArrayList<Entity>();
                //******************NORMAL MAP MODELS************************
         
                TexturedModel casaModel = new TexturedModel(NormalMappedObjLoader.loadOBJ("house", loader),
                new ModelTexture(loader.loadTexture("house")));
                casaModel.getTexture().setNormalMap(loader.loadTexture("houseNormal"));
                casaModel.getTexture().setShineDamper(10);
                casaModel.getTexture().setReflectivity(0.5f);
                
                
                
                TexturedModel boxModel = new TexturedModel(NormalMappedObjLoader.loadOBJ("box", loader),
                new ModelTexture(loader.loadTexture("box")));
                boxModel.getTexture().setNormalMap(loader.loadTexture("boxNormal"));
                boxModel.getTexture().setShineDamper(10);
                boxModel.getTexture().setReflectivity(0.5f);
                
                
                
                TexturedModel barrilModel = new TexturedModel(NormalMappedObjLoader.loadOBJ("barril", loader),
                new ModelTexture(loader.loadTexture("barril")));
                barrilModel.getTexture().setNormalMap(loader.loadTexture("barrilNormal"));
                barrilModel.getTexture().setShineDamper(10);
                barrilModel.getTexture().setReflectivity(0.5f);
          
                TexturedModel pedraModel = new TexturedModel(NormalMappedObjLoader.loadOBJ("boulder", loader),
                new ModelTexture(loader.loadTexture("boulder")));
                pedraModel.getTexture().setNormalMap(loader.loadTexture("boulderNormal"));
                pedraModel.getTexture().setShineDamper(10);
                pedraModel.getTexture().setReflectivity(0.5f);
                
                TexturedModel pedra= new TexturedModel(OBJFileLoader.loadOBJ("boulder", loader),
                new ModelTexture(loader.loadTexture("boulder")));
                pedra.getTexture().setHasTransparency(true);
                
                TexturedModel arvore= new TexturedModel(OBJFileLoader.loadOBJ("pine", loader),
                new ModelTexture(loader.loadTexture("pine")));
                arvore.getTexture().setHasTransparency(true);
                
                //************ENTITIES*******************

                Random random = new Random(23442);

                for(int i=0;i<17;i++){
                    normalMapEntities.add(new Entity(boxModel, new Vector3f(random.nextFloat()*800 - 400,5,random.nextFloat() * -600),0,0,0,0.05f));
                    normalMapEntities.add(new Entity(barrilModel, new Vector3f(random.nextFloat()*800 - 400,5,random.nextFloat() * -600),0,0,0,1));
                }
		
                for(int i=0; i<6; i++){
                    entities.add(new Entity(arvore, 1, new Vector3f(random.nextFloat()*800 - 400,0,random.nextFloat() * -600),0,0,0,5));
                }
                
                for(int i=0;i<3;i++){
                    normalMapEntities.add(new Entity(casaModel, new Vector3f(random.nextFloat()*800 - 400,0,random.nextFloat() * -600),0,0,0,1));
                    normalMapEntities.add(new Entity(pedraModel, new Vector3f(random.nextFloat()*800 - 400,12,random.nextFloat() * -600),0,0,0,3));
                    entities.add(new Entity(pedraModel, new Vector3f(random.nextFloat()*800 - 400,12,random.nextFloat() * -600),0,0,0,3));
                }
                
		Light light = new Light(new Vector3f(20000,20000,2000),new Vector3f(1,1,1));
		
		Terrain terrain = new Terrain(0,-1,loader,new ModelTexture(loader.loadTexture("grassy3")));
		Terrain terrain2 = new Terrain(-1,-1,loader,new ModelTexture(loader.loadTexture("grassy3")));
		
		Camera camera = new Camera();	
		List<Light> lights = new ArrayList<Light>();
                Light sun = new Light(new Vector3f(10000, 10000, -10000), new Vector3f(1.3f, 1.3f, 1.3f));
                lights.add(sun);
 
                MasterRenderer renderer = new MasterRenderer(loader);
		
		while(!Display.isCloseRequested()){
			camera.move();
			
			renderer.processTerrain(terrain);
			renderer.processTerrain(terrain2);
			renderer.renderScene(entities, normalMapEntities, lights, camera, new Vector4f(0, 1, 0, 1));
			//renderer.render(light, camera, new Vector4f(0, -1, 0, 100000));
			DisplayManager.updateDisplay();
		}

		renderer.cleanUp();
		loader.cleanUp();
		DisplayManager.closeDisplay();

	}

}
