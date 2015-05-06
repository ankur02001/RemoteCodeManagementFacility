//----<     Search For Semantic Score   >--------------------------------------------------
public Integer semanticScoreSearch(String nodeSearch1, String nodeSearch2){
	Iterator<Node> depthIterator = tree.iterator("SemanticRoot");
	
	// Found node for specific Category 1
	Node nodeSearchforID1 = null;
	while (depthIterator.hasNext()) {
		nodeSearchforID1 = depthIterator.next();
		if (nodeSearchforID1.getIdentifier().contentEquals(nodeSearch1)){
			break;
		}
		System.out.println(nodeSearchforID1.getParent());
	}
	
	// Found node for specific Category 2
	Node nodeSearchforID2 = null;
	while (depthIterator.hasNext()) {
		nodeSearchforID2 = depthIterator.next();
		if (nodeSearchforID2.getIdentifier().contentEquals(nodeSearch2)){
			break;
		}
		System.out.println(nodeSearchforID2.getParent());
	}

	// Calculating semantic score
	return scoreCalculate(nodeSearchforID1, nodeSearchforID2);
}

//----<     Calculate Semantic Score   >--------------------------------------------------
public Integer scoreCalculate(Node nodeSearchforID1, Node nodeSearchforID2){

	// Loop till match not found or till root node in oour case SemanticRoot not found
	while ((nodeSearchforID1.getIdentifier() != "SemanticRoot" || nodeSearchforID2.getIdentifier() != "SemanticRoot")
		&& (nodeSearchforID1.getIdentifier() != nodeSearchforID2.getIdentifier())){
		nodeSearchforID1 = nodeSearchforID1.getParent();
		nodeSearchforID2 = nodeSearchforID2.getParent();
	}
	if (nodeSearchforID1.getIdentifier() == nodeSearchforID2.getIdentifier()){
		return nodeSearchforID1.getLevel(); // Returning Score if matches node or category found 
	}
	return 1; // Returning Score 1 if not found any matching node or category found
}