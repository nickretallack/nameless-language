let f = (
  emit: DefinitionAction.t => unit,
  implementation: GraphImplementation.t,
  action: GraphAction.t,
  state: GraphState.t,
): ReactUpdate.update<GraphAction.t, GraphState.t> =>
  switch action {
  | SelectNode({nodeID, additive}) =>
    ReactUpdate.Update({
      ...state,
      selection: switch state.selection {
      | SelectedNodes(nodeIDs) =>
        if additive {
          if Belt.Set.String.has(nodeIDs, nodeID) {
            let newNodeIDs = Belt.Set.String.remove(nodeIDs, nodeID)
            if Belt.Set.String.isEmpty(newNodeIDs) {
              NoSelection
            } else {
              SelectedNodes(newNodeIDs)
            }
          } else {
            SelectedNodes(Belt.Set.String.add(nodeIDs, nodeID))
          }
        } else {
          SelectedNodes(Belt.Set.String.fromArray([nodeID]))
        }
      | _ => SelectedNodes(Belt.Set.String.fromArray([nodeID]))
      },
    })
  | RemoveSelectedNodes =>
    switch state.selection {
    | SelectedNodes(nodeIDs) =>
      ReactUpdate.UpdateWithSideEffects(
        {...state, selection: NoSelection},
        _ => {
          emit(RemoveNodes(nodeIDs))
          None
        },
      )
    | _ => ReactUpdate.NoUpdate
    }
  | SelectConnection(connectionSide) =>
    ReactUpdate.Update({
      ...state,
      selection: SelectedConnection(connectionSide),
    })
  | PointerAction({pointerID, action}) =>
    switch action {
    | StartDrawing(drawingConnection) =>
      ReactUpdate.Update({
        ...state,
        pointers: Belt.Map.set(state.pointers, pointerID, DrawingConnection(drawingConnection)),
      })
    | StartDragging(nodeID) =>
      ReactUpdate.Update({
        ...state,
        pointers: Belt.Map.set(state.pointers, pointerID, DraggingNode(nodeID)),
      })
    | MovePointer(point) =>
      switch Belt.Map.get(state.pointers, pointerID) {
      | Some(DrawingConnection(drawingConnection)) =>
        ReactUpdate.Update({
          ...state,
          pointers: Belt.Map.set(
            state.pointers,
            pointerID,
            DrawingConnection({...drawingConnection, point: point}),
          ),
        })
      | _ => ReactUpdate.NoUpdate
      }
    | FinishDragging(nodeScope) =>
      switch Belt.Map.get(state.pointers, pointerID) {
      | Some(DraggingNode(nodeID)) =>
        ReactUpdate.SideEffects(
          _ => {
            emit(ChangeNodeScope({nodeID: nodeID, nodeScope: nodeScope}))
            None
          },
        )
      | _ => ReactUpdate.NoUpdate
      }
    | FinishDrawing({connectionSide: endNib, isSource: endIsSource}) =>
      switch Belt.Map.get(state.pointers, pointerID) {
      | Some(DrawingConnection({
          explicitConnectionSide: {isSource: startIsSource, connectionSide: startNib},
        })) =>
        if startIsSource == endIsSource {
          if startNib == endNib {
            ReactUpdate.Update({
              ...state,
              error: None,
              selection: SelectedNib({
                connectionSide: startNib,
                isSource: startIsSource,
              }),
            })
          } else {
            ReactUpdate.Update({
              ...state,
              error: Some(
                startIsSource
                  ? "Can't connect a source to a source"
                  : "Can't connect a sink to a sink",
              ),
            })
          }
        } else {
          let (source, sink) = startIsSource ? (startNib, endNib) : (endNib, startNib)
          if !ConnectionCheckScope.f(source, sink, implementation.nodes) {
            ReactUpdate.Update({
              ...state,
              error: Some(
                "When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.",
              ),
            })
          } else if (
            DetectCycles.f(
              Belt.Map.set(implementation.connections, sink, source),
              implementation.nodes,
            )
          ) {
            ReactUpdate.Update({
              ...state,
              error: Some("Can't create cycles"),
            })
          } else {
            ReactUpdate.UpdateWithSideEffects(
              {
                ...state,
                error: None,
                pointers: Belt.Map.remove(state.pointers, pointerID),
              },
              _ => {
                emit(CreateConnection({source: source, sink: sink}))
                None
              },
            )
          }
        }
      | _ => ReactUpdate.NoUpdate
      }

    | ReleasePointer =>
      switch Belt.Map.get(state.pointers, pointerID) {
      | Some(DrawingConnection(_)) =>
        ReactUpdate.Update({
          ...state,
          pointers: Belt.Map.remove(state.pointers, pointerID),
        })
      | Some(DraggingNode(nodeID)) =>
        ReactUpdate.UpdateWithSideEffects(
          {...state, pointers: Belt.Map.remove(state.pointers, pointerID)},
          _ => {
            emit(ChangeNodeScope({nodeID: nodeID, nodeScope: GraphScope}))
            None
          },
        )
      | _ => ReactUpdate.NoUpdate
      }
    }
  }
