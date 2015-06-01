echo "" > final.txt

filename="unlabeledTrainData.txt"
indice=0
while read -r line
do
	let indice=indice+1
	echo "------ $indice ----"
    name=$line
    review_id=$(echo $name | awk '{print $1}')
 	review_text=$(echo $name | awk '{$1=""; print $0}')
 	review_text=$(echo $review_text | tr '\<\>\{\}\[\]\(\)\\/\-\_\:\"' ' ' | tr "'" " ")

    #echo "##### REVIEW ID ####"
 	#echo "${review_id}"
 	#echo "##### SENTIMENT ####"
 	#echo "${sentiment}"
 	#echo "##### REVIEW TEXT ###"
 	#echo "${review_text}"
 	
 	clasif1=$(curl -s -X POST 'http://sentiment.vivekn.com/api/batch/' -d'["$review_text"]' | jq .[0].result )
 	clasif2=$(curl -s --data-urlencode "text=$review_text" 'https://api.sentigem.com/external/get-sentiment?api-key=3b92cd4aa6833e941aa0567448511c02kAI63O0oixe7G9_KtM8EQbBvn25gPfDT' | jq .polarity )
 	clasif3=$(curl -s -d "text=$review_text" "http://text-processing.com/api/sentiment/"| jq .label )

 	echo "REVIEW: ${review_id} - UNO: '${clasif1}' - DOS: '${clasif2}' - TRES: '${clasif3}'" >> final.txt

done < "$filename"

echo "FINNN"