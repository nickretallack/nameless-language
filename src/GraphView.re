[@react.component]
let make =
    (
      ~definitionID: DefinitionID.t,
      ~definitions: DefinitionMap.t,
      ~implementation: GraphImplementation.t,
      ~definition: Definition.t,
      ~display: DefinitionDisplay.t,
      ~documentation: Documentation.t,
      ~emit: DefinitionAction.t => unit,
      ~error: AppError.t,
      ~stackFrame: option(MaterializedStackFrame.t),
    ) => {
  let (state, dispatch) =
    ReactUpdate.useReducer(
      GraphState.{
        pointers: Belt.Map.make(~id=(module PointerComparable.C)),
        error: None,
        selection: NoSelection,
      },
      GraphReducer.f(emit, implementation),
    );

  /* Prevent iOS from scrolling all over the place */
  React.useEffect(() => {
    Webapi.Dom.EventTarget.addEventListenerWithOptions(
      "touchmove",
      Webapi.Dom.Event.preventDefault,
      {"passive": false, "capture": true, "once": false},
      DocumentElement.v,
    );
    Some(
      () =>
        Webapi.Dom.EventTarget.removeEventListener(
          "touchmove",
          Webapi.Dom.Event.preventDefault,
          DocumentElement.v,
        ),
    );
  });

  let sourceToIndex =
    Belt.Map.reduce(
      implementation.connections,
      Belt.Map.fromArray([||], ~id=(module ConnectionSideComparable.C)),
      (acc, _sink, source) =>
      if (!Belt.Map.has(acc, source)) {
        Belt.Map.set(acc, source, Belt.Map.size(acc));
      } else {
        acc;
      }
    );
  let getNode = (nodeID: NodeID.t) =>
    Belt.Map.String.getExn(implementation.nodes, nodeID);
  let columnizedNodes =
    SortedColumnizeNodes.f(
      implementation.nodes,
      implementation.connections,
      definitions,
      display,
    );
  let scopedNodeIDs =
    Belt.Map.String.reduce(
      implementation.nodes,
      Belt.Map.make(~id=(module NodeScopeComparable.C)),
      (scopes, id, node) =>
      Belt.Map.update(scopes, node.scope, nodeIDs =>
        Some(
          Belt.Set.String.add(
            switch (nodeIDs) {
            | None => Belt.Set.String.empty
            | Some(nodeIDs) => nodeIDs
            },
            id,
          ),
        )
      )
    );
  let (nodeLayouts, graphSize) =
    LayoutGraph.f(
      scopedNodeIDs,
      columnizedNodes,
      definitions,
      implementation.connections,
    );
  let connectionBypasses =
    CalculateConnectionBypasses.f(
      nodeLayouts,
      implementation.connections,
      implementation.nodes,
      definition,
      definitions,
      graphSize.columns,
    );
  let graphSize =
    NodePosition.{
      columns: graphSize.columns,
      rows:
        max(
          graphSize.rows,
          max(
            Belt.List.length(definition.display.inputOrdering),
            Belt.List.length(definition.display.outputOrdering),
          ),
        ),
    };
  let nodeWidth = 120.0;
  let textHeight = 20.0;
  let xPadding = 60.0;
  /* let yPadding = 20.0; */
  let columnWidth = nodeWidth +. xPadding;
  let getNodePosition = nodeID => {
    let position = Belt.Map.String.getExn(nodeLayouts, nodeID).position;
    Point.{
      x: float_of_int(position.columns + 1) *. columnWidth,
      y: float_of_int(position.rows) *. textHeight,
    };
  };
  let sizeToPixels = (size: NodePosition.t) =>
    Point.{
      x: float_of_int(size.columns) *. columnWidth -. xPadding,
      y: float_of_int(size.rows) *. textHeight,
    };
  let graphSizePixels = sizeToPixels(graphSize);
  let getNodeSize = nodeID =>
    sizeToPixels(Belt.Map.String.getExn(nodeLayouts, nodeID).size);
  let getNodeDepth = nodeID =>
    Belt.Map.String.getExn(nodeLayouts, nodeID).depth;
  let isNibInternal = (node: Node.t, nib: ConnectionNib.t) =>
    switch (node.kind) {
    | DefinedNode({kind: FunctionDefinitionNode}) =>
      switch (nib) {
      | NibConnection(_) => true
      | _ => false
      }
    | _ => false
    };
  let nibPositionFormula =
      (isInternal, isSink, nodePosition, nodeSize, nibIndex) => {
    let rightSide = isInternal ? !isSink : isSink;
    Point.{
      x:
        nodePosition.x
        +. (
          isInternal
            ? rightSide ? nodeSize.x -. nodeWidth : nodeWidth
            : rightSide ? nodeSize.x : 0.0
        ),
      y:
        float_of_int(nibIndex)
        *. textHeight
        +. textHeight
        /. 2.0
        +. nodePosition.y,
    };
  };
  let getNibPosition = (connectionSide: ConnectionSide.t, isSink: bool) =>
    switch (connectionSide.node) {
    | NodeConnection(nodeID) =>
      let nodePosition = getNodePosition(nodeID);
      let nodeSize = getNodeSize(nodeID);
      let node = getNode(nodeID);
      let isInternal = isNibInternal(node, connectionSide.nib);
      let nibIndex =
        NodeGetNibIndex.f(node, definitions, connectionSide.nib, isSink);
      nibPositionFormula(
        isInternal,
        isSink,
        nodePosition,
        nodeSize,
        nibIndex,
      );
    | GraphConnection =>
      let nibIndex =
        ListFindByIndexExn.f(
          isSink
            ? definition.display.outputOrdering
            : definition.display.inputOrdering,
          nibID =>
          ConnectionNib.NibConnection(nibID) == connectionSide.nib
        );
      nibPositionFormula(
        true,
        isSink,
        {x: 0.0, y: textHeight},
        graphSizePixels,
        nibIndex,
      );
    };
  let allNibs = DefinitionGetExplicitDisplayNibs.f(definition, definitions);
  let renderedConnections =
    ReasonReact.array(
      Belt.Array.map(
        Belt.Map.toArray(implementation.connections),
        ((sink, source)) => {
          let sourceIndex = Belt.Map.getExn(sourceToIndex, source);
          let debugState =
            switch (stackFrame) {
            | None => ConnectionDebug.NoDebugConnection
            | Some(stackFrame) =>
              let stackConnectionSide = stackFrame.explicitConnectionSide;
              if (!stackConnectionSide.isSource
                  && stackConnectionSide.connectionSide == sink) {
                switch (stackFrame.action) {
                | Evaluating => EvaluatingConnection
                | Returning(_) => ReturningConnection
                };
              } else {
                ConnectionDebug.NoDebugConnection;
              };
            };
          <ConnectionView
            key={ConnectionSideToString.f(sink)}
            sinkPosition={getNibPosition(sink, true)}
            sourcePosition={getNibPosition(source, false)}
            onClick={_event => dispatch(SelectConnection(sink))}
            isSelected={state.selection == SelectedConnection(sink)}
            color={Belt.Array.getExn(
              ConnectionColors.v,
              sourceIndex mod Belt.Array.length(ConnectionColors.v),
            )}
            segments={Belt.List.map(
              Belt.Map.getExn(connectionBypasses, sink), column =>
              (float_of_int(column) +. 0.5) *. textHeight
            )}
            nodeWidth
            xPadding
            sourceIndex
            debugState
          />;
        },
      ),
    );
  let renderedNibs =
    ReasonReact.array(
      Belt.Array.map(
        Belt.List.toArray(allNibs),
        ({name, explicitConnectionSide}) => {
          let ExplicitConnectionSide.{connectionSide, isSource} = explicitConnectionSide;
          let value =
            if (isSource) {
              switch (stackFrame) {
              | None => None
              | Some(stackFrame) =>
                Belt.Map.get(stackFrame.scope.sourceValues, connectionSide)
              };
            } else {
              None;
            };
          <NibView
            key={ExplicitConnectionSideKey.f(explicitConnectionSide)}
            text=name
            isSource
            connectionSide
            position={getNibPosition(connectionSide, !isSource)}
            emit=dispatch
            isHighlighted={
              switch (state.selection) {
              | SelectedNib(highlightedExplicitConnectionSide) =>
                highlightedExplicitConnectionSide == explicitConnectionSide
              | _ => false
              }
            }
            value
          />;
        },
      ),
    );
  let renderedSides =
    <DefinitionBoxView
      name={DefinitionGetDisplayName.f(definition, "en")}
      position={x: 0.0, y: 0.0}
      size=graphSizePixels
      nodeWidth
      textHeight
    />;
  let renderedNodes =
    ReasonReact.array(
      Belt.List.toArray(
        Belt.List.map(
          ListSortBy.f(
            Belt.Map.String.toList(implementation.nodes), ((nodeID, _)) =>
            getNodeDepth(nodeID)
          ),
          ((nodeID, node)) =>
          <NodeView
            key=nodeID
            node
            definitions
            position={getNodePosition(nodeID)}
            size={getNodeSize(nodeID)}
            nodeWidth
            textHeight
            selected={
              switch (state.selection) {
              | SelectedNodes(nodeIDs) =>
                Belt.Set.String.has(nodeIDs, nodeID)
              | _ => false
              }
            }
            onClick={event =>
              dispatch(
                SelectNode({
                  nodeID,
                  additive: ReactEvent.Mouse.shiftKey(event),
                }),
              )
            }
            onMouseDown={_ =>
              dispatch(
                PointerAction({
                  pointerID: Mouse,
                  action: StartDragging(nodeID),
                }),
              )
            }
            onMouseUp={event =>
              switch (Belt.Map.get(state.pointers, Mouse)) {
              | Some(DraggingNode(_)) =>
                ReactEvent.Mouse.stopPropagation(event);
                dispatch(
                  PointerAction({
                    pointerID: Mouse,
                    action: FinishDragging(NodeScope(nodeID)),
                  }),
                );
              | _ => ()
              }
            }
          />
        ),
      ),
    );
  let renderedDrawingConnections =
    RenderMap.f(
      ((pointerID: PointerID.t, pointerState: PointerState.t)) =>
        switch (pointerState) {
        | DrawingConnection({
            point,
            explicitConnectionSide: {connectionSide, isSource: startIsSource},
          }) =>
          let adjustedPoint = Point.{x: point.x, y: point.y -. 18.0};
          <ConnectionView
            key={PointerIDToString.f(pointerID)}
            sourcePosition={
              startIsSource
                ? getNibPosition(connectionSide, false) : adjustedPoint
            }
            sinkPosition={
              startIsSource
                ? adjustedPoint : getNibPosition(connectionSide, true)
            }
            color="black"
            nodeWidth
            xPadding
            sourceIndex=0
          />;
        | _ => ReasonReact.null
        },
      state.pointers,
    );
  <div>
    <svg
      width={FloatToPixels.f(graphSizePixels.x)}
      height={FloatToPixels.f(graphSizePixels.y)}
      onMouseMove={event => {
        ReactEvent.Mouse.preventDefault(event);
        dispatch(
          PointerAction({
            pointerID: Mouse,
            action: MovePointer(PointFromMouseEvent.f(event)),
          }),
        );
      }}
      onTouchMove={event =>
        EventIterateTouches.f(event, touch =>
          dispatch(
            PointerAction({
              pointerID: Touch(touch##identifier),
              action:
                MovePointer({
                  x:
                    touch##clientX
                    -.
                    ReactEvent.Touch.currentTarget(event)##offsetLeft,
                  y:
                    touch##clientY
                    -.
                    ReactEvent.Touch.currentTarget(event)##offsetTop,
                }),
            }),
          )
        )
      }
      onMouseUp={_ =>
        dispatch(PointerAction({pointerID: Mouse, action: ReleasePointer}))
      }
      onTouchEnd={event =>
        EventIterateTouches.f(event, touch =>
          dispatch(
            PointerAction({
              pointerID: Touch(touch##identifier),
              action: ReleasePointer,
            }),
          )
        )
      }>
      renderedSides
      renderedNodes
      renderedConnections
      renderedDrawingConnections
      renderedNibs
    </svg>
    <DefinitionHeaderView
      definitionID
      definitions
      documentation
      emit
      error
      placeholder="(nameless function)"
    />
    {switch (state.error) {
     | Some(error) =>
       <div className="error-message"> {ReasonReact.string(error)} </div>
     | None => ReasonReact.null
     }}
    {switch (state.selection) {
     | SelectedNib(explicitConnectionSide) =>
       <>
         <button
           onClick={_event => emit(EvaluateNib(explicitConnectionSide))}>
           {ReasonReact.string("Debug")}
         </button>
         <NodeMenuView
           emit
           definitions
           nodes={implementation.nodes}
           nib=explicitConnectionSide
         />
       </>
     | SelectedConnection(connectionSide) =>
       <>
         <button
           onClick={_event =>
             emit(EvaluateNib({connectionSide, isSource: false}))
           }>
           {ReasonReact.string("Debug")}
         </button>
         <button onClick={_event => emit(RemoveConnection(connectionSide))}>
           {ReasonReact.string("Remove connection")}
         </button>
       </>
     | SelectedNodes(_) =>
       <button onClick={_event => dispatch(RemoveSelectedNodes)}>
         {ReasonReact.string("Remove Node(s)")}
       </button>
     | NoSelection => ReasonReact.null
     }}
    <h2> {ReasonReact.string("Interface")} </h2>
    <InterfaceView
      definitions
      interface={implementation.interface}
      documentation
      display
      emit
    />
  </div>;
};
