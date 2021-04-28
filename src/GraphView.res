@react.component
let make = (
  ~definitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~implementation: GraphImplementation.t,
  ~definition: Definition.t,
  ~display: DefinitionDisplay.t,
  ~documentation: Documentation.t,
  ~languageName: LanguageName.t,
  ~emit: DefinitionAction.t => unit,
  ~error: AppError.t,
  ~stackFrame: option<MaterializedStackFrame.t>,
	~urlHashRest: array<string>,
) => {
  let svg = React.useRef(Js.Nullable.null)
  let (state, dispatch) = ReactUpdate.useReducer(
    {
      open GraphState
      {
        pointers: Belt.Map.make(~id=module(PointerComparable.C)),
        error: None,
        selection: NoSelection,
        scroll: {x: 0.0, y: 0.0},
        zoom: 1.0,
      }
    },
    GraphReducer.f(emit, implementation),
  )

  let sourceToIndex = Belt.Map.reduce(
    implementation.connections,
    Belt.Map.fromArray([], ~id=module(ConnectionSideComparable.C)),
    (acc, _sink, source) =>
      if !Belt.Map.has(acc, source) {
        Belt.Map.set(acc, source, Belt.Map.size(acc))
      } else {
        acc
      },
  )
  let getNode = (nodeID: NodeID.t) => Belt.Map.String.getExn(implementation.nodes, nodeID)
  let columnizedNodes = SortedColumnizeNodes.f(
    implementation.nodes,
    implementation.connections,
    definitions,
    display,
  )
  let scopedNodeIDs = Belt.Map.String.reduce(
    implementation.nodes,
    Belt.Map.make(~id=module(NodeScopeComparable.C)),
    (scopes, id, node) =>
      Belt.Map.update(scopes, node.scope, nodeIDs => Some(
        Belt.Set.String.add(
          switch nodeIDs {
          | None => Belt.Set.String.empty
          | Some(nodeIDs) => nodeIDs
          },
          id,
        ),
      )),
  )
  let (nodeLayouts, graphSize) = LayoutGraph.f(
    scopedNodeIDs,
    columnizedNodes,
    definitions,
    implementation.connections,
  )
  let connectionBypasses = CalculateConnectionBypasses.f(
    nodeLayouts,
    implementation.connections,
    implementation.nodes,
    definition,
    definitions,
    graphSize.columns,
  )
  let graphSize = {
    open NodePosition
    {
      columns: graphSize.columns,
      rows: max(
        graphSize.rows,
        max(
          Belt.List.length(definition.display.inputOrdering),
          Belt.List.length(definition.display.outputOrdering),
        ),
      ),
    }
  }
  let nodeWidth = 120.0
  let textHeight = 20.0
  let xPadding = 60.0
  /* let yPadding = 20.0; */
  let columnWidth = nodeWidth +. xPadding
  let getNodePosition = nodeID => {
    let position = Belt.Map.String.getExn(nodeLayouts, nodeID).position
    open Point
    {
      x: float_of_int(position.columns + 1) *. columnWidth,
      y: float_of_int(position.rows) *. textHeight,
    }
  }
  let sizeToPixels = (size: NodePosition.t) => {
    open Point
    {
      x: float_of_int(size.columns) *. columnWidth -. xPadding,
      y: float_of_int(size.rows) *. textHeight,
    }
  }
  let graphSizePixels = sizeToPixels(graphSize)
  let getNodeSize = nodeID => sizeToPixels(Belt.Map.String.getExn(nodeLayouts, nodeID).size)
  let getNodeDepth = nodeID => Belt.Map.String.getExn(nodeLayouts, nodeID).depth
  let isNibInternal = (node: Node.t, nib: ConnectionNib.t) =>
    switch node.kind {
    | DefinedNode({kind: FunctionDefinitionNode}) =>
      switch nib {
      | NibConnection(_) => true
      | _ => false
      }
    | _ => false
    }
  let nibPositionFormula = (isInternal, isSink, nodePosition, nodeSize, nibIndex) => {
    let rightSide = isInternal ? !isSink : isSink
    open Point
    {
      x: nodePosition.x +. (
        isInternal
          ? switch rightSide {
            | true => nodeSize.x -. nodeWidth
            | false => nodeWidth
            }
          : switch rightSide {
            | true => nodeSize.x
            | false => 0.0
            }
      ),
      y: float_of_int(nibIndex) *. textHeight +. textHeight /. 2.0 +. nodePosition.y,
    }
  }
  let getNibPosition = (connectionSide: ConnectionSide.t, isSink: bool) =>
    switch connectionSide.node {
    | NodeConnection(nodeID) =>
      let nodePosition = getNodePosition(nodeID)
      let nodeSize = getNodeSize(nodeID)
      let node = getNode(nodeID)
      let isInternal = isNibInternal(node, connectionSide.nib)
      let nibIndex = NodeGetNibIndex.f(node, definitions, connectionSide.nib, isSink)
      nibPositionFormula(isInternal, isSink, nodePosition, nodeSize, nibIndex)
    | GraphConnection =>
      let nibIndex = ListFindByIndexExn.f(
        isSink ? definition.display.outputOrdering : definition.display.inputOrdering,
        nibID => ConnectionNib.NibConnection(nibID) == connectionSide.nib,
      )
      nibPositionFormula(true, isSink, {x: 0.0, y: textHeight}, graphSizePixels, nibIndex)
    }
  let allNibs = DefinitionGetExplicitDisplayNibs.f(definition, definitions, languageName)
  let renderedConnections = React.array(
    Belt.Array.map(Belt.Map.toArray(implementation.connections), ((sink, source)) => {
      let sourceIndex = Belt.Map.getExn(sourceToIndex, source)
      let debugState = switch stackFrame {
      | None => ConnectionDebug.NoDebugConnection
      | Some(stackFrame) =>
        let stackConnectionSide = stackFrame.explicitConnectionSide
        if !stackConnectionSide.isSource && stackConnectionSide.connectionSide == sink {
          switch stackFrame.action {
          | Evaluating => EvaluatingConnection
          | Returning(_) => ReturningConnection
          }
        } else {
          ConnectionDebug.NoDebugConnection
        }
      }
      <ConnectionView
        key={ConnectionSideToString.f(sink)}
        sinkPosition={getNibPosition(sink, true)}
        sourcePosition={getNibPosition(source, false)}
        onClick={_event => dispatch(SelectConnection(sink))}
        isSelected={state.selection == SelectedConnection(sink)}
        color={Belt.Array.getExn(
          ConnectionColors.v,
          mod(sourceIndex, Belt.Array.length(ConnectionColors.v)),
        )}
        segments={Belt.List.map(Belt.Map.getExn(connectionBypasses, sink), column =>
          (float_of_int(column) +. 0.5) *. textHeight
        )}
        nodeWidth
        xPadding
        sourceIndex
        debugState
      />
    }),
  )
  let renderedNibs = React.array(
    Belt.Array.map(Belt.List.toArray(allNibs), ({name, explicitConnectionSide}) => {
      let {ExplicitConnectionSide.connectionSide: connectionSide, isSource} = explicitConnectionSide
      let value = if isSource {
        switch stackFrame {
        | None => None
        | Some(stackFrame) => Belt.Map.get(stackFrame.scope.sourceValues, connectionSide)
        }
      } else {
        None
      }
      <NibView
        key={ExplicitConnectionSideKey.f(explicitConnectionSide)}
        text=name
        isSource
        connectionSide
        languageName
        position={getNibPosition(connectionSide, !isSource)}
        emit=dispatch
        isHighlighted={switch state.selection {
        | SelectedNib(highlightedExplicitConnectionSide) =>
          highlightedExplicitConnectionSide == explicitConnectionSide
        | _ => false
        }}
        definitions
        value
      />
    }),
  )
  let renderedSides =
    <DefinitionBoxView
      name={DefinitionGetDisplayName.f(definition, languageName)}
      position={x: 0.0, y: 0.0}
      size=graphSizePixels
      nodeWidth
      textHeight
      nodeScope=NodeScope.GraphScope
      emit=dispatch
    />
  let renderedNodes = React.array(
    Belt.List.toArray(
      Belt.List.map(
        ListSortBy.f(Belt.Map.String.toList(implementation.nodes), ((nodeID, _)) =>
          getNodeDepth(nodeID)
        ),
        ((nodeID, node)) =>
          <NodeView
            key=nodeID
            node
            nodeID
            definitions
            languageName
            position={getNodePosition(nodeID)}
            size={getNodeSize(nodeID)}
            nodeWidth
            textHeight
            emit=dispatch
            selected={switch state.selection {
            | SelectedNodes(nodeIDs) => Belt.Set.String.has(nodeIDs, nodeID)
            | _ => false
            }}
            onClick={event =>
              dispatch(
                SelectNode({
                  nodeID: nodeID,
                  additive: ReactEvent.Mouse.shiftKey(event),
                }),
              )}
            onPointerDown={event => {
			        ReactEvent.Pointer.stopPropagation(event)
              let pointerID = ReactEvent.Pointer.pointerId(event)
              let _ = ReactEvent.Pointer.target(event)["setPointerCapture"](pointerID)
              dispatch(PointerAction({pointerID: pointerID, action: StartDragging(nodeID)}))
            }}
            onPointerUp={event => FireEventOnDropTarget.f(event, "finish-dragging")}
          />,
      ),
    ),
  )
  let renderedDrawingConnections = RenderMap.f(
    ((pointerID: PointerID.t, pointerState: PointerState.t)) =>
      switch pointerState {
      | DrawingConnection({
          point,
          explicitConnectionSide: {connectionSide, isSource: startIsSource},
        }) =>
        let svgPosition = switch svg.current->Js.Nullable.toOption {
        | Some(domSvg) =>
					open BoundingClientRect
          let rect = getBoundingClientRect(domSvg)
          open! Point
          {x: rect.left, y: rect.top}
        | None => {x: 0.0, y: 0.0}
        }
        let adjustedPoint = PointSubtract.f(point, svgPosition)
        <ConnectionView
          key={PointerIDToString.f(pointerID)}
          sourcePosition={startIsSource ? getNibPosition(connectionSide, false) : adjustedPoint}
          sinkPosition={startIsSource ? adjustedPoint : getNibPosition(connectionSide, true)}
          color="black"
          nodeWidth
          xPadding
          sourceIndex=0
        />
      | _ => React.null
      },
    state.pointers,
  )
  <div id="graph-view">
		<nav id="graph-nav" className="nav-buttons">
			<a href=`#${definitionID}/documentation` className={urlHashRest==["documentation"] ? "active" : ""}>{React.string("Documentation")}</a>
			<a href=`#${definitionID}` className={urlHashRest==[] ? "active" : ""}>{React.string("Implementation")}</a>
		</nav>
		{switch(urlHashRest) {
			| ["documentation"] =>

    <div id="documentation">
      <DefinitionHeaderView
        definitionID
        definitions
        documentation
        languageName
        emit
        error
        placeholder="(nameless function)"
      />
      {switch state.error {
      | Some(error) => <div className="error-message"> {React.string(error)} </div>
      | None => React.null
      }}
      {switch state.selection {
      | SelectedNib(explicitConnectionSide) => <>
          <button onClick={_event => emit(EvaluateNib(explicitConnectionSide))}>
            {React.string("Debug")}
          </button>
          <NodeMenuView
            emit definitions languageName nodes=implementation.nodes nib=explicitConnectionSide
          />
        </>
      | SelectedConnection(connectionSide) => <>
          <button
            onClick={_event =>
              emit(EvaluateNib({connectionSide: connectionSide, isSource: false}))}>
            {React.string("Debug")}
          </button>
          <button onClick={_event => emit(RemoveConnection(connectionSide))}>
            {React.string("Remove connection")}
          </button>
        </>
      | SelectedNodes(_) =>
        <button onClick={_event => dispatch(RemoveSelectedNodes)}>
          {React.string("Remove Node(s)")}
        </button>
      | NoSelection => React.null
      }}
      <h2> {React.string("Interface")} </h2>
      <InterfaceView
        definitions interface=implementation.interface documentation display emit languageName
      />
    </div>
		| _ => React.null
		}}
    <div className="graph-canvas"
		        onPointerDown={event => {
          let pointerID = ReactEvent.Pointer.pointerId(event)
          let _ = ReactEvent.Pointer.target(event)["setPointerCapture"](pointerID)
          dispatch(
            PointerAction({
              pointerID: pointerID,
              action: StartScrolling(PointFromPointerEvent.f(event)),
            }),
          )
        }}
        onPointerUp={event => {
          dispatch(
            PointerAction({
              pointerID: ReactEvent.Pointer.pointerId(event),
              action: ReleasePointer,
            }),
          )
        }}
        onPointerMove={event => {
          ReactEvent.Pointer.preventDefault(event)
          ReactEvent.Pointer.stopPropagation(event)
          dispatch(
            PointerAction({
              pointerID: ReactEvent.Pointer.pointerId(event),
              action: MovePointer(PointFromPointerEvent.f(event)),
            }),
          )
        }}
        onLostPointerCapture={event => {
          dispatch(
            PointerAction({
              pointerID: ReactEvent.Pointer.pointerId(event),
              action: ReleasePointer,
            }),
          )}}>
      <svg
				ref={ReactDOM.Ref.domRef(svg)}
        style={ReactDOM.Style.make(
          ~position="absolute",
          ~left=FloatToPixels.f(state.scroll.x),
          ~top=FloatToPixels.f(state.scroll.y),
					~transform=`scale(${Printf.sprintf("%g", state.zoom)})`,
          (),
        )}
        width={FloatToPixels.f(graphSizePixels.x)}
        height={FloatToPixels.f(graphSizePixels.y)}>
        renderedSides renderedNodes renderedConnections renderedDrawingConnections renderedNibs
      </svg>
    </div>
  </div>
}
