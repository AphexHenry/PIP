# Projection Interieure Projection

Art installation created by Marten Berkan and Baptiste Bohelay. The system project people shape into 3D videos. Based on [Cinder 0.8.6.](http://libcinder.org/download/).

Presented to :
- [Whitehorse Art Underground Gallery](http://lebaptiste.com/#projects+html&other&pip.html).
- [Interactive Future '14] (http://if2014.ecuad.ca/exhibition/marten-berkman-baptiste-bohelay/).

The applicaton has the following features:
- Read stereoscopic images and videos.
- multi-threaded video loading.
- allow to chose between 4 stereoscopy rendering (anaglyph, side-by-side keep ratio, side-by-side ratio widh/2, and Alioscopy).
- display particules in stereoscopy on a layer. 
- interaction can be done with different devices (Kinect, camera, mouse). The application will pick the available one. Kinect first, then camera,  then mouse.
- xml based scene editor.

Exemple of xml file content:
```xml
<Ambient> <!-- Define global parameters-->
    <audio file="Sounds/H2H REFLECTIONS MUSIC 2.wav" volume="0"></audio> <!-- Background music-->
    <transition type="FadeToBlack" duration="0.6"></transition> <!-- Fade In/Out between scenes-->
</Ambient>

<Composition> <!-- Define the scenes, in playing order.-->
    <scene> <!-- First scene-->
        <particles count="300" type="circle" size="1.6" x="0." y="0.f" z="0.5" scale="0.05" reflection="0" lifeTime="0.19" brightnessMin="0.1" brightnessMax="1.1" colorMin="0.86,0.61,0.7" colorMax="0.4,0.6,0.4" reflection="0" opacity="0.2"></particles> <!-- add 300 "circle" particles with opacity 0.2, etc.-->
        <media backMedia="SceneMarten/h2h_reflections_1.mov" volume="0"></media> <!-- just play a video behind, sound muted.-->
    </scene>
    <scene>     <!-- Second scene-->
        <particles count="400" type="tree" size="1.6" x="0." y="0.f" z="0.5" scale="0.05" reflection="0" lifeTime="0.49" brightnessMin="0.1" brightnessMax="1.1" useKinectColor="0" colorMin="0.86,0.61,0.7" colorMax="0.4,0.6,0.4" reflection="0" opacity="1."></particles>
        <media backMedia="ScenePlaineMouches2/ScenePlaineMouche.png" frontImage="ScenePlaineMouches2/ScenePlaineMoucheFront.png" audioMedia="Sounds/pip_boreal2.wav" volume="0.4"></media> <!-- put an image behind particles and a mask in front for better integration, audio file will be played during scene.-->
    </scene>
  </Compositon>
  ```
  
  Installation:
  - Put project into CINDER_PATH/samples.
  - Open with Xcode and compile it.
  - In the same directory of your application create a directory "/Assets"
  - Put your resources : videos, images and sounds.
  - open your_app_content/Resources/setup.xml
  - Build the scenes like the xml example. Path are relative to /Assets.
  - Use F2, F3, F4, etc. to switch 3D rendering.
