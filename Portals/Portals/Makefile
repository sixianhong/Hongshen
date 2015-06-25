CC = g++ -ansi -w -DLINUX -DNO_TEX3D -DNO_TGA -DNO_PCX -DNO_JPEG -DNO_BMP
RELEASE = -Os -ffast-math
DEBUG = -g

FW_PATH  = ../Framework2
APP_NAME = Portals
APP_PATH = .

FW_BASE_MATH = $(FW_PATH)/Math/Vector.cpp
FW_BASE_IMAGING = $(FW_PATH)/Imaging/Image.cpp $(FW_PATH)/Imaging/Image3D.cpp
FW_BASE_RENDERER = $(FW_PATH)/OpenGL/glExtensions.cpp $(FW_PATH)/Renderer.cpp $(FW_PATH)/OpenGL/OpenGLRenderer.cpp
FW_BASE_APP = $(FW_PATH)/App.cpp $(FW_PATH)/OpenGL/OpenGLApp.cpp $(FW_PATH)/Linux/LinuxBase.cpp $(FW_PATH)/Globals.cpp $(FW_PATH)/CPU.cpp $(FW_PATH)/DisplayMode.cpp $(FW_PATH)/Menu.cpp
FW_BASE_UTIL = $(FW_PATH)/Util/String.cpp $(FW_PATH)/Util/Font.cpp $(FW_PATH)/Util/Config.cpp $(FW_PATH)/Util/Tokenizer.cpp

FW = $(FW_BASE_MATH) $(FW_BASE_APP) $(FW_BASE_RENDERER) $(FW_BASE_IMAGING) $(FW_BASE_UTIL)
FW_EXTRA = $(FW_PATH)/Util/Model.cpp $(FW_PATH)/Util/OpenGLModel.cpp $(FW_PATH)/Util/ParticleSystem.cpp
APP = $(APP_PATH)/Main.cpp $(APP_PATH)/Portals.cpp

rel: $(APP) $(FW) $(FW_EXTRA)
	$(CC) $(RELEASE) $(APP) $(FW) $(FW_EXTRA) -o $(APP_NAME) -L/usr/X11R6/lib -lGL -lXxf86vm -L/usr/lib -lpng -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL 
dbg: $(APP) $(FW) $(FW_EXTRA)
	$(CC) $(DEBUG) $(APP) $(FW) $(FW_EXTRA) -o $(APP_NAME) -L/usr/X11R6/lib -lGL -lXxf86vm -L/usr/lib -lpng -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL 

clean:
	@rm $(APP_NAME)
