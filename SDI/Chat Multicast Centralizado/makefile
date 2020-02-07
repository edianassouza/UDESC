JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
								Client.java \
								MulticastReceiver.java \
								Server.java \
								clientThread.java 

default: classes

classes: $(CLASSES:.java=.class)

clean:
								$(RM) *.class *.chat *.serv *.client*