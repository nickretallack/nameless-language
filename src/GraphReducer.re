let f = (state: GraphState.t, action: GraphAction.t): GraphState.t =>
  switch (action) {
  | SelectNode({nodeID, additive}) => {
      ...state,
      selection:
        switch (state.selection) {
        | SelectedNodes(nodeIDs) =>
          if (additive) {
            if (Belt.Set.String.has(nodeIDs, nodeID)) {
              let newNodeIDs = Belt.Set.String.remove(nodeIDs, nodeID);
              if (Belt.Set.String.isEmpty(newNodeIDs)) {
                NoSelection;
              } else {
                SelectedNodes(newNodeIDs);
              };
            } else {
              SelectedNodes(Belt.Set.String.add(nodeIDs, nodeID));
            };
          } else {
            SelectedNodes(Belt.Set.String.fromArray([|nodeID|]));
          }
        | _ => SelectedNodes(Belt.Set.String.fromArray([|nodeID|]))
        },
    }
  | RemoveSelectedNodes =>
    switch (state.selection) {
    | SelectedNodes(_) => {...state, selection: NoSelection}
    | _ => state
    }
  | SelectConnection(connectionSide) => {
      ...state,
      selection: SelectedConnection(connectionSide),
    }
  | PointerAction({pointerID, action}) =>
    switch (action) {
    | StartDrawing(drawingConnection) => {
        ...state,
        pointers:
          Belt.Map.set(
            state.pointers,
            pointerID,
            DrawingConnection(drawingConnection),
          ),
      }
    | StartDragging(nodeID) => {
        ...state,
        pointers:
          Belt.Map.set(state.pointers, pointerID, DraggingNode(nodeID)),
      }
    | MovePointer(point) =>
      switch (Belt.Map.get(state.pointers, pointerID)) {
      | Some(DrawingConnection(drawingConnection)) => {
          ...state,
          pointers:
            Belt.Map.set(
              state.pointers,
              pointerID,
              DrawingConnection({...drawingConnection, point}),
            ),
        }
      | _ => state
      }
    | FinishDrawing(_) => {
        ...state,
        error: None,
        pointers: Belt.Map.remove(state.pointers, pointerID),
      }
    | ReleasePointer =>
      switch (Belt.Map.get(state.pointers, pointerID)) {
      | Some(_) => {
          ...state,
          pointers: Belt.Map.remove(state.pointers, pointerID),
        }
      | _ => state
      }
    }
  };
