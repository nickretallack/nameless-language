let f = (nodeLayout: NodeLayout.t, nodePosition: NodePosition.t): bool =>
  nodePosition.columns >= nodeLayout.position.columns
  && nodePosition.columns < nodeLayout.position.columns
  + nodeLayout.size.columns
  && nodePosition.rows >= nodeLayout.position.rows
  && nodePosition.rows <= nodeLayout.position.rows
  + nodeLayout.size.rows;
