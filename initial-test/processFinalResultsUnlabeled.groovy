Thread.start{
  println " PRUEBA - STARTTTTTTTT"
def indice = 0
def vector=[]

new File('final.txt').eachLine { line ->
  indice++
  if(!(indice%1000)){
  	println "PRUEBA - voy por $indice"
  }
  
  
   def linea = line.split("-")

def id= linea[0]
id=id.replace("REVIEW: ","")
id=id.replace(" ","")
  
def segundoStatus= linea[2].replace("DOS:","")
segundoStatus=segundoStatus.replace(" ","")

def terceroStatus= linea[3].replace("TRES:","")
terceroStatus=terceroStatus.replace(" ","")

def numero=4

if(segundoStatus=="'\"negative\"'" && terceroStatus=="'\"neg\"'"){
numero=0
}
if(segundoStatus=="'\"negative\"'" && terceroStatus=="''"){
numero=0
}

if(segundoStatus=="''" && terceroStatus=="'\"neg\"'"){
numero=0
}
if(segundoStatus=="'\"positive\"'" && terceroStatus=="'\"pos\"'"){
numero=1
}
if(segundoStatus=="'\"positive\"'" && terceroStatus=="''"){
numero=1
}
if(segundoStatus=="''" && terceroStatus=="'\"pos\"'"){
numero=1
}
if(segundoStatus=="'\"neutral\"'" && terceroStatus=="'\"pos\"'"){
numero=1
}

if(segundoStatus=="'\"positive\"'" && terceroStatus=="'\"neutral\"'"){
numero=1
}

if(segundoStatus=="'\"neutral\"'" && terceroStatus=="'\"neg\"'"){
numero=0
}
if(segundoStatus=="'\"negative\"'" && terceroStatus=="'\"neutral\"'"){
numero=0
}
if(segundoStatus=="'\"neutral\"'" && terceroStatus=="'\"neutral\"'"){
numero=2
}
if(segundoStatus=="''" && terceroStatus=="''"){
numero=3
}
  
  if(segundoStatus=="'\"positive\"'" && terceroStatus=="'\"neg\"'"){
numero=1
}
  if(segundoStatus=="'\"negative\"'" && terceroStatus=="'\"pos\"'"){
numero=0
}

  if(numero!=2 && numero!=3 && numero!=4){
    vector.add("${id}  ${numero}\n")}
  
}

new File('unlabeledTrainDataClassified.txt').withWriter{writerAux->
	vector.each{lineToSave->
		writerAux<<lineToSave

	}
} 
  println " PRUEBA - FIIIIIIN"
}