Thread.start{
  println " PRUEBA - STARTTTTTTTT"
def vector=[:]
def finalVector=["id  sentiment review\n"]

new File('unlabeledTrainDataClassified.txt').eachLine { line -> 
   def linea = line.split("  ")
   vector.put(linea[0],linea[1])
}


new File('unlabeledTrainData.txt').eachLine { line -> 
   def linea = line.split(" ")
   println "${linea[0]}"
   def valueSentiment = vector["${linea[0]}"]
   //println valueSentiment
   if (valueSentiment!=null){
     finalVector.add("${linea[0]} ${valueSentiment} ${linea[1]}\n") 
   }
}
  
new File('unlabeledTrainDataClassifiedReview.txt').withWriter{writerAux->
  finalVector.each{lineToSave->
    writerAux<<lineToSave

  }
} 
  println " PRUEBA - FIIIIIIN"
}