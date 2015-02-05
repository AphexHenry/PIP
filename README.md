# PIP
Art installation projecting people shape into a 3D video. Based on [Cinder 0.8.6.](http://libcinder.org/download/).

The applicaton has the following features:
- Read stereoscopic images and videos.
- multi-threaded video loading.
- allow to chose between 4 stereoscopy rendering (anaglyph, side-by-side keep ratio, side-by-side ratio widh/2, and Alioscopy)
- display particules in the space, interaction done with available device (Kinect, camera, mouse).
- xml based scene editor.

Exemple of xml file content:
```
<Ambient>
    <audio file="Sounds/H2H REFLECTIONS MUSIC 2.wav" volume="0"></audio>
    <transition type="FadeToBlack" duration="0.6"></transition>
</Ambient>

<Composition>
    <!-- First scene-->
    <scene>
        <particles count="300" type="circle" size="1.6" x="0." y="0.f" z="0.5" scale="0.05" reflection="0" lifeTime="0.19" brightnessMin="0.1" brightnessMax="1.1" useKinectColor="0" colorMin="0.86,0.61,0.7" colorMax="0.4,0.6,0.4" reflection="0" opacity="0.2"></particles>
        <media backMedia="SceneMarten/h2h_reflections_1.mov" audioMedia="Sounds/pip_boreal2.wav" volume="0"></media>
    </scene>
    <!-- Second scene-->
    <scene>
        <particles count="400" type="tree" size="1.6" x="0." y="0.f" z="0.5" scale="0.05" reflection="0" lifeTime="0.19" brightnessMin="0.1" brightnessMax="1.1" useKinectColor="0" colorMin="0.86,0.61,0.7" colorMax="0.4,0.6,0.4" reflection="0" opacity="0.2"></particles>
        <media backMedia="ScenePlaineMouches2/ScenePlaineMouche.mov" frontImage="ScenePlaineMouches2/ScenePlaineMoucheFront.png" audioMedia="Sounds/pip_boreal2.wav" volume="0"></media>
    </scene>
  </Compositon>```
