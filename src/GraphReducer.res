let f = (
  emit: DefinitionAction.t => unit,
  implementation: GraphImplementation.t,
  state: GraphState.t,
  action: GraphAction.t,
): ReactUpdate.update<GraphAction.t, GraphState.t> => {
  switch action {
  | SelectNode({nodeID, additive}) =>
    Js.log("select node")
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
    | StartScrolling(position) =>
      ReactUpdate.Update({
        ...state,
        pointers: Belt.Map.set(state.pointers, pointerID, ScrollZoom(position)),
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
      | Some(ScrollZoom(originalPoint)) =>
        // get all of them and make a box
        let otherPoints: list<Point.t> = Belt.Map.reduce(state.pointers, list{}, (
          result,
          thePointerID,
          value,
        ) =>
          switch value {
          | ScrollZoom(point) =>
            if thePointerID != pointerID {
              List.rev_append(result, list{point})
            } else {
              result
            }
          | _ => result
          }
        )

        let (scroll, distanceRatio) = if Js.List.isEmpty(otherPoints) {
          (PointAdd.f(PointSubtract.f(point, originalPoint), state.scroll), 1.0)
        } else {
          let originalAverage = PointAverage.f(Js.List.revAppend(otherPoints, list{originalPoint}))
          let newAverage = PointAverage.f(Js.List.revAppend(otherPoints, list{point}))
          let otherAverage = PointAverage.f(otherPoints)
          let originalDistance = PointDistance.f(originalPoint, otherAverage)
          let newDistance = PointDistance.f(point, otherAverage)
          let distanceRatio = newDistance /. originalDistance
          let motion = PointSubtract.f(newAverage, originalAverage)
          let newScroll = PointAdd.f(motion, state.scroll)
          (newScroll, distanceRatio)
        }

        ReactUpdate.Update({
          ...state,
          scroll: scroll,
          zoom: state.zoom *. distanceRatio,
          pointers: Belt.Map.set(state.pointers, pointerID, ScrollZoom(point)),
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
      ReactUpdate.Update({
        ...state,
        pointers: Belt.Map.remove(state.pointers, pointerID),
      })
    }
  }
}
